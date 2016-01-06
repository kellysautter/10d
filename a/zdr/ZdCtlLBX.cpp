/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctllbx.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon ListBox control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.03.04    TMV
//    Fix for Drag & Drop within a listbox / drop after last item
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.21    DKS   Enhance
//    Added SetCtrlMapping to remap a Control.
//
// 2001.09.18    DKS    WEB
//    Back out Delta Data implementation.
//
// 2001.09.01    DKS    WEB
//    Handle case of single add/delete in lists.
//
// 2001.06.06    BL     WEB
//    Reset Flag DeleteAllRows
//
// 2001.05.30    BL     WEB
//    Has Listbox no RowDatas, set Flag DeleteAllRows
//
// 2001.05.29    DKS    WEB
//    Implement merge of list data.
//
// 2001.05.29    BL     WEB
//    Bugifx for ListBox without Header Buttons
//
// 2001.05.23    BL     WEB
//    Implement SetRowColText and GetRowColText for Web
//
// 2001.05.16    BL     WEB
//    Modified MapToRemoteDataset for Multiselection: delete all selectd
//    Items
//
// 2001.05.14    BL     WEB
//    Set Application View Cursor, set Select flag in Application View,
//    Bugfix Multi Selection
//
// 2001.05.03    DKS    WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2000.04.29    DKS    WEB
//    Handle special case for "do not hilite at position" in WEB.
//
// 2001.04.19    DKS    Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2000.03.17    TMV    WEB
//    validate parent window's handle prior to call Create/CreateEx
//
// 2000.03.01    DKS    Z10    RemoteServer
//    ListBox trying to run multi-select in Remote Server.
//
// 1999.12.03    DKS    Z10    QS999
//    ListBox now protects against invalid views properly.
//
// 1999.08.27    DKS    Z10    QS999
//    Protect against dropping incorrect local list view.
//
// 1999.06.25    DKS    Z10    QS999
//    Protect against null return pointer from GetCapture.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//
// 1998.10.30    DKS
//    Eliminated "hanging views" by dropping when possible.
//
// 1998.08.13    DKS
//    Fixed list box when listing with scope of OI ... NOT YET IMPLEMENTED
//
// 1998.08.13    DKS
//    Permit setting of font from Zeidon.ini
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlLbx.h"
#include "ZdCtlLbh.h"
#include "ZdCtlGbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

#ifdef zREMOTE_SERVER

IMPLEMENT_DYNAMIC( ZListBox, ZRmtDataSet )
BEGIN_MESSAGE_MAP( ZListBox, ZRmtDataSet )
END_MESSAGE_MAP( )

#else

IMPLEMENT_DYNAMIC( ZListBox, CListBox )
BEGIN_MESSAGE_MAP( ZListBox, CListBox )
   ON_CONTROL_REFLECT( LBN_DBLCLK, LBNDblclk )
   ON_CONTROL_REFLECT( LBN_ERRSPACE, LBNErrspace )
   ON_CONTROL_REFLECT( LBN_SELCANCEL, LBNSelcancel )
   ON_CONTROL_REFLECT( LBN_SELCHANGE, LBNSelchange )
   ON_CONTROL_REFLECT( LBN_SETFOCUS, LBNSetFocus )
   ON_CONTROL_REFLECT( LBN_KILLFOCUS, LBNKillFocus )
   ON_WM_CREATE( )
   ON_WM_ERASEBKGND( )
   ON_WM_NCPAINT( )
   ON_WM_PAINT( )
   ON_WM_ENABLE( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_LBUTTONUP( )
// ON_WM_LBUTTONDBLCLK( )
   ON_WM_RBUTTONDOWN( )
   ON_WM_RBUTTONUP( )
   ON_WM_RBUTTONDBLCLK( )
   ON_WM_KEYDOWN( )
   ON_WM_CHAR( )
   ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP( )

#endif

#define zLBN_ERRSPACE    (-2)
#define zLBN_SELCHANGE     1
#define zLBN_DBLCLK        2
#define zLBN_SELCANCEL     3
#define zLBN_SETFOCUS      4
#define zLBN_KILLFOCUS     5

// The default select set used by the Object Engine is 1.  There are two
// possible select sets used by a list box at any point ... the DISPLAY
// select set (entities marked as selected in the DISPLAY select set are
// displayed in the list box) and the SELECT select set (this select set
// is maintained by the list box to indicate those entities which have
// been selected by the user via the list box).  If the application wants
// to use a DISPLAY select set, it can use the default select set, but
// the SELECT select set must be different from the DISPLAY select set.

#define m_pzsScope  m_pzsAName // AN was stolen for the scoping entity name

#define zLISTBOX_SORTEDHEADERS        zCONTROL_SPECIFIC_2
#define zLISTBOX_COLUMNHEADERS        zCONTROL_SPECIFIC_3

#ifdef zREMOTE_SERVER
   zCPCHAR cpcListBox         = "ListBox";
   zCPCHAR cpcLBDataRowEntity = "ListRowData";
   zCPCHAR cpcLBSelItemEntity = "SelectedItem";
   zCPCHAR cpcLBColumnEntity  = "ListBoxColumn";
#endif

// ZListBox - ctor
ZListBox::ZListBox( ZSubtask *pZSubtask,
                    CWnd     *pWndParent,
                    ZMapAct  *pzmaComposite,
                    zVIEW    vDialog,
                    zSHORT   nOffsetX,
                    zSHORT   nOffsetY,
                    zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
#ifdef zREMOTE_SERVER
          ZRmtDataSet( ),
#else
          CListBox( ),
#endif
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
          m_csViewNameList( "__" )
{
#ifdef DEBUG_ALL
   if ( m_pzsVName )
   {
      TraceLineS( "ListBox::ctor Name ", *m_pzsTag );
      TraceLineS( "ListBox::ctor View Name ", *m_pzsVName );
   }
#endif

   zSHORT nLists = 2;

#ifdef zREMOTE_SERVER
   m_nColLastSorted = -1;
   m_pSortColumnList = 0;
   m_bAutoSort = FALSE;

   if ( (pCtrlDef->Subtype & zLISTBOX_COLUMNHEADERS) ||
        (pCtrlDef->Subtype & zLISTBOX_SORTEDHEADERS) )
   {
      nLists = 3;
      m_bShowHeaderButtons = TRUE;

      if ( pCtrlDef->Subtype & zLISTBOX_SORTEDHEADERS )
         m_bAutoSort = TRUE;
   }
   else
   {
      m_bShowHeaderButtons = FALSE;
   }

#endif

   m_vApp = m_vAppList = 0;
   m_csViewName += *m_pzsTag;
   m_csViewNameList += *m_pzsTag;

   m_lEntityCnt = 0;
   m_pchListInfo = 0;
   m_lCharHeight = 0;
   m_lCharWidth = 0;
   m_nCols = 0;
   m_pCol = 0;
   m_nDisplaySelSet = 0;
   m_nSelectSelSet = 1;
   m_nFirstIndex = LB_ERR;
   m_nSelCnt = 0;
   m_ulBkColor = 0;
// m_plCharWidth = new long[ 256 ];

   m_pchMap = new char[ 512 ];   // initialize to something reasonable
   m_pchMapLth = 456;  // this little lie provides a 56 byte overflow buffer

   if ( pCtrlDef->Subtype & zLISTBOX_SCOPE_OI )
      m_ulFlag = zQUAL_SCOPE_OI;
   else
      m_ulFlag = 0;

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

   // The painter must ensure Entity if ViewName
   if ( m_pzsVName )
   {
#ifdef DEBUG_ALL
      TraceLineS( "In ctor for ZListBox", *m_pzsTag );
      TraceLineS( "ZListBox View ", *m_pzsVName );
      if ( m_pzsEName )
         TraceLineS( "ZListBox Entity ", *m_pzsEName );
      if ( m_pzsScope )
         TraceLineS( "ZListBox Scope ", *m_pzsScope );
#endif

      GetAttributeLength( &m_ulListLth, vDialog, szlCtrl, szlCtrlBOI );
      if ( m_ulListLth )
      {
         m_pchListInfo = new char[ m_ulListLth ];
         GetVariableFromAttribute( m_pchListInfo, &m_ulListLth, zTYPE_BLOB,
                                   m_ulListLth, vDialog, szlCtrl, szlCtrlBOI,
                                   0, 0 );

         // Convert now forces the first tab stop to always be zero.
         // We now have to convert longs to integers prior to calling the
         // Windows API SetTabStops (for NT, we should not have to do this).
         m_nCols = (zSHORT) (*((zPLONG) (m_pchListInfo + sizeof( zLONG ))));
         m_pCol = new zLB_COL[ (m_nCols + 1) ];
         zmemset( m_pCol, 0, sizeof( zLB_COL ) * (m_nCols + 1) );

         // If we have the headers in the structure:
         //
         // We first have 3 longs + one long for each column in the listbox,
         // then there is the list of tag/text labels for each column,
         // followed by the mapping information for each column.
         //
         // Otherwise:
         // We first have 2 longs + one long for each column in the listbox,
         // followed by the mapping information for each column.
         zULONG ulTagLth = (zUSHORT) (m_nCols + 3) * sizeof( zLONG );
         zULONG ulLth;
         if ( nLists == 2 )
            ulLth = (zUSHORT) (m_nCols + 2) * sizeof( zLONG );
         else
            ulLth = (zUSHORT) (*((zPLONG) (m_pchListInfo +
                                                      2 * sizeof( zLONG ))));
         for ( zSHORT nCol = 1; nCol <= m_nCols; nCol++ )
         {
            m_pCol[ nCol ].pchText = 0;
            m_pCol[ nCol ].nTabPos = (zSHORT)
            *((zPLONG) (m_pchListInfo + ((nCol + nLists) * sizeof( zLONG ))));
            if ( m_pZSubtask->m_bUseMapDlgUnits )
            {
               m_pCol[ nCol ].nTabPos =
                           mConvertMapDlgToPixelX( m_pCol[ nCol ].nTabPos );
            }
            else
            {
               m_pCol[ nCol ].nTabPos =
                           mConvertDlgUnitToPixelX( m_pCol[ nCol ].nTabPos );
            }

            if ( nCol &&
                 m_pCol[ nCol ].nTabPos < m_pCol[ nCol - 1 ].nExtent + 8 )
            {
               m_pCol[ nCol - 1 ].nExtent = m_pCol[ nCol ].nTabPos - 8;
            }

            // Get the number of dialog units allotted for this attribute.
            m_pCol[ nCol ].nColLth =
                                (zSHORT) *((zPLONG) (m_pchListInfo + ulLth));
            if ( m_pCol[ nCol ].nColLth < 0 ) // convert flag to right justify
            {
               m_pCol[ nCol ].nJustify = 1;  // right justify
               m_pCol[ nCol ].nColLth = -(m_pCol[ nCol ].nColLth);
            }
            else
            {
               m_pCol[ nCol ].nJustify = 0;  // left justify
            }

            if ( m_pZSubtask->m_bUseMapDlgUnits )
            {
               m_pCol[ nCol ].nExtent =
                       mConvertMapDlgToPixelX( m_pCol[ nCol ].nColLth ) + 2;
               m_pCol[ nCol ].nColLth =
                            mConvertMapDlgToCharX( m_pCol[ nCol ].nColLth );
            }
            else
            {
               m_pCol[ nCol ].nExtent =
                       mConvertDlgUnitToPixelX( m_pCol[ nCol ].nColLth ) + 2;
               m_pCol[ nCol ].nColLth =
                            mConvertDlgUnitToCharX( m_pCol[ nCol ].nColLth );
            }

#ifdef DEBUG_ALL
            TraceLineI( "nChar DlgToChar = ", m_pCol[ nCol ].nColLth );
            TraceLineI( "nExtent DlgToPixel = ", m_pCol[ nCol ].nExtent );
#endif
            zLONG k;

            ulLth += sizeof( zLONG );
            if ( m_pchListInfo[ ulLth ] )
            {
               k = *((zPLONG) (m_pchListInfo + ulLth));
               ulLth += sizeof( zLONG );
               m_pCol[ nCol ].pchEntity = new char[ k + 1 ];
               strcpy_s( m_pCol[ nCol ].pchEntity, k + 1, m_pchListInfo + ulLth );
               ulLth += k + 1;   // skip past entity name (embedded names
                                 // are forced by convert to maintain
                                 // sizeof( zLONG ) byte boundaries)
            }
            else
            {
               k = zstrlen(*m_pzsEName) + 1;
               ulLth += sizeof( zLONG );
               m_pCol[ nCol ].pchEntity = new char[ k ];
               strcpy_s( m_pCol[ nCol ].pchEntity, k, *m_pzsEName );
            }

            k = *((zPLONG) (m_pchListInfo + ulLth));
            ulLth += sizeof( zLONG );

            m_pCol[ nCol ].pchAttrib = new char[ k + 1 ];
            strcpy_s( m_pCol[ nCol ].pchAttrib, k + 1, m_pchListInfo + ulLth );
            ulLth += k + 1;   // skip past attribute name (embedded names
                              // are forced by convert to maintain
                              // sizeof( zLONG ) byte boundaries)

            k = *((zPLONG) (m_pchListInfo + ulLth));
            ulLth += sizeof( zLONG );
            if ( k == 0 )  // if k is 0 then there is no context
            {
               m_pCol[ nCol ].pchContext = 0;
            }
            else
            {
               m_pCol[ nCol ].pchContext = new char[ k + 1 ];
               strcpy_s( m_pCol[ nCol ].pchContext, k + 1, m_pchListInfo + ulLth );
               ulLth += k + 1;   // skip past context (embedded names
                                 // are forced by convert to maintain
                                 // sizeof( zLONG ) byte boundaries)
            }

#ifdef zREMOTE_SERVER
            if ( m_bShowHeaderButtons )
            {
               // Now get the tag and text information
               k = *((zPLONG) (m_pchListInfo + ulTagLth));
               ulTagLth += sizeof( zLONG );
               m_pCol[ nCol ].pchTag = m_pchListInfo + ulTagLth;
               ulTagLth += k + 1;   // skip past tag name (embedded names
                                    // are forced by convert to maintain
                                    // sizeof( zLONG ) byte boundaries)

               k = *((zPLONG) (m_pchListInfo + ulTagLth));
               ulTagLth += sizeof( zLONG );
               m_pCol[ nCol ].pchText = new char[ k + 1 ];
               zmemcpy( m_pCol[ nCol ].pchText, m_pchListInfo + ulTagLth, k + 1 );
               zPCHAR pch = zstrchr( m_pCol[ nCol ].pchText, '\t' );
               if ( pch )
               {
                  zLONG  lNLS;
                  zPCHAR pchTab = zstrchr( pch + 1, '\t' );
                  if ( pchTab )
                  {
                     *pchTab = 0;
                     lNLS = *((zPLONG) (pchTab + 2));
                     if ( lNLS >= 0 )
                     {
                        SetAttributeFromInteger( vDialog, szlCtrl, "NLS", lNLS );
                        GetAddrForAttribute( (zPCHAR *) &pchTab, vDialog,
                                             szlCtrl, "D_Text" );
                        if ( *pchTab )
                        {
                           delete [] m_pCol[ nCol ].pchText;
                           m_pCol[ nCol ].pchText = new char[ zstrlen( pchTab ) + 1 ];
                           strcpy_s( m_pCol[ nCol ].pchText, zstrlen( pchTab ) + 1, pchTab );
                        }
                     }
                  }

                  *pch = 0;
                  m_pCol[ nCol ].pchMB_Text = new char[ zstrlen( pch + 1 ) + 1 ];
                  strcpy_s( m_pCol[ nCol ].pchMB_Text, zstrlen( pch + 1 ) + 1, pch + 1 );
               }
               else
               {
                  m_pCol[ nCol ].pchMB_Text = 0;
               }

               ulTagLth += k + 1;   // skip past text value (embedded names
                                    // are forced by convert to maintain
                                    // sizeof( zLONG ) byte boundaries)

            } //end of:  if ( m_bShowHeaderButtons )
#endif  // zREMOTE_SERVER

         }  // end of:  for ( zSHORT nCol = 0; nCol < m_nCols; nCol++ )

         m_pCol[ 1 ].nTabPos = 3;      // hard-code border indent

#ifdef DEBUG_ALL
         for ( nCol = 1; nCol <= m_nCols; nCol++ )
         {
            TraceLineI( "nTabPos = ", m_pCol[ nCol ].nTabPos );
            TraceLineI( "nColLth = ", m_pCol[ nCol ].nColLth );
            TraceLineI( "nJustify = ", m_pCol[ nCol ].nJustify );
            TraceLineI( "nExtent = ", m_pCol[ nCol ].nExtent );
         }
#endif
      }
   }

   Attr.Style &= ~LBS_STANDARD;
   Attr.Style |= WS_CHILD | WS_TABSTOP | WS_VSCROLL |
                 WS_HSCROLL | LBS_NOREDRAW | LBS_NOTIFY |
                 LBS_WANTKEYBOARDINPUT | LBS_OWNERDRAWFIXED;

// if ( pCtrlDef->Subtype & zCONTROL_SIZEABLEBORDER )
//    Attr.Style |= WS_THICKFRAME;
// else
// if ( !(pCtrlDef->Subtype & zCONTROL_BORDEROFF) )
//    Attr.Style |= WS_BORDER;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

// if ( pCtrlDef->Subtype & zLISTBOX_NOINTEGRALHEIGHT ) // always with 3D
//    Attr.Style |= LBS_NOINTEGRALHEIGHT;  NOT WITH OWNERDRAW

   if ( pCtrlDef->Subtype & zLISTBOX_MULTIPLESEL )
   {
      m_chMultiSelect = 'Y';
      if ( (pCtrlDef->Subtype & zLISTBOX_EXTENDMULTIPLESEL) ==
                                zLISTBOX_EXTENDMULTIPLESEL )
         Attr.Style |= LBS_EXTENDEDSEL;
      else
         Attr.Style |= LBS_MULTIPLESEL;
   }
   else
   {
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
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
      CreateZ( );
   }
}

void
ZListBox::CreateZ( )
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
         CreateEx( WS_EX_CLIENTEDGE, "LISTBOX", *m_pzsText,
                   Attr.Style, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef zREMOTE_SERVER

int
ZListBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnCreate ", *m_pzsTag );
#endif

   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   SetFontOverride( );
   if ( m_lCharWidth == 0 )
   {
      // Loop processing each column.
      for ( zSHORT nCol = 1; nCol <= m_nCols; nCol++ )
         m_lCharWidth += m_pCol[ nCol ].nExtent;
   }

   SetHorizontalExtent( m_lCharWidth );
   return( nRC );
}

BOOL
ZListBox::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::DestroyWindow ", *m_pzsTag );
   TraceLineI( "ZListBox::DestroyWindow EntityCnt ", m_lEntityCnt );
#endif

   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }

   zPCHAR pchMem;
   zLONG  k;

   for ( k = 0; k < m_lEntityCnt; k++ )
   {
      pchMem = (zPCHAR) GetItemData( k );
      if ( pchMem && pchMem != (zPCHAR) -1 && *pchMem )
      {
         pchMem = (zPCHAR) DrUnlockTaskMemory( pchMem );
         DrFreeTaskMemory( pchMem );
      }
   }

   m_lEntityCnt = 0;
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI |
                        zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CListBox::DestroyWindow( ) );
}

#endif

void
ZListBox::SetDisplaySelSet( zSHORT nSelSet )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::SetDisplaySelSet ", nSelSet );
#endif
   m_nDisplaySelSet = nSelSet;
   if ( m_nDisplaySelSet )
      m_ulFlag |= zQUAL_SELECTED;
   else
      m_ulFlag &= ~zQUAL_SELECTED;
}

void
ZListBox::SetSelectSelSet( zSHORT nSelSet )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::SetSelectSet", nSelSet );
#endif
   m_nSelectSelSet = nSelSet;
}

// Load m_pchMap with the value to put in the listbox
zLONG
ZListBox::FormatTextAtPosition( zLONG lPos )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::FormatTextAtPosition ", *m_pzsTag );
#endif
   *m_pchMap = 0; // initialize to null string in case there are problems

   // If we don't have a list view ... quit.
   if ( m_vAppList == 0 || m_pchListInfo == 0 )
   {
      return( 0 );
   }

   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
      if ( GetViewByName( &m_vApp, *m_pzsVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
         {
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, sizeof( szMsg ),
                      "Unable to load view: %s for %s.%s.%s",
                      (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                      (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString());
            TraceLineS( "ZListBox::FormatTextAtPosition ", szMsg );
         }

         m_vAppList = 0;
         return( 0 );
      }
      else
      {
         SetNameForView( m_vApp, m_csViewName, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
         CreateViewFromViewForTask( &m_vAppList, m_vApp, m_pZSubtask->m_vDialog );
         SetNameForView( m_vAppList, m_csViewNameList,
                         m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
      // SfLockView( m_vAppList );  // we lock this view because it is
                                    // unnamed ... to prevent the OE from
                                    // automatically cleaning it up during
                                    // the deletion of an OE subtask.
      }
   }

   zPCHAR  pchPos;
   zULONG  ulKey;
   zSHORT  nOffset;
   zSHORT  nCol;
   zLONG   k;

#ifdef zREMOTE_SERVER
   ulKey = GetEntityKey( m_vAppList, *m_pzsEName );
#else
   ulKey = 0;
#endif
   nOffset = 0;
   for ( nCol = 1; nCol <= m_nCols; nCol++ )
   {
      // this is the attribute to present to the list
      zSHORT nRC = GetVariableFromAttribute( m_pchMap + nOffset, 0, zTYPE_STRING,
                                            max( 0, m_pchMapLth - nOffset ),
                                            m_vAppList,
                                            m_pCol[ nCol ].pchEntity,
                                            m_pCol[ nCol ].pchAttrib,
                                            m_pCol[ nCol ].pchContext,
                                            m_pCol[ nCol ].pchContext ?
                                               zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

      if ( nRC == zVAR_NULL )
         m_pchMap[ nOffset ] = 0;

      // using tab delimiters ... so replace any tabs in text with spaces
      while ( (pchPos = zstrchr( m_pchMap + nOffset, '\t' )) != 0 )
      {
         *pchPos = ' ';
      }

      k = zstrlen( m_pchMap + nOffset );
      nOffset += (zSHORT) k;
      if ( nCol < m_nCols )  // don't tab delimit the last column
      {
#ifdef zREMOTE_SERVER
         m_pchMap[ nOffset ] = zREMOTE_DATA_DELIMITER;;
#else
         m_pchMap[ nOffset ] = '\t';
#endif
         nOffset++;
      }

   }  // end of:  for ( nCol = 1; nCol <= m_nCols; ... looping thru fields

   if ( nOffset > m_pchMapLth )
      nOffset = m_pchMapLth;

   m_pchMap[ nOffset ] = 0;

#if 0
   // replace any carriage returns or line feeds with spaces
   while ( (pchPos = zstrchr( m_pchMap + nOffset, '\n' )) != 0 )
   {
      *pchPos = ' ';
   }

   while ( (pchPos = zstrchr( m_pchMap + nOffset, '\r' )) != 0 )
   {
      *pchPos = ' ';
   }
#endif

#if 0    // #ifdef zREMOTE_SERVER

   if ( zstrchr( m_pchMap, '&' ) )
   {
      zPCHAR  pchBuffer = new char[ (zstrlen( m_pchMap ) + 1) * 2 ];
      zPCHAR  pchAmpersand = zstrchr( m_pchMap, '&' );
      zPCHAR  pchPos = m_pchMap;
      zULONG  ulLth = 0;

      pchPos = m_pchMap;
      while ( pchAmpersand )
      {
         strcpy_s( pchBuffer + ulLth, commented out???, pchPos );
         ulLth += pchAmpersand - pchPos + 1;
         pchBuffer[ ulLth++ ] = '&';

         pchPos = pchAmpersand + 1;
         pchAmpersand = zstrchr( pchPos, '&' );
      }

      strcpy_s( pchBuffer + ulLth, commented out???, pchPos );
      strcpy_s( m_pchMap, commented out???, pchBuffer );
      delete [] pchBuffer;
   }
#endif

#ifdef DEBUG_ALL
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "%ld %s", lPos, m_pchMap );
      TraceLineS( "ZListBox::Formatted Text: ", szMsg );
   }
#endif
   return( ulKey );
}

zSHORT
ZListBox::MapFromOI( WPARAM wFlag )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineS( "ZListBox::MapFromOI ", *m_pzsTag );
      TraceLineS( "ZListBox::MapFromOI ViewName ", *m_pzsVName );
   }
#endif

   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

   m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
   zLONG lTop = GetTopIndex( );
   if ( GetViewByName( &m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   {
      DropView( m_vAppList );
      m_vAppList = 0;
   }
// else
//    TraceLineS( "ZListBox::MapFromOI m_vAppList null ", *m_pzsTag );

   // Auto-enable if this control was auto-disabled.
   if ( m_ulMapActFlags & zMAPACT_AUTO_DISABLED )
   {
      m_ulMapActFlags |= zMAPACT_ENABLED;
      m_ulMapActFlags &= ~zMAPACT_AUTO_DISABLED;
      if ( mIs_hWnd( m_hWnd ) )
         EnableWindow( TRUE );
   }

   zPCHAR  pchMem;
   zLONG   lKey;
   zLONG   k;

   // No painting during change
   // LockWindowUpdate( );
   SetRedraw( FALSE );

   for ( k = 0; k < m_lEntityCnt; k++ )
   {
      pchMem = (zPCHAR) GetItemData( k );
      if ( pchMem && pchMem != (zPCHAR) -1 && *pchMem )
      {
         pchMem = (zPCHAR) DrUnlockTaskMemory( pchMem );
         DrFreeTaskMemory( pchMem );
      }
   }

   m_lEntityCnt = 0;
// TraceLineS( "MapFromOI ResetContent", "" );
   ResetContent( );

   // If we cannot get the view ... quit.
   if ( GetViewByName( &m_vApp, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
   {
      if ( m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
      {
         zCHAR szMsg[ 256 ];

         sprintf_s( szMsg, sizeof( szMsg ), "Unable to load view: %s for %s.%s.%s",
                   (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                   (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString() );
         TraceLineS( "ZListBox::MapFromOI ", szMsg );
      }

//    UnlockWindowUpdate();
      SetRedraw( TRUE );
      return( 0 );
   }

   SetNameForView( m_vApp, m_csViewName,
                   m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;

   zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, 1 );  // just to get the
   SetSelectSetForView( m_vApp, nPrevSelSet );             // previous selset
   while ( m_ulListLth )  // purist's goto
   {
      int  iRelativeNbr;  // this must be int for use in API call

      if ( m_chListOnlyWhenSelected )
      {
         if ( m_pzsScope == 0 || m_pzsScope->IsEmpty( ) )
         {
            mDeleteInit( m_pzsScope );
            MiGetParentEntityNameForView( m_pchMap, m_vApp, *m_pzsEName );
            if ( m_pchMap[ 0 ] )
               m_pzsScope = new CString( m_pchMap );
         }

         SetSelectSetForView( m_vApp, m_nSelectSelSet );

         // If there is no selected parent entity ... quit.
         if ( m_pzsScope == 0 || m_pzsScope->IsEmpty( ) ||
              SetEntityCursor( m_vApp, *m_pzsScope, 0,
                               m_ulFlag | zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN,
                               0, 0, 0, 0, 0, 0 ) < 0 )
         {
            break;  // goto end of while
         }
      }

      if ( m_nDisplaySelSet )
      {
         // If there are no entities to list ... quit.
         SetSelectSetForView( m_vApp, m_nDisplaySelSet );
         if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                               m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT | zQUAL_SELECTED,
                               0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
         {
            break;  // goto end of while
         }
      }
      else
      {
         // If there are no entities to list ... quit.
         if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                               m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                               0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
         {
            break;  // goto end of while
         }
      }

      CreateViewFromViewForTask( &m_vAppList, m_vApp,  m_pZSubtask->m_vDialog );
      SetNameForView( m_vAppList, m_csViewNameList, m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
   // SfLockView( m_vAppList );  // we lock this view because it is
                                 // unnamed ... to prevent the OE from
                                 // automatically cleaning it up during
                                 // the deletion of an OE subtask.

//    TraceLineS( "ZListBox::MapFromOI ObjectName ", szViewObjectName );
//    TraceLineX( "ZListBox::MapFromOI creating m_vAppList ",
//                (zLONG) m_vAppList );

      zCPCHAR cpcEntity;
      zSHORT  nTopSel = -1;
      zSHORT  nRC;

      // We have 2 longs and then a long for each column and then a long
      // for the length of the first entity name.
      cpcEntity = m_pchListInfo +
         (((zLONG) *(m_pchListInfo + sizeof( zLONG ))) + 3) * sizeof( zLONG );

      if ( *cpcEntity )
         cpcEntity += sizeof( zLONG );
      else
         cpcEntity = *m_pzsEName;

      iRelativeNbr = 0;
      nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                             m_ulFlag | zPOS_FIRST, 0, 0, 0, 0,
                             *m_pzsScope, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         lKey = FormatTextAtPosition( m_lEntityCnt );

         if ( m_pchMap[ 0 ] &&
              DrAllocTaskMemory( (zCOREMEM) &pchMem, zstrlen( m_pchMap ) + 1 ) == 0 )
         {
            pchMem = (zPCHAR) DrLockTaskMemory( pchMem );
            strcpy_s( pchMem, zstrlen( m_pchMap ) + 1, m_pchMap );
         }
         else
         {
            pchMem = "";
         }

#ifdef zREMOTE_SERVER
         if ( AddString( pchMem, lKey, m_lEntityCnt ) < 0 ||
#else
         if ( AddString( pchMem ) < 0 ||
#endif
              SetItemData( iRelativeNbr, (DWORD) pchMem ) < 0 )
         {
            TraceLineS( "(lbx) Error adding string ", pchMem );
            break;
         }

         if ( m_chMultiSelect )
         {
            if ( m_nDisplaySelSet )
               SetSelectSetForView( m_vAppList, m_nSelectSelSet );

            if ( GetSelectStateOfEntity( m_vAppList, *m_pzsEName ) )
            {
               SetSel( iRelativeNbr, TRUE );
               if ( nTopSel == -1 )
                  nTopSel = (zSHORT) m_lEntityCnt;
            }

            if ( m_nDisplaySelSet )
               SetSelectSetForView( m_vAppList, m_nDisplaySelSet );
         }
         else
         if ( nTopSel == -1 )
         {
            if ( GetSelectStateOfEntity( m_vAppList, *m_pzsEName ) )
            {
               SetCurSel( m_lEntityCnt );
               nTopSel = (zSHORT) m_lEntityCnt;
            }
         }

         iRelativeNbr++;
         m_lEntityCnt++;
         nRC = SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                                m_ulFlag | zPOS_NEXT, 0, 0, 0, 0,
                                *m_pzsScope, 0 );

      }  // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru fields


      if ( m_chHilitePosition == 'Y' && m_vApp && m_lEntityCnt ) // fix GetRel for SCOPE_OI
 //        CheckExistenceOfEntity( m_vApp, *m_pzsEName ) == zCURSOR_SET )
      {
         zLONG lOffset = GetRelativeEntityNumber( m_vApp, *m_pzsEName, *m_pzsScope, m_ulFlag );

         if ( lOffset < zCURSOR_SET && lOffset > zCALL_ERROR )
         {
            SetEntityCursor( m_vApp, *m_pzsEName, 0,
                             m_ulFlag | zPOS_PREV, 0, 0, 0, 0,
                             *m_pzsScope, 0 );
            lOffset = GetRelativeEntityNumber( m_vApp, *m_pzsEName, *m_pzsScope, m_ulFlag );
         }

         if ( lOffset >= 0 )
         {
            SetTopIndex( lOffset );
            HiliteText( TRUE, -1 );
         }

//       TraceLineI( "MapFromOI TopIndex set to ", lOffset );
      }
      else
#if 0    // we will not ask the list box to re-establish position
      {
         if ( nTopSel >= 0 )
            lTop = nTopSel;

         if ( lTop < m_lEntityCnt )
         {
            {
               SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                zPOS_FIRST | zPOS_RELATIVE |
                                      zSPARENT_KEEPCHILDREN | m_ulFlag,
                                0, 0, 0, lTop, *m_pzsScope, 0 );
            }

            SetTopIndex( lTop );
         }
      }
#endif

      if ( lTop < m_lEntityCnt )
      {
         SetTopIndex( lTop );
      }

      break;  // quit after the first time thru
   }

   SetSelectSetForView( m_vApp, nPrevSelSet );
   m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;

   // Repaint changes
// UnlockWindowUpdate();
   Invalidate( );
   SetRedraw( TRUE );

#ifdef zREMOTE_SERVER
   // Transfer data to KZXRA.
   MapToRemoteDataset( );
#endif

#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::MapFromOI term ", *m_pzsTag );
   TraceLineI( "ZListBox::MapFromOI EntityCnt ", m_lEntityCnt );
#endif

   return( 0 );
}

zSHORT
ZListBox::MapToOI( zLONG /* lFlag */ )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::MapToOI ", *m_pzsTag );
#endif

// Disable Delta Data implementation.
#ifdef zREMOTE_SERVER
   MapToRemoteDataset();
#endif
// Disable Delta Data implementation.

   return( 0 );
}

zLONG
ZListBox::GetZCtrlState( zLONG lStatusType )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::GetZCtrlState ", *m_pzsTag );
#endif
   if ( lStatusType == zCONTROL_STATUS_ENTER )
      return( TRUE );
   else
#ifdef zREMOTE_SERVER
   if ( lStatusType == zCONTROL_STATUS_SELECT_INDEX )
   {
      return( -1 );
   }
   else
#endif
      return( ZMapAct::GetZCtrlState( lStatusType ) );
}

zLONG
ZListBox::SetZCtrlState( zLONG  lStatusType,
                         zULONG ulValue )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::SetZCtrlState ", *m_pzsTag );
#endif

#ifdef zREMOTE_SERVER

   if ( lStatusType == zCONTROL_STATUS_SELECT_INDEX )
   {
      ProcessCommand( zLBN_SELCHANGE, ulValue );
      return( 0 );
   }

#endif

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
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
ZListBox::HiliteText( zBOOL bHilite,
                      zLONG lType,
                      zLONG lRange,
                      zBOOL bHiliteItem )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::Hilite type: ", lType );
#endif

// zVIEW  m_vApp;

// if ( m_pchListInfo && *m_pzsVName &&
//      GetViewByName( &m_vApp, m_csViewName,
//                     m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
   if ( m_pchListInfo && m_vApp )
   {
      zLONG  lOffset;
      zLONG  lControl;

      SetRedraw( FALSE );
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

            if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                  m_ulFlag | lControl, 0, 0, 0,
                                  lType, *m_pzsScope, 0 ) < zCURSOR_SET )
            {
               break;   // since we could not set the cursor ... nothing to do
            }

            lOffset = lType;
         }
         else
         {
            if ( lType == -1 )   // current cursor position
            {
//             if ( CheckExistenceOfEntity( m_vApp, *m_pzsEName ) < zCURSOR_SET )
               if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                     zPOS_FIRST | zTEST_CSR_RESULT | zSPARENT_KEEPCHILDREN | m_ulFlag,
                                     0, 0, 0, 0, *m_pzsScope, 0 ) < zCURSOR_SET )
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
                  lOffset = -1;
                  SetSel( lOffset, FALSE );  // all OFF
               }

               if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                     zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN | m_ulFlag,
                                     0, 0, 0, 0, *m_pzsScope, 0 ) < 0 )
               {
                  if ( m_chMultiSelect == 0 )
                  {
                     SetCurSel( -1 );
                     if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                          (m_pzsScope == 0 || **m_pzsScope == 0) )
                        SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
                     else
                        SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, *m_pzsScope );
                  }

                  break;   // nothing to do
               }
            }
            else
            if ( lType == -3 && m_chMultiSelect )   // select or deselect all entity(s)
            {
               lOffset = -1;
               SetSel( lOffset, bHilite );  // all ON or all OFF
               break;
            }
            else
               break;   // nothing to do

            if ( m_nDisplaySelSet )
               SetSelectSetForView( m_vApp, m_nDisplaySelSet );

            lOffset = GetRelativeEntityNumber( m_vApp, *m_pzsEName, *m_pzsScope, m_ulFlag );
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
                        HiliteSelectedEntity( m_vApp, *m_pzsEName, *m_pzsScope );

                     SetSelectSetForView( m_vApp, m_nSelectSelSet );
                  }
                  else
                     HiliteSelectedEntity( m_vApp, *m_pzsEName, *m_pzsScope );

                  if ( SetEntityCursor( m_vApp, *m_pzsEName, 0,
                                        m_ulFlag | zPOS_NEXT | zQUAL_SELECTED,
                                        0, 0, 0, 0, *m_pzsScope, 0 ) >= 0 )
                  {
                     lOffset = 1;
                  }
                  else
                     lOffset = -1;
               }
            }
            else
            {
               if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                    (m_pzsScope == 0 || **m_pzsScope == 0) )
                  SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
               else
                  SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, *m_pzsScope );

               SetSelectStateOfEntity( m_vApp, *m_pzsEName, bHilite );
               HiliteSelectedEntity( m_vApp, *m_pzsEName, *m_pzsScope );
            }
         }
         else
         {
            if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                 (m_pzsScope == 0 || **m_pzsScope == 0) )
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
            else
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE, *m_pzsScope );

            if ( bHilite )
            {
               SetSelectStateOfEntity( m_vApp, *m_pzsEName, TRUE );
               HiliteSelectedEntity( m_vApp, *m_pzsEName, *m_pzsScope );
            }
            else
               SetCurSel( -1 );
         }

      } while ( FALSE );    // end of: purist's goto

      SetSelectSetForView( m_vApp, nPrevSelSet );
      SetRedraw( TRUE );
   }
}

void
ZListBox::HiliteSelectedEntity( zVIEW   vApp,
                                zCPCHAR cpcEName,
                                zCPCHAR cpcScope )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::HiliteSelectedEntity ", *m_pzsTag );
#endif

   zLONG lOffset = -1;

   if ( m_nDisplaySelSet )
   {
      zSHORT nPrevSelSet = SetSelectSetForView( vApp, m_nSelectSelSet );
      if ( SetEntityCursor( vApp, cpcEName, 0,
                            zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN | m_ulFlag,
                            0, 0, 0, 0, cpcScope, 0 ) >= 0 )
      {
         while ( TRUE )
         {
            SetSelectSetForView( vApp, m_nDisplaySelSet );
            if ( GetSelectStateOfEntity( vApp, cpcEName ) )
            {
               lOffset = GetRelativeEntityNumber( vApp, cpcEName, cpcScope, m_ulFlag | zQUAL_SELECTED );
               break;
            }

            SetSelectSetForView( vApp, m_nSelectSelSet );
            if ( SetEntityCursor( vApp, cpcEName, 0,
                                  m_ulFlag | zPOS_NEXT | zQUAL_SELECTED,
                                  0, 0, 0, 0, cpcScope, 0 ) < zCURSOR_SET )
            {
               break;
            }
         }
      }

      SetSelectSetForView( vApp, nPrevSelSet );
   }
   else
   {
      lOffset = GetRelativeEntityNumber( vApp, cpcEName, cpcScope, m_ulFlag );
   }

#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::HiliteSelectedEntity ", lOffset );
#endif

   if ( m_chMultiSelect )
      SetSel( lOffset, TRUE );
   else
      SetCurSel( lOffset );
}

zLONG
ZListBox::SelectAll( )
{
// TraceLine( "ZListBox::SelectAll (Multi=%c): %s", m_chMultiSelect, *m_pzsTag );
   if ( m_chMultiSelect == 'Y' )
   {
      zSHORT nPrevSelSet = SetSelectSetForView( m_vApp, m_nSelectSelSet );

      if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
           (m_pzsScope == 0 || **m_pzsScope == 0) )
      {
         SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, TRUE, (zCPCHAR) zSCOPE_OI );
      }
      else
      {
         SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, TRUE, *m_pzsScope );
      }

      SetSelectSetForView( m_vApp, nPrevSelSet );
      HiliteText( TRUE, -3 );
   // MapFromOI( );
      return( m_lEntityCnt );
   }
   else
      return( 1 );
}

void
ZListBox::ProcessCommand( zUSHORT uNotificationCode, zLONG lSelectedIdx )
{
   if ( m_ulMapActFlag2 & zMAPACT_MAPPING_FROM_OI )
      return;

   zVIEW vApp;

#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::ProcessCommand notification ", uNotificationCode );
#endif

   // the painter must ensure Entity/Attribute if ViewName???
   if ( m_pchListInfo && m_pzsVName )
   {
      GetViewByName( &vApp, m_csViewName,
                     m_pZSubtask->m_vDialog, zLEVEL_SUBTASK );
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

         if ( SetEntityCursor( vApp, *m_pzsEName, 0,
                               m_ulFlag | zPOS_FIRST | zSPARENT_KEEPCHILDREN,
                               0, 0, 0, 0, *m_pzsScope,
                               0 ) > zCURSOR_UNCHANGED )
         {
            int    iRelIdx; // this must be int for use in API call
            zLONG  lSelCnt;
            zSHORT nSelSet;

            nSelSet = SetSelectSetForView( vApp, m_nSelectSelSet );
            if ( nPrevSelSet == 0 )
               nPrevSelSet = nSelSet;

#ifdef zREMOTE_SERVER
            if ( lSelectedIdx != -1 )
#endif
            {
               if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                    (m_pzsScope == 0 || **m_pzsScope == 0) )
               {
                  SetAllSelectStatesForEntity( vApp, *m_pzsEName, FALSE, (zCPCHAR) zSCOPE_OI );
               }
               else
                  SetAllSelectStatesForEntity( vApp, *m_pzsEName, FALSE, *m_pzsScope );
            }

            if ( m_chMultiSelect )
            {
#ifdef zREMOTE_SERVER
               lSelCnt = SetMultiSelection();
               iRelIdx = lSelectedIdx;
#else
               lSelCnt = GetSelCount( );
               if ( lSelCnt == 1 )
                  GetSelItems( 1, &iRelIdx );
#endif
            }
            else
            {
               lSelCnt = 1;
#ifdef zREMOTE_SERVER
               ResetSelectionSet( FALSE );
               SetSel( lSelectedIdx, TRUE );
               iRelIdx = lSelectedIdx;
#else
               iRelIdx = GetCurSel( );
#endif
            }

#ifdef DEBUG_ALL
            TraceLineI( "OnListBoxPick or Enter ============== SelCnt ",
                        lSelCnt );
#endif
            if ( lSelCnt == 1 )
               SelectHilitedEntity( vApp, *m_pzsEName, *m_pzsScope, iRelIdx );
            else
            if ( lSelCnt > 1 )
            {
               int *piArray = new int[ lSelCnt ]; // this must be int for use in API call

               lSelCnt = GetSelItems( lSelCnt, piArray );
               for ( iRelIdx = 0; iRelIdx < lSelCnt; iRelIdx++ )
               {
                  SelectHilitedEntity( vApp, *m_pzsEName,
                                       *m_pzsScope, piArray[ iRelIdx ] );
               }

               delete [] piArray;
            }

            //if ( nSelCnt != m_nSelCnt )       // we need to do this all the
            //{                                 // time since the number
               lSelCnt = lSelCnt ? 1 : 0;       // selected may not change,
               m_pZSubtask->AutoGrayActions( ); // but AutoGray may.
            //}
         }

         SetSelectSetForView( vApp, nPrevSelSet );
         if ( m_chMultiSelect )
         {
            LRESULT lrRelIdx = GetCaretIndex( );
            if ( lrRelIdx >= 0 )
            {
               SetEntityCursor( vApp, *m_pzsEName, 0,
                                zPOS_FIRST | zPOS_RELATIVE | zSPARENT_KEEPCHILDREN | m_ulFlag,
                                0, 0, 0, lrRelIdx, *m_pzsScope, 0 );
            }
         }
      }
   }
}

zBOOL
ZListBox::HandleQuickKey( zACCEL_DEF ad )
{
   if ( ad.uVK == VK_RETURN && ad.fAlt == 0 && ad.fCtrl == 0 )
   {
      ProcessEvent( this, zLBN_DBLCLK );
      return( TRUE );
   }

   return( FALSE );
}

zLONG
ZListBox::HandleEvent( zLONG lEvent )
{
#ifdef zREMOTE_SERVER
   // The "ButtonClick" event is the only event we get from the LightDriver.
   // Here we mock the behavior of the Windows ListBox control by permitting
   // the ButtonClick to be prevented.  Note that the ListBox events are
   // Registered in the PPE so that we get the events whether there is
   // an associated action or not.
// if ( lEvent == 10 || lEvent == 11 )
   if ( lEvent < 0 )
   {
      zSHORT nCol = (zSHORT) -lEvent;

      // Sort data and then Map from OI.
      zLONG lRC =
      ProcessImmediateEvent( this, zLISTBOX_LButtonClickPreSort,
                             (zPVOID) m_pCol[ nCol ].pchTag );
      if ( lRC == zNO_APPLICATION_EVENT || (zSHORT) zLOUSHORT( lRC ) >= 0 )
      {
         if ( m_bAutoSort )
            SortByColumn( nCol, -1 );

         ProcessImmediateEvent( this, zLISTBOX_LButtonClick,
                                (zPVOID) m_pCol[ nCol ].pchTag );
      }

      MapFromOI( );
   }
#endif

   return( 0 );
}

#ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// nColumn     - The number of the column on which to sort
//               (0 ==> resort using the current column)
// nAscending  - -1 ==> invert current sort order
//                0 ==> retain current sort order
//                1 ==> sort ascending
//                2 ==> sort descending
void
ZListBox::SortByColumn( zSHORT nCol, zSHORT nAscending )
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
      nLth = zstrlen( szBuffer );
      szBuffer[ nLth++ ] = '.';
      strcpy_s( szBuffer + nLth, sizeof( szBuffer ) - nLth, m_pCol[ nCol ].pchAttrib );
      nLth = zstrlen( szBuffer );
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
         nLth = zstrlen( szBuffer );
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

   // m_SortedHeaderCtrl.SetSortImage( nCol,
   //                                  m_chAscendDescend == 'A' ?
   //                                                         TRUE : FALSE );
   // if ( m_bRememberSort == FALSE )
         OrderEntityForView( m_vApp, *m_pzsEName, m_csSortBuffer );

      MapFromOI( );
   }
}

#endif  // #ifdef zREMOTE_SERVER

zSHORT
ZListBox::SetCtrlMapping( zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                          zCPCHAR cpcAttrib, zCPCHAR cpcContext,
                          zLONG lColumn )
{
   if ( cpcViewName && *cpcViewName )
      *m_pzsVName = cpcViewName;

   if ( lColumn > 0 && lColumn <= m_nCols )
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
      lLth = zstrlen( cpcContext ) + 1;
      m_pCol[ lColumn ].pchContext = new char[ lLth ];
      strcpy_s( m_pCol[ lColumn ].pchContext, lLth, cpcContext );
   }

   return( 0 );
}

zSHORT
ZListBox::GetCtrlMapping( zPCHAR pchViewName, zPCHAR pchEntity,
                          zPCHAR pchAttrib, zPCHAR pchContext, zLONG lColumn )
{
   if ( lColumn == 0 )
   {
      ZMapAct::GetCtrlMapping( pchViewName, pchEntity, pchAttrib, pchContext, lColumn );
   }
   else
   {
      if ( m_pzsVName )
         strcpy_s( pchViewName, zTAG_LTH, *m_pzsVName );
      else
         pchViewName[ 0 ] = 0;

      if ( lColumn > 0 && lColumn <= m_nCols )
      {
         strcpy_s( pchEntity, zTAG_LTH, m_pCol[ lColumn ].pchEntity );
         strcpy_s( pchAttrib, zTAG_LTH, m_pCol[ lColumn ].pchAttrib );
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

void
ZListBox::LBNDblclk( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::LBNDblclk ", *m_pzsTag );
#endif
// Default( );              // pass it along to parent

   ProcessCommand( zLBN_DBLCLK );
   ProcessEvent( this, zLBN_DBLCLK );
}

void
ZListBox::LBNErrspace( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::LBNErrspace ", *m_pzsTag );
#endif
   Default( );              // pass it along to parent
   ProcessEvent( this, zLBN_ERRSPACE );
}

void
ZListBox::LBNSelcancel( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::LBNSelcancel ", *m_pzsTag );
#endif
   Default( );              // pass it along to parent
   ProcessEvent( this, zLBN_SELCANCEL );
}

void
ZListBox::LBNSelchange( )
{
   static zSHORT nRecurse = 0;   //???
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::LBNSelchange nRecurse ", nRecurse );
#endif
   if ( nRecurse )
   {
      TraceLineI( "ZListBox::LBNSelchange nRecurse ", nRecurse );
      return;
   }

   nRecurse++;

// Default( );              // pass it along to parent

   ProcessCommand( zLBN_SELCHANGE );
   ProcessEvent( this, zLBN_SELCHANGE );
   nRecurse--;
}

void
ZListBox::LBNSetFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::LBNSetFocus ", *m_pzsTag );
#endif
   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
      Default( );              // pass it along to parent
      ProcessEvent( this, zLBN_SETFOCUS );
   }
}

void
ZListBox::LBNKillFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::LBNKillFocus ", *m_pzsTag );
#endif

   m_pZSubtask->InformDriverOfSetFocus( this );
   Default( );              // pass it along to parent
   ProcessEvent( this, zLBN_KILLFOCUS );
}

int
ZListBox::CompareItem( LPCOMPAREITEMSTRUCT pCompareInfo )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::CompareItem ", *m_pzsTag );
#endif
   return( pCompareInfo->itemID1 > pCompareInfo->itemID2 ? 1 : -1 );
}

void
ZListBox::MeasureItem( LPMEASUREITEMSTRUCT pMeasureInfo )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::MeasureItem ", *m_pzsTag );
#endif
   if ( m_lCharHeight == 0 )
   {
      CClientDC hDC( this );
      CSize size = hDC.GetTextExtent( "1234567890", 10 );
      m_lCharHeight = size.cy;

//    TraceLineI( "ZListBox::MeasureItem height ", m_lCharHeight );
   }

// TraceLineI( "ZListBox::MeasureItem ", measure.itemID );
   pMeasureInfo->itemHeight = m_lCharHeight;

   if ( m_lCharWidth == 0 )
   {
      // Loop processing each column.
      for ( zSHORT nCol = 1; nCol <= m_nCols; nCol++ )
         m_lCharWidth += m_pCol[ nCol ].nExtent;
   }

   pMeasureInfo->itemWidth = m_lCharWidth;
// TraceLineI( "ZListBox::MeasureItem width ", m_lCharWidth );
}

void
ZListBox::DrawItem( LPDRAWITEMSTRUCT pDrawInfo )
{
   COLORREF lTextColor;
   COLORREF lBackColor;
   zPCHAR   pchMem;

   pchMem = (zPCHAR) GetItemData( pDrawInfo->itemID );
   if ( pchMem == 0 || pchMem == (zPCHAR) -1 )
      return;

// if ( pchMem[ 0 ] == 'D' && pchMem[ 1 ] == 'K' && pchMem[ 2 ] == 'S' )
//    TraceLineS( "DrawItem Value ", pchMem );

   if ( (pDrawInfo->itemAction & ODA_SELECT) == ODA_SELECT )
   {
      zSHORT nPrevSelSet;

      // set the select state of the entity
      SetEntityCursor( m_vAppList, *m_pzsEName, 0,
                       m_ulFlag | zPOS_FIRST | zPOS_RELATIVE,
                       0, 0, 0, pDrawInfo->itemID, *m_pzsScope, 0 );

//    TraceLineS( "ZListBox::DrawItem setting select state ",
//                (pDrawInfo->itemState & ODS_SELECTED) == ODS_SELECTED ?
//                                                       "TRUE" : "FALSE" );
//    TraceLineS( "   for Entity ", *m_pzsEName );
//    TraceLineS( "   for Scope ",
//                m_pzsScope ? *m_pzsScope : "(none)" );

      nPrevSelSet = SetSelectSetForView( m_vAppList, m_nSelectSelSet );
      SetSelectStateOfEntity( m_vAppList, *m_pzsEName,
            (pDrawInfo->itemState & ODS_SELECTED) == ODS_SELECTED ? TRUE : FALSE );

      SetSelectSetForView( m_vAppList, nPrevSelSet );
   }

#if 0 //def DEBUG_ALL lots of messages
   TraceLineI( "ZListBox::DrawItem ", pDrawInfo->itemID );
   TraceLineS( " ----------- value ", pchMem );

   if ( (pDrawInfo->itemAction & ODA_DRAWENTIRE) == ODA_DRAWENTIRE )
      TraceLineS( "ZListBox::DrawItem Action ", "ODA_DRAWENTIRE" );

   if ( (pDrawInfo->itemAction & ODA_SELECT) == ODA_SELECT )
      TraceLineS( "ZListBox::DrawItem Action ", "ODA_SELECT" );

   if ( (pDrawInfo->itemAction & ODA_FOCUS) == ODA_FOCUS )
      TraceLineS( "ZListBox::DrawItem Action ", "ODA_FOCUS" );

   if ( (pDrawInfo->itemState & ODA_SELECT) == ODA_SELECT )
      TraceLineS( "ZListBox::DrawItem State ", "ODA_SELECT" );

   if ( (pDrawInfo->itemState & ODS_SELECTED) == ODS_SELECTED )
      TraceLineS( "ZListBox::DrawItem State ", "ODS_SELECTED" );

   if ( (pDrawInfo->itemState & ODS_GRAYED) == ODS_GRAYED )
      TraceLineS( "ZListBox::DrawItem State ", "ODS_GRAYED" );

   if ( (pDrawInfo->itemState & ODS_DISABLED) == ODS_DISABLED )
      TraceLineS( "ZListBox::DrawItem State ", "ODS_DISABLED" );

   if ( (pDrawInfo->itemState & ODS_CHECKED) == ODS_CHECKED )
      TraceLineS( "ZListBox::DrawItem State ", "ODS_CHECKED" );

   if ( (pDrawInfo->itemState & ODS_FOCUS) == ODS_FOCUS )
      TraceLineS( "ZListBox::DrawItem State ", "ODS_FOCUS" );
#endif

   // If ODA_SELECT, text is to be hilited.  Otherwise, use default colors.
// if ( pDrawInfo->itemAction == ODA_SELECT &&
   if ( (pDrawInfo->itemState & ODS_SELECTED) == ODS_SELECTED )
   {
      if ( (pDrawInfo->itemState & ODS_GRAYED) == ODS_GRAYED ||
           (pDrawInfo->itemState & ODS_DISABLED) == ODS_DISABLED )
         lTextColor = COLOR_GRAYTEXT;
      else
         lTextColor = COLOR_HIGHLIGHTTEXT;

      lBackColor = COLOR_HIGHLIGHT;
   }
   else
// if ( pDrawInfo->itemAction == ODA_DRAWENTIRE ||
//      pDrawInfo->itemAction == ODA_FOCUS )
   {
      if ( (pDrawInfo->itemState & ODS_GRAYED) == ODS_GRAYED ||
           (pDrawInfo->itemState & ODS_DISABLED) == ODS_DISABLED )
         lTextColor = COLOR_GRAYTEXT;
      else
         lTextColor = COLOR_WINDOWTEXT;

      lBackColor = COLOR_WINDOW;    // SYSCLR_WINDOW;
   }

   COLORREF colorText = ::SetTextColor( pDrawInfo->hDC,
                                        ::GetSysColor( lTextColor ) );
   COLORREF colorBack = ::SetBkColor( pDrawInfo->hDC,
                                      ::GetSysColor( lBackColor ) );
   HBRUSH hTextBrush = ::CreateSolidBrush( ::GetSysColor( lTextColor ) );
   HBRUSH hBackBrush = ::CreateSolidBrush( ::GetSysColor( lBackColor ) );

   HBRUSH hLiteBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNFACE ) );
   HBRUSH hDarkBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNSHADOW ) );
   HBRUSH hWhiteBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) );


   CRect  rect;

   rect.left = pDrawInfo->rcItem.left;
   rect.right = pDrawInfo->rcItem.right;
   rect.top = pDrawInfo->rcItem.top;
   rect.bottom = pDrawInfo->rcItem.bottom;

   // Clear the rectangle.
// TraceRect( "DrawItem FillRect", rect );
   ::FillRect( pDrawInfo->hDC, &rect, hBackBrush );

   rect.left = pDrawInfo->rcItem.left;
   rect.right = pDrawInfo->rcItem.right;
   rect.top = pDrawInfo->rcItem.top;
   rect.bottom = pDrawInfo->rcItem.bottom;

   zPCHAR  pchCol = pchMem;
   zPCHAR  pchNextCol;
   zSHORT  nCol;

   // Loop processing each column.
   for ( nCol = 1; pchCol && nCol <= m_nCols; nCol++ )
   {
      // Set the left side of the rectangle for the current column.
      rect.left = m_pCol[ nCol ].nTabPos;

      // Null terminate the column.
      if ( (pchNextCol = zstrchr( pchCol, '\t' )) != 0 )
         *pchNextCol++ = 0;

      // Calculate the right side of the rectangle for this column.  Note
      // that a fudge factor of 8 prevents the columns from overlapping.
      rect.right = rect.left + m_pCol[ nCol ].nExtent - 8;

//    if ( pchMem[ 0 ] == 'D' && pchMem[ 1 ] == 'K' && pchMem[ 2 ] == 'S' )
//    {
//       TraceLineS( "DrawItem: ", pchCol );
//       TraceRect( "DrawItem column", rect );
//    }

      UINT uFormat = DT_TOP | DT_NOPREFIX;

      // Draw the text either center, left or right justified.
      if ( m_pCol[ nCol ].nJustify == 0 )
         uFormat |= DT_LEFT;
      else
      if ( m_pCol[ nCol ].nJustify == 1 )
         uFormat |= DT_RIGHT;
      else
//    if ( m_pCol[ nCol ].nJustify == 2 )
         uFormat |= DT_CENTER;

      ::DrawText( pDrawInfo->hDC, pchCol, zstrlen( pchCol ), &rect, uFormat );

      // Redraw the left side of the listbox since the draw text may
      // have drawn over it.
      rect.left = 0;  rect.right = 2;  // three is hardcoded above
      ::FillRect( pDrawInfo->hDC, &rect, hBackBrush ); // SYSCLR_SHADOW );
      rect.left = 3;  rect.right = 3;
      ::FillRect( pDrawInfo->hDC, &rect, hTextBrush );

      // If specified, draw a grid.
//x   if ( TRUE )
//x   {
//x      ::DrawBorder( pDrawInfo->hDC, &rect, 1, 1, DB_STANDARD );
//x   }

      // Get the next column.
      pchCol = pchNextCol;
      if ( pchNextCol )
         pchNextCol[ -1 ] = '\t';
   }

   rect.left = pDrawInfo->rcItem.left;
   rect.right = pDrawInfo->rcItem.right;
   rect.top = pDrawInfo->rcItem.top;
   rect.bottom = pDrawInfo->rcItem.bottom;

// if ( (pDrawInfo->itemState & ODS_SELECTED) == ODS_SELECTED &&
   if ( (pDrawInfo->itemState & ODS_FOCUS) == ODS_FOCUS )
   {
      ::DrawFocusRect( pDrawInfo->hDC, &rect );
   }

#if 0
   zSHORT k;

   GetClientRect( rect );

   if ( pDrawInfo->itemID == 0 && pDrawInfo->rcItem.top == 0 )
   {
      k = rect.bottom;
      rect.bottom = rect.top + 2;
      ::FillRect( pDrawInfo->hDC, &rect, hDarkBrush );
      rect.bottom = k;
   }

#if 0
   k = rect.top;
   rect.top = rect.bottom - 1;
   ::FillRect( pDrawInfo->hDC, &rect, hWhiteBrush );
   rect.top = rect.bottom - 2;
   rect.bottom--;
   ::FillRect( pDrawInfo->hDC, &rect, hLiteBrush );
   rect.bottom++;
   rect.top = k;
#endif

   k = rect.right;
   rect.right = rect.left + 2;
   ::FillRect( pDrawInfo->hDC, &rect, hDarkBrush );
   rect.right = k;

#endif

   DeleteObject( hLiteBrush );
   DeleteObject( hDarkBrush );
   DeleteObject( hWhiteBrush );

   ::SetBkColor( pDrawInfo->hDC, colorBack );
   ::SetTextColor( pDrawInfo->hDC, colorText );
   DeleteObject( hTextBrush );
   DeleteObject( hBackBrush );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef zREMOTE_SERVER

BOOL
ZListBox::OnEraseBkgnd( CDC *pDC )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnEraseBkgnd ", *m_pzsTag );
#endif

// return( TRUE );   // the listbox will erase/paint itself
   return( CListBox::OnEraseBkgnd( pDC ) );
}

void
ZListBox::OnNcPaint( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnNcPaint ", *m_pzsTag );
#endif
#if 0
   CWindowDC dc( m_pZSubtask->m_pZView );
   CRect  rectOrig;
   CRect  rect;

   HBRUSH hLiteBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNFACE ) );
   HBRUSH hDarkBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_BTNSHADOW ) );
   HBRUSH hWhiteBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) );
   HBRUSH hBlackBrush = ::CreateSolidBrush( ::GetSysColor( COLOR_WINDOWTEXT ) );

   GetWindowRect( rect );

// TraceRect( "ZListBox::OnNcPaint", rect );

   CPoint pt( rect.left, rect.top );
   m_pZSubtask->m_pZView->ScreenToClient( &pt );
   rect.bottom += pt.y - rect.top;
   rect.right += pt.x - rect.left;
   rect.top = pt.y;
   rect.left = pt.x;

   rect.top -= 2;
   rect.left -= 2;
   rect.bottom += 2;
   rect.right += 2;
   rectOrig = rect;  // hold inflated rectangle

   // fill top two lines
   rect.bottom = rect.top + 1;
   ::FillRect( dc, &rect, hBlackBrush );
   rect.top++;
   rect.bottom++;
   rect.left++;
   rect.right--;
   ::FillRect( dc, &rect, hBlackBrush );

   // fill left two lines
   rect = rectOrig;
   rect.right = rect.left + 1;
   ::FillRect( dc, &rect, hBlackBrush );
   rect.left++;
   rect.right++;
   rect.top++;
   rect.bottom--;
   ::FillRect( dc, &rect, hBlackBrush );

   // fill right two lines
   rect = rectOrig;
   rect.left = rect.right - 1;
   ::FillRect( dc, &rect, hBlackBrush );
   rect.left--;
   rect.right--;
   rect.top++;
   rect.bottom--;
   ::FillRect( dc, &rect, hWhiteBrush );

   // fill bottom two lines
   rect = rectOrig;
   rect.bottom++;
   rect.top = rect.bottom - 1;
   ::FillRect( dc, &rect, hWhiteBrush );
   rect.top--;
   rect.bottom--;
   rect.left++;
   rect.right--;
   ::FillRect( dc, &rect, hBlackBrush );

   DeleteObject( hLiteBrush );
   DeleteObject( hDarkBrush );
   DeleteObject( hWhiteBrush );
   DeleteObject( hBlackBrush );
#endif

#ifdef DEBUG_ALL
   MSG& msg = AfxGetThreadState( )->m_lastSentMsg;
   HRGN hRgn = (HRGN) msg.wParam;
   RECT rect;
   int nRC = GetRgnBox( hRgn, &rect );
   if ( nRC == 0 )
      TraceLineI( " GetRgnBox INVALID REGION = ", nRC );
   else
   {
      TraceRect( "ZListBox::OnNcPaint Region:", rect );
      if ( nRC == NULLREGION )
         TraceLineI( " GetRgnBox NULLREGION = ", nRC );
      else
      if ( nRC == SIMPLEREGION )
         TraceLineI( " GetRgnBox SIMPLEREGION = ", nRC );
      else
      if ( nRC == COMPLEXREGION )
         TraceLineI( " GetRgnBox COMPLEXREGION = ", nRC );
   }
#endif

// if ( hRgn == (HRGN) 1 )
//    ValidateRect( 0 );

   CListBox::OnNcPaint( );
}

void
ZListBox::OnPaint( )
{
#if 0 //def DEBUG_ALL lots of messages
   TraceLineS( "ZListBox::OnPaint ", *m_pzsTag );

   CRect rect;
   GetUpdateRect( rect );
   TraceRect( "ZListBox::OnPaint update rect:", rect );
#endif

   CListBox::OnPaint( );
}

void
ZListBox::OnEnable( BOOL bEnable )
{
   if ( bEnable == FALSE && (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
      EnableWindow( );  // do not disable a readonly listbox ... 2006.08.28
}

#endif

void
ZListBox::SelectHilitedEntity( zVIEW   vApp,
                               zCPCHAR cpcEName,
                               zCPCHAR cpcScope,
                               zLONG   lRelIdx )
{
   zSHORT nPrevSelSet;

#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::SelectHilitedEntity lRelIdx ", lRelIdx );
#endif

   if ( m_nDisplaySelSet )
      nPrevSelSet = SetSelectSetForView( vApp, m_nDisplaySelSet );
   else
      nPrevSelSet = SetSelectSetForView( vApp, m_nSelectSelSet );

   if ( SetEntityCursor( vApp, cpcEName, 0,
                         zPOS_FIRST | zPOS_RELATIVE |
                              zSPARENT_KEEPCHILDREN | m_ulFlag,
                         0, 0, 0, lRelIdx, cpcScope, 0 ) >= 0 )
   {
      if ( m_nDisplaySelSet )
         SetSelectSetForView( vApp, m_nSelectSelSet );

      SetSelectStateOfEntity( vApp, cpcEName, TRUE );
   }

   SetSelectSetForView( vApp, nPrevSelSet );
}

#ifndef zREMOTE_SERVER

UINT
ZListBox::OnGetDlgCode( )
{
   return( DLGC_WANTALLKEYS );
// return( CListBox::OnGetDlgCode( ) );
}

void
ZListBox::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::OnKeyDown key ###############==>> ", uKey );
#endif
   CListBox::OnKeyDown( uKey, uRepeatCnt, uFlags );
}

void
ZListBox::OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::OnChar key ###############==>> ", uKey );
#endif

   zPCHAR pchMem;
   zLONG  k;
   zLONG  lNext;
   zCHAR  szMap[ 2 ];
   zCHAR  chLower = tolower( uKey );
   zCHAR  chUpper = toupper( uKey );

   szMap[ 0 ] = chUpper;
   szMap[ 1 ] = 0;

   k = GetCaretIndex( );
   lNext = k + 1;

   while ( lNext != k )
   {
      if ( lNext >= m_lEntityCnt )
         lNext = -1;
      else
      {
         if ( lNext >= 0 )
         {
            pchMem = (zPCHAR) GetItemData( lNext );
            if ( pchMem && pchMem != (zPCHAR) -1 &&
                 (*pchMem == chLower || *pchMem == chUpper) )
            {
               break;
            }
         }

         lNext++;
      }
   }

   if ( lNext != k )
   {
      if ( m_chMultiSelect )
      {
         SetCaretIndex( lNext, false );
      // SetSel( lNext, true );
      }
      else
         SetCurSel( lNext );

      if ( m_vApp )
      {
         SetEntityCursor( m_vApp, *m_pzsEName, 0,
                          m_ulFlag | zPOS_FIRST | zPOS_RELATIVE,
                          0, 0, 0, lNext, *m_pzsScope, 0 );
         if ( m_chMultiSelect == 0 )
         {
            if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                 (m_pzsScope == 0 || **m_pzsScope == 0) )
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName, FALSE,
                                            (zCPCHAR) zSCOPE_OI );
            else
               SetAllSelectStatesForEntity( m_vApp, *m_pzsEName,
                                            FALSE, *m_pzsScope );

            SetSelectStateOfEntity( m_vApp, *m_pzsEName, TRUE );
         }

         ProcessCommand( zLBN_SELCHANGE );
         ProcessEvent( this, zLBN_SELCHANGE );
      }
   }
}

void
ZListBox::OnRButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnRButtonDown ", *m_pzsTag );
#endif
   CListBox::OnRButtonDown( uModKeys, pt );  // let base class do its thing
   ProcessEvent( this, zLISTBOX_RButtonDown );
   if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) == 0 )
      return;

// m_pZSubtask->m_uMouseStatus = zMOUSE_CAPTURE_RIGHT;
// SetCapture( );
   m_ptMouseDown = pt;
   m_ptMouseUp = m_ptMouseDown;
}

void
ZListBox::OnRButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnRButtonUp ", *m_pzsTag );
#endif
   CRect rect;

   ProcessEvent( this, zLISTBOX_RButtonUp );

   GetClientRect( rect );
   if ( rect.PtInRect( pt ) )
   {
      ProcessEvent( this, zLISTBOX_RButtonClick );
      if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG_RIGHT) == zMOUSE_DRAG_RIGHT )
         m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_DO_DROP, 0,
                                             (zLONG)(ZMapAct *) this );
   }

// if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_RIGHT) == zMOUSE_CAPTURE_RIGHT )
//    ReleaseCapture( );

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
// Default( );              // pass it along to parent
}

void
ZListBox::OnRButtonDblClk( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnRButtonDblClk ", *m_pzsTag );
#endif
// Default( );              // pass it along to parent
   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
   ProcessEvent( this, zLISTBOX_RButtonDblclk );
}

#endif // #ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// To implement drag and drop, the selection logic becomes somewhat
// complicated.  Mouse button down plus mouse movement indicates
// the start of a drag operation.  Mouse button up is then used to
// do selection (rather than button down ... selection state cannot
// always be done on button-down events since the user may be trying
// to drag an existing selection set).
//
// The "drag source" flag must be activated for a specific list box
// in order for drag and drop processing to be initiated.  Otherwise
// standard selection processing is performed.  The parent window
// (as usual) controls the application level components of drag and
// drop including the load of two special drag and drop cursors.
//
// There are two new events on the listbox to support drag and drop:
//
//  - QueryDrop - determines if a potential drop site accepts a drop
//  - DoDrop    - drop has occurred at the drop site
//
// The clipboard could be used to pass data between the drag source and
// the drop target (this would be useful when implementing drag & drop
// to conform to existing protocols).  Zeidon drag & drop uses an internal
// mechanism to pass this data.  The data which a drop target receives is
// a Zeidon DragDrop structure.  All of the information in the structure
// is filled in by the driver when the drag begins.  The driver then offers
// the source control an opportunity to alter any information in the
// structure.  This structure is used in the QueryDrop and DoDrop functions.
// The memory for the structure is completely managed by the driver.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZListBox::GetSelectIdxFromPoint( CPoint pt )
{
#ifdef zREMOTE_SERVER
   return( LB_ERR );
#else
   zLONG lTop = GetTopIndex( );
#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::GetSelectIdxFromPoint TopIndex ", lTop );
#endif
   if ( lTop == LB_ERR )
   {
      return( LB_ERR );    // no items to select
   }

   zLONG  lHeight = GetItemHeight( 0 );
   zLONG  lSelIdx = lTop + (pt.y / lHeight);
   zLONG  lCount = GetCount( );
   if ( lSelIdx >= lCount )
      lSelIdx = LB_ERR;

#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::GetSelectIdxFromPoint ", lSelIdx );
#endif
   return( (zSHORT) lSelIdx );
#endif // zREMOTE_SERVER
}

void
ZListBox::UpdateSelection( zLONG   lSelection,
                           zUSHORT uFlags,
                           CPoint  pt )
{
#ifndef zREMOTE_SERVER

#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::UpdateSelection ", uFlags );
#endif
   if ( m_chMultiSelect )
   {
      if ( uFlags & MK_CONTROL )
      {
      }
      else
      if ( uFlags & MK_SHIFT )
      {
         // Deselect all items except this one
         SelItemRange( FALSE, 0, GetCount( ) - 1 );

         zSHORT nMin = min( m_nFirstIndex, (zSHORT) lSelection );
         zSHORT nMax = max( m_nFirstIndex, (zSHORT) lSelection );
         zVIEW  vApp;

         // Select all the items from the first to this one
         SelItemRange( TRUE, nMin, nMax );

         if ( m_pchListInfo && *m_pzsVName &&
              GetViewByName( &vApp, m_csViewName,
                             m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
         {
            zSHORT nPrevSelSet = SetSelectSetForView( vApp, m_nSelectSelSet );
            zLONG  lControl = 0;

            if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                 (m_pzsScope == 0 || **m_pzsScope == 0) )
            {
               SetAllSelectStatesForEntity( vApp, *m_pzsEName, FALSE,
                                            (zCPCHAR) zSCOPE_OI );
            }
            else
               SetAllSelectStatesForEntity( vApp, *m_pzsEName,
                                            FALSE, *m_pzsScope );

            if ( m_nDisplaySelSet )
            {
               lControl = zQUAL_SELECTED;
               SetSelectSetForView( vApp, m_nDisplaySelSet );
            }
            else
               SetSelectSetForView( vApp, nPrevSelSet );

            if ( SetEntityCursor( vApp, *m_pzsEName, 0,
                                  zPOS_FIRST | zPOS_RELATIVE | m_ulFlag |
                                       zSPARENT_KEEPCHILDREN | lControl,
                                  0, 0, 0, nMin,
                                  *m_pzsScope, 0 ) >= zCURSOR_SET )
            {
               while ( nMin++ <= nMax )
               {
                  SetSelectSetForView( vApp, m_nSelectSelSet );
                  SetSelectStateOfEntity( vApp, *m_pzsEName, TRUE );
                  if ( m_nDisplaySelSet )
                     SetSelectSetForView( vApp, m_nDisplaySelSet );
                  else
                     SetSelectSetForView( vApp, nPrevSelSet );

                  if ( SetEntityCursor( vApp, *m_pzsEName, 0,
                                        zPOS_NEXT | lControl | m_ulFlag,
                                        0, 0, 0, 0,
                                        *m_pzsScope, 0 ) < zCURSOR_SET )
                  {
                     break;
                  }
               }
            }

            SetSelectSetForView( vApp, nPrevSelSet );
         }
      }
      else
      {
         // Deselect all items except this one
         SelItemRange( FALSE, 0, GetCount( ) - 1 );
         SetSel( lSelection, TRUE );

         // Save this item index as the first of a possible range
         m_nFirstIndex = (zSHORT) lSelection;
      }
   }
   else
   {
      // Select this one
      SetCurSel( lSelection );
   }

#endif   // #ifndef zREMOTE_SERVER
}

///////////////////////////////////////////////////////////////////////////////////////////
#ifndef zREMOTE_SERVER

void
ZListBox::OnLButtonDown( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnLButtonDown ", *m_pzsTag );
#endif
   CListBox::OnLButtonDown( uModKeys, pt );  // let base class do its thing

   // Get the index of the item under the mouse (and store current position).
   m_ptMouseDown = pt;
   m_lSelSrc = GetSelectIdxFromPoint( m_ptMouseDown );

#ifdef DEBUG_ALL
   TraceLineI( "ZListBox::OnLButtonDown select index ", m_lSelSrc );
#endif

   // If not enabled as a drag source, return.
   if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) == 0 )
   {
#ifdef DEBUG_ALL
      TraceLineS( "Returning from LButtonDown ... not a drag source ",
                  *m_pzsTag );
#endif
      return;   // cannot be drag source control
   }

   if ( m_lSelSrc == LB_ERR )
   {
#ifdef DEBUG_ALL
      TraceLineS( "Returning from LButtonDown ... LB_ERR ", *m_pzsTag );
#endif
//    m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;  // enforced by other code
      return;
   }

#ifdef DEBUG_ALL
   TraceLineI( "MouseDownLeft selection = ", m_lSelSrc );
#endif
// SetCapture( m_hWnd );   // this is already done by the framework
   m_ptMouseUp = m_ptMouseDown;
   m_pZSubtask->m_uMouseStatus = zMOUSE_CAPTURE_LEFT;

   // If the item is already selected, defer selection processing until
   // mouse button up because this may be the start of a drag operation.
   if ( GetSel( m_lSelSrc ) )
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
      UpdateSelection( m_lSelSrc, uModKeys, m_ptMouseDown );
   }

   ProcessEvent( this, zLISTBOX_LButtonDown );
}

void
ZListBox::OnLButtonUp( UINT uModKeys, CPoint pt )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::OnLButtonUp ", *m_pzsTag );
   TraceLineI( "LeftMouseUp status ", m_pZSubtask->m_uMouseStatus );
#endif
   CListBox::OnLButtonUp( uModKeys, pt );  // let base class do its thing
   if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE_LEFT) == zMOUSE_CAPTURE_LEFT )
   {
//    ReleaseCapture( );      already done by OWL

      // If the mouse has been captured ... check for drop target.
      if ( (m_ulMapActFlags & zMAPACT_DRAG_ENABLED) &&
           (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) == zMOUSE_DRAG )
      {
//       if ( m_chMultiSelect == 0 &&
//            m_pchDragDrop && *m_pchDragDrop == 'B' &&
//            zstrcmp( m_pchDragDrop + (2 * sizeof( zLONG )),
//                     m_pchDragDrop + (2 * sizeof( zLONG )) +
//                    *((zLONG *) (m_pchDragDrop + sizeof( zLONG ))) ) == 0 )
         if ( m_bAutoDragSort &&
              (m_chMultiSelect == 0 ||
               (m_chMultiSelect && GetSelCount( ) == 1)) )
         {
            CRect rect;

            // Are we the target of our own drop?
            GetClientRect( rect );
            if ( rect.PtInRect( pt ) )
            {
               zVIEW vSrc;

               // If we cannot get the view ... quit.
               if ( GetViewByName( &vSrc, *m_pzsVName,
                                   m_pZSubtask->m_vDialog,
                                   zLEVEL_ANY ) > 0 )
               {
                  zVIEW vTgt;
                  int nSelSrc;

                  if ( m_chMultiSelect )
                     GetSelItems( 1, &nSelSrc );
                  else
                     nSelSrc = GetCurSel( );

#ifdef DEBUG_ALL
                  TraceLineI( "SelSrc = ", nSelSrc );
#endif
                  SetEntityCursor( vSrc, *m_pzsEName, 0,
                                   zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                   0, 0, 0, nSelSrc, *m_pzsScope, 0 );
//                SetCursorFirstSelectedEntity( vSrc, *m_pzsEName,
//                                              *m_pzsScope );

                  CreateViewFromViewForTask( &vTgt, vSrc,
                                             m_pZSubtask->m_vDialog );
                  zSHORT nSelTgt = GetSelectIdxFromPoint( pt );
#ifdef DEBUG_ALL
                  TraceLineI( "SelTgt = ", nSelTgt );
#endif
                  if ( nSelTgt == LB_ERR )
                  {
                     SetEntityCursor( vTgt, *m_pzsEName, 0,
                                      zPOS_LAST | m_ulFlag,
                                      0, 0, 0, 0, *m_pzsScope, 0 );
                  }
                  else
                  {
                     SetEntityCursor( vTgt, *m_pzsEName, 0,
                                      zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                      0, 0, 0, nSelTgt, *m_pzsScope, 0 );
                  }

                  MoveSubobject( vTgt, *m_pzsEName,
                                 vSrc, *m_pzsEName,
                                 zPOS_AFTER, zREPOS_NEXT );
                  DropView( vTgt );

                  zSHORT nPrevSelSet =
                               SetSelectSetForView( vSrc, m_nSelectSelSet );
                  if ( (m_ulFlag & zQUAL_SCOPE_OI) &&
                       (m_pzsScope == 0 || **m_pzsScope == 0) )
                     SetAllSelectStatesForEntity( vSrc, *m_pzsEName, FALSE,
                                                    (zCPCHAR) zSCOPE_OI );
                  else
                     SetAllSelectStatesForEntity( vSrc, *m_pzsEName,
                                                  FALSE, *m_pzsScope );

                  SetEntityCursor( vSrc, *m_pzsEName, 0,
                                   zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                                   0, 0, 0,
                                   nSelSrc > nSelTgt ? nSelSrc - 1 : nSelTgt,
                                   *m_pzsScope, 0 );
                  SetSelectStateOfEntity( vSrc, *m_pzsEName, TRUE );
                  SetSelectSetForView( vSrc, nPrevSelSet );
                  MapFromOI( );
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

         // We need to check the return from this call in order to know
         // if the drop was accepted or not ... the listbox doesn't care.
         m_pZSubtask->m_pZFWnd->SendMessage( zZEIDON_DO_DROP,
                                             uModKeys,
                                             (LPARAM)(ZMapAct *) this );
         pt.x = m_ptMouseDown.x;
         pt.y = m_ptMouseDown.y;
      }
      else
      if ( m_pZSubtask->m_uMouseStatus & zMOUSE_SELECT_PENDING )
      {
         m_ptMouseUp = pt;
         zSHORT nSelection = GetSelectIdxFromPoint( m_ptMouseUp );
         if ( nSelection != LB_ERR )
         {
            UpdateSelection( nSelection, uModKeys, m_ptMouseUp );
         }
      }
   }

   m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
// Default( );              // pass it along to parent
}

void
ZListBox::OnMouseMove( UINT uModKeys, CPoint pt )
{
// TraceLineS( "ZListBox::OnMouseMove: ", *m_pzsTag );
   if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_CAPTURE) == zMOUSE_CAPTURE )
   {
      CWnd *pWnd;

      if ( (pWnd = GetCapture( )) == 0 || pWnd->m_hWnd != m_hWnd )
         m_pZSubtask->m_uMouseStatus = zMOUSE_STATUS_OFF;
      else
      {
         m_ptMouseUp = pt;

         if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) != zMOUSE_DRAG )
         {
            // Has the mouse moved far enough to start a drag operation?
            if ( m_ptMouseUp.x - m_ptMouseDown.x > 4 ||
                 m_ptMouseUp.y - m_ptMouseDown.y > 4 ||
                 m_ptMouseDown.x - m_ptMouseUp.x > 4 ||
                 m_ptMouseDown.y - m_ptMouseUp.y > 4 )
            {
               m_pZSubtask->m_uMouseStatus |= zMOUSE_DRAG;
               m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_BEGIN_DRAG, uModKeys,
                                                   (LPARAM)(ZMapAct *) this );
            }
         }

         // Inform the parent that a drag operation is continuing.
         if ( (m_pZSubtask->m_uMouseStatus & zMOUSE_DRAG) == zMOUSE_DRAG )
         {
            m_pZSubtask->m_pZFWnd->PostMessage( zZEIDON_QUERY_DROP, uModKeys,
                                                (LPARAM)(ZMapAct *) this );
            return;
         }
      }
   }

   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

#endif // zREMOTE_SERVER

zBOOL
ZListBox::BeginDrag( ZDragDrop *pDragDrop )
{
   if ( m_chMultiSelect )
   {
      if ( GetSelCount( ) > 1 )
         pDragDrop->fCopyMultiple |= 0x10;
      else
         pDragDrop->fCopyMultiple &= ~0x10;
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
//
// WM_VSCROLL Message parameters:
// nScrollCode = (int) LOWORD( wParam );  // scroll bar value
// nPos = (short int) HIWORD( wParam );   // scroll box position
// hwndScrollBar = (HWND) lParam;         // handle of scroll bar
//
// Parameters:
//    nScrollCode - Value of the low-order word of wParam.  Specifies a
//       scroll bar value that indicates the user's scrolling request.
//       This parameter can be one of the following values:
//
//          Value             Meaning
//       SB_BOTTOM         Scrolls to the lower right.
//       SB_ENDSCROLL      Ends scroll.
//       SB_LINEDOWN       Scrolls one line down.
//       SB_LINEUP         Scrolls one line up.
//       SB_PAGEDOWN       Scrolls one page down.
//       SB_PAGEUP         Scrolls one page up.
//       SB_THUMBPOSITION  Scrolls to the absolute position.  The current
//                         position is specified by the nPos parameter.
//       SB_THUMBTRACK     Drags scroll box to the specified position. The
//                         current position is specified by the nPos parameter.
//       SB_TOP            Scrolls to the upper left.
//
//    nPos - Value of the high-order word of wParam.  Specifies the current
//       position of the scroll box if the nScrollCode parameter is
//       SB_THUMBPOSITION or SB_THUMBTRACK; otherwise, nPos is not used.
//
//    hwndScrollBar - Value of lParam.  If the message is sent by a scroll
//       bar, then hwndScrollBar is the handle of the scroll bar control.
//       If the message is not sent by a scroll bar, hwndScrollBar is NULL.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZListBox::QueryDrop( ZDragDrop *pDragDrop )
{
#ifdef zREMOTE_SERVER
   return( FALSE );
#else

#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::QueryDrop: ", *m_pzsTag );
   TracePoint( "ZListBox::QueryDrop ptDrop:", pDragDrop->ptDrop );
#endif

   if ( ZMapAct::QueryDrop( pDragDrop ) == FALSE &&
        m_bAutoDragSort == FALSE ||
        (m_bAutoDragSort && pDragDrop->pzmaSrc != pDragDrop->pzmaTgt) )
   {
      return( FALSE );
   }

   CRect rect;

   GetClientRect( rect );
// TraceRect( "ZListBox::QueryDrop ClientRect:", rect );
   zSHORT nItemHeight = GetItemHeight( 0 );
// TraceLineI( "ZListBox::QueryDrop ItemHeight: ", nItemHeight );
   if ( pDragDrop->ptDrop.y < rect.top + nItemHeight / 2 )
      SendMessage( WM_VSCROLL, MAKELONG( SB_LINEUP, 0 ), 0L );
   else
   if ( pDragDrop->ptDrop.y > rect.bottom - nItemHeight / 2 )
      SendMessage( WM_VSCROLL, MAKELONG( SB_LINEDOWN, 0 ), 0L );

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
#endif // zREMOTE_SERVER
}

// ZListBox - dtor
ZListBox::~ZListBox( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::~ZListBox ", *m_pzsTag );
   TraceLineI( "ZListBox::dtor EntityCnt ", m_lEntityCnt );
#endif

#ifndef zREMOTE_SERVER
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
#endif

   for ( zSHORT nCol = 1; nCol <= m_nCols; nCol++ )
   {
      mDeleteInitA( m_pCol[ nCol ].pchEntity );
      mDeleteInitA( m_pCol[ nCol ].pchAttrib );
      mDeleteInitA( m_pCol[ nCol ].pchContext );
      mDeleteInitA( m_pCol[ nCol ].pchTag );
      mDeleteInitA( m_pCol[ nCol ].pchText );
      mDeleteInitA( m_pCol[ nCol ].pchData );
      mDeleteInitA( m_pCol[ nCol ].pchMB_Text );
   }

   mDeleteInitA( m_pCol );
   mDeleteInitA( m_pchMap );
   mDeleteInitA( m_pchListInfo );
// mDeleteInitA( m_plCharWidth );
// mDeleteInit( m_pzsScope ); // please do not delete m_pzsScope cuz it's
                              // just an alias for m_pzsAName
}

/////////////////////////////////////////////////////////////////////////////
//
// Limits of List Box Controls
// Several factors limit a list box control:
// - The control structure of a list box and its tab stop settings
//   are allocated from the USER heap. If the USER heap does not have
//   enough free space for these structures, the list box will not be
//   able to operate.
//
// - In Microsoft® Windows™ version 3.0, the control structure is
//   66 bytes in length; in Windows version 3.1, it's 64 bytes in
//   length.
//
// - Windows allocates an integer to store the position of each tab
//   stop and an integer to store the number of tab stops.
//
// - List boxes store all the strings in the list box in one
//   globally allocated segment. Windows limits the total amount of
//   text in a list box to 64 kilobytes (K). If the average string
//   length in a list box is 100 bytes, for example, the list box
//   will be able to display about 650 strings. Space for strings in
//   a list box is allocated in multiples of 256 bytes.
//
//
// Avg string lth (including NULL terminator)   Maximum number of items
//
//                        15                          4352
//                        30                          2176
//                        45                          1450
//                        60                          1088
//                        90                           725
//
// - List boxes maintain information about each item in the list
//   box. Storage space for this information is globally allocated,
//   in multiples of 32 items, and limited to a total of 32K. The
//   following information is kept for each item in a list box:
//
// - A 32-bit data value
//
// - A 16-bit index into the string storage buffer for the start of
//   this string if the list box stores strings (that is, is not
//   owner-drawn or has the LBS_HASSTRINGS style)
//
// - 1 byte for the selected state if the list box has the
//   LBS_MULTIPLESEL or LBS_EXTENDEDSEL styles
//
// - 1 byte for the item height if the list box is a variable
//   height owner-drawn list box (LBS_OWNERDRAWVARIABLE style)
//
//
// No individual string item can be greater than 32K in length. The
// above values lead to the item limits in the following table.
//
// List box attributes     Maximum items   Average string length to get the
//                                         maximum number of items in the
//                                         list box (including NULL
//                                         terminator)
//
// "Default List Box"           5440              12
// LBS_EXTENDEDSEL*             4672              14
// LBS_OWNERDRAWFIXED |
//       LBS_HASSTRINGS         5440              12
// LBS_OWNERDRAWFIXED |
//       LBS_HASSTRINGS |
//       LBS_EXTENDEDSEL*       4672              14
// LBS_OWNERDRAWVARIABLE |
//       LBS_HASSTRINGS         4672              14
// LBS_OWNERDRAWVARIABLE |
//       LBS_HASSTRINGS |
//       LBS_EXTENDEDSEL*       4064              16
// LBS_OWNERDRAWFIXED           8160              N/A
// LBS_OWNERDRAWFIXED |
//       LBS_EXTENDEDSEL*       6528              N/A
// LBS_OWNERDRAWVARIABLE        6528              N/A
// LBS_OWNERDRAWVARIABLE |
//       LBS_EXTENDEDSEL*       5440              N/A
//
// * Or LBS_MULTIPLESEL
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef zREMOTE_SERVER

#if 0
void
ZListBox::ResetContent( )
{
   m_rmtDataSet.ResetContent( );
}

int
ZListBox::SetCurSel( zLONG lSelect )
{
   return( m_rmtDataSet.SetCurSel( nSelect ) );
}

int
ZListBox::SetTopIndex( int nSelect )
{
   return( m_rmtDataSet.SetTopIndex( nSelect ) );
}

int
ZListBox::SetSel( int nSelect, BOOL bSet )
{
   return( m_rmtDataSet.SetSel( nSelect, bSet ) );
}

int
ZListBox::AddString( LPCTSTR pchItem )
{
   return( m_rmtDataSet.AddString( pchItem ) );
}

void
ZListBox::SetRedraw( BOOL bRedraw )
{
   return;
}

BOOL
ZListBox::EnableWindow( BOOL bEnable )
{
   BOOL bHelper = m_brmtDisableState;
   m_brmtDisableState = bEnable;
   return( bHelper );
}

DWORD
ZListBox::GetItemData( int nIndex )
{
   return( m_rmtDataSet.GetItemData( nIndex ) );
}

int
ZListBox::SetItemData( int nIndex, DWORD dwItemData )
{
   return( m_rmtDataSet.SetItemData( nIndex, dwItemData ) );
}
#endif

//
// Get the "control's" data from ZRmtDataSet and store it
// for the remote control
//
void
ZListBox::MapToRemoteDataset( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZListBox::MapToRemoteDataset ", *m_pzsTag );
#endif

// Disable Delta Data implementation.
// // Merge RmtDataSet.
// MergeRowData( (m_pZSubtask->m_ulSubtaskFlags &
//                zSUBTASK_MAPPED_SINCE_APPLYSTATE) ? TRUE : FALSE );
// Disable Delta Data implementation.

   zVIEW  vXRA;
   zBOOL  bSelected = FALSE;

   if ( (vXRA = SetupRemoteObject( )) != 0 )
   {
      ZRmtDataRow *pDataRow = 0;
      CString csRowText;
      zLONG   lRelNbr = 0;
   // zULONG  ulState;
      zSHORT  nRC;

      if ( m_chMultiSelect )
         SetAttributeFromString( vXRA, cpcListBox, "MultiSelect", "Y" );

      // Delete all Selections.
      if ( CheckExistenceOfEntity( vXRA, cpcLBSelItemEntity ) == 0 )
      {
         nRC = SetCursorFirstEntity( vXRA, cpcLBSelItemEntity, 0 );
         while ( nRC >= zCURSOR_SET )
            nRC = DeleteEntity( vXRA, cpcLBSelItemEntity, zREPOS_FIRST );
      }

      // Delete all Data Rows.
      if ( CheckExistenceOfEntity( vXRA, cpcLBDataRowEntity ) == 0 )
      {
         nRC = SetCursorFirstEntity( vXRA, cpcLBDataRowEntity, 0 );
         while ( nRC >= zCURSOR_SET )
            nRC = DeleteEntity( vXRA, cpcLBDataRowEntity, zREPOS_FIRST );
      }

   // Disable Delta Data implementation.
   // if ( m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_MAPPED_SINCE_APPLYSTATE )
   //    SetAttributeFromString( vXRA, cpcListBox, "DeleteAllRows", "Y" );
   // Disable Delta Data implementation.

      POSITION pos = GetFirstDataRowPosition( );
// Disable Delta Data implementation.
#if 0
      while ( pos )
      {
         pDataRow = GetNextDataRow( pos );
         CreateEntity( vXRA, cpcLBDataRowEntity, zPOS_AFTER );
         ulState = pDataRow->GetState( );
         if ( (ulState & zRMT_CHANGED_KEY) == zRMT_CHANGED_KEY )
         {
            SetAttributeFromInteger( vXRA, cpcLBDataRowEntity, "Tag", pDataRow->GetKeyOrig( ) );
            SetAttributeFromInteger( vXRA, cpcLBDataRowEntity, "NewTag", pDataRow->GetKey( ) );
         }
         else
            SetAttributeFromInteger( vXRA, cpcLBDataRowEntity, "Tag", pDataRow->GetKey( ) );

         if ( (ulState & zRMT_DELETED) == zRMT_DELETED )
         {
            SetAttributeFromString( vXRA, cpcLBDataRowEntity, "Deleted", "Y" );
         }
         else
         {
            if ( ulState == 0 ||
                 (ulState & zRMT_CHANGED_ORDER) == zRMT_CHANGED_ORDER )
            {
               SetAttributeFromInteger( vXRA, cpcLBDataRowEntity, "Order", pDataRow->GetOrder( ) );
            }

            if ( ulState == 0 ||
                 (ulState & zRMT_CHANGED_DATA) == zRMT_CHANGED_DATA )
            {
               // Retrieve row data.
               csRowText = pDataRow->GetRowDataText( );
               SetAttributeFromString( vXRA, cpcLBDataRowEntity, "Text", csRowText );
            }
         }

         // If the row's select flag is set, create the SelectedItem Entity
         // to inform the LiteDriver to select it.
         if ( pDataRow->GetSel( ) == TRUE )
         {
            bSelected = TRUE;
            CreateEntity( vXRA, cpcLBSelItemEntity, zPOS_AFTER );
            SetAttributeFromInteger( vXRA, cpcLBSelItemEntity, "Tag", lRelNbr );
         }

         lRelNbr++;
      }
#else
      while ( pos )
      {
         pDataRow = GetNextDataRow( pos );

         // Retrieve row data.
         csRowText = pDataRow->GetRowDataText( );

         CreateEntity( vXRA, cpcLBDataRowEntity, zPOS_AFTER );
         SetAttributeFromString( vXRA, cpcLBDataRowEntity, "Text", csRowText );
         SetAttributeFromInteger( vXRA, cpcLBDataRowEntity, "Tag", lRelNbr );

         // If the row's select flag is set, create the SelectedItem Entity
         // to inform the LiteDriver to select it.
         if ( pDataRow->GetSel( ) == TRUE )
         {
            bSelected = TRUE;
            CreateEntity( vXRA, cpcLBSelItemEntity, zPOS_AFTER );
            SetAttributeFromInteger( vXRA, cpcLBSelItemEntity, "Tag", lRelNbr );
         }

         lRelNbr++;
      }

      if ( CheckExistenceOfEntity( vXRA, cpcLBDataRowEntity ) < zCURSOR_SET )
         SetAttributeFromString( vXRA, cpcListBox, "DeleteAllRows", "Y" );
      else
      if ( CompareAttributeToString( vXRA, cpcListBox, "DeleteAllRows", "Y" ) == 0 )
         SetAttributeFromString( vXRA, cpcListBox, "DeleteAllRows", "N" );

#endif

      if ( bSelected == FALSE )
      {
         CreateEntity( vXRA, cpcLBSelItemEntity, zPOS_AFTER );
         SetAttributeFromInteger( vXRA, cpcLBSelItemEntity, "Tag", -1 );
      }

      DropView( vXRA );
   }
}

void
ZListBox::MapFromRemoteDataset( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZComboBox::MapFromRemoteDataset ", *m_pzsTag );
#endif

   ZXRA_Item *pXRA;

   if ( m_pZSubtask->m_pZSocketTask &&
        (pXRA = m_pZSubtask->m_pZSocketTask->m_pCacheWndList->Find( *(m_pZSubtask->m_pzsRemoteWndTag),
                                                                    m_pZSubtask, "ZListBox::MapFromRemoteDataset" )) != 0 &&
        (pXRA->m_vXRA || pXRA->m_vXRA_Delta) )
   {
      zVIEW vXRA;

      CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
      while ( ResetViewFromSubobject( vXRA ) == 0 )
      {
      }

      if ( CheckExistenceOfEntity( vXRA, "Wnd" ) < zCURSOR_SET ||
           SetCursorFirstEntityByString( vXRA, "Wnd", "Tag", *(m_pZSubtask->m_pzsRemoteWndTag), 0 ) < zCURSOR_SET )
      {
         return;
      }

      // Determine the current ctrl's entity in KZXRA.  Check existence of
      // entity ListRowData.  If there is an entity we have to update data
      // by relative entity number.
      if ( PositionOnRemoteCtrl( this, vXRA, TRUE ) )
      {
         ZRmtDataRow *pDataRow = 0;
         zLONG   lRelNbr = 0;
         zULONG  lLength = 0;
         CString csRowText;
         zPCHAR  pchRowText;

         // Get remote data.
         if ( CheckExistenceOfEntity( vXRA, cpcLBDataRowEntity ) >= zCURSOR_SET ||
              SetCursorFirstEntity( vXRA, cpcLBDataRowEntity, 0 ) >= zCURSOR_SET )
         {
            // Process the first row.
            GetIntegerFromAttribute( &lRelNbr, vXRA, cpcLBDataRowEntity, "Tag" );
            pDataRow = FindDataRowByIndex( lRelNbr );
            if ( pDataRow )
            {
               GetAttributeLength( &lLength, vXRA, cpcLBDataRowEntity, "Text" );
               pchRowText = csRowText.GetBufferSetLength( lLength );

               GetStringFromAttribute( pchRowText, vXRA, cpcLBDataRowEntity, "Text" );
               csRowText.ReleaseBuffer( );
               pDataRow->SetRowDataText( csRowText );
            }
            else
            {
               // This is a new row.  Is this possible?
            }

            // Process all the other entities.
            while ( SetCursorNextEntity( vXRA, cpcLBDataRowEntity, 0 ) >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lRelNbr, vXRA, cpcLBDataRowEntity, "Tag" );
               pDataRow = FindDataRowByDataItem( (DWORD) lRelNbr );

               if ( pDataRow )
               {
                  GetAttributeLength( &lLength, vXRA, cpcLBDataRowEntity, "Text" );
                  pchRowText = csRowText.GetBufferSetLength( lLength );

                  GetStringFromAttribute( pchRowText, vXRA, cpcLBDataRowEntity,"Text" );
                  csRowText.ReleaseBuffer( );
                  pDataRow->SetRowDataText( csRowText );
               }
               else
               {
                  // This is a new row.  Is this possible?
               }
            }
         }

         // Getting Remote selected Items.
         if ( CheckExistenceOfEntity( vXRA, cpcLBSelItemEntity ) >= zCURSOR_SET ||
              SetCursorFirstEntity( vXRA, cpcLBSelItemEntity, 0 ) >= zCURSOR_SET )
         {
            // If KZXRA has at least one SelectedItem Listbox selection
            // has changed so...
            ResetSelectionSet( );

            // Process the first row.
            GetIntegerFromAttribute( &lRelNbr, vXRA, cpcLBSelItemEntity, "Tag" );
            pDataRow = FindDataRowByDataItem( (DWORD) lRelNbr );

            if ( pDataRow )
            {
               pDataRow->SetSel( TRUE );
            }
            else
            {
               // This is a new row.  Is this possible?
            }

            // Process all other rows.
            while ( SetCursorNextEntity( vXRA, cpcLBSelItemEntity, 0 ) >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lRelNbr, vXRA, cpcLBSelItemEntity, "Tag" );
               pDataRow = FindDataRowByDataItem( (DWORD) lRelNbr );

               if ( pDataRow )
               {
                  pDataRow->SetSel( TRUE );
               }
               else
               {
                  // This is a new row.  Is this possible?
               }
            }
         }
      }

      DropView( vXRA );
   }
}

zLONG
ZListBox::SetMultiSelection()
{
   zVIEW  vXRA;
   zVIEW  vXRA_Sel;
   zLONG  lSelCount = 0;

   // Get cuurent XRA from ApplyState in File zdrtask
   if ( GetViewByName( &vXRA, "__XRA_Select",  m_pZSubtask->m_vDialog, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   // Deselect all
   ResetSelectionSet( FALSE );

   CreateViewFromViewForTask( &vXRA_Sel, vXRA, m_pZSubtask->m_vDialog );

   if ( CheckExistenceOfEntity( vXRA_Sel, cpcLBSelItemEntity ) >= zCURSOR_SET )
   {
      zCHAR   szEntityName[zTAG_LTH];
      zUSHORT nLevel;
      zSHORT  nRC;
      zLONG   lSelItem;

      strcpy_s( szEntityName, sizeof( szEntityName ), cpcLBSelItemEntity );

      // XRA has a Hierarchical Cursor, you can not use SetCursorFirst
      nRC = DefineHierarchicalCursor( vXRA_Sel, cpcLBSelItemEntity );

      while ( nRC >= zCURSOR_SET && zstrcmp( szEntityName, cpcLBSelItemEntity ) == 0 )
      {
         lSelCount++;

         GetIntegerFromAttribute( &lSelItem, vXRA_Sel, szEntityName, "Tag" );
         SetSel( lSelItem, TRUE );

         nRC = SetCursorNextEntityHierarchical( &nLevel, szEntityName, vXRA_Sel );
      }

      DropHierarchicalCursor( vXRA_Sel );
   }

   DropView( vXRA_Sel );
   return( lSelCount );
}

zSHORT
ZListBox::SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::SetRowColText ", *m_pzsTag );
#endif

   if ( m_bShowHeaderButtons == FALSE || lColumn > m_nCols || lColumn <= 0 )
   {
      return( -1 );
   }

   zSHORT nRC = -1;
   zVIEW  vXRA;

   if ( (vXRA = SetupRemoteObject( )) != 0 )
   {
      zBOOL bFoundColumn = FALSE;

      if ( lRow == 0 )
      {
         mDeleteInitA( m_pCol[ lColumn ].pchText );
         m_pCol[ lColumn ].pchText = new char[ zstrlen( cpcText ) + 1 ];
         strcpy_s( m_pCol[ lColumn ].pchText, zstrlen( cpcText ) + 1, cpcText );

         // Search Colum
         if ( CheckExistenceOfEntity( vXRA, cpcLBColumnEntity ) == 0 &&
              SetCursorFirstEntityByInteger( vXRA, cpcLBColumnEntity, "Tag", lColumn, "" ) >= zCURSOR_SET )
         {
            bFoundColumn = TRUE;
         }

         if ( bFoundColumn == FALSE )
         {
            CreateEntity( vXRA, cpcLBColumnEntity, zPOS_AFTER );
            SetAttributeFromInteger( vXRA, cpcLBColumnEntity, "Tag", lColumn );
         }

         nRC = 0;
         SetAttributeFromString( vXRA, cpcLBColumnEntity, "Text", cpcText );
      }

      DropView( vXRA );
   }

   return( nRC );
}

zSHORT
ZListBox::GetRowColText( zPCHAR pchText, zUSHORT uLth,
                         zLONG lRow, zLONG lColumn )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZListBox::GetRowColText ", *m_pzsTag );
#endif

   pchText[ 0 ] = 0;
   if ( m_bShowHeaderButtons && lColumn <= m_nCols && lColumn > 0 )
   {
      if ( lRow == 0 )
      {
         strncat_s( pchText, uLth, m_pCol[ lColumn ].pchText, uLth - 1 );
         return( 0 );
      }
   }

   return( -1 );
}

#endif   // end of:  #ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
ListBox( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
#ifndef zREMOTE_SERVER
   if ( pCtrlDef->Tag[ 0 ] == '_' && pCtrlDef->Tag[ 1 ] == '_' )
   {
      if ( pCtrlDef->Tag[ 2 ] == 'P' &&
           pCtrlDef->Tag[ 3 ] == 'r' &&
           pCtrlDef->Tag[ 4 ] == 'o' &&
           pCtrlDef->Tag[ 5 ] == 'p' )
      {
         return( new ZPropertyList( pZSubtask, pWndParent, pzmaComposite, vDialog, nOffsetX, nOffsetY, pCtrlDef ) );
      }
   }
   else
   if ( (pCtrlDef->Subtype & zLISTBOX_COLUMNHEADERS) || (pCtrlDef->Subtype & zLISTBOX_SORTEDHEADERS) )
   {
      return( ListCtrl( pZSubtask, pWndParent, pzmaComposite, vDialog, nOffsetX, nOffsetY, pCtrlDef ) );
   }
#endif

   return( new ZListBox( pZSubtask, pWndParent, pzmaComposite, vDialog, nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=LB_SetDisplaySelectSet
// Source Module=zdctllbx.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SetDisplaySelectSet
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the current display select set for
//              a listbox control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nSelectSet  - The select set for the listbox to display
//
//  RETURNS:    0 - Select set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
LB_SetDisplaySelectSet( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zSHORT  nSelectSet )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->SetDisplaySelSet( nSelectSet );
         return( 0 );
      }
      else
      {
         ZListBox *pList = DYNAMIC_DOWNCAST( ZListBox, pzma->m_pCtrl );
         if ( pList )
         {
            pList->SetDisplaySelSet( nSelectSet );
            return( 0 );
         }
      }

      TraceLineS( "drvr - Invalid control type for LB_SetDisplaySelectSet ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=LB_SetSelectSelectSet
// Source Module=zdctllbx.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: LB_SetSelectSelectSet
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the current selection select set for
//              a listbox control. The selection select set is the select
//              set used by the listbox to communicate back to the
//              application any selections made.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nSelectSet  - The select set for the listbox to return
//                            selections.
//
//  RETURNS:    0 - Select set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
LB_SetSelectSelectSet( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zSHORT  nSelectSet )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZListCtrl *pListCtrl = DYNAMIC_DOWNCAST( ZListCtrl, pzma->m_pCtrl );
      if ( pListCtrl )
      {
         pListCtrl->SetSelectSelSet( nSelectSet );
         return( 0 );
      }
      else
      {
         ZListBox *pList = DYNAMIC_DOWNCAST( ZListBox, pzma->m_pCtrl );
         if ( pList )
         {
            pList->SetSelectSelSet( nSelectSet );
            return( 0 );
         }
      }

      TraceLineS( "drvr - Invalid control type for LB_SetSelectSelectSet ",
                  cpcCtrlTag );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZPropertyItem::ZPropertyItem( zSHORT  nItemType, zCPCHAR cpcPropertyName,
                              zCPCHAR cpcInitialValue, zCPCHAR cpcComboList,
                              zCPCHAR cpcDescription )
{
   m_nItemType = nItemType;
   m_csPropertyName = cpcPropertyName;
   m_csInitialValue = cpcInitialValue;
   m_csComboList = cpcComboList;
   m_csDescription = cpcDescription;
}

ZPropertyItem::~ZPropertyItem( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ZPropertyList

IMPLEMENT_DYNAMIC( ZPropertyList, CListBox )
BEGIN_MESSAGE_MAP( ZPropertyList, CListBox )
   //{{AFX_MSG_MAP( ZPropertyList )
   ON_WM_CREATE( )
   ON_CONTROL_REFLECT( LBN_SELCHANGE, LBNSelchange )
   ON_WM_LBUTTONUP( )
   ON_WM_KILLFOCUS( )
   ON_WM_LBUTTONDOWN( )
   ON_WM_MOUSEMOVE( )
   //}}AFX_MSG_MAP

   // Hide the combobox immediately after selection.
// ON_CBN_KILLFOCUS( IDC_PROPCMBBOX, OnKillfocusCmbBox )
   ON_CBN_CLOSEUP( IDC_PROPCMBBOX, OnKillfocusCmbBox )

   ON_CBN_SELCHANGE( IDC_PROPCMBBOX, OnSelchangeCmbBox )
   ON_EN_KILLFOCUS( IDC_PROPEDITBOX, OnKillfocusEditBox )
   ON_EN_CHANGE( IDC_PROPEDITBOX, OnChangeEditBox )
   ON_BN_CLICKED( IDC_PROPBTNCTRL, OnButton )
END_MESSAGE_MAP( )

// ZPropertyList - ctor
ZPropertyList::ZPropertyList( ZSubtask *pZSubtask,
                              CWnd     *pWndParent,
                              ZMapAct  *pzmaComposite,
                              zVIEW    vDialog,
                              zSHORT   nOffsetX,
                              zSHORT   nOffsetY,
                              zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
               CListBox( ),
               ZMapAct( pZSubtask,          // base class ctor
                        pzmaComposite,
                        pWndParent,
                        this,
                        vDialog,
                        nOffsetX,
                        nOffsetY,
                        pCtrlDef,
                        "Property" )
{
   m_bDivIsSet = FALSE;
   m_nDivider = 0;
   m_bTracking = FALSE;
   m_curSel = 1;

   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   CreateZ( );
}

ZPropertyList::~ZPropertyList( )
{
}

void
ZPropertyList::CreateZ( )
{
   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   if ( (m_ulMapActFlags & zMAPACT_ENABLED) || (m_ulMapActFlags & zMAPACT_DISABLE_READONLY) )
      Attr.Style &= ~WS_DISABLED;
   else
      Attr.Style |= WS_DISABLED;

   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( WS_EX_DLGMODALFRAME, "LISTBOX", *m_pzsText,
                   Attr.Style | LBS_OWNERDRAWVARIABLE |
                      LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT |
                      WS_VSCROLL | WS_TABSTOP,
                   Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
}

zSHORT
ZPropertyList::MapFromOI( WPARAM wFlag )
{
   return( 0 );
}

zSHORT
ZPropertyList::MapToOI( zLONG lFlag )
{
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// ZPropertyList message handlers

BOOL
ZPropertyList::PreCreateWindow( CREATESTRUCT& cs )
{
   if ( CListBox::PreCreateWindow( cs ) == FALSE )
      return( FALSE );

   cs.style &= ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
   cs.style |= LBS_OWNERDRAWFIXED;

   return( TRUE );
}

void
ZPropertyList::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
   lpMeasureItemStruct->itemHeight = 20; // pixels
}

void
ZPropertyList::DrawItem( LPDRAWITEMSTRUCT lpDIS )
{
   CDC dc;
   dc.Attach( lpDIS->hDC );
   CRect rectFull = lpDIS->rcItem;
   CRect rect = rectFull;
   if ( m_nDivider == 0 )
      m_nDivider = rect.Width( ) / 2;

   rect.left = m_nDivider;
   CRect rect2 = rectFull;
   rect2.right = rect.left - 1;
   UINT nIndex = lpDIS->itemID;

   if ( nIndex != (UINT) -1 )
   {
      // Draw two rectangles, one for each row column.
      dc.FillSolidRect( rect2, RGB( 192, 192, 192 ) );
      dc.DrawEdge( rect2, EDGE_SUNKEN, BF_BOTTOMRIGHT );
      dc.DrawEdge( rect, EDGE_SUNKEN, BF_BOTTOM );

      // Get the ZPropertyItem for the current row.
      ZPropertyItem *pItem = (ZPropertyItem *) GetItemDataPtr( nIndex );

      // Write the property name in the first rectangle.
      dc.SetBkMode( TRANSPARENT );
      dc.DrawText( pItem->m_csPropertyName,
                   CRect( rect2.left + 3, rect2.top + 3,
                          rect2.right - 3, rect2.bottom + 3 ),
                   DT_LEFT | DT_SINGLELINE );

      // Write the initial property value in the second rectangle.
      dc.DrawText( pItem->m_csInitialValue,
                   CRect( rect.left + 3, rect.top + 3,
                          rect.right + 3,rect.bottom + 3 ),
                   DT_LEFT | DT_SINGLELINE );
   }

   dc.Detach( );
}

zSHORT
ZPropertyList::AddItem( CString cs )
{
   int nIndex = AddString( cs );
   return( nIndex );
}

zSHORT
ZPropertyList::AddPropItem( ZPropertyItem *pItem )
{
   int nIndex = AddString( _T( "" ) );
   SetItemDataPtr( nIndex, pItem );
   return( nIndex );
}

int
ZPropertyList::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   if ( CListBox::OnCreate( lpCreateStruct ) == -1 )
      return( -1 );

   m_hCursorSize = AfxGetApp( )->LoadStandardCursor( IDC_SIZEWE );
   m_hCursorArrow = AfxGetApp( )->LoadStandardCursor( IDC_ARROW );

   m_SSerif8Font.CreatePointFont( 80, _T( "MS Sans Serif" ) );

   return( 0 );
}

void
ZPropertyList::LBNSelchange( )
{
   CRect rect;
   CString lBoxSelText;

   //m_curSel = GetCurSel( );

   GetItemRect( m_curSel, rect );
   rect.left = m_nDivider;

   ZPropertyItem *pItem = (ZPropertyItem *) GetItemDataPtr( m_curSel );

   if ( m_btnCtrl )
      m_btnCtrl.ShowWindow( SW_HIDE );

   if ( pItem->m_nItemType == PIT_COMBO )
   {
      // Display the combo box.  If the combo box has already been
      // created then simply move it to the new location, else create it.
      m_nLastBox = 0;
      if ( m_cmbBox )
         m_cmbBox.MoveWindow( rect );
      else
      {
      // Better creation of the combobox. The combobox should
      // display a vertical scrollbar when its list contains a
      // large number of items. And it may be a bit larger as well.
      // Personally I don't like the CBS_NOINTEGRALHEIGHT style.
      // But you can keep it if you want.
      // rect.bottom += 100;
      // m_cmbBox.Create( CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT |
      //                    WS_VISIBLE | WS_CHILD | WS_BORDER,
      //                  rect, this, IDC_PROPCMBBOX );
         rect.bottom += 300;
         m_cmbBox.Create( CBS_DROPDOWNLIST | WS_VSCROLL |
                            WS_VISIBLE | WS_CHILD | WS_BORDER,
                          rect, this, IDC_PROPCMBBOX );

         m_cmbBox.SetFont( &m_SSerif8Font );
      }

      // Add the choices for this particular property.
      CString csComboItems = pItem->m_csComboList;
      lBoxSelText = pItem->m_csInitialValue;

      m_cmbBox.ResetContent( );
      m_cmbBox.AddString( "" );
      zSHORT k = 0;
      zSHORT j;

      while ( (j = csComboItems.Find( '|', k )) != -1 )
      {
         m_cmbBox.AddString( csComboItems.Mid( k, j - k ) );
         k = j + 1;
      }

      m_cmbBox.ShowWindow( SW_SHOW );
      m_cmbBox.SetFocus( );

      // Jump to the property's current value in the combo box.
      j = m_cmbBox.FindStringExact( 0, lBoxSelText );
      if ( j != CB_ERR )
         m_cmbBox.SetCurSel( j );
      else
         m_cmbBox.SetCurSel( 0 );

      // Show the dropdown list of a combobox immediately on selection to
      // allow for immediate selection after only one click, instead of two.
      m_cmbBox.ShowDropDown( );
   }
   else
   if ( pItem->m_nItemType == PIT_EDIT )
   {
      // Display edit box.
      m_nLastBox = 1;
      m_prevSel = m_curSel;
      rect.bottom -= 3;
      if ( m_editBox )
         m_editBox.MoveWindow( rect );
      else
      {
         m_editBox.Create( ES_LEFT | ES_AUTOHSCROLL | WS_VISIBLE |
                             WS_CHILD | WS_BORDER,
                           rect, this, IDC_PROPEDITBOX );
         m_editBox.SetFont( &m_SSerif8Font );
      }

      lBoxSelText = pItem->m_csInitialValue;

      m_editBox.ShowWindow( SW_SHOW );
      m_editBox.SetFocus( );

      // Set the text in the edit box to the property's current value.
      m_editBox.SetWindowText( lBoxSelText );
   }
   else
      DisplayButton( rect );
}

// My program needs to use ResetContent( ) to remove all the items.
// But when PropertyList's (CButton) m_btnCtrl had already shown,
// and after ResetContent( ), if the total items are less then before.
// then the button would still remain on the blank of the listBox!!
// The fix is to call CleanAll( ) instead of ResetContent( ).
void
ZPropertyList::CleanAll( )  // don't know when to call this yet???
{
   if ( m_btnCtrl )
      m_btnCtrl.ShowWindow( SW_HIDE );

   ResetContent( );
}

void
ZPropertyList::DisplayButton( CRect region )
{
   // Displays a button if the property is a file/color/font chooser.
   m_nLastBox = 2;
   m_prevSel = m_curSel;

   if ( region.Width( ) > 25 )
      region.left = region.right - 25;

   region.bottom -= 3;

   if ( m_btnCtrl )
      m_btnCtrl.MoveWindow( region );
   else
   {
      m_btnCtrl.Create( "...", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
                        region, this, IDC_PROPBTNCTRL );
      m_btnCtrl.SetFont( &m_SSerif8Font );
   }

   m_btnCtrl.ShowWindow( SW_SHOW );
   m_btnCtrl.SetFocus( );
}

void
ZPropertyList::OnKillFocus( CWnd* pNewWnd )
{
   // m_btnCtrl.ShowWindow( SW_HIDE );
   CListBox::OnKillFocus( pNewWnd );
}

void
ZPropertyList::OnKillfocusCmbBox( )
{
   m_cmbBox.ShowWindow( SW_HIDE );
   Invalidate( );
}

void
ZPropertyList::OnKillfocusEditBox( )
{
   m_editBox.ShowWindow( SW_HIDE );
   Invalidate( );
}

void
ZPropertyList::OnSelchangeCmbBox( )
{
   CString csSel;
   if ( m_cmbBox )
   {
      m_cmbBox.GetLBText( m_cmbBox.GetCurSel( ), csSel );
      ZPropertyItem *pItem = (ZPropertyItem *) GetItemDataPtr( m_curSel );
      pItem->m_csInitialValue = csSel;
   }
}

void
ZPropertyList::OnChangeEditBox( )
{
   CString csNew;
   m_editBox.GetWindowText( csNew );

   ZPropertyItem *pItem = (ZPropertyItem *) GetItemDataPtr( m_curSel );
   pItem->m_csInitialValue = csNew;
}

void
ZPropertyList::OnButton( )
{
   ZPropertyItem *pItem = (ZPropertyItem *) GetItemDataPtr( m_curSel );

   // Display the appropriate common dialog depending on what type
   // of chooser is associated with the property.
   if ( pItem->m_nItemType == PIT_COLOR )
   {
      COLORREF clrInit;
      CString csClr = pItem->m_csInitialValue;

      // Parse the property's current color value.
      if ( csClr.Find( "RGB" ) > -1 )
      {
         int j = csClr.Find( ',', 3 );
         CString cs = csClr.Mid( 4, j - 4 );
         int nRVal = atoi( cs );
         int k = csClr.Find( ',', j + 1 );
         cs = csClr.Mid( j + 1, k - (j + 1) );
         int nGVal = atoi( cs );
         int n = csClr.Find( ')', k + 1 );
         cs = csClr.Mid( k + 1, n - (k + 1) );
         int nBVal = atoi( cs );
         clrInit = RGB( nRVal, nGVal, nBVal );
      }
      else
         clrInit = 0;

      CColorDialog ClrDlg( clrInit );

      if ( IDOK == ClrDlg.DoModal( ) )
      {
         COLORREF clrSel = ClrDlg.GetColor( );
         CString csClr;
         csClr.Format( "RGB( %d, %d, %d )", GetRValue( clrSel ),
                       GetGValue( clrSel ), GetBValue( clrSel ) );
         m_btnCtrl.ShowWindow( SW_HIDE );

         pItem->m_csInitialValue = csClr;
         Invalidate( );
      }
   }
   else
   if ( pItem->m_nItemType == PIT_FILE )
   {
      CString csSelectedFile;
      CString csFilter( "Gif Files (*.gif)|*.gif||" );

      CFileDialog FileDlg( TRUE, NULL, NULL, NULL, csFilter );

      CString csPath = pItem->m_csInitialValue;
      FileDlg.m_ofn.lpstrTitle = "Select file";
      if ( csPath.GetLength( ) > 0 )
      {
         FileDlg.m_ofn.lpstrInitialDir = csPath.Left( csPath.GetLength( ) -
                                         csPath.ReverseFind( '\\' ) );
      }

      if ( IDOK == FileDlg.DoModal( ) )
      {
         csSelectedFile = FileDlg.GetPathName( );
         m_btnCtrl.ShowWindow( SW_HIDE );
         pItem->m_csInitialValue = csSelectedFile;
         Invalidate( );
      }
   }
   else
   if ( pItem->m_nItemType == PIT_FONT )
   {
      CFontDialog FontDlg( NULL, CF_EFFECTS | CF_SCREENFONTS, NULL, this );

      if ( IDOK == FontDlg.DoModal( ) )
      {
         CString csFaceName = FontDlg.GetFaceName( );

         m_btnCtrl.ShowWindow( SW_HIDE );

         pItem->m_csInitialValue = csFaceName;
         Invalidate( );
      }
   }
}

void
ZPropertyList::OnLButtonUp( UINT uFlags, CPoint pt )
{
   if ( m_bTracking )
   {
      // If columns were being resized then this indicates
      // that mouse is up so resizing is done.  Need to redraw
      // columns to reflect their new widths.

      m_bTracking = FALSE;

      // If mouse was captured then release it.
      if ( GetCapture( ) == this )
         ::ReleaseCapture( );

      ::ClipCursor( NULL );

      CClientDC dc( this );
      InvertLine( &dc, CPoint( pt.x, m_nDivTop ),
                  CPoint( pt.x, m_nDivBtm ) );

      // Set the divider position to the new value
      m_nDivider = pt.x;

      // Redraw.
      Invalidate( );
   }
   else
   {
      BOOL loc;
      int k = ItemFromPoint( pt, loc );
      m_curSel = k;
      LBNSelchange( );
      CListBox::OnLButtonUp( uFlags, pt );
   }
}

void
ZPropertyList::OnLButtonDown( UINT uFlags, CPoint pt )
{
   if ( (pt.x >= m_nDivider - 5) && (pt.x <= m_nDivider + 5) )
   {
      // If mouse clicked on divider line, then start resizing.
      ::SetCursor( m_hCursorSize );

      CRect windowRect;
      GetWindowRect( windowRect );
      windowRect.left += 10;
      windowRect.right -= 10;

      // Do not let mouse leave the list box boundary.
      ::ClipCursor( windowRect );

      if ( m_cmbBox )
         m_cmbBox.ShowWindow( SW_HIDE );

      if ( m_editBox )
         m_editBox.ShowWindow( SW_HIDE );

      CRect clientRect;
      GetClientRect( clientRect );

      m_bTracking = TRUE;
      m_nDivTop = clientRect.top;
      m_nDivBtm = clientRect.bottom;
      m_nOldDivX = pt.x;

      CClientDC dc( this );
      InvertLine( &dc, CPoint( m_nOldDivX, m_nDivTop ),
                  CPoint( m_nOldDivX, m_nDivBtm ) );

      // Capture the mouse.
      SetCapture( );
   }
   else
   {
      m_bTracking = FALSE;
      CListBox::OnLButtonDown( uFlags, pt );
   }
}

void
ZPropertyList::OnMouseMove( UINT uFlags, CPoint pt )
{
   if ( m_bTracking )
   {
      // Move divider line to the mouse pos. if columns are
      // currently being resized.
      CClientDC dc( this );

      // Remove old divider line.
      InvertLine( &dc, CPoint( m_nOldDivX, m_nDivTop ),
                  CPoint( m_nOldDivX, m_nDivBtm ) );

      // Draw new divider line.
      InvertLine( &dc, CPoint( pt.x, m_nDivTop ),
                  CPoint( pt.x, m_nDivBtm ) );
      m_nOldDivX = pt.x;
   }
   else
   if ( (pt.x >= m_nDivider - 5) && (pt.x <= m_nDivider + 5) )
   {
      // Set the cursor to a sizing cursor if the cursor is over the
      // row divider.
      ::SetCursor( m_hCursorSize );
   }
   else
      CListBox::OnMouseMove( uFlags, pt );
}

void
ZPropertyList::InvertLine( CDC *pDC, CPoint ptFrom, CPoint ptTo )
{
   int nOldMode = pDC->SetROP2( R2_NOT );

   pDC->MoveTo( ptFrom );
   pDC->LineTo( ptTo );

   pDC->SetROP2( nOldMode );
}

void
ZPropertyList::PreSubclassWindow( )
{
   m_bDivIsSet = FALSE;
   m_nDivider = 0;
   m_bTracking = FALSE;
   m_curSel = 1;

// m_hCursorSize = AfxGetApp( )->LoadStandardCursor( IDC_SIZEWE );
// m_hCursorArrow = AfxGetApp( )->LoadStandardCursor( IDC_ARROW );

// m_SSerif8Font.CreatePointFont( 80, _T( "MS Sans Serif" ) );
}

//./ ADD NAME=PL_AddProperty
// Source Module=zdctllbx.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: PL_AddProperty
//                                          06/07/2002 Modified: 06/07/2002
//
//  PURPOSE:    This operation adds a property to a property list.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nItemType   - The type of property
//              cpcCurrentValue - Current value of the property
//              cpcComboList - List (for combo) of values the property
//                             may take on (separated by |).
//              cpcDescription - Optional third column property description.
//
//  RETURNS:    0 - Select set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
PL_AddProperty( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zSHORT  nItemType,
                zCPCHAR cpcPropertyName,
                zCPCHAR cpcInitialValue,
                zCPCHAR cpcComboList,
                zCPCHAR cpcDescription )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZPropertyList *pProperty = DYNAMIC_DOWNCAST( ZPropertyList, pzma->m_pCtrl );
      if ( pProperty )
      {
         pProperty->AddPropItem( new ZPropertyItem( nItemType,
                                                    cpcPropertyName,
                                                    cpcInitialValue,
                                                    cpcComboList,
                                                    cpcDescription ) );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for PL_AddProperty ",
                  cpcCtrlTag );
   }

   return( -1 );
}
