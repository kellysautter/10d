/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlcmb.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Zeidon ComboBox control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.10.16    DKS
//    Fix to mapping of "user data" combobox.
//
// 2002.09.25    DKS
//    Map "output editbox" portion of combobox even if the OI entity attribute
//    is not updatable.
//
// 2002.06.27    HH
//    Removed call to SetZCtrlText at the end of ZComboBox::MapToOI.
//    This was necessary for Zeidon Web, because it caused an incomplete
//    XML (no selected item) for the combobox always to be sent.
//
// 2002.02.27    DKS
//    Disabled "disable as readonly" flag for combobox.  As of yet, it is not
//    implemented properly.
//
// 2002.02.14    DKS
//    Added Domain translation via code.
//
// 2001.12.31    DKS
//    Get ComboBox events in Conversational dialog.
//
// 2001.11.05    HH
//    reset flag m_bWasSelected.
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.18    DKS    WEB
//    Back out Delta Data implementation.
//
// 2001.09.01    DKS    WEB
//    Handle case of single add/delete in lists.
//
// 2001.08.03    DKS    R55605
//    Multiple keypress location of entries in list retained when closing
//    ComboBox via tab key after opening ComboBox using mouse.
//
// 2001.07.04    BL     RemoteServer
//    Modified GetArrayWithKeyAttribute for search Key Attributes, not
//    bForeignKey Attributes
//
// 2001.06.12    DKS    RemoteServer
//    Do not deselect if there are entries in the list.
//
// 2001.06.10    BL     RemoteServer
//    Handle error situation, where no List Mapping for ComboBox is provided,
//    so that Web and Windows have the same behavior
//
// 2001.05.31    DKS/BL RemoteServer
//    Bugfix for ComboBox on a Tab Control
//
// 2001.05.31    BL     RemoteServer
//    Bugfix for GetCtrlText
//
// 2001.05.30    BL     RemoteServer
//    Modified SelectStringByKeyAttributes for check correct View.
//
// 2001.05.29    BL     RemoteServer
//    Bugfix ComboBox without Entity Instances in ListBox Part,
//    do not send a Mapping Error by load Window
//
// 2001.05.29    DKS    WEB
//    Implement merge of list data.
//
// 2001.05.21    DKS    Enhance
//    Implement multiple keypress location of entries in list.
//
// 2001.05.16    BL     RemoteServer
//    Web: Fix for ComboBox with Domain Errors
//
// 2001.05.12    BL     RemoteServer
//    Modified MapToRemoteDataset and added SelectStringByKeyAttributes:
//    If the Edit string was not found in ListBox part, then set the ListBox
//    View cursor according to the Key Attributes of the Edit field Entity.
//    This is only done for "INCLUDE" Type ComboBoxes
//
// 2001.05.12    BL     RemoteServer
//    De-selection of item when list part of combobox shows a derived
//    attribute and the edit part shows the corresponding "normal" attribute.
//
// 2001.05.11    DKS    Z10
//    Fix scoping entity for SetEntityCursor (by attribute) to use the
//    correct entity name.
//
// 2001.05.08    DKS    WEB
//    De-selection of item when list part of combobox shows a derived
//    attribute and the edit part shows the corresponding "normal" attribute.
//
// 2001.05.03    DKS    WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.11    DKS    RemoteServer
//    Set up for Delta Data project.
//
// 2001.01.17    DKS    RemoteServer
//    Sort order now honored.
//
// 2000.05.30    DKS    RemoteServer
//    List now shown in Release (as well as Debug) version.
//
// 2000.03.28    DKS    TB1018
//    Can now scroll in ComboBox with arrow keys.
//
// 2000.03.17    TMV    WEB
//    Validate parent window's handle prior to call Create/CreateEx
//
// 1999.09.07    DKS    Z10    QS999
//    Set attribute now enabled for FOREIGN_KEY when attribute value
//    not found in list.  AutoGray for FOREIGN_KEY corrected.
//
// 1999.07.11    DKS    Z10    QS999
//    SetFocus event enabled on return from sub-window.
//
// 1999.07.01    DKS    Z10    QS999
//    Protect against crash when attribute cannot be found for an entity.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Permit disable of Automatic Mapping and changed MapAct flags to long.
//    Corrected existence check for AutoGray processing.
//
// 1999.01.25    DKS
//    ComboBox now handles Esc key properly (TB 362).
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlCmb.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL
// #define DKS_TEST

#ifdef zREMOTE_SERVER

IMPLEMENT_DYNAMIC( ZComboBox, ZRmtDataSet )

BEGIN_MESSAGE_MAP( ZComboBox, ZRmtDataSet )
END_MESSAGE_MAP( )

#else

IMPLEMENT_DYNAMIC( ZComboBox, CComboBox )

BEGIN_MESSAGE_MAP( ZComboBox, CComboBox )
   ON_WM_CREATE( )
   ON_WM_KEYDOWN( )
   ON_WM_KEYUP( )
   ON_WM_CHAR( )
   ON_WM_ENABLE( )
// ON_WM_SETFOCUS( )
// ON_WM_KILLFOCUS( )
   ON_WM_CTLCOLOR_REFLECT( )
// ON_WM_PAINT( )
   ON_CONTROL_REFLECT( CBN_CLOSEUP, CBNCloseup )
   ON_CONTROL_REFLECT( CBN_DBLCLK, CBNDblclk )
   ON_CONTROL_REFLECT( CBN_DROPDOWN, CBNDropdown )
   ON_CONTROL_REFLECT( CBN_EDITCHANGE, CBNEditchange )
// ON_CONTROL_REFLECT( CBN_EDITUPDATE, CBNEditupdate )
   ON_CONTROL_REFLECT( CBN_EDITUPDATE, OnEditUpdate )
   ON_CONTROL_REFLECT( CBN_ERRSPACE, CBNErrspace )
   ON_CONTROL_REFLECT( CBN_SELCHANGE, CBNSelchange )
   ON_CONTROL_REFLECT( CBN_SELENDCANCEL, CBNSelendcancel )
   ON_CONTROL_REFLECT( CBN_SELENDOK, CBNSelendok )
   ON_CONTROL_REFLECT( CBN_SETFOCUS, CBNSetFocus )
   ON_CONTROL_REFLECT( CBN_KILLFOCUS, CBNKillFocus )
   ON_WM_MOUSEMOVE( )
   ON_MESSAGE( WM_APP + WM_GETTEXT, OnGetText )
   ON_MESSAGE( WM_APP + WM_SETTEXT, OnSetText )
END_MESSAGE_MAP( )

#endif

#define zCB_CLOSEUP        8
#define zCB_DBLCLK         2
#define zCB_DROPDOWN       7
#define zCB_EDITCHANGE     5
#define zCB_EDITUPDATE     6
#define zCB_ERRSPACE     (-1)
#define zCB_SETFOCUS       3
#define zCB_KILLFOCUS      4
#define zCB_SELENDOK       9
#define zCB_SELENDCANCEL  10
#define zCB_SELCHANGE      1
#define zCB_SELCLOSE     256
#define zCB_CLOSED    0x0000
#define zCB_OPENED    0x0001
#define zCB_SELCHG    0x0010
#define zCB_SELEND    0x0100
#define zCB_SELCHGEND 0x0110

#define zCOMBOBOX_MULTICOL    zCONTROL_SPECIFIC_1

#ifdef zREMOTE_SERVER
   zCPCHAR cpcCMBDataRowEntity = "CmbRowData";
#endif


// ZComboBox - ctor
ZComboBox::ZComboBox( ZSubtask *pZSubtask,
                      CWnd     *pWndParent,
                      ZMapAct  *pzmaComposite,
                      zVIEW    vDialog,
                      zSHORT   nOffsetX,
                      zSHORT   nOffsetY,
                      zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
#ifdef zREMOTE_SERVER
           ZRmtDataSet( ),
#else
           CComboBox( ),
#endif
           ZMapAct( pZSubtask,            // base class ctor
                    pzmaComposite,
                    pWndParent,
                    this,
                    vDialog,
                    nOffsetX,
                    nOffsetY,
                    pCtrlDef,
                    "ComboBox" ),
           m_csViewName( "_" ),
           m_csViewNameList( "__" )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZComboBox::ctor ", *m_pzsTag );
#endif

   m_vApp = m_vAppList = 0;
   m_csViewName += *m_pzsTag;
   m_csViewNameList += *m_pzsTag;
   m_lEntityCnt = 0;
   m_lEntityNbr = 0;

   m_bListLoaded = FALSE;
   m_bWasSelected = FALSE;
   m_nDisplaySelSet = 0;
   m_nSelectSelSet  = 1;
// m_ulDelayInterval = GetDoubleClickTime( ) * 2;
// m_ulLastTime = 0;
// m_lKeystrokeCnt = 0;
// m_szKeystroke[ 0 ] = 0;
// m_lItemNbr = 0;
   m_ulSubtype = pCtrlDef->Subtype;
   m_pchPairedList = 0;

   m_pchSelectItem = 0;
   m_pchListData = 0;

   m_pMCCEdit = 0;
   m_pHeadCol = 0;
   m_bMultiCol = FALSE;
   m_nTotalCols = 0;
   m_nBoundCol = -1;
   m_nShowColumn = -1;
   m_nTextItemHeight = 12;
   m_penLightGray.CreatePen( PS_SOLID, 1, GetSysColor( COLOR_3DLIGHT ) );
   m_nTotalColWidth = 0;
// m_pnColumnWidth = 0;
// m_plColumnAlignStyle = 0;
// m_pcsColItemList = 0;
   m_bAutoComplete = TRUE;
// m_pComboEdit = 0;

   if ( m_ulMapActFlags & zMAPACT_USE_SUBTASK_BRUSH )
   {
      mDeleteInit( m_pBrush );
      m_clrBk = 0;
   }

   if ( pCtrlDef->Subtype & zCOMBOBOX_MULTICOL )
      m_bMultiCol = TRUE;

   if ( m_ulSubtype & zCONTROL_SIZEABLEBORDER )
      Attr.Style |= WS_THICKFRAME;
   else
   if ( (m_ulSubtype & zCONTROL_BORDEROFF) == 0 )
      Attr.Style |= WS_BORDER;

   if ( m_ulSubtype & zCOMBOBOX_SORT )
      Attr.Style |= CBS_SORT;
   else
      Attr.Style &= ~CBS_SORT;

   if ( m_ulSubtype & zCOMBOBOX_SIMPLE )
      Attr.Style |= CBS_SIMPLE;
   else
   if ( m_ulSubtype & zCOMBOBOX_DROPDOWN )
      Attr.Style |= CBS_DROPDOWN;
   else
// if ( m_ulSubtype & zCOMBOBOX_DROPDOWNLIST )   // default
      Attr.Style |= CBS_DROPDOWNLIST;

   if ( pCtrlDef->Subtype & zCONTROL_NOTABSTOP )
      Attr.Style &= ~WS_TABSTOP;
   else
      Attr.Style |= WS_TABSTOP;

   if ( m_ulSubtype & zCOMBOBOX_SIMPLE )
      m_nIsDroppedDown = zCB_OPENED;
   else
      m_nIsDroppedDown = zCB_CLOSED;

   mDeleteInit( m_pzsVName );
   mDeleteInit( m_pzsEName );
   mDeleteInit( m_pzsAName );
   mDeleteInit( m_pzsContext );

   m_pzsListMapVName = 0;
   m_pzsListMapEName = 0;
   m_pzsListMapAName = 0;
   m_pzsListMapContext = 0;
   m_pzsListBrowseEName = 0;
   m_pzsListBrowseScopeEName = 0;
   m_pzsScope = 0;
// m_bFirstTime = TRUE;

   m_ulFlag = (m_ulSubtype & zCOMBOBOX_SCOPE_OI) ? zQUAL_SCOPE_OI : 0;

   zPCHAR pch = (zPCHAR) pCtrlDef->CtrlBOI;
   if ( pch == 0 )      // there is no CtrlBOI
      return;

   if ( *pch )
      m_pzsVName = new CString( pch );

   pch += zstrlen( pch ) + 1;
   if ( *pch )
      m_pzsEName = new CString( pch );

   pch += zstrlen( pch ) + 1;
   if ( *pch )
      m_pzsAName = new CString( pch );

   pch += zstrlen( pch ) + 1;

   if ( *pch )
   {
      m_pzsContext = new CString( pch );
      pch += zstrlen( pch ) + 1;
   }
   else
   {
      pch += sizeof( zSHORT );
      m_pzsContext = new CString;
   }

   if ( m_ulSubtype & zCOMBOBOX_POSITION_ONLY )
   {
//    pch += zstrlen( pch ) + 1;
      if ( *pch )
         m_pzsListMapVName = new CString( pch );

      pch += zstrlen( pch ) + 1;
      if ( *pch )
         m_pzsListMapEName = new CString( pch );

      pch += zstrlen( pch ) + 1;
      if ( *pch )
         m_pzsListMapAName = new CString( pch );

      pch += zstrlen( pch ) + 1;
      if ( *pch )
      {
         // We don't think this can ever be set (since the dialog does
         // not seem to support it), so in the code below, we set the
         // context from the attribute context.
         m_pzsListMapContext = new CString( pch );
         pch += zstrlen( pch ) + 1;
      }
      else
         pch += sizeof( zSHORT );

//    pch += zstrlen( pch ) + 1;
      if ( *pch )
         m_pzsListBrowseEName = new CString( pch );

      pch += zstrlen( pch ) + 1;
      if ( *pch )
      {
         m_pzsListBrowseScopeEName = new CString( pch );
         m_pzsScope = m_pzsListBrowseScopeEName;
      }
   }

   if ( m_pzsContext->IsEmpty( ) == FALSE && m_pzsListMapContext == 0 )
   {
      m_pzsListMapContext = new CString( *m_pzsContext );
   }

   if ( m_bMultiCol )
   {
      zLONG  lColWidth;
      zLONG  lAlignStyle = DT_LEFT;
      zBOOL  bError = FALSE;
      zSHORT nRC;

      SetViewToSubobject( vDialog, "CtrlCtrl" );
      nRC = SetCursorFirstEntity( vDialog, szlCtrl, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         m_nTotalCols++;
         GetIntegerFromAttribute( &lColWidth, vDialog, szlCtrl, "SZDLG_X" );
         if ( m_pZSubtask->m_bUseMapDlgUnits )
            lColWidth = mConvertMapDlgToPixelX( lColWidth );
         else
            lColWidth = mConvertDlgUnitToPixelX( lColWidth );

         if ( CheckExistenceOfEntity( vDialog, szlCtrlMap ) > zCURSOR_UNCHANGED )
         {
            zKZWDLGXO_CtrlMap_DEF CtrlMapDef;

            CtrlMapDef.CheckByte = 0xFE;
            CtrlMapDef.lStructSize = sizeof( zKZWDLGXO_CtrlMap_DEF );
            GetStructFromEntityAttrs( (zPCHAR) &CtrlMapDef, vDialog, szlCtrlMap );
            if ( CtrlMapDef.CheckByte != 0xFE )
               ::MessageBox( 0, "CtrlMapDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

            // If there is a view name, then fill in the rest of the attributes, otherwise let them stay null.
            if ( (m_ulSubtype & zCOMBOBOX_POSITION_ONLY) == 0 &&
                 zstrcmp( (*m_pzsListMapVName).GetString(), CtrlMapDef.VN ) != 0 )
            {
               bError = TRUE;
               break;
            }

            AddColumn( m_nTotalCols, (zSHORT) lColWidth, lAlignStyle,
                       CtrlMapDef.VN, CtrlMapDef.EN, CtrlMapDef.AN, CtrlMapDef.Context );
            if ( zstrcmp( CtrlMapDef.AN, (*m_pzsAName).GetString() ) == 0 &&
                 zstrcmp( CtrlMapDef.EN, (*m_pzsEName).GetString() ) == 0 )
            {
               m_nBoundCol = m_nTotalCols;   //???
               m_nShowColumn = m_nTotalCols; //???
            }
            else
            if ( (m_ulSubtype & zCOMBOBOX_POSITION_ONLY) &&
                 m_nBoundCol < 0 )
            {
               m_nBoundCol = 0;
               m_nShowColumn = 0;
               AddColumn( 0, (zSHORT) lColWidth, lAlignStyle,
                          (*m_pzsVName).GetString(), (*m_pzsEName).GetString(),
                          (*m_pzsAName).GetString(), (*m_pzsContext).GetString() );
            }
         }
         else
         {
            bError = TRUE;
            break;
         }

         nRC = SetCursorNextEntity( vDialog, szlCtrl, 0 );
      }

      ResetViewFromSubobject( vDialog );
      if ( bError || m_nTotalCols < 2 ||
           (m_nBoundCol < 0 || m_nShowColumn < 0) )
      {
         m_bMultiCol = FALSE;
         RemoveColumns( );
      }
   }

// m_lOrigSize = Attr.H; it appears that this is done for us in 32-bit Windows

   // Do ReadOnly for combobox if defined.
   if ( m_ulMapActFlags & zMAPACT_DISABLE_READONLY )
      m_pReadOnlyEdit = new CEdit( );
   else
      m_pReadOnlyEdit = 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }
}

// ZComboBox - dtor
ZComboBox::~ZComboBox( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineS( "ZComboBox::dtor ", *m_pzsTag );
      TraceLineX( "ZComboBox::dtor MapActFlags: ", m_ulMapActFlags );
   }
#endif

   mDeleteInit( m_pReadOnlyEdit );

   RemoveColumns( );

   mDeleteInitA( m_pchSelectItem );
   mDeleteInitA( m_pchListData );
   mDeleteInitA( m_pchPairedList );
   mDeleteInit( m_pzsListMapVName );
   mDeleteInit( m_pzsListMapEName );
   mDeleteInit( m_pzsListMapAName );
   mDeleteInit( m_pzsListMapContext );
   mDeleteInit( m_pzsListBrowseEName );
   mDeleteInit( m_pzsListBrowseScopeEName );

// mDeleteInitA( m_pnColumnWidth );
// mDeleteInitA( m_plColumnAlignStyle );
// mDeleteInitA( m_pcsColItemList );

   m_penLightGray.DeleteObject( );
   mDeleteInit( m_pMCCEdit );

#ifndef zREMOTE_SERVER
   if ( mIs_hWnd( m_hWnd ) )
      DestroyWindow( );
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Here's the edit control's ID (which is typically 1 less than the
// first control in the resource.h file).
#define EDIT_CTRL_ID 999

LRESULT
ZComboBox::DefWindowProc( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
   // Determine the specific messages we're trapping
   switch ( uMessage )
   {
      // Same message for enable/disable
      case WM_ENABLE:
      {
         LRESULT lRC = CComboBox::DefWindowProc( uMessage, wParam, lParam );

         // Check if disabling the combobox
         if ( wParam == 0 )
         {
            // Verify pointer exists and need to create the edit control
            if ( (m_pReadOnlyEdit) ||
                 (m_pReadOnlyEdit && (m_pReadOnlyEdit->m_hWnd == 0)) )
            {
               CRect  rectO;
               GetClientRect( &rectO );
               rectO.right -= 15;      // When want the disabled drop-down portion visible

               // Adjust rectangle for border
               int iBorderWidth = 3;

               // Adjust when drop-list is type of combobox
               if ( (GetStyle( ) & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST )
                  iBorderWidth++;

               rectO.left += iBorderWidth;
               rectO.top += iBorderWidth;
               rectO.right -= iBorderWidth;
               rectO.bottom -= iBorderWidth;

               // See if we need to create the edit control
               if ( mIs_hWnd( m_pReadOnlyEdit->m_hWnd ) == 0 )
               {
                  // Create the read-only edit control
                  if ( m_pReadOnlyEdit->Create( WS_CHILD | ES_LEFT |
                                                  ES_AUTOHSCROLL | ES_READONLY,
                                                rectO, this, EDIT_CTRL_ID ) )
                  {
                     DWORD dwExStyle = WS_EX_CLIENTEDGE | WS_EX_NOPARENTNOTIFY;

                     // Comment out code that would display a border on the edit control
                     m_pReadOnlyEdit->ModifyStyleEx( 0, dwExStyle /*, SWP_DRAWFRAME | SWP_NOSIZE*/ );
                     CFont *pCurFont = GetFont( );
                     if ( pCurFont )
                        m_pReadOnlyEdit->SetFont( pCurFont );
                  }  // End of test if edit control was created properly or not
                  else
                  {
                     mDeleteInit( m_pReadOnlyEdit );
                  }  // Error occurred, don't worry about edit control
               }  // End of test if we need to create the edit control
            }  // End of pointer validation

            // Check if edit control has been created and has a valid windows handle
            if ( m_pReadOnlyEdit && m_pReadOnlyEdit->m_hWnd )
            {
               // Try to show the edit control
               m_pReadOnlyEdit->ShowWindow( SW_SHOW );
               m_pReadOnlyEdit->SetReadOnly( TRUE );
               m_pReadOnlyEdit->ModifyStyle( WS_TABSTOP, 0 );
               CString csComboText;
               GetWindowText( csComboText );
               m_pReadOnlyEdit->SetWindowText( csComboText );
            }  // End of pointer validation
         }  // End of test if disabling the combo
         else
         {
            // Check if enabling window, and edit control exists
            if ( (wParam == 1) && m_pReadOnlyEdit )
               m_pReadOnlyEdit->ShowWindow( SW_HIDE );     // Hide the edit control
         }  // End of test if NOT disabling the combo

         return( lRC );
      }  // End of test if enable/disable of combo called for
      break;

   // This message is sent when changing "modes" of the combobox
   // (i.e. the WM_ENABLE message doesn't appear to be called to disable
   //  the combobox as one would think!)
   case WM_CANCELMODE:
      DWORD dwStyle = GetStyle( );

      // See if we're disabling the control
      if ( (dwStyle & WS_DISABLED) == WS_DISABLED )
      {
         // Check if edit control has been created and has a valid windows handle
         if ( m_pReadOnlyEdit && m_pReadOnlyEdit->m_hWnd )
         {
            // Try to show the edit control
            m_pReadOnlyEdit->ShowWindow( SW_SHOW );
            m_pReadOnlyEdit->SetReadOnly( TRUE );
            m_pReadOnlyEdit->ModifyStyle( WS_TABSTOP, 0 );
            CString csComboText;
            GetWindowText( csComboText );
            m_pReadOnlyEdit->SetWindowText( csComboText );
         }  // End of pointer validation
      }  // Check if disabled flag is on

      break;
   }  // End of message trapping

   return( CComboBox::DefWindowProc( uMessage, wParam, lParam ) );
}  // CTComboBox::DefWindowProc( UINT uMessage, WPARAM wParam, LPARAM lParam )

#if 0
void
ZComboBox::SetColumnWidth( zSHORT nColumnWidth0, ... )
{
   va_list ColumnWidthPointer;
   zLONG lTotalColWidth;

   ASSERT( m_nTotalCols > 1 ); // call FormatComboBox first

   m_pnColumnWidth[ 0 ] = nColumnWidth0;
   lTotalColWidth = nColumnWidth0 + SPACE_BETWEEN_COLUMN +
                    LEFTMARGIN_OF_DROPDOWNLIST + RIGHTMARGIN_OF_DROPDOWNLIST;

   va_start( ColumnWidthPointer, nColumnWidth0 );
   for ( zSHORT nIdx = 1; nIdx < m_nTotalCols; nIdx++ )
   {
      m_pnColumnWidth[ nIdx ] = va_arg( ColumnWidthPointer, UINT );
      if ( m_pnColumnWidth[ nIdx ] > 0 )
         lTotalColWidth += (m_pnColumnWidth[ nIdx ] + SPACE_BETWEEN_COLUMN);
   }
   va_end( ColumnWidthPointer );

   if ( (GetWindowLong( m_hWnd, GWL_STYLE ) & 0x3) == CBS_SIMPLE )
   {
      CRect rectWnd;
      GetWindowRect( rectWnd );
      SetWindowPos( 0, 0, 0, lTotalColWidth, rectWnd.bottom - rectWnd.top, SWP_NOMOVE | SWP_NOZORDER );
   }
   else
   {
      SetDroppedWidth( lTotalColWidth );
   }
}

void
ZComboBox::SetColumnAlignStyle( zSHORT nColumnAlignStyle0, ... )
{
   va_list pColumnAlignStyle;

   ASSERT( m_nTotalCols > 1 ); // call FormatComboBox first

   m_plColumnAlignStyle[ 0 ] = nColumnAlignStyle0;

   va_start( pColumnAlignStyle, nColumnAlignStyle0 );
   for ( zSHORT nIdx = 1; nIdx < m_nTotalCols; nIdx++ )
      m_plColumnAlignStyle[ nIdx ] = va_arg( pColumnAlignStyle, zSHORT );

   va_end( pColumnAlignStyle );
}

zLONG
ZComboBox::AddRow( zCPCHAR cpcString0, ... )
{
   va_list pStringPtr;
   zSHORT  nIdx;
   zLONG   lItemIdx;
   zCPCHAR cpcString;

   ASSERT( m_nTotalCols > 1 ); // call FormatComboBox first

   if ( m_nBoundCol < 0 )
      lItemIdx = CComboBox::AddString( cpcString0 );
   else
   {
      va_start( pStringPtr, cpcString0 );
      for ( nIdx = 1; nIdx < m_nTotalCols; nIdx++ )
      {
         cpcString = va_arg( pStringPtr, zCPCHAR );
         if ( m_nBoundCol == nIdx )
            lItemIdx = CComboBox::AddString( cpcString );
      }
   }

   if ( lItemIdx == 0 )
   {
      m_pcsColItemList[ 0 ].AddHead( cpcString0 );
   }
   else
   {
      if ( lItemIdx == (zSHORT) CComboBox::GetCount( ) - 1 )
      {
         m_pcsColItemList[ 0 ].AddTail( cpcString0 );
      }
      else
      {
         m_pcsColItemList[ 0 ].
                   InsertBefore( m_pcsColItemList[ 0 ].FindIndex( lItemIdx ),
                                 cpcString0 );
      }
   }

   va_start( pStringPtr, cpcString0 );
   for ( nIdx = 1; nIdx < m_nTotalCols; nIdx++ )
   {
      cpcString = va_arg( pStringPtr, zCPCHAR );

      if ( lItemIdx == 0 )
      {
         m_pcsColItemList[ nIdx ].AddHead( cpcString );
      }
      else
      {
         if ( lItemIdx == (zSHORT) CComboBox::GetCount( ) - 1 )
         {
            m_pcsColItemList[ nIdx ].AddTail( cpcString );
         }
         else
         {
            m_pcsColItemList[ nIdx ].
                InsertBefore( m_pcsColItemList[ nIdx ].FindIndex( lItemIdx ), cpcString );
         }
      }
   }
   va_end( pStringPtr );

   return( lItemIdx );
}
#endif

void
ZComboBox::FormatComboBox( )
{
#ifndef zREMOTE_SERVER
   ASSERT( m_nTotalCols > 1 );   // use CComboBox class instead of this class

   if ( mIs_hWnd( m_hWnd ) == 0 )
      return;

// m_pnColumnWidth = new zSHORT[ m_nTotalCols ];
// m_plColumnAlignStyle = new zLONG[ m_nTotalCols ];
// m_pcsColItemList = new CStringList[ m_nTotalCols ];

   zLONG lComboBoxStyle = GetWindowLong( m_hWnd, GWL_STYLE ) & 0x3;
   m_nTotalColWidth = 0;
   if ( lComboBoxStyle != CBS_SIMPLE )
   {
      zSHORT nComboBoxWidth = GetDroppedWidth( ) - LEFTMARGIN_OF_DROPDOWNLIST - SPACE_BETWEEN_COLUMN - 1;

      m_nTotalColWidth = LEFTMARGIN_OF_DROPDOWNLIST + RIGHTMARGIN_OF_DROPDOWNLIST;

      ZComboColumn *pCol = m_pHeadCol;
      while ( pCol )
      {
         m_nTotalColWidth += (pCol->m_nColWidth + SPACE_BETWEEN_COLUMN);
         pCol = pCol->m_pNext;
      }

      SetDroppedWidth( m_nTotalColWidth );
   }

   if ( lComboBoxStyle == CBS_DROPDOWN )
   {
      m_pMCCEdit = new ZMCComboEdit( );
      m_pMCCEdit->SubclassWindow( (GetWindow( GW_CHILD ))->m_hWnd );
   }
   else
   if ( lComboBoxStyle == CBS_SIMPLE )
   {
      m_pMCCEdit = new ZMCComboEdit( );
      m_pMCCEdit->SubclassWindow( (GetWindow( GW_CHILD )->GetWindow( GW_HWNDNEXT ))->m_hWnd );
   }

   CDC *pDC = GetWindowDC( );
   m_nTextItemHeight = (zSHORT) (pDC->GetTextExtent( " ", 1 )).cy;
// SetItemHeight( -1, m_nTextItemHeight );  // ownerdraw variable
   SetItemHeight( 0, m_nTextItemHeight );   // ownerdraw fixed
   ReleaseDC( pDC );
#endif
}

void
ZComboBox::CreateZ( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "StateComboDKS" ) == 0 )
   {
      TraceLineS( "ZComboBox::CreateZ ", *m_pzsTag );
      TraceLineX( "ZComboBox::CreateZ MapActFlags: ", m_ulMapActFlags );
   }
#endif

   if ( m_ulMapActFlags & zMAPACT_VISIBLE )
      Attr.Style |= WS_VISIBLE;
   else
      Attr.Style &= ~WS_VISIBLE;

   Attr.Style &= ~WS_DISABLED;

   if ( m_bMultiCol )
      Attr.Style |= CBS_OWNERDRAWFIXED;
   else
      Attr.Style &= ~CBS_OWNERDRAWFIXED;

#ifdef zREMOTE_SERVER
   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
#else
   if ( mIs_hWnd( m_hWnd ) ||
        (m_pWndParent && mIs_hWnd( m_pWndParent->m_hWnd ) &&
         CreateEx( WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR |
                      WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
                   "COMBOBOX", *m_pzsText,
                   Attr.Style | WS_VSCROLL, Attr.X, Attr.Y, Attr.W, Attr.H,
                   m_pWndParent->m_hWnd, (HMENU) m_nIdNbr )) )
   {
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   }
#endif
}

#ifndef zREMOTE_SERVER

int
ZComboBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "StateComboDKS" ) == 0 )
   {
      TraceLineS( "ZComboBox::OnCreate ", *m_pzsTag );
      TraceLineX( "ZComboBox::OnCreate MapActFlags: ", m_ulMapActFlags );
   }
#endif

   // This has to be done prior to FormatComboBox since FormatComboBox
   // requires the hWnd to be valid.
   int nRC = (int) Default( );
   if ( nRC == -1 )
      return( -1 );

   SetWindowText( *m_pzsTag );
   if ( m_bMultiCol )
      FormatComboBox( );

   SetFontOverride( );

// CClientDC dc( this );
// CSize size = dc.GetTextExtent( "A", 1 );
// m_lExtentY = size.cy;
   m_lExtentY = 0;  // it looks like this is done for us in Win32

   if ( m_ulSubtype & zCOMBOBOX_EXTENDED_LIST )
      SetExtendedUI( TRUE );

// ModifyStyle( CBS_SORT, 0 );   // remove the sort style bit

   if ( m_bMultiCol )
   {
   // FormatComboBox( );
      if ( (GetWindowLong( m_hWnd, GWL_STYLE ) & 0x3) == CBS_SIMPLE )
      {
         CRect rectWnd;
         GetWindowRect( rectWnd );
         SetWindowPos( 0, 0, 0, m_nTotalColWidth,
                       rectWnd.bottom - rectWnd.top,
                       SWP_NOMOVE | SWP_NOZORDER );
      }
      else
      {
         SetDroppedWidth( m_nTotalColWidth );
      }
   }
   else
   if ( m_ulSubtype & zCOMBOBOX_SIMPLE )
   {
      BuildListFromOI( );
      m_nIsDroppedDown = zCB_OPENED;
   }

   CWnd *pWnd = GetWindow( GW_CHILD );
   if ( pWnd )
   {
      CRect rectWnd;

      m_hWndChild = pWnd->m_hWnd;
      pWnd->GetWindowRect( rectWnd );
   // m_pComboEdit = new ZComboEdit( this, pWnd );
   // TraceRect( *m_pzsTag, rectWnd );
   }

   return( nRC );
}

BOOL
ZComboBox::DestroyWindow( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineS( "ZComboBox::DestroyWindow ", *m_pzsTag );
      TraceLineX( "ZComboBox::DestroyWindow MapActFlags: ", m_ulMapActFlags );
   }
#endif
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CComboBox::DestroyWindow( ) );
}

#endif

LPDOMAIN
GetAppDomain( zVIEW vSubtask, zCPCHAR cpcDomainName )
{
   LPAPP lpApp;
   LPDOMAIN lpDomain;

   SfGetApplicationForSubtask( &lpApp, vSubtask );
   if ( lpApp )
   {
      for ( lpDomain = (LPDOMAIN) zGETPTR( lpApp->hFirstDomain );
            lpDomain;
            lpDomain = (LPDOMAIN) zGETPTR( lpDomain->hNextDomain ) )
      {
         if ( zstrcmp( lpDomain->szName, cpcDomainName ) == 0 )
            break;
      }
   }
   else
      lpDomain = 0;

   return( lpDomain );
}

zBOOL
FindDefaultContext( zCPCHAR *pcpcContext, zVIEW vApp,
                    zCPCHAR cpcEName, zCPCHAR cpcAName )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   zLPCONTEXT   lpContext;
   zBOOL  bRequired = FALSE;

   *pcpcContext = 0;
   lpViewEntity = MiGetViewEntityForView( vApp, cpcEName );
   if ( lpViewEntity )
   {
      for ( lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         if ( zstrcmp( lpViewAttrib->szName, cpcAName ) == 0 )
         {
            if ( lpViewAttrib->bRequired )
               bRequired = TRUE;

            LPDOMAIN lpDomain = (LPDOMAIN) zGETPTR( lpViewAttrib->hDomain );
            if ( GetDefaultContext( &lpContext, lpDomain ) &&
                 lpContext->szName[ 0 ] )
            {
               *pcpcContext = lpContext->szName;
            }

            break;
         }
      }
   }

   return( bRequired );
}

zSHORT
ZComboBox::BuildListFromOI( zBOOL bRefresh )
{
   zLONG   lCursor;
   zCHAR   szMap[ 256 ];
   zVIEW   vApp = 0;
   zSHORT  nCnt = 2;   // one for the edit box part of the combo box and
                       // one fudge factor
   zSHORT  nRC;

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "BuildListFromOI bListLoaded = ", m_bListLoaded );
#endif
   if ( (m_bListLoaded == TRUE && bRefresh == FALSE) ||
        (m_ulSubtype & zCOMBOBOX_EXTERNAL_DATA) )
   {
      return( 0 );
   }

// SetRedraw( FALSE );  // tried to do this ... repainting occurred improperly
   ResetContent( );

   m_bListLoaded = TRUE;
   if ( (m_ulSubtype & zCOMBOBOX_POSITION_ONLY) || m_bMultiCol )
   {
      if ( m_bMultiCol && m_vAppList )
      {
         DropView( m_vAppList );
         m_vAppList = 0;
      }

      if ( m_pzsListMapVName &&
           GetViewByName( &vApp, *m_pzsListMapVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         zVIEW   vAppList;
         CString *pzsEntity;
         zCPCHAR cpcBrowseScope = 0;
         zCPCHAR cpcMapContext = 0;
         zSHORT  nFlag = 0;

#ifdef zREMOTE_SERVER
         // If it's a sorted combobox, do an activate from OI and sort
         // the entities later on.
         if ( m_ulSubtype & zCOMBOBOX_SORT )
            ActivateOI_FromOI_ForTask( &vAppList, vApp, 0, zMULTIPLE );
         else
            CreateViewFromViewForTask( &vAppList, vApp, m_pZSubtask->m_vDialog );
#else
         CreateViewFromViewForTask( &vAppList, vApp, m_pZSubtask->m_vDialog );
#endif

         if ( m_pzsListBrowseEName )
            pzsEntity = m_pzsListBrowseEName;
         else
            pzsEntity = m_pzsListMapEName;

         if ( m_pzsListBrowseScopeEName )
            cpcBrowseScope = *m_pzsListBrowseScopeEName;

         if ( m_pzsListMapContext == 0 || m_pzsListMapContext->IsEmpty( ) )
         {
            zCPCHAR cpcContext;

            FindDefaultContext( &cpcContext, vApp, *pzsEntity, *m_pzsListMapAName );
            if ( cpcContext )
            {
               mDeleteInit( m_pzsListMapContext );
               m_pzsListMapContext = new CString( cpcContext );
            // TraceLineS( "ZComboBox default context ", cpcContext );
            }
         }

         if ( m_pzsListMapContext )
            cpcMapContext = *m_pzsListMapContext;
         else
         {
            // Borrow szMap to display the message ... this should not
            // happen!
            sprintf_s( szMap, sizeof( szMap ), "ZComboBox::BuildListFromOI unable to locate attribute %s for entity ",
                       (*m_pzsListMapAName).GetString() );
            TraceLineS( szMap, *pzsEntity );
         }

         if ( m_nDisplaySelSet )
            SetSelectSetForView( vAppList, m_nDisplaySelSet );

#ifdef zREMOTE_SERVER
         if ( CheckExistenceOfEntity( vAppList, *pzsEntity ) == 0 &&
              (m_ulSubtype & zCOMBOBOX_SORT) )
         {
            zCHAR szAttribute[ 35 ];
            strcpy_s( szAttribute, sizeof( szAttribute ), *m_pzsListMapAName );
            strcat_s( szAttribute, sizeof( szAttribute ), " A" );
            OrderEntityForView( vAppList, *pzsEntity, szAttribute );
         }
#endif

         nRC = SetEntityCursor( vAppList, *pzsEntity, 0,
                                zPOS_FIRST | m_ulFlag, 0, 0, 0, 0,
                                cpcBrowseScope, 0 );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                            sizeof( szMap ),
                                            vAppList, *m_pzsListMapEName,
                                            *m_pzsListMapAName,
                                            cpcMapContext, nFlag );
            if ( nRC == zVAR_NULL )
               szMap[ 0 ] = 0;

            TranslateValue( szMap, sizeof( szMap ), TRUE );

#ifdef zREMOTE_SERVER
            if ( AddString( szMap,
                            GetEntityKey( vAppList, *pzsEntity ),
                            nCnt ) < 0 )
#else
            if ( AddString( szMap ) < 0 )
#endif
            {
               TraceLineS( "Error adding string ", szMap );
            }

            nCnt++;
            nRC = SetEntityCursor( vAppList, *pzsEntity, 0,
                                   zPOS_NEXT | m_ulFlag, 0, 0, 0, 0,
                                   cpcBrowseScope, 0 );

         }  // end of:  while ( nRC > zCURSOR_UNCHA ... looping thru entities

#ifdef zREMOTE_SERVER
         // If its a sorted type of combobox, vAppList was created by
         // ActivateOIFromOI ... otherwise by CreateViewFromViewForTask.
         // Drop them properly.
         if( m_ulSubtype & zCOMBOBOX_SORT )
            DropObjectInstance( vAppList );
         else
            DropView( vAppList );
#else
         DropView( vAppList );
#endif

         if ( GetViewByName( &vApp, *m_pzsVName,
                             m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
         {
            if ( (m_ulSubtype & zCOMBOBOX_NO_NULL) == 0 )
            {
               zSHORT  nMinCardinality;
               zSHORT  nMaxCardinality;

               // Insert a blank line at the beginning if the cardinality
               // permits it.
               MiGetCardinalityForEntity( (zPUSHORT) &nMinCardinality,
                                          (zPUSHORT) &nMaxCardinality,
                                          vApp, *m_pzsEName );

               // If this combo box is permitted to have a null entry, then
               // put it in.
               if ( nMinCardinality == 0 )
               {
                  // Use "Home" for keyboard interface to null string since
                  // it is always at the top of the list.
#ifdef zREMOTE_SERVER
                  InsertString( "", 0, 0 );
#else
                  InsertString( 0, "" );
#endif
                  nCnt++;
               }
            }

            if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
            {
               GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                         sizeof( szMap ),
                                         vApp, *m_pzsEName,
                                         *m_pzsAName, *m_pzsContext,
                                         m_pzsContext->IsEmpty( ) ? zUSE_DEFAULT_CONTEXT : 0 );
               SelectExactString( -1, szMap );
            }
         }
      }
   }
   else                                   // it must be a domain fixed list
   if ( m_pzsVName &&
        GetViewByName( &vApp, *m_pzsVName,
                       m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0
#ifdef zREMOTE_SERVER
        && ( (m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI) &&
             CheckExistenceOfEntity( vApp, *m_pzsEName ) >= zCURSOR_SET )
#endif
      )
   {
      ZCorrelatedItem *pItem;
      zBOOL  bRequired = FALSE;

      if ( m_pTranslationList == 0 ||
           (pItem = m_pTranslationList->Find( (zLONG) 0 )) == 0 )
      {
      // if ( m_pzsContext->IsEmpty( ) )  always do it to determine "required"
         {
            zCPCHAR cpcContext;

            bRequired = FindDefaultContext( &cpcContext, vApp, *m_pzsEName, *m_pzsAName );
            if ( m_pzsContext->IsEmpty( ) && cpcContext )
            {
               mDeleteInit( m_pzsContext );
               m_pzsContext = new CString( cpcContext );
            // TraceLineS( "ZComboBox default context ", cpcContext );
            }
         }

         zBOOL  bNullAdded = FALSE;
         zBOOL  bFirstValue;
      // zSHORT nMinCardinality;
      // zSHORT nMaxCardinality;

      // MiGetCardinalityForEntity( (zPUSHORT) &nMinCardinality,
      //                            (zPUSHORT) &nMaxCardinality,
      //                            vApp, *m_pzsEName );

      // if ( zstrcmp( *m_pzsTag, "FamilyRole" ) == 0 )
      //    TraceLine( "ZComboBox FamilyRole (%s) minimum cardinality: %d",
      //               *m_pzsEName, nMinCardinality );

      // if ( nMinCardinality == 0 )
         if ( bRequired == FALSE )
         {
            nRC = 0;
            szMap[ 0 ] = 0;
            bFirstValue = FALSE;
         }
         else
         {
            nRC = GetFirstTableEntryForAttribute( szMap, sizeof( szMap ), vApp, *m_pzsEName, *m_pzsAName, *m_pzsContext, &lCursor );
            bFirstValue = TRUE;
         }

         // If cardinality permits it, the first table entry will be a null value, if this combobox is not to have a null entry, then
         // don't add it to the combo list.
         if ( szMap[ 0 ] == 0 && m_ulSubtype & zCOMBOBOX_NO_NULL )
            bNullAdded = TRUE;

         while ( nRC == 0 )
         {
            if ( szMap[ 0 ] || (szMap[ 0 ] == 0 && bNullAdded == FALSE && bRequired == FALSE) ) // nMinCardinality == 0) )
            {
               if ( szMap[ 0 ] == 0 )
                  bNullAdded = TRUE;

               nCnt++;
#ifdef zREMOTE_SERVER
               if ( AddString( szMap, nCnt, nCnt ) < 0 )
#else
            // if ( zstrcmp( *m_pzsTag, "FamilyRole" ) == 0 )
            //    TraceLineS( "ZComboBox adding FamilyRole: ", szMap );

               if ( AddString( szMap ) < 0 )
#endif
               {
                  TraceLineS( "Error adding string ", szMap );
               }
            }

            if ( bFirstValue )
            {
               nRC = GetNextTableEntryForAttribute( szMap, sizeof( szMap ), vApp, *m_pzsEName, *m_pzsAName, *m_pzsContext, &lCursor );
            }
            else
            {
               nRC = GetFirstTableEntryForAttribute( szMap, sizeof( szMap ), vApp, *m_pzsEName, *m_pzsAName, *m_pzsContext, &lCursor );

               bFirstValue = TRUE;
            }

         }  // end of:  while ( nRC == 0 )  ... looping thru entrys
      }
      else
      {
         zPCHAR pchInternal;
         zPCHAR pchDisplay;
         zPCHAR pchSemiInternal;
         zPCHAR pchSemiDisplay;
         zPCHAR pch =
                pItem->m_csValue.GetBuffer( pItem->m_csValue.GetLength( ) );

         while ( pch && *pch )
         {
            pchSemiInternal = zstrchr( pch, ';' );
            if ( pchSemiInternal )
            {
               pchInternal = pch;
               *pchSemiInternal = 0;
               pch = pchSemiInternal + 1;
               pchDisplay = pch;
               pchSemiDisplay = zstrchr( pch, ';' );
               if ( pchSemiDisplay )
               {
                  *pchSemiDisplay = 0;
                  pch = pchSemiDisplay + 1;
               }
               else
               {
                  pch = 0;
               }

               strcpy_s( szMap, sizeof( szMap ), pchDisplay );
               *pchSemiInternal = ';';
               if ( pchSemiDisplay )
                  *pchSemiDisplay = ';';

               nCnt++;

#ifdef zREMOTE_SERVER
               if ( AddString( szMap, nCnt, nCnt ) < 0 )
#else
               if ( AddString( szMap ) < 0 )
#endif
               {
                  TraceLineS( "Error adding string ", szMap );
               }
            }
            else
               pch = 0;
         }

         pItem->m_csValue.ReleaseBuffer( -1 );

      }

      GetVariableFromAttribute( szMap, 0, zTYPE_STRING, sizeof( szMap ),
                                vApp, *m_pzsEName, *m_pzsAName,
                                *m_pzsContext,
                                m_pzsContext->IsEmpty( ) ? zUSE_DEFAULT_CONTEXT : 0 );

      TranslateValue( szMap, sizeof( szMap ), TRUE );

#ifdef zREMOTE_SERVER
      if ( GetCount( ) == 0 && szMap[ 0 ] )
      {
         if ( AddString( szMap, GetEntityKey( vApp, *m_pzsEName ), 0  ) < 0 )
            TraceLineS( "Error adding string ", szMap );
      }
#endif

      SelectExactString( -1, szMap );
      Invalidate( TRUE );
   }
   else
   {
#ifdef zREMOTE_SERVER
      // Transfer data to KZXRA.
      MapToRemoteDataset( );
#endif

      Invalidate( TRUE );
      return( -1 );
   }

#if 0    // it appears that this is done for us in 32-bit Windows
   // set the dropdown length to a minimum of 1 item and a maximum of
   // the number of items in the list
   if ( !(m_ulSubtype & zCOMBOBOX_SIMPLE) )
   {
      // the 3 below is one for the size of the editbox + 1 fudge factor + 1
      // for an item.  Note that Attr.H is set to the size of the combobox
      // when folded up, so it is of no value in these calculations
      Attr.H = max( m_lExtentY * 3, min( m_lExtentY * nCnt, m_lOrigSize ) );

      ::MoveWindow( m_hWnd, Attr.X, Attr.Y, Attr.W, Attr.H, TRUE );
   }
#endif

#ifdef zREMOTE_SERVER
   // Transfer data to KZXRA.
   MapToRemoteDataset( );
#endif

   return( 0 );
}

void
ZComboBox::SetDisplaySelSet( zSHORT nSelSet )
{
   m_nDisplaySelSet = nSelSet;
   if ( m_nDisplaySelSet )
      m_ulFlag |= zQUAL_SELECTED;
   else
   if ( m_ulFlag & zQUAL_SELECTED )
      m_ulFlag &= ~zQUAL_SELECTED;
}

void
ZComboBox::SetSelectSelSet( zSHORT nSelSet )
{
   m_nSelectSelSet = nSelSet;
}

zSHORT
ZComboBox::SetData( zCPCHAR cpcSelectItem, zCPCHAR cpcListData )
{
   zLONG lLth;

   m_ulSubtype |= zCOMBOBOX_EXTERNAL_DATA;
   mDeleteInitA( m_pchSelectItem );
   lLth = zstrlen( cpcSelectItem ) + 1;
   m_pchSelectItem = new char[ lLth ];
   strcpy_s( m_pchSelectItem, lLth, cpcSelectItem );
   lLth = zstrlen( cpcListData ) + 1;
   mDeleteInit( m_pchListData );
   m_pchListData = new char[ lLth ];
   strcpy_s( m_pchListData, lLth, cpcListData );
   mDeleteInit( m_pzsVName );
   mDeleteInit( m_pzsEName );
   mDeleteInit( m_pzsAName );
   if ( m_ulMapActFlag2 & zMAPACT_MAPPED_FROM_OI )
      MapFromOI( );

   return( 0 );
}

zSHORT
ZComboBox::SetDataFromOI( zCPCHAR cpcSelectItem,
                          zVIEW   vAppOI,
                          zCPCHAR cpcEName,
                          zCPCHAR cpcAName,
                          zCPCHAR cpcContext,
                          zVIEW   vAppBrowseOI,
                          zCPCHAR cpcBrowseEName,
                          zCPCHAR cpcBrowseAName,
                          zCPCHAR cpcBrowseScope,
                          zCPCHAR cpcBrowseContext,
                          zSHORT  nDisplaySelSet,
                          zULONG  ulFlag )
{
   zCHAR   szMap[ 256 ];
// zVIEW   vApp = 0;
   zSHORT  nCnt = 2;   // one for the edit box part of the combo box and
                       // one fudge factor
   zSHORT  nRC;

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "SetDataFromOI bListLoaded = ", m_bListLoaded );
#endif

   zVIEW   vAppList;
   zCPCHAR cpcMapContext = 0;
   zSHORT  nOldSet;
   zSHORT  nFlag = 0;

   CreateViewFromViewForTask( &vAppList, vAppBrowseOI, m_pZSubtask->m_vDialog );

   if ( cpcBrowseContext == 0 || cpcBrowseContext[ 0 ] == 0 )
   {
      FindDefaultContext( &cpcBrowseContext, vAppList, cpcBrowseEName, cpcBrowseAName );
      // TraceLineS( "ZComboBox default context ", cpcContext );
   }

   if ( cpcBrowseContext && cpcBrowseContext[ 0 ] )
      cpcMapContext = cpcBrowseContext;
   else
   {
      // Borrow szMap to display the message ... this should not happen!
      sprintf( szMap, "ZComboBox::SetDataFromOI unable to locate attribute %s for entity ",
               cpcBrowseAName );
      TraceLineS( szMap, cpcBrowseScope );
   }

   if ( nDisplaySelSet )
      nOldSet = SetSelectSetForView( vAppList, nDisplaySelSet );

   nRC = SetEntityCursor( vAppList, cpcBrowseEName, 0,
                          zPOS_FIRST | ulFlag, 0, 0, 0, 0,
                          cpcBrowseScope, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                      sizeof( szMap ),
                                      vAppList, cpcBrowseEName,
                                      cpcBrowseAName,
                                      cpcMapContext, nFlag );
      if ( nRC == zVAR_NULL )
         szMap[ 0 ] = 0;

      TranslateValue( szMap, sizeof( szMap ), TRUE );

      if ( AddString( szMap ) < 0 )
      {
         TraceLineS( "Error adding string ", szMap );
      }

      nCnt++;
      nRC = SetEntityCursor( vAppList, cpcBrowseEName, 0,
                             zPOS_NEXT | (ulFlag & ~zCOMBOBOX_NO_NULL),
                             0, 0, 0, 0, cpcBrowseScope, 0 );

   }  // end of:  while ( nRC > zCURSOR_UNCHA ... looping thru entities

   DropView( vAppList );

// if ( GetViewByName( &vApp, cpcVName,
//                     m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   if ( vAppOI )
   {
      if ( (ulFlag & zCOMBOBOX_NO_NULL) == 0 )
      {
         zSHORT  nMinCardinality;
         zSHORT  nMaxCardinality;

         // Insert a blank line at the beginning if the cardinality
         // permits it.
         MiGetCardinalityForEntity( (zPUSHORT) &nMinCardinality,
                                    (zPUSHORT) &nMaxCardinality,
                                    vAppOI, cpcEName );

         // If this combo box is permitted to have a null entry, then
         // put it in.
         if ( nMinCardinality == 0 )
         {
            // Use "Home" for keyboard interface to null string since
            // it is always at the top of the list.
            InsertString( 0, "" );
            nCnt++;
         }
      }

      if ( CheckExistenceOfEntity( vAppOI, cpcEName ) == zCURSOR_SET )
      {
         GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                   sizeof( szMap ),
                                   vAppOI, cpcEName,
                                   cpcAName, cpcContext,
                                   cpcContext == 0 || cpcContext[ 0 ] == 0 ?
                                      zUSE_DEFAULT_CONTEXT : 0 );
         SelectExactString( -1, szMap );
      }
   }

   return( 0 );
}

zSHORT
ZComboBox::SetDataFromDomain( zCPCHAR cpcSelectItem,
                              zCPCHAR cpcVName,
                              zCPCHAR cpcEName,
                              zCPCHAR cpcAName,
                              zCPCHAR cpcContext,
                              zULONG  ulFlag )
{
   zLONG   lCursor;
   zCHAR   szMap[ 256 ];
   zVIEW   vAppOI = 0;
   zSHORT  nCnt = 2;   // one for the edit box part of the combo box and
                       // one fudge factor
   zSHORT  nRC;


   zCPCHAR cpcBrowseScope = 0;
   zCPCHAR cpcMapContext = 0;
   zSHORT  nFlag = 0;

   // It must be a domain fixed list.
   if ( cpcVName &&
        GetViewByName( &vAppOI, cpcVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
   if ( vAppOI )
   {
      ZCorrelatedItem *pItem;
      zBOOL  bRequired = FALSE;

   if ( m_pTranslationList == 0 ||
        (pItem = m_pTranslationList->Find( (zLONG) 0 )) == 0 )
      {
      // if ( m_pzsContext->IsEmpty( ) )  always do it to determine "required"
         {
            zCPCHAR cpcDfltContext;

            bRequired = FindDefaultContext( &cpcDfltContext, vAppOI, cpcEName, cpcAName );
            if ( (cpcContext == 0 || cpcContext[ 0 ] == 0) && cpcDfltContext )
            {
               cpcContext = cpcDfltContext;
            // TraceLineS( "ZComboBox default context ", cpcContext );
            }
         }

         zBOOL  bNullAdded = FALSE;
         zBOOL  bFirstValue;

         if ( bRequired == FALSE )
         {
            nRC = 0;
            szMap[ 0 ] = 0;
            bFirstValue = FALSE;
         }
         else
         {
            nRC = GetFirstTableEntryForAttribute( szMap, sizeof( szMap ), vAppOI, cpcEName, cpcAName, cpcContext, &lCursor );
            bFirstValue = TRUE;
         }

         // If cardinality permits it, the first table entry will be a
         // null value, if this combobox is not to have a null entry, then
         // don't add it to the combo list.
         if ( szMap[ 0 ] == 0 && ulFlag & zCOMBOBOX_NO_NULL )
            bNullAdded = TRUE;

         while ( nRC == 0 )
         {
            if ( szMap[ 0 ] ||
                 (szMap[ 0 ] == 0 && bNullAdded == FALSE && bRequired == FALSE) ) // nMinCardinality == 0) )
            {
               if ( szMap[ 0 ] == 0 )
                  bNullAdded = TRUE;

               nCnt++;

               if ( AddString( szMap ) < 0 )
               {
                  TraceLineS( "Error adding string ", szMap );
               }
            }

            if ( bFirstValue )
            {
               nRC = GetNextTableEntryForAttribute( szMap, sizeof( szMap ), vAppOI, cpcEName, cpcAName, cpcContext, &lCursor );
            }
            else
            {
               nRC = GetFirstTableEntryForAttribute( szMap, sizeof( szMap ), vAppOI, cpcEName, cpcAName, cpcContext, &lCursor );

               bFirstValue = TRUE;
            }

         }  // end of:  while ( nRC == 0 )  ... looping thru entrys
      }
      else
      {
         zPCHAR pchInternal;
         zPCHAR pchDisplay;
         zPCHAR pchSemiInternal;
         zPCHAR pchSemiDisplay;
         zPCHAR pch = pItem->m_csValue.GetBuffer( pItem->m_csValue.GetLength( ) );

         while ( pch && *pch )
         {
            pchSemiInternal = zstrchr( pch, ';' );
            if ( pchSemiInternal )
            {
               pchInternal = pch;
               *pchSemiInternal = 0;
               pch = pchSemiInternal + 1;
               pchDisplay = pch;
               pchSemiDisplay = zstrchr( pch, ';' );
               if ( pchSemiDisplay )
               {
                  *pchSemiDisplay = 0;
                  pch = pchSemiDisplay + 1;
               }
               else
               {
                  pch = 0;
               }

               strcpy_s( szMap, sizeof( szMap ), pchDisplay );
               *pchSemiInternal = ';';
               if ( pchSemiDisplay )
                  *pchSemiDisplay = ';';

               nCnt++;

               if ( AddString( szMap ) < 0 )
               {
                  TraceLineS( "Error adding string ", szMap );
               }
            }
            else
               pch = 0;
         }

         pItem->m_csValue.ReleaseBuffer( -1 );

      }

      GetVariableFromAttribute( szMap, 0, zTYPE_STRING, sizeof( szMap ),
                                vAppOI, cpcEName, cpcAName, cpcContext,
                                cpcContext == 0 || cpcContext[ 0 ] == 0 ? zUSE_DEFAULT_CONTEXT : 0 );

      TranslateValue( szMap, sizeof( szMap ), TRUE );

      SelectExactString( -1, szMap );
      Invalidate( TRUE );
   }
   else
   {
      Invalidate( TRUE );
      return( -1 );
   }

   return( 0 );
}

zSHORT
ZComboBox::GetSelectedString( zPCHAR pchSelectedString, zLONG lMaxLth )
{
   zLONG  lIdx = GetCurSel( );

   if ( lIdx >= 0 )
   {
      GetLBText( lIdx, pchSelectedString );
      return( (zSHORT) lIdx );
   }
   else
   {
      if ( m_pchSelectItem )
         strcpy_s( pchSelectedString, lMaxLth, m_pchSelectItem );
      else
         *pchSelectedString = 0;

      return( -2 );
   }
}

// Probably should be named "SelectExactStringButIfExactIsNotFoundChooseFirstItemThatSatisifiesRequirement".
zLONG
ZComboBox::SelectExactString( zLONG lStartAfter, zCPCHAR cpcSearchString )
{
   zLONG lPos;
   zLONG lLastPos;
   zLONG lFirstPos = SelectString( -1, cpcSearchString );
   zLONG lLth = GetLBTextLen( lFirstPos );
   zLONG lSearchLth = zstrlen( cpcSearchString );
   zLONG lCnt = GetCount();

// TraceLine( "ZComboBox::SelectExactString for String: %s  Count: %d   Position1: %d", cpcSearchString, lCnt, lFirstPos );

   lPos = lFirstPos;
   while ( lLth != lSearchLth )
   {
      if ( lLth == CB_ERR )
      {
      // TraceLine( "ZComboBox::SelectExactString for String: %s  Position2: %d", cpcSearchString, lLth );
         SetCurSel( lFirstPos );
         return( lFirstPos );
      }

      lLastPos = lPos;
      lPos = SelectString( lPos, cpcSearchString );
   // TraceLine( "ZComboBox::SelectExactString for String: %s  Position3: %d", cpcSearchString, lPos );
      lLth = GetLBTextLen( lPos );
      if ( lLastPos == lPos || lLastPos == lFirstPos )
         break;
   }

   return( lPos );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//   Automatic graying is somewhat complex for combo boxes.  There are three
//   cases ...
//
//       1.  Domains ... simple since there is one view and one entity
//           involved.  If the entity does not exist or is not updateable,
//           the control is grayed.
//
//       2.  Foreign Key ... still somewhat simple.  The source and target
//           entities must exist and the target entity must be updateable,
//           otherwise, the control is grayed.
//
//       3.  Includable ... the tough case.  The source entity must exist
//           and the target entity must be updateable, otherwise the control
//           is grayed.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZComboBox::MapFromOI( WPARAM wFlag )
{
   // If the control is not Visible, do not do mapping.
   if ( IsVisibleForMapFromOI( ) == FALSE )
      return( 0 );

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineS( "ZComboBox::MapFromOI ", *m_pzsTag );
      TraceLineX( "ZComboBox::MapFromOI MapActFlags: ", m_ulMapActFlags );
      if ( m_pzsVName )
         TraceLineS( "ZComboBox View ", *m_pzsVName );
      if ( m_pzsEName )
         TraceLineS( "ZComboBox Entity ", *m_pzsEName );
      if ( m_pzsAName )
         TraceLineS( "ZComboBox Attribute ", *m_pzsAName );
   }
#endif

   m_ulMapActFlag2 |= zMAPACT_MAPPED_FROM_OI1;
   zBOOL bDomain = FALSE;
   m_bListLoaded = FALSE;
   m_bWasSelected = FALSE;    // added for XC 243
   ResetContent( );
   Clear( );
   SetCurSel( -1 );
   if ( m_pzsVName && m_pzsEName && m_pzsAName )
   {
      zCHAR   szMap[ 256 ];
      zVIEW   vApp;
      zUSHORT uMapState;

      m_ulMapActFlag2 |= zMAPACT_MAPPING_FROM_OI;
      if ( m_ulSubtype & zCOMBOBOX_POSITION_ONLY )
      {
#ifdef DEBUG_ALL
         if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
            TraceLineS( "ZComboBox zCOMBOBOX_POSITION_ONLY ", *m_pzsTag );
#endif
         if ( m_nDisplaySelSet )
         {
            zCPCHAR cpcBrowseScope = 0;
            zSHORT  nPrevSelSet = 0;

            if ( GetViewByName( &vApp, *m_pzsListMapVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
            {
               nPrevSelSet = SetSelectSetForView( vApp, m_nDisplaySelSet );
            }

            if ( m_pzsListBrowseScopeEName )
               cpcBrowseScope = *m_pzsListBrowseScopeEName;

            if ( vApp == 0 ||
                 SetEntityCursor( vApp, *m_pzsListMapEName, 0,
                                  zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN,
                                  0, 0, 0, 0, cpcBrowseScope, 0 ) < 0 )
            {
#ifdef DEBUG_ALL
               if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
               {
                  TraceLineI( "(zdctlcmb) Disabling combobox based on Display Sel Set ",
                              m_nDisplaySelSet );
               }
#endif
               if ( nPrevSelSet )
                  SetSelectSetForView( vApp, nPrevSelSet );

               SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );
               m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
               return( 0 );
            }

            SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
            SetSelectSetForView( vApp, nPrevSelSet );
         }

         if ( m_ulSubtype & zCOMBOBOX_FOREIGN_KEY )
         {
         // uMapState = GetMappingAutoGray( &vApp, *m_pzsListMapVName, *m_pzsListMapEName, 0 );
#if 1
            uMapState = 0;

            // We don't need the MapState to be set, but we do need the
            // right view for later processing.
            if ( m_pzsListMapVName &&
                 GetViewByName( &vApp, *m_pzsListMapVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 &&
                 CheckExistenceOfEntity( vApp, *m_pzsListMapEName ) == zCURSOR_SET &&
                 GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
            {
               if ( MiGetUpdateForViewEntityAttr( vApp, *m_pzsEName, *m_pzsAName, 0 ) > 0 )
               {
                  uMapState |= zMAPACT_ENTITY_UPDATEABLE;
                  uMapState |= zMAPACT_ENTITY_EXISTS;
                  SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
               }
               else
               {
                  szMap[ 0 ] = 0;
                  GetVariableFromAttribute( szMap, 0, zTYPE_STRING, sizeof( szMap ),
                                            vApp, *m_pzsEName, *m_pzsAName,
                                            *m_pzsContext, m_pzsContext->IsEmpty( ) ?
                                                           zUSE_DEFAULT_CONTEXT : 0 );

                  TranslateValue( szMap, sizeof( szMap ), TRUE );

#ifdef zREMOTE_SERVER
                  if ( AddString( szMap, GetEntityKey( vApp, *m_pzsEName ), 0 ) < 0 )
#else
                  if ( AddString( szMap ) < 0 )
#endif
                  {
                     TraceLineS( "Error adding string: ", szMap );
                  }

                  SelectExactString( -1, szMap );

                  vApp = 0;
                  SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );
               }
            }
            else
            {
               vApp = 0;
               SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );
            }

#else
            if ( uMapState & zMAPACT_ENTITY_EXISTS )
            {
               uMapState = GetMappingAutoGray( &vApp, *m_pzsVName, *m_pzsEName, *m_pzsAName );
            }
#endif
         }
         else
         {
            uMapState = 0;
            vApp = 0;
            if ( m_pzsListMapVName == 0 )
            {
               TraceLineS( "zdctlcmb - Combo List view name is null ", *m_pzsTag );
               vApp = 0;
            }
            else
            if ( GetViewByName( &vApp, *m_pzsListMapVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
            {
            // if ( CheckExistenceOfEntity( vApp,
            //                              *m_pzsListMapEName ) != zCURSOR_NULL )   //??? SCOPE
               zCPCHAR cpcBrowseScope = 0;

               if ( m_pzsListBrowseScopeEName )
                  cpcBrowseScope = *m_pzsListBrowseScopeEName;

               if ( m_nDisplaySelSet )
               {
                  // If there are no entities to list ... quit.
                  SetSelectSetForView( vApp, m_nDisplaySelSet );
                  if ( SetEntityCursor( vApp, *m_pzsListMapEName, 0,
                                        m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT | zQUAL_SELECTED,
                                        0, 0, 0, 0, cpcBrowseScope, 0 ) >= 0 )
                  {
                     uMapState |= zMAPACT_ENTITY_EXISTS;
                  }
               }
               else
               {
                  // If there are no entities to list ... quit.
                  if ( SetEntityCursor( vApp, *m_pzsListMapEName, 0,
                                        m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                                        0, 0, 0, 0, cpcBrowseScope, 0 ) >= 0 )
                  {
                     uMapState |= zMAPACT_ENTITY_EXISTS;
                  }
               }

               // The target view must be updateable ... otherwise disable
               // the combobox.
               if ( (m_ulSubtype & zCOMBOBOX_INCLUDE) == 0 )
               {
                  if ( GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 &&
                       MiGetUpdateForView( vApp ) > 0 )
                  {
                     uMapState |= zMAPACT_ENTITY_UPDATEABLE;
                  }

                  GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
               }
               else
               if ( GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
               {
                  if ( MiGetUpdateForViewEntityAttr( vApp, *m_pzsEName, 0, zUPD_INCLUDE ) > 0 )
                  {
                     uMapState |= zMAPACT_ENTITY_UPDATEABLE; // ==> updateable
                  }
               }
            }

            // Auto Graying, if the view does not permit update for the
            // mapping entity or the entity instance does not exist,
            // disable control.
            if ( (uMapState & zMAPACT_ENTITY_EXISTS) &&
                 (uMapState & zMAPACT_ENTITY_UPDATEABLE) )
            {
               SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, TRUE );
            }
            else
               SetZCtrlState( zCONTROL_STATUS_AUTOGRAY, FALSE );

            if ( vApp == 0 ||
                 CheckExistenceOfEntity( vApp, *m_pzsEName ) != zCURSOR_SET )
            {
               uMapState = 0;
            }
         }
      }
      else  // domain processing
      {
#ifdef DEBUG_ALL
         if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
            TraceLineS( "ZComboBox Domain Processing", "" );
#endif
         bDomain = TRUE;
         uMapState = GetMappingAutoGray( &vApp, *m_pzsVName, *m_pzsEName, *m_pzsAName );
      }

#ifdef DKS_TEST
      if ( m_ulSubtype & zCOMBOBOX_INCLUDE )
         TraceLineS( "ZComboBox vApp ", "zCOMBOBOX_INCLUDE" );
      else
         TraceLineS( "ZComboBox vApp ", "NOT zCOMBOBOX_INCLUDE" );
#endif

//    if ( vApp && ((uMapState & zMAPACT_ENTITY_EXISTS) || m_ulSubtype & zCOMBOBOX_INCLUDE) )
      if ( vApp &&
           ((uMapState & zMAPACT_ENTITY_EXISTS) || bDomain == FALSE) &&
           CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
      {
         szMap[ 0 ] = 0;
         GetVariableFromAttribute( szMap, 0, zTYPE_STRING, sizeof( szMap ),
                                   vApp, *m_pzsEName, *m_pzsAName,
                                   *m_pzsContext, m_pzsContext->IsEmpty( ) ? zUSE_DEFAULT_CONTEXT : 0 );

         TranslateValue( szMap, sizeof( szMap ), TRUE );

#ifdef zREMOTE_SERVER
         if ( AddString( szMap, GetEntityKey( vApp, *m_pzsEName ), 0 ) < 0 )
#else
         if ( AddString( szMap ) < 0 )
#endif
         {
            TraceLineS( "Error adding string: ", szMap );
         }
#ifdef DKS_TEST
         else
            TraceLineS( "Added COMBO string: ", szMap );

         TraceLineS( "ZComboBox szMap: ", szMap );
#endif
//       if ( m_ulSubtype & zCOMBOBOX_INCLUDE )
//          SetZCtrlText( this, szMap );
//       else
         {
            SelectExactString( -1, szMap );
         }
      }

#ifdef zREMOTE_SERVER
      BuildListFromOI( );
#else
      if ( m_nIsDroppedDown & zCB_OPENED )
         BuildListFromOI( );
#endif

#if 1
      if ( (m_ulMapActFlags & zMAPACT_ENABLED) == 0 )
      {
         EnableWindow( FALSE );
      // ModifyStyle( 0, WS_DISABLED );
      }
#endif

      m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
      return( 0 );
   }
   else
   if ( m_ulSubtype & zCOMBOBOX_EXTERNAL_DATA )
   {
      zPCHAR pch;
      zPCHAR pchSemi;
      zLONG  lKey = 0;

      pch = m_pchListData;
      while ( pch && *pch )
      {
         lKey++;
         pchSemi = zstrchr( pch, ';' );
         if ( pchSemi )
            *pchSemi = 0;

#ifdef zREMOTE_SERVER
         if ( AddString( pch, lKey, 0 ) < 0 )
#else
         if ( AddString( pch ) < 0 )
#endif
         {
            TraceLineS( "Error adding string: ", pch );
         }
         else
            TraceLine( "Added string: '%s'", pch );

         if ( pchSemi )
         {
            *pchSemi = ';';
            pch = pchSemi + 1;
         }
         else
            pch = 0;
      }

      if ( m_pchSelectItem )
         SelectExactString( -1, m_pchSelectItem );

      m_ulMapActFlag2 &= ~zMAPACT_MAPPING_FROM_OI;
   }

   return( -1 );
}

// Get the text for the combo box control and map it to the data OI.
zSHORT
ZComboBox::MapToOI( zLONG lFlag )
{
   zVIEW vApp = 0;

   // Note that if ViewName is set ... EntityName and AttributeName
   // must be set as required for a particular control???

   // If the control is not Visible and Enabled, or if the view is not found,
   // do not do mapping.
   if ( m_pzsVName == 0 || (vApp = IsVisibleForMapToOI( *m_pzsVName )) == 0 )
      return( 0 );

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "FullName" ) == 0 )
   {
      TraceLineS( "ZComboBox::MapToOI ", *m_pzsTag );
      TraceLineX( "ZComboBox::MapToOI MapActFlags: ", m_ulMapActFlags );
   }
#endif

   zCHAR  szMap[ 256 ];
   zLONG  lIdx = GetCurSel( );
   zSHORT nRC = 0;

   if ( lIdx >= 0 )
      GetLBText( lIdx, szMap );
   else
   {
#ifdef zREMOTE_SERVER
      GetZCtrlText( this, szMap, sizeof( szMap ) );
#else
      GetWindowText( szMap, sizeof( szMap ) - 1 );
#endif
   }

   TranslateValue( szMap, sizeof( szMap ), FALSE );

   if ( m_ulSubtype & zCOMBOBOX_POSITION_ONLY )
   {
      zVIEW vMap = 0;

      if ( m_pzsListMapVName &&
           GetViewByName( &vMap, *m_pzsListMapVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) > 0 )
      {
         CString *pzsEntity;
         zSHORT  nPrevSelSet;
         zCPCHAR cpcBrowseScope = 0;
         zCPCHAR cpcMapContext = 0;

         // Use the Browse scope entity name to browse if specified,
         // otherwise use the Mapping entity name as the default to
         // traverse.
         if ( m_pzsListBrowseScopeEName )
         {
            pzsEntity = m_pzsListMapEName;
            cpcBrowseScope = *m_pzsListBrowseScopeEName;
         }
         else
         if ( m_pzsListBrowseEName )
         {
            pzsEntity = m_pzsListMapEName;
            cpcBrowseScope = *m_pzsListBrowseEName;
         }
         else
         {
            pzsEntity = m_pzsListMapEName;
         }

         if ( m_pzsListMapContext )
            cpcMapContext = *m_pzsListMapContext;

         if ( m_nDisplaySelSet )
            nPrevSelSet = SetSelectSetForView( vMap, m_nDisplaySelSet );
         else
            nPrevSelSet = 0;

         if ( szMap[ 0 ] == 0 ||
              SetEntityCursor( vMap, *pzsEntity,
                               *m_pzsListMapAName,
                               zPOS_FIRST | zQUAL_STRING | zSPARENT_KEEPCHILDREN | m_ulFlag,
                               szMap, 0, 0, 0, cpcBrowseScope, cpcMapContext ) >= 0 ||
              ((m_ulSubtype & zCOMBOBOX_DROPDOWN) &&
               (m_ulSubtype & zCOMBOBOX_INCLUDE) == 0) )
         {
            if ( m_ulSubtype & zCOMBOBOX_SELECT )
            {
               if ( nPrevSelSet == 0 )
                  nPrevSelSet = SetSelectSetForView( vMap, m_nSelectSelSet );
               else
                  SetSelectSetForView( vMap, m_nSelectSelSet );

               SetAllSelectStatesForEntity( vMap, *m_pzsListMapEName, FALSE, cpcBrowseScope );
               if ( szMap[ 0 ] )
                  SetSelectStateOfEntity( vMap, *m_pzsListMapEName, TRUE );
            }

            if ( vApp )
            {
               if ( m_ulSubtype & zCOMBOBOX_INCLUDE )
               {
                  if ( m_bWasSelected )
                  {
                     if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) == zCURSOR_SET )
                        ExcludeEntity( vApp, *m_pzsEName, zREPOS_BEFORE );

                     if ( szMap[ 0 ] )
                     {
                        nRC = IncludeSubobjectFromSubobject( vApp, *m_pzsEName, vMap, *m_pzsListMapEName, zPOS_AFTER );
                     }

                     m_bWasSelected = FALSE;
                  }
                  else
                  if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) < zCURSOR_SET )
                  {
                     zSHORT  nMinCardinality;
                     zSHORT  nMaxCardinality;

                     // if the user has not yet made a selection and minimum
                     // cardinality requires a selection ... flag it
                     if ( m_ulSubtype & zCOMBOBOX_NO_NULL )
                        nMinCardinality = 1;
                     else
                        MiGetCardinalityForEntity( (zPUSHORT) &nMinCardinality,
                                                   (zPUSHORT) &nMaxCardinality,
                                                   vApp, *m_pzsEName );
                     if ( nMinCardinality )
                     {
                        // "KZOEE245 - Null string not allowed for a required attribute"
                        IssueOE_Error( vApp, 8, 245,
                                       zMSGQ_REQUIRED_DATA_ITEM_ERROR,
                                       0, *m_pzsEName, *m_pzsAName );
                        return( -1 );
                     }
                  }
               }
               else
               {
                  if ( m_ulSubtype & zCOMBOBOX_FOREIGN_KEY )
                  {
                     SetAttributeFromVariable( vApp, *m_pzsEName,
                                               *m_pzsAName, szMap,
                                               zTYPE_STRING, 0,
                                               *m_pzsContext,
                                               (zSHORT) (m_pzsContext->IsEmpty( ) ?
                                                lFlag | zUSE_DEFAULT_CONTEXT : lFlag) );
                  }
               }
            }
         }

         if ( nPrevSelSet )
            SetSelectSetForView( vMap, nPrevSelSet );
      }
   }
   else  // it must be a domain fixed list
   if ( vApp )
   {
      if ( CheckExistenceOfEntity( vApp, *m_pzsEName ) != 0 )
         return( 0 );

      nRC = SetAttributeFromVariable( vApp, *m_pzsEName,
                                      *m_pzsAName, szMap,
                                      zTYPE_STRING, 0,
                                      *m_pzsContext,
                                      (zSHORT) (m_pzsContext->IsEmpty( ) ?
                                         lFlag | zUSE_DEFAULT_CONTEXT : lFlag) );
   }

#ifndef zREMOTE_SERVER
   SetZCtrlText( this, szMap );
#endif
   return( nRC );
}

zLONG
ZComboBox::GetZCtrlState( zLONG lStatusType )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZComboBox::GetZCtrlState ", *m_pzsTag );
#endif

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
ZComboBox::SetZCtrlState( zLONG  lStatusType,
                          zULONG ulValue )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZComboBox::SetZCtrlState ", *m_pzsTag );
#endif

#ifdef zREMOTE_SERVER

   if ( lStatusType == zCONTROL_STATUS_SELECT_INDEX )
   {
      SetCurSel( ulValue );
      ProcessCommand( zCB_SELCHANGE, -2 );
      return( 0 );
   }

#else

   if ( lStatusType == zCONTROL_STATUS_DISABLE_READONLY )
   {
      zLONG lRC = ZMapAct::SetZCtrlState( lStatusType, ulValue );
      if ( ulValue )
      {
         if ( m_pReadOnlyEdit == 0 )
            m_pReadOnlyEdit = new CEdit( );
      }
      else
      {
         mDeleteInit( m_pReadOnlyEdit );
      }

      return( lRC );
   }

#endif

   return( ZMapAct::SetZCtrlState( lStatusType, ulValue ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//  PURPOSE:    Set new Mapping for a ComboBox or a ComboBox column
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
ZComboBox::SetCtrlMapping( zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                           zCPCHAR cpcAttrib, zCPCHAR cpcContext,
                           zLONG lColumn )
{
   if ( lColumn == 0 )  // remap the combobox
   {
      ZMapAct::SetCtrlMapping( cpcViewName, cpcEntity, cpcAttrib, cpcContext, lColumn );
      if ( GetViewByName( &m_vAppList, m_csViewNameList,
                          m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) > 0 )
      {
         DropView( m_vAppList );
      }
   }
   else
   if ( lColumn == -1 )  // remap the list part of the combobox
   {
      mDeleteInit( m_pzsListMapVName );
      mDeleteInit( m_pzsListMapEName );
      mDeleteInit( m_pzsListMapAName );
      mDeleteInit( m_pzsListMapContext );
      m_pzsListMapVName = new CString( cpcViewName );
      m_pzsListMapEName = new CString( cpcEntity );
      m_pzsListMapAName = new CString( cpcAttrib );
      m_pzsListMapContext = new CString( cpcContext );
   }
   else
   if ( lColumn == -2 )  // remap the browse part of the combobox
   {
      mDeleteInit( m_pzsListBrowseEName );
      mDeleteInit( m_pzsListBrowseScopeEName );
      m_pzsScope = 0;

      m_pzsListBrowseEName = new CString( cpcEntity );
      m_pzsListBrowseScopeEName = new CString( cpcAttrib );
      m_pzsScope = m_pzsListBrowseScopeEName;
   }
   else
   {
      AddColumn( (zSHORT) lColumn, -1, 0, cpcViewName, cpcEntity, cpcAttrib, cpcContext );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  PURPOSE:    Get Mapping for a ComboBox or a ComboBox column
//
//  PARAMETERS: pchViewName      - Mapping View Name
//              pchEntityName    - Mapping Entity Name
//              pchAttributeName - Mapping Attribute Name
//              pchContextName   - Mapping Context Name
//              lColumn          - Column for which the new mapping is to be
//                                 retrieved (0 ==> retrieve listbox mapping).
//
//  RETURNS:    0 - Mapping returned successfully
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
ZComboBox::GetCtrlMapping( zPCHAR pchViewName, zPCHAR pchEntity,
                           zPCHAR pchAttrib, zPCHAR pchContext, zLONG lColumn )
{
   if ( lColumn == 0 )  // remap the combobox
   {
      ZMapAct::GetCtrlMapping( pchViewName, pchEntity, pchAttrib, pchContext, lColumn );
   }
   else
   if ( lColumn == -1 )  // remap the list part of the combobox
   {
      if ( m_pzsListMapVName )
         strcpy_s( pchViewName, zTAG_LTH, *m_pzsListMapVName );
      else
         pchViewName[ 0 ] = 0;

      if ( m_pzsListMapEName )
         strcpy_s( pchEntity, zTAG_LTH, *m_pzsListMapEName );
      else
         pchEntity[ 0 ] = 0;

      if ( m_pzsListMapAName )
         strcpy_s( pchAttrib, zTAG_LTH, *m_pzsListMapAName );
      else
         pchAttrib[ 0 ] = 0;

      if ( m_pzsListMapContext )
         strcpy_s( pchContext, zTAG_LTH, *m_pzsListMapContext );
      else
         pchContext[ 0 ] = 0;
   }
   else
   if ( lColumn == -2 )  // remap the browse part of the combobox
   {
      if ( m_pzsListMapVName )
         strcpy_s( pchViewName, zTAG_LTH, *m_pzsListMapVName );
      else
         pchViewName[ 0 ] = 0;

      if ( m_pzsListBrowseEName )
         strcpy_s( pchEntity, zTAG_LTH, *m_pzsListBrowseEName );
      else
         pchEntity[ 0 ] = 0;

      if ( m_pzsListBrowseScopeEName )
         strcpy_s( pchAttrib, zTAG_LTH, *m_pzsListBrowseScopeEName );
      else
         pchAttrib[ 0 ] = 0;

      pchContext[ 0 ] = 0;
   }
   else
   {
   // GetColumn( (zSHORT) lColumn, -1, 0, pchViewName, pchEntity, pchAttrib, pchContext );
   }

   return( 0 );
}

void
ZComboBox::ProcessCommand( zUSHORT uNotificationCode, zLONG lSelectedIdx )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "ComboBox1" ) == 0 )
   {
      TraceLineS( "ZComboBox::ProcessCommand ", *m_pzsTag );
      TraceLineX( "ZComboBox::ProcessCommand MapActFlags: ", m_ulMapActFlags );
      TraceLineI( "ZComboBox::ProcessCommand NotifyCode = ", uNotificationCode );
   }
#endif
   if ( uNotificationCode == zCB_DROPDOWN ||
        uNotificationCode == zCB_SETFOCUS )
   {
      if ( uNotificationCode == zCB_SETFOCUS )
      {
         if ( m_nIsDroppedDown & zCB_SELCHG )
         {
#ifndef zREMOTE_SERVER
            ShowDropDown( TRUE );
#endif
            return;
         }
      }
      else
//    if ( uNotificationCode == zCB_DROPDOWN )
      {
         m_nIsDroppedDown &= ~zCB_SELEND;
         m_nIsDroppedDown |= zCB_OPENED;
         BuildListFromOI( );
      }
   }
   else
   if ( uNotificationCode == zCB_SELCHANGE )
   {
      m_bWasSelected = TRUE;
      m_nIsDroppedDown |= zCB_SELCHG;
   }
   else
   if ( uNotificationCode == zCB_SELENDOK ||
        uNotificationCode == zCB_SELENDCANCEL )
   {
      m_nIsDroppedDown |= zCB_SELEND;
   }

   // Do positioning before sending events for combo box close-up and
   // selchange.  These messages position on the instance data to make
   // the message useful to the application.
   if ( uNotificationCode == zCB_CLOSEUP ||
        uNotificationCode == zCB_SELCHANGE )
   {
      if ( uNotificationCode == zCB_CLOSEUP )
         m_nIsDroppedDown &= ~zCB_OPENED;

      if ( m_ulSubtype & zCOMBOBOX_POSITION_ONLY )
      {
         // Save the subtype and turn off inclusion and foreign key assignment
         // so no REAL MAPPING takes place in MapToOI, ONLY POSITIONING.
         zULONG ulSave = m_ulSubtype;
         if ( m_pzmaComposite == 0 || m_pzmaComposite->m_lKeyId != 2010 )
            m_ulSubtype &= ~(zCOMBOBOX_INCLUDE | zCOMBOBOX_FOREIGN_KEY);

         MapToOI( 0 );
         m_ulSubtype = ulSave;
      }
   }

   CWnd *pWnd = GetActiveWindow( );
   ZSubtask *pZSubtask = m_pZSubtask->GetSplitterMainSubtask( );
   if ( pZSubtask == 0 )
      pZSubtask = m_pZSubtask;

   if ( pWnd == 0 || pZSubtask->m_pZFWnd == 0 ||
        pWnd->m_hWnd != pZSubtask->m_pZFWnd->m_hWnd )
   {
      m_nIsDroppedDown &= ~zCB_SELEND;
      return;
   }

   if ( lSelectedIdx >= -1 )
   {
      ProcessEvent( this, uNotificationCode );
      if ( (m_nIsDroppedDown & zCB_SELCHGEND) == zCB_SELCHGEND )  // composite flag
      {
         m_nIsDroppedDown &= ~zCB_SELCHGEND;
         ProcessEvent( this, zCB_SELCLOSE );
      }
   }
}

#ifndef zREMOTE_SERVER

void
ZComboBox::OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::OnChar ==========> ", uKey );
#endif

   CComboBox::OnChar( uKey, uRepeatCnt, uFlags );

#if 0
   zULONG  ulThisTime = GetTickCount( );
   zULONG  ulInterval = ulThisTime - m_ulLastTime;

   while ( ulInterval < m_ulDelayInterval )  // purist's go to
   {
      int nLastItemNbr = GetCurSel( );

   // TraceLineI( "ZComboBox::OnChar CurSel: ", nLastItemNbr );
      if ( nLastItemNbr < 0 )
         break;

      m_szKeystroke[ m_lKeystrokeCnt ] = (char) uKey;
      m_lKeystrokeCnt++;
      m_szKeystroke[ m_lKeystrokeCnt ] = 0;

      m_lItemNbr = FindString( nLastItemNbr - 1, m_szKeystroke );
      if ( m_lItemNbr < 0 )
         m_lItemNbr = FindString( -1, m_szKeystroke );

      if ( m_lItemNbr >= 0 )
      {
         CString cs;
         LPARAM  lParam;
         zBOOL   bSet = FALSE;

      // TraceLineI( "Would SetCurSel: ", m_lItemNbr );
         m_lItemNbr -= nLastItemNbr;
      // TraceLineI( "m_lItemNbr = ", m_lItemNbr );

         // To fix a problem in which I can find no other solution, the
         // following hack sends down or up arrow messages to the combobox
         // to cause the correct item to be selected (and retained after
         // the user "tabs" out of the combobox.
         if ( m_lItemNbr > 0 )
         {
            if ( m_lItemNbr > 10 )
            {
               SetRedraw( FALSE );
               bSet = TRUE;
            }

            while ( m_lItemNbr )
            {
               lParam = 1;
               m_lItemNbr--;
            // TraceLineX( "lParam = ", lParam );
               SendMessage( WM_KEYDOWN, 40, lParam );
               lParam |= 0x00030000;
               SendMessage( WM_KEYUP, 40, lParam );
            }
         }
         else
         if ( m_lItemNbr < 0 )
         {
            m_lItemNbr = -m_lItemNbr;
            if ( m_lItemNbr > 10 )
            {
               SetRedraw( FALSE );
               bSet = TRUE;
            }

            while ( m_lItemNbr )
            {
               lParam = 1;
               m_lItemNbr--;
               SendMessage( WM_KEYDOWN, 38, lParam );
               lParam |= 0x00030000;
               SendMessage( WM_KEYUP, 38, lParam );
            }
         }

         if ( bSet )
         {
            CRect rect;

            SetRedraw( TRUE );
            GetDroppedControlRect( rect );
            InvalidateRect( rect );
            Invalidate( );
            UpdateWindow( );
         }

         m_ulLastTime = GetTickCount( );
         return; // end purist's goto
      }

      break;
   }

   m_ulLastTime = ulThisTime;
   m_szKeystroke[ 0 ] = (char) uKey;
   m_szKeystroke[ 1 ] = 0;
   m_lKeystrokeCnt = 1;
   CComboBox::OnChar( uKey, uRepeatCnt, uFlags );
#endif
}

void
ZComboBox::OnEnable( BOOL bEnable )
{
// if ( zstrcmp( "CBAdmissionsTrack", *m_pzsTag ) == 0 )
//    TraceLineS( "", "" );

   CComboBox::OnEnable( bEnable );
}

void
ZComboBox::OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
   // Note that keyboard events (other than tab) need to build the list
   // since the keyboard interface will not work otherwise.
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineI( "ZComboBox::OnKeyDown ", uKey );
      TraceLineI( "ZComboBox::OnKeyDown Repeat: ", uRepeatCnt );
      TraceLineX( "ZComboBox::OnKeyDown Flags: ", uFlags );
   }
#endif
   if ( uKey != VK_TAB && uKey != VK_SHIFT && m_bListLoaded == FALSE )
   {
      BuildListFromOI( );
   }

   if ( uKey == VK_SPACE )
   {
      zCHAR szMap[ 256 ];

      GetLBText( 0, szMap );
//    TraceLineS( "COMBO VK_SPACE", szMap );
      if ( *szMap == 0 )
      {
         SetCurSel( 0 );
         return;
      }
   }
   else
   if ( uKey == VK_DOWN || uKey == VK_UP )
   {
      if ( GetDroppedState( ) == FALSE )
      {
         ShowDropDown( TRUE );
         return;
      }
   }

   CComboBox::OnKeyDown( uKey, uRepeatCnt, uFlags );
}

void
ZComboBox::OnKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags )
{
   // Note that keyboard events (other than tab) need to build the list
   // since the keyboard interface will not work otherwise.
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::OnKeyUp Key: ", uKey );
      TraceLineI( "ZComboBox::OnKeyUp Repeat: ", uRepeatCnt );
      TraceLineX( "ZComboBox::OnKeyUp Flags: ", uFlags );
#endif
   CComboBox::OnKeyUp( uKey, uRepeatCnt, uFlags );
}

#endif

void
ZComboBox::CBNDropdown( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::CBNDropdown NotifyCode = ", zCB_DROPDOWN );
#endif

   m_pZSubtask->m_pzmaAccel = this;

   m_nIsDroppedDown &= ~zCB_SELEND;
   m_nIsDroppedDown |= zCB_OPENED;

   BuildListFromOI( );

// Default( );             // pass it along to parent
   ProcessCommand( zCB_DROPDOWN );
}

void
ZComboBox::CBNCloseup( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::CBNClosup NotifyCode = ", zCB_CLOSEUP );
#endif

   m_pZSubtask->m_pzmaAccel = 0;

// Default( );             // pass it along to parent
   ProcessCommand( zCB_CLOSEUP );
}

void
ZComboBox::CBNDblclk( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::CBNDblclk NotifyCode = ", zCB_DBLCLK );
#endif
// Default( );             // pass it along to parent
   ProcessCommand( zCB_DBLCLK );
}

void
ZComboBox::CBNEditchange( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::CBNEditChange NotifyCode = ", zCB_EDITCHANGE );
#endif
// Default( );             // pass it along to parent
}

void
ZComboBox::OnEditUpdate( )
{
   // If we are not to auto update the text, get outta here.
   if ( m_bAutoComplete == FALSE )
       return;

   // Get the text in the edit box.
   CString cs;
   GetWindowText( cs );
   int nLth = cs.GetLength( );

   // Currently selected range.
   DWORD dwCurSel = GetEditSel( );
   WORD  wStart   = LOWORD( dwCurSel );
   WORD  wEnd     = HIWORD( dwCurSel );

   // Search for, and select in, any string in the combo box that is prefixed
   // by the text in the edit box.
   if ( SelectString( -1, cs ) == CB_ERR )
   {
       SetWindowText( cs );   // no text selected, so restore what was there before
       if ( dwCurSel != CB_ERR )
         SetEditSel( wStart, wEnd );   // restore cursor postion
   }

   // Set the text selection as the additional text that we have added.
   if ( wEnd < nLth && dwCurSel != CB_ERR )
       SetEditSel( wStart, wEnd );
   else
       SetEditSel( nLth, -1 );

   ProcessCommand( zCB_EDITCHANGE );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

BOOL
ZComboBox::PreTranslateMessage( MSG *pMsg )
{
   // Need to check for backspace/delete. These will modify the text in
   // the edit box, causing the auto complete to just add back the text
   // the user has just tried to delete.
   if ( m_ulSubtype & zCOMBOBOX_DROPDOWN )
   {
      if ( pMsg->message == WM_KEYDOWN )
      {
         m_bAutoComplete = TRUE;
         int nVirtKey = (int) pMsg->wParam;
         if ( nVirtKey == VK_DELETE || nVirtKey == VK_BACK )
            m_bAutoComplete = FALSE;
      }
   }

   return( CComboBox::PreTranslateMessage( pMsg ) );
}

void
ZComboBox::CBNErrspace( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::CBNErrspace NotifyCode = ", zCB_ERRSPACE );
#endif
// Default( );             // pass it along to parent
   ProcessCommand( zCB_ERRSPACE );
}

void
ZComboBox::CBNSelchange( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineI( "ZComboBox::CBNSelchange = ", zCB_SELCHANGE );
   // TraceLineI( "ZComboBox::CBNSelchange FirstTime = ", m_bFirstTime );
      TraceLineI( "ZComboBox::CBNSelchange CurSel: ", GetCurSel( ) );
   }
#endif
// Default( );             // pass it along to parent

// if ( m_lItemNbr <= 0 )
      ProcessCommand( zCB_SELCHANGE );
}

void
ZComboBox::CBNSelendcancel( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineI( "ZComboBox::CBNSelendcancel = ", zCB_SELENDCANCEL );
      TraceLineI( "ZComboBox::CBNSelendcancel CurSel: ", GetCurSel( ) );
   }
#endif
   m_pZSubtask->m_pzmaAccel = 0;
// Default( );             // pass it along to parent

   m_nIsDroppedDown |= zCB_SELEND;
   ProcessCommand( zCB_SELENDCANCEL );
}

void
ZComboBox::CBNSelendok( )
{
#ifdef DEBUG_ALL
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
   {
      TraceLineI( "ZComboBox::CBNSelendok NotifyCode = ", zCB_SELENDOK );
      TraceLineI( "ZComboBox::CBNSelendok CurSel: ", GetCurSel( ) );
   }
#endif
   m_pZSubtask->m_pzmaAccel = 0;
// Default( );             // pass it along to parent

   m_nIsDroppedDown |= zCB_SELEND;
   ProcessCommand( zCB_SELENDOK );
}

#if 0
void
ZComboBox::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZComboBox::OnSetFocus: ", *m_pzsTag );
#endif
   Default( );              // pass it along to parent
   m_pZSubtask->InformDriverOfSetFocus( this );
}

void
ZComboBox::OnKillFocus( CWnd *pWndGetFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZComboBox::OnKillFocus: ", *m_pzsTag );
#endif

   Default( );              // pass it along to parent
   m_pZSubtask->InformDriverOfKillFocus( this );
}
#endif

void
ZComboBox::CBNSetFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZComboBox::CBNSetFocus: ", *m_pzsTag );
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
//    TraceLineI( "ZComboBox::CBNSetFocus NotifyCode = ", zCB_SETFOCUS );
#endif
// Default( );             // pass it along to parent

   if ( m_nIsDroppedDown & zCB_SELCHG )
   {
#ifndef zREMOTE_SERVER
      ShowDropDown( TRUE );
#endif
   // return;
   }

   if ( m_pZSubtask->InformDriverOfSetFocus( this ) )
   {
//    BuildListFromOI( );
      ProcessCommand( zCB_SETFOCUS );
   }
}

void
ZComboBox::CBNKillFocus( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZComboBox::CBNKillFocus: ", *m_pzsTag );
// if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
//    TraceLineI( "ZComboBox::CBNKillFocus NotifyCode = ", zCB_KILLFOCUS );
#endif

#ifndef zREMOTE_SERVER
   ShowDropDown( FALSE );
#endif

   m_pZSubtask->InformDriverOfKillFocus( this );
// Default( );             // pass it along to parent
   ProcessCommand( zCB_KILLFOCUS );
}

#if 0
// OnCtlColor will not be called for the list box of a drop-down
// combo box because the drop-down list box is actually a child of
// the combo box and not a child of the window.  To change the color
// of the drop-down list box, create a CComboBox with an override of
// OnCtlColor that checks for CTLCOLOR_LISTBOX in the nCtlColor
// parameter.  In this handler, the SetBkColor member function must
// be used to set the background color for the text.

HBRUSH
ZComboBox::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlColor )
{
   HBRUSH hBrush = (HBRUSH) Default( );
   switch ( uCtlColor )
   {
      case CTLCOLOR_EDIT:
      {
         TraceLineS( "ZComboBox::OnCtlColor EDIT ", *m_pzsTag );
      }

      case CTLCOLOR_LISTBOX:
      {
         TraceLineS( "ZComboBox::OnCtlColor LIST ", *m_pzsTag );
      }
   }

   return( hBrush );
}

#else

HBRUSH
ZComboBox::CtlColor( CDC *pDC, UINT uCtlColor )
{
#if 0 //def DEBUG_ALL lots of messages
// if ( zstrcmp( "EdtNcPaint", *m_pzsTag ) == 0 )
   {
      CString cs;
      GetWindowText( cs );
      TraceLineS( "ZComboBox::CtlColor Tag ", *m_pzsTag );
   // TraceLineS( "ZComboBox::CtlColor Text ", cs );
   // TraceLineI( "ZComboBox::CtlColor nCtlColor ", nCtlColor );
   // TraceLineX( "ZComboBox::CtlColor colorText ", (zLONG) m_clrText );
   }
#endif

// HBRUSH hBrush = CWnd::OnCtlColor( pDC, this, uCtlColor ); // these cause
// HBRUSH hBrush = (HBRUSH) Default( );                      // infinite loop
   HBRUSH hBrush = 0; // I hope this is OK. It doesn't seem to cause problems

   if ( m_clrText )
      pDC->SetTextColor( m_clrText );

   if ( m_pBrush )
   {
      hBrush = (HBRUSH) *m_pBrush;
      pDC->SetBkColor( m_clrBk );
   }

   return( hBrush );
}
#endif

void
ZComboBox::ParentMove( )
{
#ifndef zREMOTE_SERVER
   if ( mIs_hWnd( m_hWnd ) )
      ShowDropDown( FALSE );
#endif
}

zBOOL
ZComboBox::HandleAccel( zACCEL_DEF& ad )
{
#ifndef zREMOTE_SERVER
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineI( "ZComboBox::HandleAccel: ", ad.uVK );
#endif
   if ( ad.uVK == VK_ESCAPE && ad.fAlt == 0 && ad.fCtrl == 0 )
   {
      if ( GetDroppedState( ) )
      {
         ShowDropDown( FALSE );
         return( TRUE );
      }
   }
#endif

   return( FALSE );
}

#ifndef zREMOTE_SERVER

void
ZComboBox::OnMouseMove( UINT uModKeys, CPoint pt )
{
   // Inform the frame window that the mouse is over this ctrl.
   m_pZSubtask->InformDriverOfFlyBy( this );

   Default( );              // pass it along to parent
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef zREMOTE_SERVER

zSHORT
ZComboBox::GetArrayWithKeyAttribute( zVIEW vApp )
{
   if ( m_aKeyAttr.GetSize( ) > 0 )
      return( 0 );


   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;

   lpViewEntity = MiGetViewEntityForView( vApp, *m_pzsEName );

   if ( lpViewEntity )
   {
      for ( lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         if ( lpViewAttrib->bKey && lpViewAttrib->bForeignKey == 0 )
         {
            m_aKeyAttr.Add( lpViewAttrib->szName );
         }
      }
   }

   if ( m_aKeyAttr.GetSize( ) == 0 )
      return( -1 );

   return( 0 );
}

zBOOL
ZComboBox::SearchStringByKeyAttributes( zVIEW vApp, zVIEW vListApp )
{
   zCPCHAR cpcBrowseScope = 0;
   zBOOL   bFound = FALSE;
   zSHORT  nRC;
   zSHORT  k;

   if ( m_pzsListBrowseScopeEName )
      cpcBrowseScope = *m_pzsListBrowseScopeEName;

   nRC = SetCursorFirstEntityByAttr( vListApp, *m_pzsListMapEName,
                                     m_aKeyAttr[ 0 ],
                                     vApp, *m_pzsEName, m_aKeyAttr[ 0 ],
                                     cpcBrowseScope );

   while ( nRC >= zCURSOR_SET  )
   {
      for ( k = 0; k < m_aKeyAttr.GetSize( ); k++ )
      {
         if ( CompareAttributeToAttribute( vListApp, *m_pzsListMapEName,
                                           m_aKeyAttr[ k ],
                                           vApp, *m_pzsEName,
                                           m_aKeyAttr[ k ] ) != 0 )
         {
            nRC = SetCursorNextEntityByAttr( vListApp, *m_pzsListMapEName,
                                             m_aKeyAttr[ 0 ],
                                             vApp, *m_pzsEName,
                                             m_aKeyAttr[ 0 ],
                                             cpcBrowseScope );
            bFound = FALSE;
            break;
         }
         else
            bFound = TRUE;
      }

      if ( k >= m_aKeyAttr.GetSize( ) )
         break;
   }

   return( bFound );
}

// If the Edit string was not found in ListBox part, then set the ListBox
// View cursor according to the Key Attributes of the Edit field Entity.
// This is only done for "INCLUDE" Type ComboBoxes
zSHORT
ZComboBox::SelectStringByKeyAttributes( zVIEW vXRA )
{
   zVIEW   vApp;
   zVIEW   vListApp;
   zSHORT  nRC = -1;

   if ( !vXRA )
      return( -1 );

   if ( m_pzsVName == 0 || m_pzsListMapVName == 0 || m_pzsEName == 0 || m_pzsListMapEName == 0 ||
        GetViewByName( &vApp, *m_pzsVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) < 0 ||
        GetViewByName( &vListApp, *m_pzsListMapVName, m_pZSubtask->m_vDialog, zLEVEL_ANY ) < 0 ||
        CheckExistenceOfEntity( vApp, *m_pzsEName ) != zCURSOR_SET ||
        CheckExistenceOfEntity( vListApp, *m_pzsListMapEName ) != zCURSOR_SET )
   {
      SetZCtrlText( this, "" );
      SetAttributeFromInteger( vXRA, "ComboBox", "SelectedIndex", -1 );
      return( -1 );
   }

   if ( (m_ulSubtype & zCOMBOBOX_INCLUDE) == 0 )
      return( -1 );

   if ( GetArrayWithKeyAttribute( vApp ) < 0 )
      return( -1 );

   zVIEW  vListSearch;
   CreateViewFromViewForTask( &vListSearch, vListApp, m_pZSubtask->m_vDialog );

   zBOOL bFound = SearchStringByKeyAttributes( vApp, vListSearch );

   if ( bFound )
   {
      zCHAR szMap[ 256 ];
      GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                sizeof( szMap ),
                                vListSearch, *m_pzsListMapEName,
                                *m_pzsListMapAName, *m_pzsListMapContext,
                                m_pzsListMapContext->IsEmpty( ) ?
                                zUSE_DEFAULT_CONTEXT : 0 );
      SelectExactString( -1, szMap );

      zLONG lNewSelIdx = GetCurSel( );
      if ( lNewSelIdx >= 0 )
      {
         SetZCtrlText( this, szMap );
         SetAttributeFromInteger( vXRA, "ComboBox", "SelectedIndex", lNewSelIdx );
         nRC = 0;
      }
   }

   DropView( vListSearch );
   return( nRC );
}

//
// Get the "control's" data from ZRmtDataSet and store it for the
// remote control.
//
void
ZComboBox::MapToRemoteDataset( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZComboBox::MapToRemoteDataset ", *m_pzsTag );
#endif

// Disable Delta Data implementation.
// // Merge RmtDataSet.
// MergeRowData( (m_pZSubtask->m_ulSubtaskFlags &
//                zSUBTASK_MAPPED_SINCE_APPLYSTATE) ? TRUE : FALSE );
// Disable Delta Data implementation.

   zVIEW vXRA;

   if ( (vXRA = SetupRemoteObject( )) != 0 )
   {
      zSHORT  nRC;

      // Delete all Data Rows.
      if ( CheckExistenceOfEntity( vXRA, cpcCMBDataRowEntity ) == 0 )
      {
         nRC = SetCursorFirstEntity( vXRA, cpcCMBDataRowEntity, 0 );
         while ( nRC >= zCURSOR_SET )
            nRC = DeleteEntity( vXRA, cpcCMBDataRowEntity, zREPOS_FIRST );
      }

   // Disable Delta Data implementation.
   // if ( m_pZSubtask->m_ulSubtaskFlags & zSUBTASK_MAPPED_SINCE_APPLYSTATE )
   //    SetAttributeFromString( vXRA, "ComboBox", "DeleteAllRows", "Y" );
   // Disable Delta Data implementation.

      ZRmtDataRow *pDataRow = 0;
      CString  csRowText;
      CString  csCtrlText = "";
      zLONG    lSelIdx = GetCurSel( );
      zLONG    lRelNbr = 0;
   // zULONG   ulState;
      POSITION pos = GetFirstDataRowPosition( );

#if 0    // Disable Delta Data implementation.
      while ( pos )
      {
         pDataRow = GetNextDataRow( pos );
         CreateEntity( vXRA, cpcCMBDataRowEntity, zPOS_AFTER );
         ulState = pDataRow->GetState( );
         if ( (ulState & zRMT_CHANGED_KEY) == zRMT_CHANGED_KEY )
         {
            SetAttributeFromInteger( vXRA, cpcCMBDataRowEntity, "Tag", pDataRow->GetKeyOrig( ) );
            SetAttributeFromInteger( vXRA, cpcCMBDataRowEntity, "NewTag", pDataRow->GetKey( ) );
         }
         else
            SetAttributeFromInteger( vXRA, cpcCMBDataRowEntity, "Tag", pDataRow->GetKey( ) );

         if ( (ulState & zRMT_DELETED) == zRMT_DELETED )
         {
            SetAttributeFromString( vXRA, cpcCMBDataRowEntity, "Deleted", "Y" );
         }
         else
         {
            if ( lSelIdx == lRelNbr )
               csCtrlText = pDataRow->GetRowDataText( FALSE ); // no separator

            if ( ulState == 0 ||
                 (ulState & zRMT_CHANGED_ORDER) == zRMT_CHANGED_ORDER )
            {
               SetAttributeFromInteger( vXRA, cpcCMBDataRowEntity, "Order", pDataRow->GetOrder( ) );
            }

            lRelNbr++;

            if ( ulState == 0 || (ulState & zRMT_CHANGED_DATA) == zRMT_CHANGED_DATA )
            {
               // Retrieve row data.
               csRowText = pDataRow->GetRowDataText( FALSE ); // no separator
            // TraceLineS( "ZComboBox::MapToRemoteDataset: ", csRowText );
               SetAttributeFromString( vXRA, cpcCMBDataRowEntity, "Text", csRowText );
            }
         }
      }
#else
      while ( pos )
      {
         pDataRow = GetNextDataRow( pos );

         if ( lSelIdx == lRelNbr )
            csCtrlText = pDataRow->GetRowDataText( FALSE ); // no separator

         // Retrieve row data.
         csRowText = pDataRow->GetRowDataText( FALSE ); // no separator
      // TraceLineS( "ZComboBox::MapToRemoteDataset: ", csRowText );
         CreateEntity( vXRA, cpcCMBDataRowEntity, zPOS_AFTER );
         SetAttributeFromString( vXRA, cpcCMBDataRowEntity, "Text", csRowText );
         SetAttributeFromInteger( vXRA, cpcCMBDataRowEntity, "Tag", lRelNbr );
         lRelNbr++;
      }
#endif   // Disable Delta Data implementation.

      // Do not deselect (when lSelIdx < 0) if there are entries in the
      // list.  This is to take care of the case in which the data in
      // the list is presented from a derived attribute, but the data
      // in the editbox is presented from a single attribute.
      zLONG  lIndex;

      GetIntegerFromAttribute( &lIndex, vXRA, "ComboBox", "SelectedIndex" );
      if ( lIndex < 0 ||
           CompareAttributeToString( vXRA, "ComboBox", "SelectedIndex", "" ) == 0 )
      {
         if ( lSelIdx >= 0 )
         {
            SetZCtrlText( this, csCtrlText );
            SetAttributeFromInteger( vXRA, "ComboBox", "SelectedIndex", lSelIdx );
         }
         else
         {
            // If the Edit string was not found in ListBox part, then
            // search the Edit string with the Key Attributes
            SelectStringByKeyAttributes( vXRA );
         }
      }

      DropView( vXRA );
   }
}

void
ZComboBox::MapFromRemoteDataset( )
{
#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "DKS" ) == 0 )
      TraceLineS( "ZComboBox::MapFromRemoteDataset ", *m_pzsTag );
#endif

   ZXRA_Item *pXRA;

   if ( m_pZSubtask->m_pZSocketTask &&
        (pXRA = m_pZSubtask->m_pZSocketTask->
          m_pCacheWndList->Find( *(m_pZSubtask->m_pzsRemoteWndTag),
                                 m_pZSubtask,
                                 "ZComboBox::MapFromRemoteDataset" )) != 0 &&
        (pXRA->m_vXRA || pXRA->m_vXRA_Delta) )
   {
      zVIEW  vXRA;
      zSHORT nPass;

      if ( pXRA->m_vXRA_Delta == 0 )
         nPass = 1;
      else
         nPass = 0;

      while ( TRUE )
      {
         if ( nPass == 0 )
            CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
         else
            CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA, m_pZSubtask->m_vDialog );

         while ( ResetViewFromSubobject( vXRA ) == 0 )
         {
         }

         if ( CheckExistenceOfEntity( vXRA, "Wnd" ) < zCURSOR_SET ||
              SetCursorFirstEntityByString( vXRA, "Wnd", "Tag",
                                            *(m_pZSubtask->m_pzsRemoteWndTag),
                                            0 ) < zCURSOR_SET )
         {
            DropView( vXRA );
            if ( nPass == 0 )
            {
               nPass = 1;
               continue;
            }
            else
            {
               return;
            }
         }

         if ( PositionOnRemoteCtrl( this, vXRA, FALSE ) )
         {
            zLONG  lIndex;

            // There is only one selected item in a combobox.
            if ( GetIntegerFromAttribute( &lIndex, vXRA, "ComboBox", "SelectedIndex" ) >= 0 )
            {
               ResetSelectionSet( FALSE );
               SetSel( lIndex, TRUE );
            }
         }
         else
         if ( nPass == 0 )
         {
            nPass = 1;
            DropView( vXRA );
            continue;
         }

         DropView( vXRA );
         break;
      }
   }
}

#endif // end of:  #ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
// By Xiao Wu Guang

#if 0
void
ZComboBox::ResetContent( )
{
   CComboBox::ResetContent( );
// for ( zSHORT nColIdx = 0; nColIdx < m_nTotalCols; nColIdx++ )
//    m_pcsColItemList[ nColIdx ].RemoveAll( );
};
#endif

zLONG
ZComboBox::GetCurrColumnItem( zPCHAR pchReturnText,
                              zSHORT nMaxLth, zSHORT nCol )
{
#ifndef zREMOTE_SERVER
   zLONG lIdx;

   if ( (lIdx = CComboBox::GetCurSel( )) != CB_ERR && nCol < m_nTotalCols )
   {
      FormatTextAtPosition( pchReturnText, lIdx, nCol, nMaxLth );
      return( lIdx );
   }
#endif
   return( -1 );
}

// Load m_pchMap with the value to put in the listbox.
zSHORT
ZComboBox::FormatTextAtPosition( zPCHAR pchText,
                                 zLONG  lRow,
                                 zSHORT nCol,
                                 zSHORT nMaxLth )
{
   ZComboColumn *pCol;

   pchText[ 0 ] = 0; // initialize to null string in case there are problems

   // If we don't have the column or we are in a delete state ... quit.
   if ( (pCol = FindColumn( nCol )) == 0 ||
        (ZSubtask::GetSubtaskState( m_pZSubtask ) & zSUBTASK_STATE_DELETED) )
   {
      return( -1 );
   }

#ifdef DEBUG_ALL
   if ( zstrcmp( *m_pzsTag, "lbAttribute" ) == 0 )
   {
      TraceLineS( "ZComboBox::FormatTextAtPosition lbAttribute: ",
                  *(m_pZSubtask->m_pzsWndTag) );
      TraceLineS( "ZComboBox::FormatTextAtPosition Attrib: ",
                  pCol->m_pchAttribute );
   }
#endif

   if ( GetViewByName( &m_vAppList, m_csViewNameList,
                       m_pZSubtask->m_vDialog, zLEVEL_SUBTASK ) <= 0 )
   {
      // If we don't have the view for the list, see if we can create one.
      if ( GetViewByName( &m_vApp, *m_pzsListMapVName,
                          m_pZSubtask->m_vDialog, zLEVEL_ANY ) <= 0 )
      {
         if ( m_pZSubtask->m_pZTask->m_nTraceAction > 1 )
         {
            zCHAR szMsg[ 256 ];

            sprintf_s( szMsg, sizeof( szMsg ), "Unable to load view: %s for %s.%s.%s",
                      (*m_pzsVName).GetString(), (*(m_pZSubtask->m_pzsDlgTag)).GetString(),
                      (*(m_pZSubtask->m_pzsWndTag)).GetString(), (*m_pzsTag).GetString() );
            TraceLineS( "ZComboBox::FormatTextAtPosition ", szMsg );
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
         m_lEntityNbr = GetRelativeEntityNumber( m_vApp, *m_pzsListMapEName,
                                                 m_pzsScope ? *m_pzsScope : "", m_ulFlag );
         SetEntityCursor( m_vAppList, *m_pzsListMapEName, 0,
                          zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                          0, 0, 0, m_lEntityNbr,
                          m_pzsScope ? *m_pzsScope : "", 0 );
      }
   }

   // Ensure at least one entity.
   if ( SetEntityCursor( m_vAppList, *m_pzsListMapEName, 0,
                         m_ulFlag | zPOS_FIRST | zTEST_CSR_RESULT,
                         0, 0, 0, 0,
                         m_pzsScope ? *m_pzsScope : "", 0 ) < 0 )
   {
      zCHAR szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ),
                "Error in entity count for view: %s for %s.%s.%s",
                (*m_pzsVName).GetString(), (*m_pZSubtask->m_pzsDlgTag).GetString(),
                (*m_pZSubtask->m_pzsWndTag).GetString(), (*m_pzsTag).GetString() );
   // TraceLineS( "ZComboBox::FormatTextAtPosition ", szMsg );
      DropView( m_vAppList );
      m_vAppList = 0;
      return( -1 );
   }

   if ( m_lEntityNbr != lRow )
   {
      zSHORT nPrevSelSet;

      if ( m_nDisplaySelSet )
      {
         nPrevSelSet = SetSelectSetForView( m_vAppList, m_nDisplaySelSet );
      // TraceLineI( "ZListCtrl::FormatText DisplaySelectSet: ", m_nDisplaySelSet );
      }
      else
      {
         nPrevSelSet = SetSelectSetForView( m_vAppList, m_nSelectSelSet );
      // TraceLineI( "ZListCtrl::FormatText SelectSelectSet: ", m_nSelectSelSet );
      }

      if ( m_lEntityNbr == -1 || (lRow - m_lEntityNbr) != 1 )
      {
         if ( SetEntityCursor( m_vAppList, *m_pzsListMapEName, 0,
                               zPOS_FIRST | zPOS_RELATIVE | m_ulFlag,
                               0, 0, 0, lRow,
                               m_pzsScope ? *m_pzsScope : "", 0 ) >= zCURSOR_SET )
         {
            m_lEntityNbr = lRow;
         }
         else
         {
         // TraceLineI( "ZListCtrl::FormatText REL_FIRST failed: ", lRow );
            SetSelectSetForView( m_vAppList, nPrevSelSet );
            return( -1 );
         }
      }
      else
      {
         lRow -= m_lEntityNbr;
         if ( lRow > 0 )
         {
            if ( SetEntityCursor( m_vAppList, *m_pzsListMapEName, 0,
                                  zPOS_NEXT | m_ulFlag,
                                  0, 0, 0, lRow,
                                  m_pzsScope ? *m_pzsScope : "", 0 ) >= zCURSOR_SET )
            {
               SetSelectSetForView( m_vAppList, nPrevSelSet );
               m_lEntityNbr += lRow;
            }
            else
            {
            // TraceLineI( "ZListCtrl::FormatText REL_NEXT failed: ", lRow );
               return( -1 );
            }
         }
         else
         {
            lRow = -lRow;
            if ( SetEntityCursor( m_vAppList, *m_pzsListMapEName, 0,
                                  zPOS_PREV | zPOS_RELATIVE | m_ulFlag,
                                  0, 0, 0, lRow,
                                  m_pzsScope ? *m_pzsScope : "", 0 ) >= zCURSOR_SET )
            {
               m_lEntityNbr -= lRow;
            }
            else
            {
            // TraceLineI( "ZListCtrl::FormatText REL_PREV failed: ",
            //             lRow );
               SetSelectSetForView( m_vAppList, nPrevSelSet );
               return( -1 );
            }
         }
      }

      SetSelectSetForView( m_vAppList, nPrevSelSet );
   }

   zVIEW v;

   if ( nCol == 0 )
   {
      GetViewByName( &v, pCol->m_pchViewName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
      if ( v == 0 )
         v = m_vAppList;
   }
   else
      v = m_vAppList;

   // This is the attribute to present to the list.
   zSHORT nRC =
   GetVariableFromAttribute( pchText, 0, zTYPE_STRING,
                             255, // nMaxLth - 1 ... bombs when > 255
                             v,
                             pCol->m_pchEntity,
                             pCol->m_pchAttribute,
                             pCol->m_pchContext,
                             pCol->m_pchContext ?
                                    zACCEPT_NULL_ENTITY : zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT );

   if ( nRC == zVAR_NULL )
      pchText[ 0 ] = 0;
   else
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

   TranslateValue( pchText, nMaxLth, TRUE, nCol );

#ifdef DEBUG_ALL
// if ( zstrcmp( "CurrentShipments", *m_pzsTag ) == 0 )
   if ( zstrcmp( *m_pzsTag, "lbAttribute" ) == 0 )
      TraceLineS( "ZComboBox::FormatText Text: ", pchText );
#endif

   return( 0 );
}

#if 0
void
ZComboBox::OnPaint( )
{
   CComboBox::OnPaint( );
   if ( m_hWndChild && GetDroppedState( ) == FALSE )
   {
      DWORD dwCurSel = GetEditSel( );
      if ( dw != CB_ERR )
      {
         CWnd *pWndChild = CWnd::FromHandle( m_hWndChild );
         WORD  wStart = LOWORD( dwCurSel );
         WORD  wEnd   = HIWORD( dwCurSel );
         if ( zstrcmp( *m_pzsTag, "SubFlag" ) == 0 )
         {
            TraceLine( "ZComboBox::OnPaint Tag: %s  Start: %d   End: %d", *m_pzsTag, wStart, wEnd );
         }

         if ( wHi == wLo )  // nothing selected
         {
            CRect rect;
            pWndChild->GetClientRect( rect );
            CClientDC dc( pWndChild ); // device context for painting
            int nSave = dc.SaveDC( );

            CString cs;
            pWndChild->GetWindowText( cs );

            dc.SelectObject( ::GetStockObject( DEFAULT_GUI_FONT ) );

         // if ( mIs_hWndEnabled( m_hWnd ) == FALSE )
         //    dc.SetBkColor( GetSysColor( COLOR_3DFACE ) );

         // dc.FillRect( &rect, &CBrush( dc.GetBkColor( ) ) );
            dc.SetTextColor( zRGB( 255, 0, 0 ) );
            dc.DrawText( cs, rect,
                         DT_SINGLELINE | DT_NOPREFIX | DT_TOP ); // DT_VCENTER );
         // dc.DrawText( cs, rect,
         //              DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER );
                        // DT_NOCLIP |  | DT_BOTTOM );
            dc.RestoreDC( nSave );
         }
      }
   }
}
#endif

void
ZComboBox::DrawItem( LPDRAWITEMSTRUCT pdis )
{
   ASSERT( m_nTotalCols > 1 ); // use CComboBox class instead of this class

   CPen     *pPenOld;
   CDC      *pDC = CDC::FromHandle( pdis->hDC );
   CWnd     *pWndDropDown = pDC->GetWindow( );
   zBOOL    bHighLightText = (pdis->itemAction & ODA_FOCUS) || (pdis->itemState & ODS_SELECTED);
   CRect    rectText = pdis->rcItem;
   int      k = 0;

   pDC->SetTextColor( m_ulCtrlErr ? zRGB( 255, 0, 0 ) :
                      GetSysColor( mIs_hWndEnabled( m_hWnd ) ?
                  (bHighLightText ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT) : COLOR_WINDOWTEXT) );
   pDC->SetBkColor( GetSysColor( mIs_hWndEnabled( m_hWnd ) ?
                          (bHighLightText ? COLOR_HIGHLIGHT : COLOR_WINDOW) : COLOR_3DFACE ) );
   pDC->FillRect( rectText, &CBrush( pDC->GetBkColor( ) ) );

   if ( bHighLightText )
      pDC->DrawFocusRect( rectText );

// TraceLine( "ZComboBox::DrawItem pdis CtlType: %d   CtlID: %d   itemID: %d "
//               "  itemAction: 0x%08x   itemState: 0x%08x   hwndItem: 0x%08x",
//            pdis->CtlType, pdis->CtlID, pdis->itemID, pdis->itemAction,
//            pdis->itemState, pdis->hwndItem );
   if ( pdis->itemID != (UINT) -1 )
   {
      ZComboColumn *pCol;
      zCHAR szText[ 256 ];

   // if ( zstrcmp( *m_pzsTag, "StateCombo" ) == 0 && pdis->itemID > 0 )
   //    TraceLine( "ZComboBox::DrawItem Tag: %s", *m_pzsTag );

      BOOL bDropped = GetDroppedState( );
      if ( bDropped && pWndDropDown == 0 )
         pWndDropDown = CWnd::FromHandle( pdis->hwndItem );

   // if ( pWndDropDown && pWndDropDown != this ) // draw items in drop down list
      if ( pWndDropDown && pdis->itemState < 0x1000 )
      {
         if ( rectText.top == 0 &&
              (GetWindowLong( m_hWnd, GWL_STYLE ) & 0x3) != CBS_SIMPLE )
         {
            // Only "dropdown" & "drop list" combo box can move their drop
            // down window.
            CRect  rectWnd;
            pWndDropDown->GetWindowRect( rectWnd );

            // Move drop down window to left if its left side is off the
            // screen.
            if ( rectWnd.right > GetSystemMetrics( SM_CXSCREEN ) )
            {
               rectWnd.OffsetRect( GetSystemMetrics( SM_CXSCREEN ) - rectWnd.right, 0 );
               pWndDropDown->MoveWindow( rectWnd );
            }
         }

         if ( bHighLightText == FALSE )
         {
            pPenOld = pDC->SelectObject( &m_penLightGray );
            pDC->MoveTo( rectText.left, rectText.bottom - 1 );
            pDC->LineTo( rectText.right, rectText.bottom - 1 );
         }

         rectText.left += LEFTMARGIN_OF_DROPDOWNLIST;
         rectText.right = rectText.left;

         for ( zSHORT nColIdx = 1; nColIdx <= m_nTotalCols; nColIdx++ )
         {
            pCol = FindColumn( nColIdx );
            rectText.left = rectText.right;

            if ( pCol->m_nColWidth )
            {
               if ( nColIdx > 2 ||
                    (nColIdx == 2 && FindColumn( 1 )->m_nColWidth != 0) )
               {
                  rectText.left += SPACE_BETWEEN_COLUMN;
                  if ( bHighLightText == FALSE )
                  {
                     // Draw vertical gray line.
                     pDC->MoveTo( rectText.left - SPACE_BETWEEN_COLUMN / 2, rectText.top );
                     pDC->LineTo( rectText.left - SPACE_BETWEEN_COLUMN / 2, rectText.bottom );
                  }
               }

               rectText.right = rectText.left + pCol->m_nColWidth;
               rectText.left++;

               // If vertical scroll bar is visible ...
               if ( pWndDropDown &&
                    (nColIdx == m_nTotalCols ||
                     (nColIdx == m_nTotalCols - 1 &&
                      FindColumn( m_nTotalCols )->m_nColWidth == 0)) &&
                    (GetWindowLong( pWndDropDown->m_hWnd, GWL_STYLE) & WS_VSCROLL ) )
               {
                  rectText.right -= GetSystemMetrics( SM_CXVSCROLL );
               }

            // if ( pdis->itemID == 3 )
            //    TraceLineS( "", "" );

               FormatTextAtPosition( szText, pdis->itemID, nColIdx, sizeof( szText ) );
            // sprintf_s( szText, sizeof( szText ), "%s.%d", "BOX", pdis->itemID );
            // pdis->itemID /= k;

               pDC->DrawText( szText, -1, rectText,
                              pCol->m_lAlignStyle | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER );
            }
         }

         if ( bHighLightText == FALSE )
            pDC->SelectObject( pPenOld );
      }
      else              // draw item in edit box control/static text control
      {
         rectText.left += LEFTMARGIN_OF_CONTROL;

         pCol = FindColumn( m_nBoundCol );
         if ( pCol->m_nColWidth > 0 )
         {
            FormatTextAtPosition( szText, pdis->itemID, m_nBoundCol, sizeof( szText ) );
         // sprintf_s( szText, sizeof( szText ), "%s.%d", "EDIT BOX", pdis->itemID );
         // if ( pdis->itemID == 3 )
         //    pdis->itemID /= k;
         // TraceLineS( "FormatText for edit box: ", szText );
            pDC->DrawText( szText, -1, rectText,
                           pCol->m_lAlignStyle | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER );
         }
         else           // bound column is hidden
         {
            // Seek a column whose width is not 0.
//          for ( zSHORT nColIdx = 0;
//                m_pnColumnWidth[ nColIdx ] == 0 && nColIdx < m_nTotalCols;
//                nColIdx++ )
//          {
//             // do nothing
//          }

//          pDC->DrawText( m_pcsColItemList[ nColIdx ].
//                           GetAt( m_pcsColItemList[ nColIdx ].
//                             FindIndex( pdis->itemID ) ), -1, rectText,
//                         m_plColumnAlignStyle[ nColIdx ] | DT_SINGLELINE |
//                           DT_NOPREFIX | DT_NOCLIP | DT_VCENTER );
            FormatTextAtPosition( szText, pdis->itemID, m_nShowColumn, sizeof( szText ) );
         // sprintf_s( szText, sizeof( szText ), "%s.%d", "EDIT", pdis->itemID );
         // if ( pdis->itemID == 3 )
         //    pdis->itemID /= k;

         // TraceLineS( "FormatText for first column: ", szText );
            pDC->DrawText( szText, -1, rectText,
                           pCol->m_lAlignStyle | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER );
         }
      }
   }
}

void
ZComboBox::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
   lpMeasureItemStruct->itemHeight = GetSystemMetrics( SM_CYVSCROLL );
}

int
ZComboBox::CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct )
{
   ASSERT( lpCompareItemStruct->CtlType == ODT_COMBOBOX );
   zCPCHAR cpcText1 = (zCPCHAR) lpCompareItemStruct->itemData1;
   ASSERT( cpcText1 );
   zCPCHAR cpcText2 = (zCPCHAR) lpCompareItemStruct->itemData2;
   ASSERT( cpcText2 );

   return( zstrcmp( cpcText2, cpcText1 ) );
}

LRESULT
ZComboBox::OnSetText( WPARAM wParam, LPARAM lParam )
{
#if 0
   zSHORT nShowColItemIdx;
   zSHORT nBoundColItemCnt;

   nBoundColItemCnt = m_pcsColItemList[ m_nBoundCol ].GetCount( );
   for ( nShowColItemIdx = 0;
         nShowColItemIdx < nBoundColItemCnt &&
           m_pcsColItemList[ m_nBoundCol ].
             GetAt( m_pcsColItemList[ m_nBoundCol ].
               FindIndex( nShowColItemIdx ) ) != (zCPCHAR) lParam;
         nShowColItemIdx++ );

   if ( nShowColItemIdx < nBoundColItemCnt )
      return( (LRESULT)(zCPCHAR) (m_pcsColItemList[ m_nShowColumn ].
                GetAt( m_pcsColItemList[ m_nShowColumn ].
                  FindIndex( nShowColItemIdx ) )) );
#endif
   return( 0 );
}

LRESULT
ZComboBox::OnGetText( WPARAM wParam, LPARAM lParam )
{
#if 0
   zSHORT nShowColItemIdx;
   zSHORT nBoundColItemCnt;

   nBoundColItemCnt = m_pcsColItemList[ m_nShowColumn ].GetCount( );

//case sensitive:
// for ( nShowColItemIdx = 0;
//       nShowColItemIdx < nBoundColItemCnt &&
//         m_pcsColItemList[ m_nShowColumn ].
//           GetAt( m_pcsColItemList[ m_nShowColumn ].
//             FindIndex( nShowColItemIdx ) ) != (zCPCHAR) lParam;
//       nShowColItemIdx++ );
//case insensitive:
   for ( nShowColItemIdx = 0;
         nShowColItemIdx < nBoundColItemCnt &&
            (m_pcsColItemList[ m_nShowColumn ].
              GetAt( m_pcsColItemList[ m_nShowColumn ].
                FindIndex( nShowColItemIdx ) )).
                  CompareNoCase( (zCPCHAR) lParam ) != 0;
         nShowColItemIdx++ );

   if ( nShowColItemIdx < nBoundColItemCnt )
   {
      zmemcpy( (void *)(zCPCHAR) lParam,
               m_pcsColItemList[ m_nBoundCol ].
                 GetAt( m_pcsColItemList[ m_nBoundCol ].
                   FindIndex( nShowColItemIdx ) ),
               wParam );
      return( 1 );
   }
   else
#endif
      return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZComboColumn *
ZComboBox::FindColumn( zSHORT nCol ) const
{
   ZComboColumn *pCol = m_pHeadCol;
   while ( pCol )
   {
      if ( pCol->m_nCol == nCol )
         return( pCol );

      pCol = pCol->m_pNext;
   }

   return( 0 );
}

void
ZComboBox::AddColumn( zSHORT nCol, zSHORT nColWidth, zLONG lAlignStyle,
                      zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                      zCPCHAR cpcAttribute, zCPCHAR cpcContext )
{
   ZComboColumn *pCol = FindColumn( nCol );
   if ( pCol )
   {
      pCol->SetColumnEAC( cpcViewName, cpcEntity, cpcAttribute, cpcContext );
   }
   else
   {
      pCol = new ZComboColumn( nCol, nColWidth, lAlignStyle, cpcViewName, cpcEntity, cpcAttribute, cpcContext );
      if ( m_pHeadCol )
      {
         ZComboColumn *pColLast = m_pHeadCol;
         while ( pColLast->m_pNext )
            pColLast = pColLast->m_pNext;

         pColLast->m_pNext = pCol;
      }
      else
      {
         m_pHeadCol = pCol;
      }
   }
}

void
ZComboBox::RemoveColumns( )
{
   ZComboColumn *pCol = m_pHeadCol;
   ZComboColumn *pColNext;

   m_pHeadCol = 0;
   while ( pCol )
   {
      pColNext = pCol->m_pNext;
      pCol->m_pNext = 0;
      delete( pCol );
      pCol = pColNext;
   }
}

ZComboColumn::ZComboColumn( zSHORT nCol, zSHORT nColWidth, zLONG lAlignStyle,
                            zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                            zCPCHAR cpcAttribute, zCPCHAR cpcContext )
{
   m_nCol = nCol;
   if ( nColWidth < 0 )
      m_nColWidth = 40;
   else
      m_nColWidth = nColWidth;

   m_lAlignStyle = lAlignStyle;
   m_pchViewName = 0;
   m_pchEntity = 0;
   m_pchAttribute = 0;
   m_pchContext = 0;
   SetColumnEAC( cpcViewName, cpcEntity, cpcAttribute, cpcContext );

   m_pNext = 0;
}

void
ZComboColumn::SetColumnEAC( zCPCHAR cpcViewName,
                            zCPCHAR cpcEntity,
                            zCPCHAR cpcAttribute,
                            zCPCHAR cpcContext )
{
   mDeleteInitA( m_pchViewName );
   mDeleteInitA( m_pchEntity );
   mDeleteInitA( m_pchAttribute );
   mDeleteInitA( m_pchContext );
   zLONG lLth = zstrlen( cpcViewName ) + 1;
   m_pchViewName = new char[ lLth ];
   strcpy_s( m_pchViewName, lLth, cpcViewName );
   lLth = zstrlen( cpcEntity ) + 1;
   m_pchEntity = new char[ lLth ];
   strcpy_s( m_pchEntity, lLth, cpcEntity );
   lLth = zstrlen( cpcAttribute ) + 1;
   m_pchAttribute = new char[ lLth ];
   strcpy_s( m_pchAttribute, lLth, cpcAttribute );
   if ( cpcContext && *cpcContext )
   {
      lLth = zstrlen( cpcContext ) + 1;
      m_pchContext = new char[ lLth ];
      strcpy_s( m_pchContext, lLth, cpcContext );
   }
}

ZComboColumn::~ZComboColumn( )
{
   mDeleteInitA( m_pchViewName );
   mDeleteInitA( m_pchEntity );
   mDeleteInitA( m_pchAttribute );
   mDeleteInitA( m_pchContext );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
IMPLEMENT_DYNAMIC( ZComboEdit, CEdit )

BEGIN_MESSAGE_MAP( ZComboEdit, CEdit )
   ON_WM_PAINT( )
END_MESSAGE_MAP( )

ZComboEdit::ZComboEdit( ZComboBox *pComboBox, HWND hWnd )
{
   zLONG lID = GetWindowLong( pWnd->m_hWnd, GWL_ID );
   SubclassDlgItem( lID, pComboBox );
   m_pComboBox = pComboBox;
   Attach( hWnd );
}

ZComboEdit::~ZComboEdit( )
{
   if ( m_hWnd )
      Detach( );
}

void
ZComboEdit::OnPaint( )
{
   Default( );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
ComboBox( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   return( new ZComboBox( pZSubtask, pWndParent,
                          pzmaComposite, vDialog,
                          nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=CB_SetDisplaySelectSet
// Source Module=zdctlcmb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CB_SetDisplaySelectSet
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the current display select set for
//              a ComboBox control.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              nSelectSet  - The select set for the ComboBox to display
//
//  RETURNS:    0 - Select set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
CB_SetDisplaySelectSet( zVIEW   vSubtask,
                        zCPCHAR cpcCtrlTag,
                        zSHORT  nSelectSet )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZComboBox *pCombo = DYNAMIC_DOWNCAST( ZComboBox, pzma->m_pCtrl );
      if ( pCombo )
      {
         pCombo->SetDisplaySelSet( nSelectSet );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for CB_SetDisplaySelectSet ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=CB_SetSelectSelectSet
// Source Module=zdctlcmb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CB_SetSelectSelectSet
//                                              07/18/93 Modified: 07/18/93
//
//  PURPOSE:    This operation sets the current selection select set for
//              a combobox control. The selection select set is the select
//              set used by the combobox to communicate back to the
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
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
CB_SetSelectSelectSet( zVIEW   vSubtask,
                       zCPCHAR cpcCtrlTag,
                       zSHORT  nSelectSet )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZComboBox *pCombo = DYNAMIC_DOWNCAST( ZComboBox, pzma->m_pCtrl );
      if ( pCombo )
      {
         pCombo->SetSelectSelSet( nSelectSet );
         return( 0 );
      }

      TraceLineS( "drvr - Invalid control type for CB_SetSelectSelectSet ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=CB_SetData
// Source Module=zdctlcmb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CB_SetData
//                                          04/03/2002 Modified: 04/03/2002
//
//  PURPOSE:    This operation sets the data for a "fixed data" combobox.
//              Mapping is ignored.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cpcSearchString - The string to select.
//              cpcListData - Semi-colon separated list of strings (data).
//              lFlag       - 0 ==> ListData is a semi-colon separated list
//                            1 ==> ListData is to be set from the specified
//                                  TableDomain;Context
//
//  RETURNS:    0 - Data set successfully set
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
CB_SetData( zVIEW   vSubtask,
            zCPCHAR cpcCtrlTag,
            zCPCHAR cpcSelectItem,
            zCPCHAR cpcListData,
            zLONG   lFlag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZComboBox *pCombo = DYNAMIC_DOWNCAST( ZComboBox, pzma->m_pCtrl );
      if ( pCombo )
      {
         zPCHAR   pchTableData = 0;
         zCPCHAR  cpcData;
         zSHORT   nRC;

         if ( lFlag & 1 )
         {
            zPVOID   pvCursor;
            LPDOMAIN lpDomain;
            zPCHAR   pchEntity;
            zPCHAR   pchAttribute;
            zPCHAR   pchDomain;
            zPCHAR   pchContext;
            zCHAR    szEntity[ zTAG_LTH ];
            zCHAR    szAttribute[ zTAG_LTH ];
            zCHAR    szDomain[ zTAG_LTH ];
            zCHAR    szContext[ zTAG_LTH ];
            zLONG    lLth = 64000;

            pchTableData = new char[ lLth ];
            strcpy_s( pchTableData, lLth, cpcListData ); // borrow pchTableData
            pchEntity = pchTableData;
            if ( pchEntity &&
                 (pchAttribute = zstrchr( pchEntity, ';' )) != 0 &&
                 (pchDomain = zstrchr( pchAttribute + 1, ';' )) != 0 )
            {
               *pchAttribute = 0;
               pchAttribute++;
               *pchDomain = 0;
               pchDomain++;
               pchContext = zstrchr( pchDomain, ';' );
               if ( pchContext )
               {
                  *pchContext = 0;
                  pchContext++;
                  strcpy_s( szContext, sizeof( szContext ), pchContext );
               }
               else
                  szContext[ 0 ] = 0;

               strcpy_s( szEntity, sizeof( szEntity ), pchEntity );
               strcpy_s( szAttribute, sizeof( szAttribute ), pchAttribute );
               strcpy_s( szDomain, sizeof( szDomain ), pchDomain );
               lpDomain = GetAppDomain( vSubtask, szDomain );

               pchTableData[ 0 ] = 0;
               if ( lpDomain )
               {
                  zLONG  lCurrLth;
                  if ( lFlag & 2 )  // required
                  {
                     lFlag = 1;  // required (for fnGetTableEntryForDomain)
                     lCurrLth = 0;
                  }
                  else
                  {
                     lFlag = 0;
                     lCurrLth = 1;
                     pchTableData[ 0 ] = ';';
                     pchTableData[ 1 ] = 0;
                  }

                  VIEWENTITY ve; // These are not "real" ViewEntity and ViewAttribute
                  VIEWATTRIB va; // structures, but we mock them up just enough to
                                 // permit the call to fnGetTableEntryForDomain to work.

                  strcpy_s( ve.szName, sizeof( ve.szName ), szEntity );
                  strcpy_s( va.szName, sizeof( ve.szName ), szAttribute );
                  va.hDomain = (struct DomainStruct *) SysGetHandleFromPointer( lpDomain );

                  do
                  {
                     if ( lCurrLth > 0 && pchTableData[ lCurrLth - 1 ] != ';' )
                     {
                        pchTableData[ lCurrLth++ ] = ';';
                        pchTableData[ lCurrLth ] = 0;
                     }

                     if ( lCurrLth >= lLth - 256 )
                     {
                        zPCHAR pch = pchTableData;
                        lLth += 4096;
                        pchTableData = new char[ lLth ];
                        strcpy_s( pchTableData, lLth, pch );
                        delete [] pch;
                     }

                     nRC = fnGetTableEntryForDomain( vSubtask, &ve, &va,
                                                     lpDomain, szContext,
                                                     pchTableData + lCurrLth,
                                                     lLth - lCurrLth,
                                                     &pvCursor, lFlag );
                     lCurrLth += zstrlen( pchTableData + lCurrLth );
                     lFlag |= 2;  // next

                  } while ( nRC >= 0 );

                  if ( lCurrLth && pchTableData[ lCurrLth - 1 ] == ';' )
                  {
                     lCurrLth--;
                     pchTableData[ lCurrLth ] = 0;
                  }
               }
            }

            cpcData = pchTableData;
         }
         else
            cpcData = cpcListData;

         nRC = pCombo->SetData( cpcSelectItem, cpcData );
         mDeleteInitA( pchTableData );
         return( nRC );
      }

      TraceLineS( "drvr - Invalid control type for CB_SetData ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=CB_GetSelectedString
// Source Module=zdctlcmb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CB_GetSelectedString
//                                          04/03/2002 Modified: 04/03/2002
//
//  PURPOSE:    This operation gets the selected string for a combobox.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              pchSelectedString - The selected string (returned) ... must
//                            be long enough to receive longest string in
//                            ComboBox ... 256 should be good.
//
//  RETURNS: >= 0 - Selected string successfully returned.
//             -1 - Error locating control
//             -2 - No selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
CB_GetSelectedString( zVIEW   vSubtask,
                      zCPCHAR cpcCtrlTag,
                      zPCHAR  pchSelectedString,
                      zLONG   lMaxLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZComboBox *pCombo = DYNAMIC_DOWNCAST( ZComboBox, pzma->m_pCtrl );
      if ( pCombo )
      {
         return( pCombo->GetSelectedString( pchSelectedString, lMaxLth ) );
      }

      TraceLineS( "drvr - Invalid control type for CB_GetSelectedString ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=CB_SelectString
// Source Module=zdctlcmb.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CB_SetSelectedString
//                                          04/03/2002 Modified: 04/03/2002
//
//  PURPOSE:    This operation sets the selected string for a combobox.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be disabled.
//              cpcCtrlTag  - The unique name of the control.
//              cpcSelectedString - The string to select ... if the string
//                            is not found in the list, it is not selected.
//              lFlags      - 0 ==> ignore case; 1 ==> exact match
//
//  RETURNS: >= 0 - String successfully selected.
//             -1 - Error locating control
//             -2 - No selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG OPERATION
CB_SelectString( zVIEW   vSubtask,
                 zCPCHAR cpcCtrlTag,
                 zCPCHAR cpcSearchString,
                 zLONG   lFlags )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZComboBox *pCombo = DYNAMIC_DOWNCAST( ZComboBox, pzma->m_pCtrl );
      if ( pCombo )
      {
         zLONG lFirstPos;

         if ( lFlags & 1 )
         {
            lFirstPos = pCombo->SelectExactString( -1, cpcSearchString );
         // TraceLine( "CB_SelectString for String: %s  Position: %d", cpcSearchString, lFirstPos );
         }
         else
         {
            lFirstPos = pCombo->SelectString( -1, cpcSearchString );
            if ( lFirstPos >= 0 )
               pCombo->SetCurSel( lFirstPos );
            else
               lFirstPos = -2;
         }

         return( lFirstPos );
      }

      TraceLineS( "drvr - Invalid control type for CB_SelectString ", cpcCtrlTag );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZMCComboEdit

BEGIN_MESSAGE_MAP( ZMCComboEdit, CEdit )
   //{{AFX_MSG_MAP( ZMCComboEdit )
   ON_WM_PAINT( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

ZMCComboEdit::ZMCComboEdit( )
{
}

ZMCComboEdit::~ZMCComboEdit( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ZMCComboEdit message handlers

LRESULT
ZMCComboEdit::WindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch ( uMsg )
   {
      case WM_GETTEXTLENGTH:
      {
         zCHAR  szString[ 256 ];
         zSHORT nShowItemLth;

         GetLine( 0, szString, sizeof( szString ) );
         for ( nShowItemLth = 0;
               szString[ nShowItemLth ] != 0;
               nShowItemLth++ )
         {
            // do nothing
         }

         if ( GetParent( )->SendMessage( WM_APP + WM_GETTEXT, 255, (LPARAM) szString ) != 0 )
         {
            zSHORT  nBoundItemLth;
            for ( nBoundItemLth = 0;
                  szString[ nBoundItemLth ] != 0;
                  nBoundItemLth++ )
            {
               // do nothing
            }

            return( max( nBoundItemLth + 1, nShowItemLth + 1 ) );
         }
         else
            return( nShowItemLth + 1 );

         break;
      }

      case WM_GETTEXT:
         CEdit::WindowProc( uMsg, wParam, lParam );  // get text in edit ctrl
         return( GetParent( )->SendMessage( WM_APP + WM_GETTEXT, wParam, lParam ) ); // replace it
         break;

      case WM_SETTEXT:     // replace text, then set text
         return( CEdit::WindowProc( uMsg, wParam,
                                    (LPARAM) GetParent( )->SendMessage( WM_APP + WM_SETTEXT, wParam, lParam ) ) );
         break;

      default:
         return( CEdit::WindowProc( uMsg, wParam, lParam ) );
         break;
   }
}

void
ZMCComboEdit::OnPaint( )
{
   CPaintDC dc( this ); // device context for painting

   zCHAR szString[ 256 ];

   GetLine( 0, szString, sizeof( szString ) );
   dc.SelectObject( ::GetStockObject( DEFAULT_GUI_FONT ) );

   if ( mIs_hWndEnabled( m_hWnd ) == FALSE )
      dc.SetBkColor( GetSysColor( COLOR_3DFACE ) );

   dc.FillRect( &(dc.m_ps.rcPaint), &CBrush( dc.GetBkColor( ) ) );
   dc.DrawText( szString, &(dc.m_ps.rcPaint),
                DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_BOTTOM );

   // Do not call CEdit::OnPaint( ) for painting messages
}
