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
// Source file for implementation of Zeidon ToolBar control.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.04.10    DKS
//    Prohibit focus via Tab key.  Process event whenever change occurs in
//    ComboBox on ToolBar.
//
// 2002.03.14    DKS
//    Added text mode and supporting operations.
//

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlBar.h"
#include "ZdCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
// ZEditBar

#if 1

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zTOOLBAR_PALETTE      zCONTROL_SPECIFIC_1
#define zTOOLBAR_TEXT         zCONTROL_SPECIFIC_2
#define zTOOLBAR_TOOLTIPS     zCONTROL_SPECIFIC_3
#define zTOOLBAR_BOTTOM       zCONTROL_CENTERJUSTIFY
#define zTOOLBAR_RIGHT        zCONTROL_RIGHTJUSTIFY
#define zTOOLBAR_LEFT         zCONTROL_LEFTJUSTIFY

#define COMBOBOX_WIDTH  150
#define COMBOBOX_HEIGHT 150

#if 0
#define COMBOBOX_INDEX 5
#define COMBOBOX_BITMAP 4
#endif

IMPLEMENT_DYNAMIC( ZEditBar, CToolBar )
BEGIN_MESSAGE_MAP( ZEditBar, CToolBar )
   //{{AFX_MSG_MAP( ZEditBar )
   ON_WM_CREATE( )
   ON_WM_NCCALCSIZE( )
   ON_WM_PAINT( )
   ON_WM_NCPAINT( )
   ON_WM_SYSCOLORCHANGE( )
   ON_WM_LBUTTONDBLCLK( )
   ON_WM_WINDOWPOSCHANGING( )
   ON_WM_WINDOWPOSCHANGED( )
   ON_WM_SETFOCUS( )
   ON_CONTROL_REFLECT( CN_COMMAND, OnControlReflectCommand )
// ON_CONTROL_REFLECT( TBN_DROPDOWN, OnButtonDropDown )
   ON_MESSAGE( zZEIDON_GET_TOOLTIP_TEXT, OnGetToolTipText )
   ON_MESSAGE( zZEIDON_TOOLBAR_DROPDOWN, OnToolBarDropDown )
   ON_MESSAGE( zZEIDON_TOOLBAR_SET_SELECT, OnToolBarSetSelect )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZEditBar
//
// The child window ID of a control bar is important.  The default layout of
// control bar only works for control bars with IDs in the range of
// AFX_IDW_CONTROLBAR_FIRST to AFX_IDW_CONTROLBAR_LAST.  Note that even
// though there is a range of 256 control bar IDs, the first 32 of these
// control bar IDs are special since they are directly supported by the print
// preview architecture.

ZEditBar::ZEditBar( ZSubtask *pZSubtask,        // ctor
                    CWnd     *pWndParent,
                    ZMapAct  *pzmaComposite,
                    zVIEW    vDialog,
                    zSHORT   nOffsetX,
                    zSHORT   nOffsetY,
                    zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
          CToolBar( ),
          ZMapAct( pZSubtask,          // base class ctor
                   pzmaComposite,
                   pWndParent,
                   this,
                   vDialog,
                   nOffsetX,
                   nOffsetY,
                   pCtrlDef,
                   "ToolBar" )
{
   zULONG ulButtonInfoLth;

   m_bColor = TRUE;
   m_nBtnCnt = 0;
   m_nBarItemCnt = 0;
   m_pchButtonInfo = 0;
   m_puButtons = 0;
   m_ulFirstBarCmdId = 0;
   m_bMenuRemoved = FALSE;
   m_uLastDropDownId = -1;
   m_lLastSelect = -1;

   m_bShowIconMode = FALSE;      // small icon
   m_sizeSmallIcon = CSize( 16, 16 );
   m_sizeLargeIcon = CSize( 32, 32 );
   m_bOldFloating = FALSE;

   m_nTextWidth = 0;
   m_nTextHeight = 0;

   Attr.Style = WS_CHILD | WS_VISIBLE;
   if ( pCtrlDef->Subtype & zTOOLBAR_BOTTOM )
   {
      m_bVertical = FALSE;
      Attr.Style |= CBRS_BOTTOM;
   }
   else
   if ( pCtrlDef->Subtype & zTOOLBAR_RIGHT )
   {
      m_bVertical = TRUE;
      Attr.Style |= CBRS_RIGHT;
   }
   else
   if ( pCtrlDef->Subtype & zTOOLBAR_LEFT )
   {
      m_bVertical = TRUE;
      Attr.Style |= CBRS_LEFT;
   }
   else
   {
      m_bVertical = FALSE;
      Attr.Style |= CBRS_TOP;
   }

   if ( pCtrlDef->Subtype & zTOOLBAR_TOOLTIPS )
      Attr.Style |= (CBRS_TOOLTIPS | CBRS_FLYBY);

   if ( pCtrlDef->Subtype & zTOOLBAR_TEXT )
      m_bShowIconText = TRUE;       // text
   else
      m_bShowIconText = FALSE;      // no text

   Attr.Style &= ~WS_TABSTOP;
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;

   GetAttributeLength( &ulButtonInfoLth, vDialog, szlCtrl, szlCtrlBOI );
   if ( ulButtonInfoLth )
   {
      zULONG   ulId;
      zULONG   ulType;
      zPCHAR   pchSemiColon;
      zCPCHAR  cpcTag;
      zCPCHAR  cpcText;
      zCPCHAR  cpcTipText;
      zCPCHAR  cpcAction;
      zCPCHAR  cpcViewName;
      zCPCHAR  cpcEntity;
      zCPCHAR  cpcAttribute;
      zCPCHAR  cpcScope;
      zCPCHAR  cpc;
      zSHORT   nBtnCnt;
      zSHORT   k;
      zSHORT   j;

      m_pchButtonInfo = new char[ ulButtonInfoLth ];
      GetVariableFromAttribute( m_pchButtonInfo, &ulButtonInfoLth, zTYPE_BLOB,
                                ulButtonInfoLth, vDialog, szlCtrl, szlCtrlBOI,
                                0, 0 );

      // We first have 4 longs followed by the bitmap res_id;file_name,
      // followed by the list of:
      //    type/tag/text/tip/action/view/entity/attribute/scope
      // labels for each button (only combobox and dropdown button types
      // require mapping information).
      k = 4 * sizeof( zLONG );
      m_csDLL_Name = m_pchButtonInfo + k;
      k += m_csDLL_Name.GetLength( ) + 1;

      pchSemiColon = zstrchr( m_pchButtonInfo + k, ';' );
      if ( pchSemiColon )
      {
         *pchSemiColon = 0;
         m_csResName = m_pchButtonInfo + k;
         m_csBitmapFile = pchSemiColon + 1;
         *pchSemiColon = ';';
      }
      else
      {
         m_csResName = m_pchButtonInfo + k;
         m_csBitmapFile = "";
      }

      k += (zSHORT) zstrlen( m_pchButtonInfo + k ) + 1;
      nBtnCnt = (zSHORT) (*((zPLONG) (m_pchButtonInfo + (2 * sizeof( zLONG )))));

      m_nBarItemCnt =  // includes separators
            (zSHORT) (*((zPLONG) (m_pchButtonInfo + (3 * sizeof( zLONG )))));
      m_puButtons = new UINT[ m_nBarItemCnt + 1 ];
      m_ulFirstBarCmdId = m_pZSubtask->m_ulNextBarTipCmd;

      k = (zSHORT) (*((zPLONG) m_pchButtonInfo) + (4 * sizeof( zLONG )));
      cpc = m_pchButtonInfo + k;  // point to first item
      for ( k = 0, j = 0;
            k < nBtnCnt && (m_pchButtonInfo + ulButtonInfoLth) > cpc;
            k++ )
      {
         ulType = (*(zPULONG) cpc);
         cpc += sizeof( zLONG );

         // Same order as in the bitmap.
         ulId = m_ulFirstBarCmdId + k;
         m_puButtons[ j++ ] = ulId;
         if ( ulType & 0x80000000 )
         {
            m_puButtons[ j++ ] = ID_SEPARATOR;
            ulType &= ~0x80000000;
         }

         if ( *cpc )
         {
            cpcTag = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcTag = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcText = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcText = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcTipText = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcTipText = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcAction = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcAction = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcViewName = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcViewName = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcEntity = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcEntity = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcAttribute = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcAttribute = 0;
            cpc++;
         }

         if ( *cpc )
         {
            cpcScope = cpc;
            cpc += zstrlen( cpc ) + 1;
         }
         else
         {
            cpcScope = 0;
            cpc++;
         }

         AddBarItem( ulId, ulType, cpcTag, cpcText, cpcTipText, cpcAction,
                     cpcViewName, cpcEntity, cpcAttribute, cpcScope );

         RegisterEvent( m_pZSubtask->m_ulNextBarTipCmd );  // change this soon???
         m_pZSubtask->m_ulNextBarTipCmd++;
      }

      if ( k != nBtnCnt )
         TraceLineI( "ZToolBar ctor invalid button count: ", k );

      if ( j != m_nBarItemCnt )
         TraceLineI( "ZToolBar ctor invalid item count: ", j );

      m_puButtons[ m_nBarItemCnt ] = 0;
      CreateZ( );
   }
}

void
ZEditBar::OnLButtonDblClk( UINT nFlags, CPoint pt )
{
   // Stop bar from being changed to window via double click.
}

// Call when the toolbar is moved.  There is a bug when the toolbar is in
// flat mode, and in the vertical position.  The separators are not counted
// in the height, so the last button is not completely displayed.
void
ZEditBar::OnWindowPosChanging( LPWINDOWPOS pWindowPos )
{
// TraceLineS( "ZEditBar::OnWindowPosChanging: ", *m_pzsTag );

   // Default processing.
   if ( m_bOldFloating != (IsFloating( ) ? TRUE : FALSE) )
   {
      m_bOldFloating = !m_bOldFloating;
      pWindowPos->flags |= SWP_DRAWFRAME;
   }

   CToolBar::OnWindowPosChanging( pWindowPos );
}

// Remove close button from floating toolbar.
void
ZEditBar::OnWindowPosChanged( WINDOWPOS FAR *pWndPos )
{
   CToolBar::OnWindowPosChanged( pWndPos );

   // Should only be called once, when floated.
   if ( IsFloating( ) )
   {
   // TraceLineS( "ZEditBar::OnWindowPosChanged (floating): ", *m_pzsTag );
      if ( m_pDockBar && m_bMenuRemoved == FALSE )
      {
         CWnd *pParent = m_pDockBar->GetParent( );
         if ( pParent->IsKindOf( RUNTIME_CLASS( CMiniFrameWnd ) ) )
         {
            pParent->ModifyStyle( WS_SYSMENU, 0, 0 );
            m_bMenuRemoved = TRUE;
            PostMapFromOI( 0 );
         }
      }
   }
   else
   {
   // TraceLineS( "ZEditBar::OnWindowPosChanged (docked): ", *m_pzsTag );
      if ( m_bMenuRemoved )
      {
         m_bMenuRemoved = FALSE;
         PostMapFromOI( 0 );
      }
   }
}

void
ZEditBar::CreateZ( )
{
   zBOOL bRC = FALSE;
   LPLIBRARY hLibrary;
   HINSTANCE hInstDLL = 0;

   if ( m_csDLL_Name && *m_csDLL_Name )   // DLL_Name
   {
      if ( m_pZSubtask->m_pZTask->m_uAppState & zAPP_STATE_NT )
      {
         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               m_csDLL_Name,
                                               zLOADLIB_RESOURCES );
         hInstDLL = (HINSTANCE) SysGetInstance( hLibrary );
      }
      else
      {
         zFARPROC_HINSTANCE lpfnDynRoutine;

         hLibrary = SysLoadLibraryWithErrFlag( m_pZSubtask->m_vDialog,
                                               m_csDLL_Name, 0 );

         lpfnDynRoutine = (zFARPROC_HINSTANCE)
            GetOperationDynamicCallAddress( m_pZSubtask->m_vDialog,
                                            (LPLPLIBRARY) &hLibrary,
                                            m_csDLL_Name, "GetDLL_Instance",
                                            0 );  // no error message
   //                                       "(ZEditBar)" );
         if ( lpfnDynRoutine )
            hInstDLL = (*lpfnDynRoutine)( m_pZSubtask->m_vDialog );
      }
   }
   else
   {
      hInstDLL = m_pZSubtask->m_hInstRes;
   }

   if ( hInstDLL )
   {
      HINSTANCE hInst = AfxGetResourceHandle( );
      AfxSetResourceHandle( hInstDLL );

      bRC = Init( m_pZSubtask->m_pZFWnd, TRUE );
      AfxSetResourceHandle( hInst );
   }
   else
   {
      bRC = FALSE;
   }

   if ( bRC == FALSE )
   {
      TraceLine( "ToolBar Failed to Load Bitmap Id: %d for %d items  "
                    "Res: %s  File: %s  Tag: %s.%s.%s",
                 m_nIdNbr, m_nBarItemCnt,
                 (zCPCHAR) m_csResName, (zCPCHAR) m_csBitmapFile,
                 *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag),
                 *m_pzsTag );

      DestroyWindow( );
      return;
   }

   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   m_pZSubtask->AddBarTip( m_hWnd, m_ulFirstBarCmdId,
                           m_ulFirstBarCmdId + m_nBtnCnt - 1 );
   SetWindowText( *m_pzsText );  // when the toolbar is not docked

   // TODO: Delete these three lines if you don't want the toolbar to
   // be dockable.
   EnableDocking( CBRS_ALIGN_ANY );
   m_pZSubtask->m_pZFWnd->EnableDocking( CBRS_ALIGN_ANY );
   m_pZSubtask->m_pZFWnd->DockControlBar( this );
}

/////////////////////////////////////////////////////////////////////////////
//
// All the button images in the toolbar are taken from one bitmap, which
// must contain one image for each button. All images must be the same
// size; the default is 16 pixels wide and 15 pixels high.  Images must
// be side by side in the bitmap.
//
// Remarks: SetButtons sets each toolbar button's command ID to the value
// specified by the corresponding element of the array lpIDArray.  If an
// element of the array has the value ID_SEPARATOR, a separator is created
// in the corresponding position of the toolbar.  This function also sets
// each button's style to TBBS_BUTTON and each separator's style to
// TBBS_SEPARATOR, and assigns an image index to each button.  The image
// index specifies the position of the button's image within the bitmap.
//
// You do not need to account for separators in the bitmap because this
// function does not assign image indexes for separators.  If your toolbar
// has buttons at positions 0, 1, and 3 and a separator at position 2,
// the images at positions 0, 1, and 2 in your bitmap are assigned to the
// buttons at positions 0, 1, and 3, respectively.
// If lpIDArray is null, this function allocates space for the number of
// items specified by nIDCount.
//
/////////////////////////////////////////////////////////////////////////////
zBOOL
ZEditBar::Init( CWnd *pParentWnd, zBOOL bColor )
{
   m_bColor = bColor;
// m_bVertical = FALSE;

   // Start out with no borders.
   DWORD  dwStyle = Attr.Style | CBRS_SIZE_DYNAMIC;
   zCHAR  szBuffer[ 40 ];
   zSHORT nLth = zsizeof( "-2147483647," ) * 4; // style, bar item cnt, x, y
   zPCHAR pchBuffer = new char[ nLth + 1 ];
   zPCHAR pch;
   zPCHAR pchComma;
   zLONG  lStyle = 0;
   zSHORT nItemCnt = 0;
   zLONG  lPosX = 0;
   zLONG  lPosY = 0;

   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   GetWindowPreferenceString( m_pZSubtask->m_vDialog,
                              szBuffer, pchBuffer, nLth );
   if ( *pchBuffer )
   {
      pch = pchBuffer;
      pchComma = zstrchr( pch, ',' );
      if ( pchComma )
         *pchComma = 0;

      lStyle = zatol( pch );
      if ( pchComma )
      {
         pch = pchComma + 1;
         pchComma = zstrchr( pch, ',' );
         if ( pchComma )
            *pchComma = 0;

         nItemCnt = (zSHORT) zatol( pch );
         if ( pchComma )
         {
            pch = pchComma + 1;
            pchComma = zstrchr( pch, ',' );
            if ( pchComma )
               *pchComma = 0;

            lPosX = zatol( pch );
            if ( pchComma )
            {
               pch = pchComma + 1;
               pchComma = zstrchr( pch, ',' );
               if ( pchComma )
                  *pchComma = 0;

               lPosY = zatol( pch );
            }
         }
      }
   }

   if ( lStyle && nItemCnt == m_nBarItemCnt )
   {
      if ( (lStyle & CBRS_LEFT) == CBRS_LEFT )
      {
         dwStyle &= ~(CBRS_TOP | CBRS_BOTTOM | CBRS_RIGHT);
         dwStyle |= CBRS_LEFT;
         m_bVertical = TRUE;
      }
      else
      if ( (lStyle & CBRS_BOTTOM) == CBRS_BOTTOM )
      {
         dwStyle &= ~(CBRS_TOP | CBRS_LEFT | CBRS_RIGHT);
         dwStyle |= CBRS_BOTTOM;
         m_bVertical = FALSE;
      }
      else
      if ( (lStyle & CBRS_RIGHT) == CBRS_RIGHT )
      {
         dwStyle &= ~(CBRS_TOP | CBRS_LEFT | CBRS_BOTTOM);
         dwStyle |= CBRS_RIGHT;
         m_bVertical = TRUE;
      }
      else
      if ( (lStyle & CBRS_TOP) == CBRS_TOP )
      {
         dwStyle &= ~(CBRS_LEFT | CBRS_BOTTOM | CBRS_RIGHT);
         dwStyle |= CBRS_TOP;
         m_bVertical = FALSE;
      }
   }

   mDeleteInitA( pchBuffer );

   if ( Create( pParentWnd, dwStyle, m_nIdNbr ) == FALSE ||
        SetColor( m_bColor ) == FALSE ||
        SetButtons( m_puButtons, m_nBarItemCnt ) == FALSE )
   {
      return( FALSE );
   }

   if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_nTraceAction > 2 )
   {
      TraceLine( "ToolBar Loaded Bitmap Id: %d for %d items  "
                    "Res: %s  File: %s  Tag: %s.%s.%s",
                 m_nIdNbr, m_nBarItemCnt,
                 (zCPCHAR) m_csResName, (zCPCHAR) m_csBitmapFile,
                 *(m_pZSubtask->m_pzsDlgTag), *(m_pZSubtask->m_pzsWndTag),
                 *m_pzsTag );
   }

   ZBarItem *pBarItem;
   CRect    rect( -COMBOBOX_WIDTH, -COMBOBOX_HEIGHT, 0, 0 );
   zULONG   uID;

   for ( zLONG k = 0; k < m_nBtnCnt; k++ )
   {
      pBarItem = LocateBarItem( m_ulFirstBarCmdId + k );
      if ( pBarItem )
      {
         if ( pBarItem->m_ulType == 2 )  // DropButton
         {
            SetButtonDropDown( pBarItem->m_ulId );
         }
         else
         if ( pBarItem->m_ulType == 3 ||  // ComboBox
              pBarItem->m_ulType == 4 )   // EditCombo
         {
            // The ID of the ComboBox is important for two reasons.  One,
            // so you can receive notifications from the control.  And also
            // for ToolTips.  During hit testing, if the ToolBar sees that the
            // mouse is on a child control, the toolbar will look up the
            // control's ID and search for a string in the string table with
            // the same ID to use for ToolTips and StatusBar info.
            uID = pBarItem->m_ulId + 0x0100;
            pBarItem->m_pBarCombo = new ZTB_Combo( uID );

            if ( pBarItem->m_ulType == 3 )
               lStyle = CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS;
            else
               lStyle = CBS_DROPDOWN | CBS_AUTOHSCROLL | CBS_HASSTRINGS;

            if ( pBarItem->m_pBarCombo->Create( WS_CHILD | WS_VSCROLL | lStyle,
                                                rect, this, uID ) == 0 )
            {
               mDeleteInit( pBarItem->m_pBarCombo );
               return( FALSE );
            }
         }
      }
   }

   // Retrieve the height of the toolbar before putting text.
   CToolBarCtrl& bar = GetToolBarCtrl( );
// ZBarItem *pBarItem;
   TBBUTTON tb;
   zSHORT   nIndex = 0;
   CRect    rectNoText( 0, 0, 0, 0 );
   bar.GetItemRect( 0, rectNoText );

   // Set the text for each button.
   for ( nIndex = bar.GetButtonCount( ); nIndex >= 0; nIndex-- )
   {
      zmemset( &tb, 0, sizeof( TBBUTTON ) );
      bar.GetButton( nIndex, &tb );

      // Do we have a separator?
      if ( (tb.fsStyle & TBSTYLE_SEP) == TBSTYLE_SEP )
          continue;

      // Do we have a valid command id?
      if ( tb.idCommand == 0 )
         continue;

      pBarItem = LocateBarItem( tb.idCommand );
      if ( pBarItem && pBarItem->m_pchText && pBarItem->m_pchText[ 0 ] )
      {
         SetButtonText( nIndex, pBarItem->m_pchText );
      }
   }

   // Calculate the effect of the text on the toolbar.
// CRect rect( 0, 0, 0, 0 );
   rect.SetRect( 0, 0, 0, 0 );
   CSize sizeMax( 0, 0 );

   for ( nIndex = bar.GetButtonCount( ); nIndex >= 0; nIndex-- )
   {
      bar.GetItemRect( nIndex, rect );

      rect.NormalizeRect( );
      sizeMax.cx = __max( rect.Size( ).cx, sizeMax.cx );
      sizeMax.cy = __max( rect.Size( ).cy, sizeMax.cy );
   }

   // Resize the toolbar.
   // The text width is the maximum width of the bitmap.  All toolbar sizes
   // must be at least this width.
   // The text height is the height added to the button.  Even in large mode
   // we must add this text height.
   m_nTextWidth = (zSHORT) sizeMax.cx;
   m_nTextHeight = (zSHORT) (sizeMax.cy - rectNoText.Size( ).cy);

   ResizeToolBar( );

#if 0
   // Build the image list.
   CClientDC dc( this );
   CSize size;
   int nNbBits = dc.GetDeviceCaps( BITSPIXEL );
   for ( k = 0; k < NB_POSSIBLE_MODE; k++ )
   {
      UINT nColorMode = ILC_COLOR;
      if ( nNbBits > 8 )
         nColorMode = ILC_COLORDDB;

      size = m_sizeLargeIcon;
      if ( k < 3 )
         size = m_sizeSmallIcon;

      m_ImageList[ k ].Create( size.cx, size.cy,
                               nColorMode | ILC_MASK,
                               bar.GetButtonCount( ), 10 );
   }
#endif

   if ( m_bVertical )
   {
      if ( SetVertical( ) == FALSE )
         return( FALSE );
   }
   else
   {
      if ( SetHorizontal( ) == FALSE )
         return( FALSE );
   }

   return( TRUE );
}

// Trap to set the flat bar style.
int
ZEditBar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CToolBar::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   // Set the style to flat.
   // There is a bug when docking vertically.  The size of the separator
   // is not calculated correctly by MFC.  Only in flat style.
   ModifyStyle( 0, TBSTYLE_FLAT );

   // Set the drop arrow.
   CToolBarCtrl& bar = GetToolBarCtrl( );
   bar.SetExtendedStyle( TBSTYLE_EX_DRAWDDARROWS );

   // Initialization of the ListCtrl
// m_lstPopup.Create( WS_CHILD | LVS_REPORT |
//                    LVS_SINGLESEL | LVS_SHOWSELALWAYS,
   m_lstPopup.Create( LVS_SMALLICON | LVS_LIST | LVS_SINGLESEL |
                      LVS_SHOWSELALWAYS | LVS_AUTOARRANGE,
                      CRect( 0, 0, 195, 125 ), this, 2 );
// m_lstPopup.SetExtendedStyle( LVS_EX_TRACKSELECT );

   return( nRC );  // toolbar is now created
}

BOOL
ZEditBar::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEditBar::DestroyWindow ", *m_pzsTag );
#endif
   zCHAR szBuffer[ 40 ]; // tag length + 7

   zLONG  lLth = zsizeof( "-2147483647," ) * 4;
   zPCHAR pch = new char[ lLth ];
   zLONG  lStyle = GetBarStyle( );
   zSHORT nStyleLth;

   if ( (lStyle & CBRS_TOP) == CBRS_TOP )
   {
      lStyle = CBRS_TOP;
   }
   else
   if ( (lStyle & CBRS_LEFT) == CBRS_LEFT )
   {
      lStyle = CBRS_LEFT;
   }
   else
   if ( (lStyle & CBRS_BOTTOM) == CBRS_BOTTOM )
   {
      lStyle = CBRS_BOTTOM;
   }
   else
   if ( (lStyle & CBRS_RIGHT) == CBRS_RIGHT )
   {
      lStyle = CBRS_RIGHT;
   }
   else
   {
      lStyle = 0;
   }

   _ltoa_s( lStyle, pch, lLth, 10 );
   nStyleLth = (zSHORT) zstrlen( pch );
   pch[ nStyleLth++ ] = ',';
   _ltoa_s( m_nBarItemCnt, pch + nStyleLth, lLth - nStyleLth, 10 );

   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );
   mDeleteInitA( pch );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( CToolBar::DestroyWindow( ) );
}

ZEditBar::~ZEditBar( )  // dtor
{
   CleanupBarItems( );
   mDeleteInitA( m_pchButtonInfo );
   mDeleteInitA( m_puButtons );
}

zSHORT
ZEditBar::MapFromOI( WPARAM wFlag )
{
   ZBarItem *pBarItem;

   for ( zLONG k = 0; k < m_nBtnCnt; k++ )
   {
      pBarItem = LocateBarItem( m_ulFirstBarCmdId + k );
      if ( pBarItem &&
           (pBarItem->m_ulType == 3 ||  // ComboBox
            pBarItem->m_ulType == 4) && // EditCombo
           pBarItem->m_pBarCombo &&
           mIs_hWnd( pBarItem->m_pBarCombo->m_hWnd ) )
      {
         HFONT hFont = (HFONT) GetStockObject( DEFAULT_GUI_FONT );
         if ( hFont == 0 )
            hFont = (HFONT) GetStockObject( ANSI_VAR_FONT );

         pBarItem->m_pBarCombo->SendMessage( WM_SETFONT, (WPARAM) hFont );

         // Populate the ComboBox.
         pBarItem->m_pBarCombo->ResetContent( );
         if ( pBarItem->m_cpcViewName )
         {
            zVIEW v1;

            GetViewByName( &v1, pBarItem->m_cpcViewName,
                           m_pZSubtask->m_vDialog, zLEVEL_ANY );
            if ( v1 )
            {
               zVIEW  v;
               zCHAR  szMap[ 256 ];
               zLONG  lCnt = 0;
               zSHORT nRC;

               CreateViewFromViewForTask( &v, v1, m_pZSubtask->m_vDialog );
               nRC = SetCursorFirstEntity( v, pBarItem->m_cpcEntity, 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                                  zsizeof( szMap ) - 1, v,
                                                  pBarItem->m_cpcEntity,
                                                  pBarItem->m_cpcAttribute,
                                                  0, zUSE_DEFAULT_CONTEXT );
                  if ( nRC == zVAR_NULL )
                     szMap[ 0 ] = 0;

                  if ( pBarItem->m_pBarCombo->InsertString( lCnt,
                                                            szMap ) == -1 )
                  {
                     TraceLineI( "(ToolCB) Error inserting item: ", lCnt );
                     break;
                  }

                  lCnt++;
                  nRC = SetCursorNextEntity( v, pBarItem->m_cpcEntity, 0 );
               }

               DropView( v );
               nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                               zsizeof( szMap ) - 1, v1,
                                               pBarItem->m_cpcEntity,
                                               pBarItem->m_cpcAttribute,
                                               0, zUSE_DEFAULT_CONTEXT );
               if ( nRC == zVAR_NULL )
                  szMap[ 0 ] = 0;

               if ( lCnt > 0 )
                  pBarItem->m_pBarCombo->SelectString( -1, szMap );
            }
         }
      }
   }

   return( 0 );
}

zSHORT
ZEditBar::PostMapFromOI( WPARAM wFlag )
{
   if ( m_bVertical )
      SetVertical( );
   else
      SetHorizontal( );

   return( 0 );
}

#if 0 // the default method should be just fine
zBOOL
ZEditBar::IsEventHandled( zLONG lEvent )
{
   return( TRUE );
}
#endif

zLONG
ZEditBar::HandleEvent( zLONG lEvent )
{
   ZBarItem *pItem = LocateBarItem( lEvent );
   if ( pItem && pItem->m_cpcAction )
   {
      zLONG lSyncIdx;

#if 0 //def DEBUG_ALL  lots of messages
      TraceLineS( "ZEditBar::HandleEvent Action: ", pItem->m_cpcAction );
#endif
      if ( SetCursorFirstEntityByString( m_pZSubtask->m_vDialog, "Act",
                                         "Tag", pItem->m_cpcAction,
                                         0 ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lSyncIdx, m_pZSubtask->m_vDialog,
                                  "Act", "SyncIdx" );

         if ( m_pZSubtask->m_pchEnabledAction[ lSyncIdx +
                                                     sizeof( zSHORT ) ] > 0 )
         {
            return( 1 );   // inform CmdUI that event has a handler
         }
      }
   }

#if 0 //def DEBUG_ALL  lots of messages
   TraceLineI( "ZEditBar::HandleEvent Action: ", lEvent );
#endif
   return( 0 );   // inform CmdUI that event does not have a handler
}

zBOOL
ZEditBar::SetHorizontal( )
{
   m_bVertical = FALSE;
// SetBarStyle( GetBarStyle( ) | CBRS_ALIGN_TOP );

   ZBarItem *pBarItem;

   for ( zLONG k = 0; k < m_nBtnCnt; k++ )
   {
      pBarItem = LocateBarItem( m_ulFirstBarCmdId + k );
      if ( pBarItem &&
           (pBarItem->m_ulType == 3 ||  // ComboBox
            pBarItem->m_ulType == 4) && // EditCombo
           pBarItem->m_pBarCombo &&
           mIs_hWnd( pBarItem->m_pBarCombo->m_hWnd ) )
      {
         int   iImage;
         UINT  uStyle;
         UINT  uId = pBarItem->m_ulId + 0x0100;
         zLONG lIndex = CommandToIndex( pBarItem->m_ulId );

         // This member function retrieves the control ID, style, and image
         // index of the toolbar button or separator at the location specified
         // by nIndex.  Those values are assigned to the variables referenced
         // by nID, nStyle, and iImage.  The image index is the position of
         // the image within the bitmap that contains images for all the
         // toolbar buttons.  The first image is at position 0.  If nIndex
         // specifies a separator, iImage is set to the separator width in pixels.
         GetButtonInfo( lIndex, uId, uStyle, iImage );

         // Call this member function to set the button's command ID, style,
         // and image number.  For separators (which have the style
         // TBBS_SEPARATOR), this function sets the separator's width in
         // pixels to the value stored in iImage.
         SetButtonInfo( lIndex, uId, TBBS_SEPARATOR, COMBOBOX_WIDTH );
         if ( mIs_hWnd( pBarItem->m_pBarCombo->m_hWnd ) )
         {
            CRect rect;
            GetItemRect( lIndex, rect );

            if ( m_bShowIconText )
               rect.top += m_nTextHeight / 2;

            pBarItem->m_pBarCombo->SetWindowPos( 0, rect.left,
                                                 rect.top,
                                                 rect.right,
                                                 rect.bottom,
                                                 SWP_NOZORDER |
                                                    SWP_NOACTIVATE |
                                                    SWP_NOSIZE |
                                                    SWP_NOCOPYBITS );
            pBarItem->m_pBarCombo->ShowWindow( SW_SHOW );
         }
      }
   }

   return( TRUE );
}

zBOOL
ZEditBar::SetVertical( )
{
   ZBarItem *pBarItem;

   m_bVertical = TRUE;
   for ( zLONG k = 0; k < m_nBtnCnt; k++ )
   {
      pBarItem = LocateBarItem( m_ulFirstBarCmdId + k );
      if ( pBarItem &&
           (pBarItem->m_ulType == 3 ||  // ComboBox
            pBarItem->m_ulType == 4) && // EditCombo
           pBarItem->m_pBarCombo &&
           mIs_hWnd( pBarItem->m_pBarCombo->m_hWnd ) )
      {
         int   iImage;
         UINT  uStyle;
         zLONG lIndex = CommandToIndex( pBarItem->m_ulId );
#if 1
         UINT  uId = pBarItem->m_ulId;

         GetButtonInfo( lIndex, uId, uStyle, iImage );
         SetButtonInfo( lIndex, pBarItem->m_ulId, TBBS_BUTTON, k );
         if ( mIs_hWnd( pBarItem->m_pBarCombo->m_hWnd ) )
            pBarItem->m_pBarCombo->ShowWindow( SW_HIDE );
#else
         UINT  uId = pBarItem->m_ulId + 0x0100;
         CSize size = CalcButtonSize( );

         // Call this member function to set the button's command ID, style,
         // and image number.  For separators (which have the style
         // TBBS_SEPARATOR), this function sets the separator's width in
         // pixels to the value stored in iImage.
         GetButtonInfo( lIndex, uId, uStyle, iImage );
         SetButtonInfo( lIndex, uId, TBBS_SEPARATOR, size.cx );
         if ( mIs_hWnd( pBarItem->m_pBarCombo->m_hWnd ) )
         {
            CRect rect;
            GetItemRect( lIndex, rect );
            rect.right = rect.left + size.cx;
            pBarItem->m_pBarCombo->SetWindowPos( 0, rect.left,
                                                 rect.top,
                                                 rect.right,
                                                 rect.bottom,
                                                 SWP_NOZORDER |
                                                    SWP_NOACTIVATE |
                                                 // SWP_NOSIZE |
                                                    SWP_NOCOPYBITS );

            pBarItem->m_pBarCombo->SetDroppedWidth( COMBOBOX_WIDTH );
            pBarItem->m_pBarCombo->ShowWindow( SW_SHOW );
         }
#endif
      }
   }

   return( TRUE );
}

zBOOL
ZEditBar::SetColor( zBOOL bColor )
{
   m_bColor = bColor;
   zLONG lResId = atoi( m_csResName );
   if ( lResId )
      return( LoadBitmap( lResId ) );
   else
      return( 0 );
#if 0
   if ( m_bColor )
      return( LoadBitmap( IDR_COLOR_EDITBAR ) );
   else
      return( LoadBitmap( IDR_MONO_EDITBAR ) );
#endif
}

#if 0
void
ZEditBar::SetIconMode( zBOOL bShowIconMode )  // FALSE - Small Icon
{                                             // TRUE  - Large Icon
   m_bShowIconMode = bShowIconMode;
   AssignImageList( );              // load the image list
   ResizeToolBar( );                // resize the toolbar
}

// Get the large or small icon mode.
zBOOL
ZEditBar::GetIconMode( ) const
{
   return( m_bShowIconMode );
}
#endif

// Set the text on or off.
zBOOL
ZEditBar::SetTextMode( zBOOL bShowIconText )  // TRUE  - view text
{                                             // FALSE - hide text
   zBOOL bPrevMode = m_bShowIconText;
   m_bShowIconText = bShowIconText;
   ResizeToolBar( );  // resize the toolbar
   if ( m_bVertical == FALSE )
      SetHorizontal( );  // reset ComboBox position

   return( bPrevMode );
}

// Returns TRUE if the text is on, FALSE otherwise.
zBOOL
ZEditBar::GetTextMode( ) const
{
   return( m_bShowIconText );
}

// To resize the toolbar once something changes.
void
ZEditBar::ResizeToolBar( )
{
   // Set the size of the toolbar corresponding to the current mode.
   CSize size = m_bShowIconMode ? m_sizeLargeIcon : m_sizeSmallIcon;
   CSize sizeButton = CalcButtonSize( );

   // Resize the toolbar and the dock frame.
   SetSizes( sizeButton, size );
   MoveWindow( 0, 0, 450, sizeButton.cy );
   SendMessage( WM_SIZE, SIZE_RESTORED );
   CFrameWnd *pFrameWnd = GetDockingFrame( );
   pFrameWnd->DelayRecalcLayout( );
}

#if 0
CSize
ZEditBar::CalcDynamicLayout( int nLength, DWORD dwMode )
{
   if ( (nLength == -1) &&
        (dwMode & LM_MRUWIDTH) == 0 && (dwMode & LM_COMMIT) == 0 &&
        ((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)) )
   {
      return( CalcFixedLayout( dwMode & LM_STRETCH, dwMode & LM_HORZDOCK ) );
   }

   return( CalcLayout( dwMode, nLength ) );
}
#endif

CSize
ZEditBar::CalcDynamicLayout( int nLength, DWORD dwMode )
{
   // If we're committing set the buttons appropriately.
   if ( dwMode & LM_COMMIT )
   {
      if ( dwMode & LM_VERTDOCK )
      {
         if ( m_bVertical == FALSE )
            SetVertical( );
      }
      else
      {
         if ( m_bVertical )
            SetHorizontal( );
      }

      return( CToolBar::CalcDynamicLayout( nLength, dwMode ) );
   }
   else
   {
      zBOOL bOld = m_bVertical;
      zBOOL bSwitch = (dwMode & LM_HORZ) ? bOld : !bOld;

      if ( bSwitch )
      {
         if ( bOld )
            SetHorizontal( );
         else
            SetVertical( );
      }

      CSize sizeResult = CToolBar::CalcDynamicLayout( nLength, dwMode );

      if ( bSwitch )
      {
         if ( bOld )
            SetHorizontal( );
         else
            SetVertical( );
      }

      return( sizeResult );
   }
}

// To draw the gripper on the toolbar.
void
ZEditBar::DrawGripper( CDC& dc ) const    // device context to draw gripper
{
   // No gripper if floating
   if ( m_dwStyle & CBRS_FLOATING )
      return;

   // Retrieve the window coord and calculate the gripper position.
   CRect rectGripper;
   GetWindowRect( rectGripper );
   ScreenToClient( rectGripper );
   rectGripper.OffsetRect( -rectGripper.left, -rectGripper.top );

   // Draw it accordingly to the orientation of the toolbar.
   COLORREF clrBtnHilight = ::GetSysColor( COLOR_BTNHILIGHT );
   COLORREF clrBtnShadow = ::GetSysColor( COLOR_BTNSHADOW );
   if ( m_dwStyle & CBRS_ORIENT_HORZ )
   {
      // Gripper at left.
      rectGripper.DeflateRect( 3, 3 );
      rectGripper.right = rectGripper.left + 3;
      dc.Draw3dRect( rectGripper, clrBtnHilight, clrBtnShadow );

      rectGripper.OffsetRect( 5, 0 );
      dc.Draw3dRect( rectGripper, clrBtnHilight, clrBtnShadow );
   }
   else
   {
      // Gripper at top.
      rectGripper.DeflateRect( 3, 3 );
      rectGripper.bottom = rectGripper.top + 3;
      dc.Draw3dRect( rectGripper, clrBtnHilight, clrBtnShadow );

      rectGripper.OffsetRect( 0, 5 );
      dc.Draw3dRect( rectGripper, clrBtnHilight, clrBtnShadow );
   }
}

// Return a pointer to a CBrush object of a window background brush
// by using a window class name.
CBrush *
ZEditBar::GetBackgroundBrush( HWND hWnd )
{
   zCHAR szClassName[ 1024 ];

   WNDCLASSEX WndClassEx;
   zmemset( &WndClassEx, 0, sizeof( WndClassEx ) );
   WndClassEx.cbSize = sizeof( WndClassEx );

   if ( ::GetClassName( hWnd, szClassName,
                        zsizeof( szClassName ) / sizeof( zCHAR ) ) &&
        ::GetClassInfoEx( ::AfxGetApp( )->m_hInstance,
                          szClassName, &WndClassEx ) )
   {
      return( CBrush::FromHandle( WndClassEx.hbrBackground ) );
   }

   return( 0 );
}

// Hides toolbar separators that intersect with toolbar items.
void
ZEditBar::HideSeparators( CDC *pDC )
{
   HWND hWnd = GetSafeHwnd( );
   if ( mIs_hWnd( hWnd ) == 0 )
      return;

   CBrush *pBrush = GetBackgroundBrush( hWnd );
   if ( pBrush == 0 )
      return;

   HWND  hWndChild;
   CRect rectClip;

   pDC->GetClipBox( rectClip );
   if ( rectClip.IsRectEmpty( ) )
      return;

// CRect rectBar;
// ::GetWindowRect( m_hWnd, rectBar );
// ScreenToClient( rectBar );
// TraceRect( "HideSeparators ====== Bar Rect ======:", rectBar );

   for ( hWndChild = ::GetWindow( hWnd, GW_CHILD );
         hWndChild;
         hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT ) )
   {
      if ( hWndChild == m_lstPopup.m_hWnd )
         continue;

      CRect rectChild;
      ::GetWindowRect( hWndChild, rectChild );
      ScreenToClient( rectChild );
   // TraceRect( "HideSeparators Child Rect:", rectChild );

      zSHORT nToolBarItemCnt = GetCount( );
      for ( zSHORT k = 0; k < nToolBarItemCnt; k++ )
      {
         UINT uID = 0;
         UINT uStyle = 0;
         int  iImage = 0;

         GetButtonInfo( k, uID, uStyle, iImage );
         if ( uStyle != TBBS_SEPARATOR )
            continue;

         CRect rectFill;
         CRect rectSeparator;
         GetItemRect( k, rectSeparator );
         if ( rectChild == rectSeparator )
         {
         // TraceRect( "HideSeparators <===> Separator Rect:", rectSeparator );
            continue;
         }

      // TraceRect( "HideSeparators Separator Rect:", rectSeparator );
         if ( rectFill.IntersectRect( rectChild, rectSeparator ) == 0 )
            continue;

      // TraceRect( "HideSeparators Intersect Separator:Child:", rectChild );
         if ( rectFill.IntersectRect( rectSeparator, rectClip ) == 0 )
            continue;

      // TraceRect( "HideSeparators Intersect Separator:Clip:", rectClip );
         pDC->FillRect( rectFill, pBrush );
      }
   }
}

// Method to calculate the current size of the button.
CSize
ZEditBar::CalcButtonSize( )
{
   // Calculate the width of the drop button.
   CSize sizeButton;
   if ( m_bShowIconMode )
   {
      sizeButton = CSize( m_sizeLargeIcon.cx + 8,
                          m_sizeLargeIcon.cy + 7 );
   }
   else
   {
      sizeButton = CSize( m_sizeSmallIcon.cx + 8,
                          m_sizeSmallIcon.cy + 7 );
   }

   // Check the text mode and set or hide the text.
   if ( m_bShowIconText )
   {
      // The text width is the maximum width of the bitmap.  All toolbar
      // sizes must at least be this width.
      if ( sizeButton.cx < m_nTextWidth )
      {
         sizeButton.cx = m_nTextWidth;
      }

      // The text height is the height added to the button.  Even in large mode
      // we must add this text height.
      sizeButton.cy += m_nTextHeight;
   }

   return( sizeButton );
}

// To display the dropdown button for the given button.
void
ZEditBar::SetButtonDropDown( int nID ) // id to show the drop down
{
   // Change button style to dropdown.
   UINT u = CommandToIndex( nID );
   DWORD dwStyle = GetButtonStyle( u );
   dwStyle |= TBSTYLE_DROPDOWN;
   SetButtonStyle( u, dwStyle );

   // Calculate the drop button size.
   CToolBarCtrl& bar = GetToolBarCtrl( );
   CSize sizeStdButton = CalcButtonSize( );
   CRect rect( 0, 0, 0, 0 );
   bar.GetItemRect( u, rect );
   m_nDropButtonSize = (zSHORT) (rect.Width( ) - sizeStdButton.cx);
}

zLONG
ZEditBar::GetLastSelection( zPCHAR pchText, zLONG lMaxTextLth )
{
   strncpy_s( pchText, lMaxTextLth, m_csLastSelect, lMaxTextLth - 1 );
   pchText[ lMaxTextLth ] = 0;
   return( m_lLastSelect );
}

CSize
ZEditBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
{
   DWORD dwMode = bStretch ? LM_STRETCH : 0;
   dwMode |= bHorz ? LM_HORZ : 0;

   return( CalcLayout( dwMode ) );
}

//CSize
//ZEditBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
//{
//      if ( !bHorz && (m_dwStyle & CBRS_SIZE_DYNAMIC) &&
//           !(m_dwStyle & CBRS_FLOATING) )
//  {
//      CSize size;
//
//      size = CControlBar::CalcFixedLayout( bStretch, bHorz );
//
//      CRect rect;
//      rect.SetRectEmpty( );
//
//      CalcInsideRect( rect, bHorz );
//
//      size.cx = m_sizeVert.cx +
//                (m_rectInsideVert.Width( ) - rect.Width( ));
//      size.cy = m_sizeVert.cy +
//                (m_rectInsideVert.Height( ) - rect.Height( ));
//      return( size );
//  }
//  else
//  {
//      if ( m_bVertical )
//          SetHorizontal( );
//
//      return( CToolBar::CalcFixedLayout( bStretch, bHorz ) );
//  }
//}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZEditBar message handlers

// Correctly size the toolbar with the carat.
void
ZEditBar::OnNcCalcSize( BOOL bCalcValidRects,         // MFC Specific message
                        NCCALCSIZE_PARAMS *lpncsp )   // MFC Specific message
{
   // No gripper if floating.
   if ( (m_dwStyle & CBRS_FLOATING) == 0 )
   {
      // Adjust non-client area for gripper at left or top.
      if ( m_dwStyle & CBRS_ORIENT_HORZ )
      {
         lpncsp->rgrc[ 0 ].left += 5;
         lpncsp->rgrc[ 0 ].right += 5;
      }
      else
      {
         lpncsp->rgrc[ 0 ].top += 5;
         lpncsp->rgrc[ 0 ].bottom += 5;
      }
   }

   CToolBar::OnNcCalcSize( bCalcValidRects, lpncsp );
}

void
ZEditBar::OnPaint( )
{
   if ( m_bDelayedButtonLayout )
      Layout( );

   CPaintDC dc( this ); // device context for painting
   DefWindowProc( WM_PAINT, WPARAM( dc.m_hDC ), 0 );
   HideSeparators( &dc );  // hide separator
}

//  This will draw the gripper on the toolbar, then repaints client areas.
void
ZEditBar::OnNcPaint( )
{
   CControlBar::EraseNonClient( );
   CWindowDC dc( this );
   DrawGripper( dc );

   CRect rect;
   GetClientRect( &rect );
   InvalidateRect( &rect, TRUE );

   CWnd *pWnd = &GetToolBarCtrl( );
   pWnd->GetClientRect( &rect );
   pWnd->InvalidateRect( &rect, TRUE );
}

// Call whenever the system colors are changing.  Trap to rebuild
// the toolbar image list with the good background color.
void
ZEditBar::OnSysColorChange( )
{
   // Call the base class method.
   CToolBar::OnSysColorChange( );
}

void
ZEditBar::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEditBar::OnSetFocus ", *m_pzsTag );
#endif

   if ( m_pZSubtask->m_pzmaWithFocus &&
        m_pZSubtask->m_pzmaWithFocus->m_pCtrl )
   {
      m_pZSubtask->m_pzmaWithFocus->m_pCtrl->SetFocus( );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT
ZEditBar::OnGetToolTipText( WPARAM wParam, LPARAM lParam )
{
   zLONG lId = (zLONG) wParam;

#ifdef DEBUG_ALL
   TraceLineI( "ZEditBar::OnGetToolTipText", lId );
#endif

   zPCHAR pchText = (zPCHAR) lParam;
   ZBarItem *pBarItem = LocateBarItem( lId );
   if ( pBarItem && pBarItem->m_pchTipText )
   {
      strncpy_s( pchText, zZEIDON_TOOLTIP_TEXTLTH, pBarItem->m_pchTipText, zZEIDON_TOOLTIP_TEXTLTH - 1 );
      pchText[ zZEIDON_TOOLTIP_TEXTLTH - 1 ] = 0;
   }
   else
   {
      pchText[ 0 ] = 0;
   }

   return( 0 );
}

LRESULT
ZEditBar::OnToolBarSetSelect( WPARAM wParam, LPARAM lParam )
{
   m_lLastSelect = wParam;
   m_csLastSelect = *((CString *) lParam );
   ZBarItem *pBarItem = LocateBarItem( m_uLastDropDownId );
   if ( m_lLastSelect >= 0 && pBarItem )
   {
      if ( pBarItem->m_cpcViewName )
      {
         zVIEW v;

         GetViewByName( &v, pBarItem->m_cpcViewName, m_pZSubtask->m_vDialog, zLEVEL_ANY );
         if ( v )
         {
            SetCursorFirstEntityByString( v, pBarItem->m_cpcEntity, pBarItem->m_cpcAttribute, m_csLastSelect, 0 );
         }
      }

      if ( pBarItem->m_cpcAction )
      {
         ProcessActionLoop( m_pZSubtask, pBarItem->m_cpcAction, 0, 0, 0, FALSE, "ToolBar" );
      }
   }

   return( 0 );
}

LRESULT
ZEditBar::OnToolBarDropDown( WPARAM wParam, LPARAM lParam )
{
   m_uLastDropDownId = wParam;
#ifdef DEBUG_ALL
   TraceLineI( "ZEditBar::OnToolBarDropDown = ", m_uLastDropDownId );
#endif

   ZTB_Popup *pPopup;
   CRect  rectCaller;

   // Get rect of toolbar item.
   GetItemRect( CommandToIndex( m_uLastDropDownId ), rectCaller );
   ClientToScreen( rectCaller );
   rectCaller.bottom += 2;

   pPopup = (ZTB_Popup *) RUNTIME_CLASS (ZTB_Popup)->CreateObject( );

   // Populate the ListCtrl.
   m_lstPopup.DeleteAllItems( );
   ZBarItem *pBarItem = LocateBarItem( m_uLastDropDownId );
   if ( pBarItem && pBarItem->m_cpcViewName )
   {
      zVIEW v;

      GetViewByName( &v, pBarItem->m_cpcViewName,
                     m_pZSubtask->m_vDialog, zLEVEL_ANY );
      if ( v )
      {
         zCHAR  szMap[ 256 ];
         zLONG  lCnt = 0;
         zSHORT nRC;

         CreateViewFromViewForTask( &v, v, m_pZSubtask->m_vDialog );
         nRC = SetCursorFirstEntity( v, pBarItem->m_cpcEntity, 0 );
         while ( nRC >= zCURSOR_SET )
         {
            nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                            zsizeof( szMap ) - 1, v,
                                            pBarItem->m_cpcEntity,
                                            pBarItem->m_cpcAttribute,
                                            0, zUSE_DEFAULT_CONTEXT );
            if ( nRC == zVAR_NULL )
               szMap[ 0 ] = 0;

            if ( m_lstPopup.InsertItem( lCnt, szMap ) == -1 )
            {
               TraceLineI( "(ToolDB) Error inserting item: ", lCnt );
               break;
            }

            lCnt++;
            nRC = SetCursorNextEntity( v, pBarItem->m_cpcEntity, 0 );
         }

         DropView( v );
      }
   }

   // Popup the ListCtrl.
   VERIFY( pPopup->Display( &m_lstPopup, this, rectCaller, &m_lstPopup.m_xPopupCtrlEvent ) );
   return( 0 );
}

void
ZEditBar::OnControlReflectCommand( )
{
   const MSG *pMsg = CWnd::GetCurrentMessage( );
   UINT uId = LOWORD( pMsg->wParam );
   UINT uNotifyCode = HIWORD( pMsg->wParam );
   HWND hWndCtrl = (HWND) pMsg->lParam;

#ifdef DEBUG_ALL
   TraceLineI( "ZEditBar::OnControlReflectCommand = ", uId );

// TraceLineS( "ZEditBar::OnControlReflectCommand", "" );
// TraceLineI( "ZEditBar::OnControlReflectCommand wParam ", pMsg->wParam );
// TraceLineI( "ZEditBar::OnControlReflectCommand lParam ", pMsg->lParam );

// TraceLineI( "ZEditBar::OnControlReflectCommand NotifyCode = ", uNotifyCode );
// TraceLineI( "ZEditBar::OnControlReflectCommand hWndCtrl = ", (zLONG) hWndCtrl );
#endif

   if ( uId >= (UINT) m_ulFirstBarCmdId &&
        uId <= (UINT) (m_ulFirstBarCmdId + m_nBtnCnt) )
   {
   // TraceLineI( "ZEditBar::OnControlReflectCommand 1-based index = ",
   //             (uId - m_ulFirstBarCmdId + 1) );
      ZBarItem *pBarItem = LocateBarItem( uId );
      CSize size = CalcButtonSize( );
      CPoint pt = pMsg->pt;

      if ( pBarItem && pBarItem->m_cpcAction )
      {
         ProcessActionLoop( m_pZSubtask, pBarItem->m_cpcAction, 0, 0, 0, FALSE, "ToolBar" );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// 1998 MSJ - Paul DiLascia
// Make the parent frame my owner (important for status bar prompts to work).
//
// Stuff below is copied from MFC ... my mods marked **PD**
//
#ifdef _MAC
#define CX_OVERLAP  1
#else
#define CX_OVERLAP  0
#endif

/////////////////////////////////////////////////////////////////////////////
// 1998 MSJ - Paul DiLascia
// I renamed this from _GetButton.
void
ZEditBar::GetButton( int nIndex, TBBUTTON *pButton ) const
{
   CToolBar *pBar = (CToolBar *) this;
   VERIFY( pBar->SendMessage( TB_GETBUTTON, nIndex, (LPARAM) pButton ) );

   // TBSTATE_ENABLED == TBBS_DISABLED so invert it
   pButton->fsState ^= TBSTATE_ENABLED;
}

/////////////////////////////////////////////////////////////////////////////
// 1998 MSJ - Paul DiLascia
// I renamed this from _SetButton.
void
ZEditBar::SetButton( int nIndex, TBBUTTON *pButton )
{
   // Get original button state.
   TBBUTTON button;
   VERIFY( SendMessage( TB_GETBUTTON, nIndex, (LPARAM) &button ) );

   // Prepare for old/new button comparsion.
   button.bReserved[ 0 ] = 0;
   button.bReserved[ 1 ] = 0;

   // TBSTATE_ENABLED == TBBS_DISABLED so invert it.
   pButton->fsState ^= TBSTATE_ENABLED;
   pButton->bReserved[ 0 ] = 0;
   pButton->bReserved[ 1 ] = 0;

   // Nothing to do if they are the same.
   if ( zmemcmp( pButton, &button, sizeof( TBBUTTON ) ) != 0 )
   {
      // Don't redraw anything while setting the button.
      DWORD dwStyle = GetStyle( );
      ModifyStyle( WS_VISIBLE, 0 );
      VERIFY( SendMessage( TB_DELETEBUTTON, nIndex, 0 ) );
      VERIFY( SendMessage( TB_INSERTBUTTON, nIndex, (LPARAM) pButton ) );
      ModifyStyle( 0, dwStyle & WS_VISIBLE );

      // Invalidate appropriate parts.
      if ( ((pButton->fsStyle ^button.fsStyle) & TBSTYLE_SEP ) ||
           ((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap) )
      {
         // Changing a separator.
         Invalidate( FALSE );
      }
      else
      {
         // Invalidate just the button.
         CRect rect;
         if ( SendMessage( TB_GETITEMRECT, nIndex, (LPARAM) &rect ) )
            InvalidateRect( rect, FALSE );  // don't erase background
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// 1998 MSJ - Paul DiLascia
// This is the all-important function that gets the true size of a button,
// instead of using m_sizeButton. And it's virtual, so you can override it if
// my algorithm doesn't work, as will surely be the case in some circumstances.
//
CSize
ZEditBar::GetButtonSize( TBBUTTON *pData, int nButton )
{
   // Get the actual size of the button, not what's in m_sizeButton.
   // Make sure to do SendMessage instead of calling MFC's GetrectItem,
   // which has all sorts of bad side-effects! (Go ahead, take a look at it).
   CRect rect;
   SendMessage( TB_GETITEMRECT, nButton, (LPARAM) &rect );
   CSize sz = rect.Size( );

   // Now must do special case for various versions of comctl32.dll,
   DWORD dwStyle = pData[ nButton ].fsStyle;
   if ( (pData[ nButton ].fsState & TBSTATE_WRAP) )
   {
      if ( dwStyle & TBSTYLE_SEP )
      {
         // This is the last separator in the row (e.g. vertically docked)
         // fudge the height, and ignore the width.  TB_GETrectItem will
         // return size = (8 x 22) even for a separator in vertical toolbar.
         sz.cy = sz.cx;
         sz.cx = 0;        // separator takes no width if it's the last one
      }
      else
      if ( dwStyle & TBSTYLE_DROPDOWN )
      {  //&&
         //!m_bShowDropdownArrowWhenVertical ) {
         // ignore width of dropdown
         sz.cx = 0;
      }
   }

   return( sz );
}

CSize
ZEditBar::CalcSize( TBBUTTON *pData, int nCount )
{
   ASSERT( pData && nCount > 0 );

   CPoint cur( 0,0 );
   CSize sizeResult( 0,0 );
   int cyTallestOnRow = 0;

   for ( int k = 0; k < nCount; k++ )
   {
      if ( pData[ k ].fsState & TBSTATE_HIDDEN )
         continue;

      // **PD** Load actual size of button into local var
      // that obscures CToolBar::m_sizeButton.
      CSize m_sizeButton = GetButtonSize( pData, k );

      // **PD** I also changed the logic below to be more correct.
      cyTallestOnRow = __max( cyTallestOnRow, m_sizeButton.cy );
      sizeResult.cx = __max( cur.x + m_sizeButton.cx, sizeResult.cx );
      sizeResult.cy = __max( cur.y + m_sizeButton.cy, sizeResult.cy );

      cur.x += m_sizeButton.cx - CX_OVERLAP;

      if ( pData[ k ].fsState & TBSTATE_WRAP )
      {
         cur.x = 0;
         cur.y += cyTallestOnRow;
         cyTallestOnRow = 0;
         if ( pData[ k ].fsStyle & TBSTYLE_SEP )
            cur.y += m_sizeButton.cy;
      }
   }

   return( sizeResult );
}

int
ZEditBar::WrapToolBar( TBBUTTON *pData, int nCount, int nWidth )
{
   ASSERT( pData && nCount > 0 );

   int nResult = 0;
   int x = 0;
   for ( int k = 0; k < nCount; k++ )
   {
      pData[ k ].fsState &= ~TBSTATE_WRAP;

      if ( pData[ k ].fsState & TBSTATE_HIDDEN )
         continue;

      int dx, dxNext;

      // **PD** Load actual size of button into local var
      // that obscures CToolBar::m_sizeButton.
      CSize m_sizeButton = GetButtonSize( pData, k );

      dx = m_sizeButton.cx;
      dxNext = dx - CX_OVERLAP;

      if ( x + dx > nWidth )
      {
         zBOOL bFound = FALSE;
         for ( int j = k;
               j >= 0 && (pData[ j ].fsState & TBSTATE_WRAP) == 0;
               j-- )
         {
            // Find last separator that isn't hidden
            // a separator that has a command ID is not
            // a separator, but a custom control.
            if ( (pData[ j ].fsStyle & TBSTYLE_SEP) &&
                 (pData[ j ].idCommand == 0) &&
                 (pData[ j ].fsState & TBSTATE_HIDDEN) == 0 )
            {
               bFound = TRUE;
               k = j;
               x = 0;
               pData[ j ].fsState |= TBSTATE_WRAP;
               nResult++;
               break;
            }
         }

         if ( bFound == FALSE )
         {
            for ( int j = k - 1;
                  j >= 0 && (pData[ j ].fsState & TBSTATE_WRAP) == 0;
                  j-- )
            {
               // Never wrap anything that is hidden, or any custom controls.
               if ( (pData[ j ].fsState & TBSTATE_HIDDEN) ||
                    ((pData[ j ].fsStyle & TBSTYLE_SEP) &&
                    (pData[ j ].idCommand != 0)) )
               {
                  continue;
               }

               bFound = TRUE; k = j; x = 0;
               pData[ j ].fsState |= TBSTATE_WRAP;
               nResult++;
               break;
            }

            if ( bFound == FALSE )
               x += dxNext;
         }
      }
      else
         x += dxNext;
   }

   return( nResult + 1 );
}

/////////////////////////////////////////////////////////////////////////////
// 1998 MSJ - Paul DiLascia
// Functions below are NOT modified. They're only here because they
// call the modified functions above, which are NOT virtual.
void
ZEditBar::SizeToolBar( TBBUTTON *pData, int nCount,
                       int nLength, zBOOL bVert )
{
   ASSERT( pData && nCount > 0 );

   if ( bVert == FALSE )
   {
      int nMin, nMax, nTarget, nCurrent, nMid;

      // Wrap ToolBar as specified.
      nMax = nLength;
      nTarget = WrapToolBar( pData, nCount, nMax );

      // Wrap ToolBar vertically.
      nMin = 0;
      nCurrent = WrapToolBar( pData, nCount, nMin );

      if ( nCurrent != nTarget )
      {
         while ( nMin < nMax )
         {
            nMid = ( nMin + nMax ) / 2;
            nCurrent = WrapToolBar( pData, nCount, nMid );

            if ( nCurrent == nTarget )
               nMax = nMid;
            else
            {
               if ( nMin == nMid )
               {
                  WrapToolBar( pData, nCount, nMax );
                  break;
               }

               nMin = nMid;
            }
         }
      }

      CSize size = CalcSize( pData, nCount );
      WrapToolBar( pData, nCount, size.cx );
   }
   else
   {
      CSize sizeMax, sizeMin, sizeMid;

      // Wrap ToolBar vertically
      WrapToolBar( pData, nCount, 0 );
      sizeMin = CalcSize( pData, nCount );

      // Wrap ToolBar horizontally
      WrapToolBar( pData, nCount, 32767 );
      sizeMax = CalcSize( pData, nCount );

      while ( sizeMin.cx < sizeMax.cx )
      {
         sizeMid.cx = ( sizeMin.cx + sizeMax.cx ) / 2;
         WrapToolBar( pData, nCount, sizeMid.cx );
         sizeMid = CalcSize( pData, nCount );

         if ( nLength < sizeMid.cy )
         {
            if ( sizeMin == sizeMid )
            {
               WrapToolBar( pData, nCount, sizeMax.cx );
               return;
            }

            sizeMin = sizeMid;
         }
         else
         if ( nLength > sizeMid.cy )
            sizeMax = sizeMid;
         else
            return;
      }
   }
}

struct _AFX_CONTROLPOS
{
   int   nIndex;
   int   nID;
   CRect rectOldPos;
};

CSize
ZEditBar::CalcLayout( DWORD dwMode, int nLength )
{
   ASSERT_VALID( this );
   ASSERT( mIs_hWnd( m_hWnd ) );
   if ( dwMode & LM_HORZDOCK )
      ASSERT( dwMode & LM_HORZ );

   int nCount;
   TBBUTTON *pData;
   CSize sizeResult( 0,0 );

   // Load Buttons.
   {
      nCount = SendMessage( TB_BUTTONCOUNT, 0, 0 );
      if ( nCount )
      {
         int k;
         pData = new TBBUTTON[ nCount ];
         for ( k = 0; k < nCount; k++ )
            GetButton( k, &pData[ k ] ); // **PD** renamed from _GetButton
      }
   }

   if ( nCount > 0 )
   {
      if ( (m_dwStyle & CBRS_SIZE_FIXED) == 0 )
      {
         zBOOL bDynamic = (m_dwStyle & CBRS_SIZE_DYNAMIC) ? TRUE : FALSE;

         if ( bDynamic && (dwMode & LM_MRUWIDTH) )
            SizeToolBar( pData, nCount, m_nMRUWidth );
         else
         if ( bDynamic && (dwMode & LM_HORZDOCK) )
            SizeToolBar( pData, nCount, 32767 );
         else
         if ( bDynamic && (dwMode & LM_VERTDOCK) )
            SizeToolBar( pData, nCount, 0 );
         else
         if ( bDynamic && (nLength != -1) )
         {
            CRect rect; rect.SetRectEmpty( );
            CalcInsideRect( rect, (dwMode & LM_HORZ) );
            zBOOL bVert = (dwMode & LM_LENGTHY) ? TRUE : FALSE;
            int nLen = nLength + (bVert ? rect.Height( ) : rect.Width( ));

            SizeToolBar( pData, nCount, nLen, bVert );
         }
         else
         if ( bDynamic && (m_dwStyle & CBRS_FLOATING) )
            SizeToolBar( pData, nCount, m_nMRUWidth );
         else
            SizeToolBar( pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0 );
      }

      sizeResult = CalcSize( pData, nCount );

      if ( dwMode & LM_COMMIT )
      {
         int k;
         _AFX_CONTROLPOS *pControl = 0;
         int nControlCount = 0;
         zBOOL bIsDelayed = m_bDelayedButtonLayout;
         m_bDelayedButtonLayout = FALSE;

         for ( k = 0; k < nCount; k++ )
         {
            if ( (pData[ k ].fsStyle & TBSTYLE_SEP) &&
                 (pData[ k ].idCommand != 0) )
            {
               nControlCount++;
            }
         }

         if ( nControlCount > 0 )
         {
            pControl = new _AFX_CONTROLPOS[ nControlCount ];
            nControlCount = 0;

            for ( k = 0; k < nCount; k++ )
            {
               if ( (pData[ k ].fsStyle & TBSTYLE_SEP) &&
                    (pData[ k ].idCommand != 0) )
               {
                  pControl[ nControlCount ].nIndex = k;
                  pControl[ nControlCount ].nID = pData[ k ].idCommand;

                  CRect rect;
                  GetItemRect( k, &rect );
                  ClientToScreen( &rect );
                  pControl[ nControlCount ].rectOldPos = rect;

                  nControlCount++;
               }
            }
         }

         if ( (m_dwStyle & CBRS_FLOATING) &&
              (m_dwStyle & CBRS_SIZE_DYNAMIC) )
         {
            m_nMRUWidth = sizeResult.cx;
         }

         for ( k = 0; k < nCount; k++ )
            SetButton( k, &pData[ k ] ); // **PD** renamed from _SetButton

         if ( nControlCount > 0 )
         {
            for ( int k = 0; k < nControlCount; k++ )
            {
               CWnd *pWnd = GetDlgItem( pControl[ k ].nID );
               if ( pWnd )
               {
                  CRect rect;
                  pWnd->GetWindowRect( &rect );
                  CPoint pt = rect.TopLeft( ) -
                                      pControl[ k ].rectOldPos.TopLeft( );
                  GetItemRect( pControl[ k ].nIndex, &rect );
                  pt = rect.TopLeft( ) + pt;
                  pWnd->SetWindowPos( 0, pt.x, pt.y, 0, 0,
                             SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER );
               }
            }

            delete [] pControl;
         }

         m_bDelayedButtonLayout = bIsDelayed;
      }

      delete [] pData;
   }

   // Adjust Margins.
   {
      CRect rect; rect.SetRectEmpty( );
      CalcInsideRect( rect, (dwMode & LM_HORZ) );
      sizeResult.cy -= rect.Height( );
      sizeResult.cx -= rect.Width( );

      CSize size = CControlBar::CalcFixedLayout( (dwMode & LM_STRETCH),
                                                 (dwMode & LM_HORZ) );
      sizeResult.cx = __max( sizeResult.cx, size.cx );
      sizeResult.cy = __max( sizeResult.cy, size.cy );
   }

   return( sizeResult );
}

// If the second parameter is FALSE, the button is not disabled,
// otherwise, the button is disabled ... so we need to check the
// status of the appropriate action and enable/disable appropriately.
//
// Only overloaded to use our CmdUI instead of theirs.  This is the same code
// found in CToolBar::OnUpdateCmdUI, but we replace the CCmdUI by CCoolCmdUI.
void
ZEditBar::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler )
{
   CCoolCmdUI state; // this is the only line that's different--PD
// CToolCmdUI state;
   state.m_pOther = this;

   state.m_nIndexMax = (UINT) DefWindowProc( TB_BUTTONCOUNT, 0, 0 );
#if 0 //def DEBUG_ALL lots of messages
   TraceLineI( "ZEditBar::OnUpdateCmdUI ", state.m_nIndexMax );
#endif
   for ( state.m_nIndex = 0;
         state.m_nIndex < state.m_nIndexMax;
         state.m_nIndex++ )
   {
      // Get buttons state.
      TBBUTTON button;
      GetButton( state.m_nIndex, &button );
      state.m_nID = button.idCommand;

      // Ignore separators.
      if ( (button.fsStyle & TBSTYLE_SEP) == 0 )
      {
         // Allow reflections.
         if ( CWnd::OnCmdMsg( 0,
                              MAKELONG( (int) CN_UPDATE_COMMAND_UI,
                                        WM_COMMAND + WM_REFLECT_BASE ),
                              &state, NULL ) )
         {
            continue;
         }

         // Allow the toolbar itself to have update handlers.
         if ( CWnd::OnCmdMsg( state.m_nID, CN_UPDATE_COMMAND_UI,
                              &state, NULL ) )
         {
            continue;
         }

         // Allow the owner to process the update.
         state.DoUpdate( pTarget, bDisableIfNoHndler );
      }
   }

   // Update the dialog controls added to the toolbar.
   UpdateDialogControls( pTarget, bDisableIfNoHndler );
}

BOOL
ZEditBar::OnCmdMsg( UINT uId, int nCode, void *pExtra,
                    AFX_CMDHANDLERINFO *pHandlerInfo )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZEditBar::OnCmdMsg uId ", uId );
   TraceLineI( "        OnCmdMsg nCode ", nCode );
#endif

   if ( nCode == CBN_SELENDOK )
      TraceLineI( "ZEditBar::OnCmdMsg SELENDOK ", uId );

#if 0

      if ( nCode == CBN_SELENDOK )
      {
         zVIEW  vDialogList;
         zVIEW  vTZPNTROO;
         zCHAR  szTag[ 34 ];
         zSHORT nIdx;
         zPCHAR pchViewName;
         zPCHAR pchWindowPage;

      // TraceLineS( "ControlBar WndList EndOK", "" );
         nIdx = m_wndListWindows.GetCurSel( );
         if ( nIdx >= 0 )
            m_wndListWindows.GetLBText( nIdx, szTag );
         else
            m_wndListWindows.GetWindowText( szTag, zsizeof( szTag ) - 1 );

         if ( m_fState & zTZCB_XSLT )
         {
            pchViewName = szlXSLT;
            pchWindowPage = szlXSLTL;
         }
         else
         if ( m_fState & zTZCB_REPORT )
         {
            pchViewName = szlPAGEL;
            pchWindowPage = szlPage;
         }
         else
         {
            pchViewName = szlTZWINDOWL;
            pchWindowPage = szlWindow;
         }

         // Get the view to the window list.
         if ( GetViewByName( &vDialogList, pchViewName,
                             m_vSubtask, zLEVEL_TASK ) > 0 &&
              SetCursorFirstEntityByString( vDialogList, pchWindowPage, szlTag,
                                            szTag, 0 ) == zCURSOR_SET )
         {
            GetViewByName( &vTZPNTROO, "TZPNTROO", m_vSubtask, zLEVEL_TASK );
            PainterCall( zMSG_UPDATEWINDOWBYNAME, vTZPNTROO, this, vDialogList, szTag, zsizeof( szTag ) );
         }

         return( TRUE );
      }

#endif

   return( FALSE );  // not handled here ... pass it on
}

#if 0
void
ZEditBar::ParentNotify( UINT message, LPARAM lParam )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZEditBar::ParentNotify", "" );
#endif
}
#endif

zSHORT
ZEditBar::SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn )
{
   if ( lColumn <= m_nBtnCnt && lColumn > 0 )
   {
      CToolBarCtrl& bar = GetToolBarCtrl( );
      TBBUTTON tb;
      zSHORT   nIndex;

      lColumn--;
      ZBarItem *pBarItem = m_BarItemList[ lColumn ];

      // Set the text for this button.
      if ( lRow == 0 )
      {
         mDeleteInitA( pBarItem->m_pchText );
         if ( cpcText )
         {
            pBarItem->m_lTextLth = zstrlen( cpcText ) + 1;
            pBarItem->m_pchText = new char[pBarItem->m_lTextLth];
            strcpy_s( pBarItem->m_pchText, pBarItem->m_lTextLth, cpcText );
         }

         for ( nIndex = bar.GetButtonCount( ); nIndex >= 0; nIndex-- )
         {
            zmemset( &tb, 0, sizeof( TBBUTTON ) );
            bar.GetButton( nIndex, &tb );

            // Do we have a separator?
            if ( (tb.fsStyle & TBSTYLE_SEP) == TBSTYLE_SEP )
               continue;

            // Do we have a valid command id?
            if ( tb.idCommand == 0 )
               continue;

            if ( tb.idCommand == (int) pBarItem->m_ulId )
            {
               SetButtonText( nIndex, pBarItem->m_pchText ? pBarItem->m_pchText : "" );
               return( 0 );
            }
         }
      }
      else
      if ( lRow == 1 )
      {
         mDeleteInitA( pBarItem->m_pchTipText );
         if ( cpcText )
         {
            zLONG lLth = zstrlen( cpcText ) + 1;
            pBarItem->m_pchTipText = new char[ lLth ];
            strcpy_s( pBarItem->m_pchTipText, lLth, cpcText );
         }
         return( 0 );
      }
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// palette.cpp : implementation of the Floating tool palette class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
/////////////////////////////////////////////////////////////////////////////
// ZPaletteBar

IMPLEMENT_DYNAMIC( ZPaletteBar, CToolBar )
BEGIN_MESSAGE_MAP( ZPaletteBar, CToolBar )
   //{{AFX_MSG_MAP( ZPaletteBar )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZPaletteBar construction/destruction

ZPaletteBar::ZPaletteBar( ZSubtask *pZSubtask,        // ctor
                          CWnd     *pWndParent,
                          ZMapAct  *pzmaComposite,
                          zVIEW    vDialog,
                          zSHORT   nOffsetX,
                          zSHORT   nOffsetY,
                          zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
             CToolBar( ),
             ZMapAct( pZSubtask,          // base class ctor
                      pzmaComposite,
                      pWndParent,
                      this,
                      vDialog,
                      nOffsetX,
                      nOffsetY,
                      pCtrlDef,
                      "Palette" )
{
   m_nColumns = 2;
   m_cxLeftBorder = 5;
   m_cxRightBorder = 5;
   m_cyTopBorder = 5;
   m_cyBottomBorder = 5;
}

ZPaletteBar::~ZPaletteBar( )
{
}

/////////////////////////////////////////////////////////////////////////////
// ZPaletteBar diagnostics

#ifdef _DEBUG
void
ZPaletteBar::AssertValid( ) const
{
   CToolBar::AssertValid( );
}

void
ZPaletteBar::Dump( CDumpContext& dc ) const
{
   CToolBar::Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ZPaletteBar message handlers

void
ZPaletteBar::SetColumns( UINT nColumns )
{
   m_nColumns = nColumns;
   int nCount = GetToolBarCtrl( ).GetButtonCount( );

   for ( int k = 0; k < nCount; k++ )
   {
      UINT nStyle = GetButtonStyle( k );
      zBOOL bWrap = (((k + 1) % nColumns) == 0);
      if ( bWrap )
         nStyle |= TBBS_WRAPPED;
      else
         nStyle &= ~TBBS_WRAPPED;

      SetButtonStyle( k, nStyle );
   }

   Invalidate( );
   if ( m_pZSubtask == 0 || m_pZSubtask->m_pZTask->m_pZFWndHidden == 0 )
      GetParentFrame( )->RecalcLayout( );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// MFC Overloaded method.
// The following code is to make the command update UI mechanism
// work properly for flat tool bars. The main idea is to convert
// a "checked" button state into a "pressed" button state. Changed
// lines marked with "PD" (from Paul DiLascia Article).
void
CCoolCmdUI::Enable( BOOL bOn )
{
#ifdef DEBUG_ALL
// TraceLineS( "CCoolCmdUI::Enable", "" );  lots of messages
#endif

   m_bEnableChanged = TRUE;
   CToolBar *pToolBar = (CToolBar *) m_pOther;
   ASSERT( pToolBar );
   ASSERT_KINDOF( CToolBar, pToolBar );
   ASSERT( m_nIndex < m_nIndexMax );

   UINT nNewStyle = pToolBar->GetButtonStyle( m_nIndex ) & ~TBBS_DISABLED;
   if ( bOn == FALSE )
   {
      nNewStyle |= TBBS_DISABLED;

      // WINBUG: If a button is currently pressed and then is disabled
      // COMCTL32.DLL does not unpress the button, even after the mouse
      // button goes up!  We work around this bug by forcing TBBS_PRESSED
      // off when a button is disabled.
      nNewStyle &= ~TBBS_PRESSED;
   }

   ASSERT( (nNewStyle & TBBS_SEPARATOR) == 0 );
   pToolBar->SetButtonStyle( m_nIndex, nNewStyle );
}

// From Paul DiLascia article.
void
CCoolCmdUI::SetCheck( int nCheck )
{
#ifdef DEBUG_ALL
   TraceLineS( "CCoolCmdUI::SetCheck", "" );
#endif

   ASSERT( nCheck >= 0 && nCheck <= 2 ); // 0=>off, 1=>on, 2=>indeterminate
   CToolBar *pToolBar = (CToolBar *) m_pOther;
   ASSERT( pToolBar );
   ASSERT_KINDOF( CToolBar, pToolBar );
   ASSERT( m_nIndex < m_nIndexMax );

   UINT nOldStyle = pToolBar->GetButtonStyle( m_nIndex ); // PD
   UINT nNewStyle = nOldStyle &
                    ~(TBBS_PRESSED | TBBS_INDETERMINATE); // PD

   if ( nCheck == 1 )
      nNewStyle |= TBBS_PRESSED; // PD
   else
   if ( nCheck == 2 )
      nNewStyle |= TBBS_INDETERMINATE;

   // Following is to fix display bug for TBBS_CHECKED:
   // If new state is unchecked, repaint--but only if style actually changing
   // (otherwise will end up with flicker).
   //
   if ( nNewStyle != nOldStyle )
   {
      ASSERT( (nNewStyle & TBBS_SEPARATOR) == 0 );
      pToolBar->SetButtonStyle( m_nIndex, nNewStyle );
      pToolBar->Invalidate( );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#else

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZToolBar::ZToolBar( ZSubtask *pZSubtask,        // ctor
                    CWnd     *pWndParent,
                    ZMapAct  *pzmaComposite,
                    zVIEW    vDialog,
                    zSHORT   nOffsetX,
                    zSHORT   nOffsetY,
                    zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
          CToolBar( ),
          ZMapAct( pZSubtask,          // base class ctor
                   pzmaComposite,
                   pWndParent,
                   this,
                   vDialog,
                   nOffsetX,
                   nOffsetY,
                   pCtrlDef )
{
   // Set the default flag.
   m_nShowIconMode = 1;       // Large icon.
   m_bShowIconText = false;   // Text.

   m_nTextWidth = 0;
   m_nTextHeight = 0;
   m_sizeSmallIcon = CSize( 16, 16 );
   m_sizeLargeIcon = CSize( 32, 32 );

   m_bOldFloating = FALSE;
}

ZToolBar::~ZToolBar( )     // dtor
{
   // Free the image list associated memory.
   for ( int k = 0; k < NB_POSSIBLE_MODE; k++ )
   {
      if ( m_ImageList[ k ].GetSafeHandle( ) )
         m_ImageList[ k ].DeleteImageList( );
   }
}

// Loading information.  Returns TRUE if the tool bar is loaded.
zBOOL
ZToolBar::LoadToolBar( UINT uResourceId )       // toolbar resource id
{
   // Convert the resource id into a resource name and load the toolbar
   // using the base class method.
   CString csResourceName;
   csResourceName.Format( "%d", uResourceId );
   zBOOL bReturn = CToolBar::LoadToolBar( csResourceName );

   if ( bReturn == FALSE )
      return( FALSE );  // couldn't load the toolbar

   // Retrieve the height of the toolbar before putting text.
   CToolBarCtrl& bar = GetToolBarCtrl( );
   int nIndex = 0;
   CRect NoTextRect( 0, 0, 0, 0 );
   bar.GetrectItem( 0, NoTextRect );

   // Set the text for each button.
   TBBUTTON tb;
   for ( nIndex = bar.GetButtonCount( ); nIndex >= 0; nIndex-- )
   {
      ZeroMemory( &tb, zsizeof( TBBUTTON ) );
      bar.GetButton( nIndex, &tb );

      // Do we have a separator?
      if ( (tb.fsStyle & TBSTYLE_SEP) == TBSTYLE_SEP )
          continue;

      // Do we have a valid command id?
      if ( tb.idCommand == 0 )
         continue;

      // Get the resource string if there is one.
      CString csText( (LPCSTR) tb.idCommand );
      LPCTSTR pszButtonText = 0;
      CString csButtonText( _T( &"" ) );
      _TCHAR seps[ ] = _T( &"\n" );

      if ( csText.IsEmpty( ) == FALSE )
      {
         pszButtonText = _tcstok( csText, seps );
         while ( pszButtonText )
         {
            csButtonText = pszButtonText;
            pszButtonText = _tcstok( 0, seps );
         }
      }

      if ( csButtonText.IsEmpty( ) == FALSE )
      {
         SetButtonText( nIndex, csButtonText );
      }
   }

   // Calculate the effect of the text on the toolbar.
   CRect rect( 0, 0, 0, 0 );
   CSize sizeMax( 0, 0 );

   for ( nIndex = bar.GetButtonCount( ); nIndex >= 0; nIndex-- )
   {
      bar.GetrectItem( nIndex, rect );

      rect.NormalizeRect( );
      sizeMax.cx = __max( rect.Size( ).cx, sizeMax.cx );
      sizeMax.cy = __max( rect.Size( ).cy, sizeMax.cy );
   }

   // Resize the toolbar.
   // The text width is the maximum width of the bitmap.  All toolbar sizes
   // must be at least this width.
   // The text height is the height added to the button.  Even in large mode
   // we must add this text height.
   m_nTextWidth = sizeMax.cx;
   m_nTextHeight = sizeMax.cy - (NoTextRect.Size( ).cy);
   ResizeToolBar( );

   // Build the image list.
   CClientDC dc( this );
   int nNbBits = dc.GetDeviceCaps( BITSPIXEL );
   for ( int k = 0; k < NB_POSSIBLE_MODE; k++ )
   {
      UINT nColorMode = ILC_COLOR;
      if ( nNbBits > 8 )
         nColorMode = ILC_COLORDDB;

      CSize Size = m_LargeIconSize;
      if ( k < 3 )
         Size = m_SmallIconSize;

      m_ImageList[ k ].Create( Size.cx, Size.cy,
                               nColorMode | ILC_MASK,
                               bar.GetButtonCount( ), 10 );
   }

   return( bReturn );
}

// Set the image list.
void
ZToolBar::SetImageList( eImageMode eMode,    // image mode
                        CImageList& rList )  // hover image list
{
   // Store the list handle for future use.
   m_ImageList[ eMode ].Attach( rList.Detach( ) );
}

// Set the current mode appropriate image list.
void
ZToolBar::AssignImageList( )
{
    // Prepare the list associated with the current mode.
   CImageList *pTempHotList;
   CImageList *pTempNormalList;
   CImageList *pTempDisableList;
   if ( m_nShowIconMode == 0 )
   {
      pTempHotList = &m_ImageList[ SmallHot ];
      pTempNormalList = &m_ImageList[ SmallStandard ];
      pTempDisableList = &m_ImageList[ SmallDisable ];
   }
   else
   if ( m_nShowIconMode == 1 )
   {
      pTempHotList = &m_ImageList[ LargeHot ];
      pTempNormalList = &m_ImageList[ LargeStandard ];
      pTempDisableList = &m_ImageList[ LargeDisable ];
   }

   // Set the list in the control.
   SetHotImageList( pTempHotList );
   SetStandardImageList( pTempNormalList );
   SetDisabledImageList( pTempDisableList );
}

// Set the image list in the control.
void
ZToolBar::InitImageList( )
{
   // Set the image list according to the current mode.
   AssignImageList( );
}

// Set the hover image list.
void
ZToolBar::SetHotImageList( CImageList *pList )      // hover image list
{
   // Retrieve the tool bar control and set the list reference.
   CWnd *pWnd = &GetToolBarCtrl( );
   pWnd->SendMessage( TB_SETHOTIMAGELIST, 0, (LPARAM)(HIMAGELIST) *pList );
}

// To set the normal image list.
void
ZToolBar::SetStandardImageList( CImageList *pList ) // normal image list
{
   // Retrieve the tool bar control and set the list reference.
   CWnd *pWnd = &GetToolBarCtrl( );
   pWnd->SendMessage( TB_SETIMAGELIST, 0, (LPARAM)(HIMAGELIST) *pList );
}

// To set the disable image list.
void
ZToolBar::SetDisabledImageList( CImageList *pList )    // normal image list
{
   // Retrieve the tool bar control and set the list reference.
   CWnd *pWnd = &GetToolBarCtrl( );
   pWnd->SendMessage( TB_SETDISABLEDIMAGELIST, 0, (LPARAM)( HIMAGELIST) *pList );
}

IMPLEMENT_DYNAMIC( ZBmpToolBar, ZToolBar )
BEGIN_MESSAGE_MAP( ZBmpToolBar, ZToolBar )
   //{{AFX_MSG_MAP( ZBmpToolBar )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

// ctor
ZBmpToolBar::ZBmpToolBar( ZSubtask *pZSubtask,        // ctor
                          CWnd     *pWndParent,
                          ZMapAct  *pzmaComposite,
                          zVIEW    vDialog,
                          zSHORT   nOffsetX,
                          zSHORT   nOffsetY,
                          zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
             ZToolBar( pZSubtask, pWndParent, pzmaComposite,
                       vDialog, nOffsetX, nOffsetY, pCtrlDef )
{
   CreateZ( );
}

// dtor
ZBmpToolBar::~ZBmpToolBar( )
{
}

void
ZBmpToolBar::CreateZ( )
{
   zBOOL bRC = FALSE;

// if ( CreateEx( m_pZSubtask->m_pZFWnd, TBSTYLE_FLAT,
//                WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
//                   CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) == 0 )
   if ( mIs_hWnd( m_hWnd ) == FALSE &&
        Create( m_pZSubtask->m_pZFWnd ) == 0 )
   {
      TraceLineS( "Failed to create toolbar:", *m_pzsTag );
      return;      // fail to create
   }

   if ( m_pZSubtask->m_hInstRes )
   {
      HINSTANCE hInst = AfxGetResourceHandle( );
      AfxSetResourceHandle( m_pZSubtask->m_hInstRes );

   // if ( LoadToolBar( IDR_MAINBAR ) == 0 )
      if ( LoadBitmap( 4104 ) == 0 )
      {
         TraceLineS( "Failed to load toolbar bitmap:", *m_pzsTag );
      }
      else
      {
         bRC = TRUE;
      }

      AfxSetResourceHandle( hInst );
   }
   else
   {
      bRC = FALSE;
   }

   if ( bRC == FALSE )
   {
      DestroyWindow( );
      m_ulMapActFlag2 &= ~zMAPACT_CREATED;
      return;
   }

   m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
   UINT *m_IdArray = new UINT[ 9 ];
   for ( UINT k = 0; k < 9; k++ )
      m_IdArray[ k ] = 5000 + k;

   SetButtons( m_IdArray, 9 );

   SetWindowText( "DKS ToolBar" );

   // TODO: Delete these three lines if you don't want the toolbar to
   //  be dockable
   EnableDocking( CBRS_ALIGN_ANY );
   m_pZSubtask->m_pZFWnd->EnableDocking( CBRS_ALIGN_ANY );
   m_pZSubtask->m_pZFWnd->DockControlBar( this );
}

zSHORT
ZBmpToolBar::MapFromOI( WPARAM wFlag )
{
   return( 0 );
}

// Set the image list in the control.
void
ZBmpToolBar::InitImageList( )
{
   // Build the image list.
   for ( int k = 0; k < NB_POSSIBLE_MODE; k++ )
   {
      BuildImageList( m_ImageList[ k ], m_256Image[ k ] );
   }

   // Set the image list according to the current mode.
   AssignImageList( );
}

// Build an image list based on the given bitmap and size.
void
ZBmpToolBar::BuildImageList( CImageList& rImageList, // image list to build
                             const CExBitmap& rBitmap ) // bitmap to put image list
{
   CBitmap bmpImage;
   bmpImage.LoadBitmap( rBitmap.m_nResourceId );
   rImageList.Add( &bmpImage, rBitmap.m_clrTransparent );
}

// Image list information method to set the small hover bitmap.
void
ZBmpToolBar::SetBitmap( UINT nBitmap,     // bitmap id
                        eImageMode eMode, // image mode
                        COLORREF clrBk,   // background color
                        zBOOL b256 )      // nb of color
{
   // Initialize the bitmap info.
   if ( b256 )
   {
      m_256Image[ eMode ].m_nResourceId = nBitmap;
      m_256Image[ eMode ].m_clrTransparent = clrBk;
   }
   else
   {
      m_16Image[ eMode ].m_nResourceId = nBitmap;
      m_16Image[ eMode ].m_clrTransparent = clrBk;
   }
}

// Macro and other definitions.
IMPLEMENT_DYNAMIC( ZIconToolBar, ZToolBar )
BEGIN_MESSAGE_MAP( ZIconToolBar, ZToolBar )
   //{{AFX_MSG_MAP( ZIconToolBar )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

// ctor
ZIconToolBar::ZIconToolBar( ZSubtask *pZSubtask,        // ctor
                            CWnd     *pWndParent,
                            ZMapAct  *pzmaComposite,
                            zVIEW    vDialog,
                            zSHORT   nOffsetX,
                            zSHORT   nOffsetY,
                            zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
              ZToolBar( pZSubtask, pWndParent, pzmaComposite,
                        vDialog, nOffsetX, nOffsetY, pCtrlDef )
{
}

// dtor
ZIconToolBar::~ZIconToolBar( )
{
   // Free the image list associated memory.
   for ( int k = 0; k < NB_POSSIBLE_MODE; k++ )
   {
      if ( m_ImageList[ k ].GetSafeHandle( ) )
         m_ImageList[ k ].DeleteImageList( );
   }
}

// Convert the given icon into grayscale.
// Returns: the gray scale icon.
HICON
ZIconToolBar::ConvertIconToGrayScale( HICON hIcon, // icon to convert
                                      CSize sizeImage )  // icon size
{
   // Validate the given icon.
   if ( hIcon == 0 )
   {
       return( 0 );
   }

   // Create a device context and initialise it.
   CDC dcMem;
   dcMem.CreateCompatibleDC( 0 );
   dcMem.SetBkMode( TRANSPARENT );

   // We need to load the icon into the context to initialise as a full color
   // context.
   ICONINFO IconInformation;
   GetIconInfo( hIcon, &IconInformation );

   // Create the bitmap to store the gray image.
   CBitmap bmpMem;
   bmpMem.CreateBitmap( sizeImage.cx, sizeImage.cx, 1, 16, 0 );
   CBitmap *pOldBmp = (CBitmap *) dcMem.SelectObject( &bmpMem );

   // Draw the icon in the context.
   dcMem.DrawState( CPoint( 0, 0 ), sizeImage, (HICON) hIcon,
                    DST_ICON | DSS_NORMAL, (CBrush *) 0 );

   // Retrieve each pixels and set them in gray scale.
   for ( int nRow = 0; nRow < sizeImage.cy; nRow++ )
   {
      for ( int nColumn = 0; nColumn < sizeImage.cx; nColumn++ )
      {
         COLORREF clrPixels = dcMem.GetPixel( nColumn, nRow );
         long lSquareSum = GetRValue( clrPixels ) * GetRValue( clrPixels ) +
                           GetBValue( clrPixels ) * GetBValue( clrPixels ) +
                           GetGValue( clrPixels ) * GetGValue( clrPixels );

         int nGray = (int) sqrt( ((double) lSquareSum) / 3 );
         dcMem.SetPixel( nColumn, nRow, RGB( nGray, nGray, nGray ) );
      }
   }

   dcMem.SelectObject( pOldBmp );

   // Set the grayscale bitmap in the icon.
   IconInformation.hbmColor = (HBITMAP) bmpMem.Detach( );

   // Create it and return.
   return( CreateIconIndirect( &IconInformation ) );
}

// Set the button icon.
void
ZIconToolBar::SetIcon( UINT nStandardIcon,  // button standard icon
                       UINT nDisableIcon,   // button disable icon
                       UINT nHotIcon )      // button icon when mouse is over
{
   // If the hot icon is 0, use the standard icon.
   if ( nHotIcon == 0 )
   {
      nHotIcon = nStandardIcon;
   }

   // Retrieve the small icon from the resource and add them in the image
   // list.
   {
      CSize Size = m_SmallIconSize;

      HICON hStandardIcon =
          (HICON) ::LoadImage( AfxGetApp( )->m_hInstance,
                               MAKEINTRESOURCE( nStandardIcon ),
                               IMAGE_ICON, Size.cx, Size.cy, 0 );
      HICON hHotIcon =
          (HICON) ::LoadImage( AfxGetApp( )->m_hInstance,
                               MAKEINTRESOURCE( nHotIcon ),
                               IMAGE_ICON, Size.cx, Size.cy, 0 );

      HICON hDisableIcon;
      if ( nDisableIcon == 0 )
      {
          hDisableIcon = ConvertIconToGrayScale( hStandardIcon, Size );
      }
      else
      {
          hDisableIcon =
          (HICON) ::LoadImage( AfxGetApp( )->m_hInstance,
                               MAKEINTRESOURCE( nDisableIcon ),
                               IMAGE_ICON, Size.cx, Size.cy, 0 );
      }

      // Add them in the image list.
      m_ImageList[ SmallHot ].Add( hHotIcon );
      m_ImageList[ SmallStandard ].Add( hStandardIcon );
      m_ImageList[ SmallDisable ].Add( hDisableIcon );
   }

   // Retrieve the large icon from the resource and add them to the image
   // list.
   {
      CSize Size = m_LargeIconSize;

      HICON hStandardIcon =
          (HICON) ::LoadImage( AfxGetApp( )->m_hInstance,
                               MAKEINTRESOURCE( nStandardIcon ),
                               IMAGE_ICON, Size.cx, Size.cy, 0 );
      HICON hHotIcon =
          (HICON) ::LoadImage( AfxGetApp( )->m_hInstance,
                               MAKEINTRESOURCE( nHotIcon ),
                               IMAGE_ICON, Size.cx, Size.cy, 0 );

      HICON hDisableIcon;
      if ( nDisableIcon == 0 )
      {
          hDisableIcon = ConvertIconToGrayScale( hStandardIcon, Size );
      }
      else
      {
          hDisableIcon =
          (HICON) ::LoadImage( AfxGetApp( )->m_hInstance,
                               MAKEINTRESOURCE( nDisableIcon ),
                               IMAGE_ICON, Size.cx, Size.cy, 0 );
      }

      // Add them in the image list.
      m_ImageList[ LargeHot ].Add( hHotIcon );
      m_ImageList[ LargeStandard ].Add( hStandardIcon );
      m_ImageList[ LargeDisable ].Add( hDisableIcon );
   }
}

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
ZBarItem::ZBarItem( zULONG ulId, zULONG ulType,
                    zCPCHAR cpcTag, zCPCHAR cpcText,
                    zCPCHAR cpcTipText, zCPCHAR cpcAction,
                    zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                    zCPCHAR cpcAttribute )
{
   m_ulId = ulId;
   m_ulType = ulType;
   m_cpcTag = cpcTag;

   if ( cpcText )
   {
      m_lTextLth = zstrlen( cpcText ) + 1;
      m_pchText = new char[ m_lTextLth ];
      strcpy_s( m_pchText, m_lTextLth, cpcText );
   }
   else
      m_pchText = 0;

   if ( cpcTipText )
   {
      m_lTipLth = zstrlen( cpcTipText ) + 1;
      m_pchTipText = new char[ m_lTipLth ];
      strcpy_s( m_pchTipText, m_lTipLth, cpcTipText );
   }
   else
      m_pchTipText = 0;

   m_cpcAction = cpcAction;
   m_cpcViewName = cpcViewName;
   m_cpcEntity = cpcEntity;
   m_cpcAttribute = cpcAttribute;
   m_pBarCombo = 0;
}

ZBarItem::~ZBarItem( )
{
   mDeleteInitA( m_pchText );
   mDeleteInitA( m_pchTipText );
   mDeleteInit( m_pBarCombo );
}

void
ZEditBar::AddBarItem( zULONG ulId, zULONG ulType,
                      zCPCHAR cpcTag, zCPCHAR cpcText,
                      zCPCHAR cpcTipText, zCPCHAR cpcAction,
                      zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                      zCPCHAR cpcAttribute, zCPCHAR cpcScope )
{
   ZBarItem *pBarItem;
   zSHORT nPos = m_nBtnCnt;

   while ( nPos )
   {
      nPos--;
      pBarItem = m_BarItemList[ nPos ];
      if ( pBarItem->m_ulId == ulId )
      {
         // 1 - Button; 2 - DropButton; 3 - ComboBox; 4 - EditCombo
         pBarItem->m_ulType = ulType;

         pBarItem->m_cpcTag = cpcTag;
         mDeleteInitA( pBarItem->m_pchText );
         if ( cpcText )
         {
            pBarItem->m_lTextLth = zstrlen( cpcText ) + 1;
            pBarItem->m_pchText = new char[ pBarItem->m_lTextLth ];
            strcpy_s( pBarItem->m_pchText, pBarItem->m_lTextLth, cpcText );
         }

         mDeleteInitA( pBarItem->m_pchTipText );
         if ( cpcTipText )
         {
            zLONG lLth = zstrlen( cpcTipText ) + 1;
            pBarItem->m_pchTipText = new char[ lLth ];
            strcpy_s( pBarItem->m_pchTipText, lLth, cpcTipText );
         }

         pBarItem->m_cpcAction = cpcAction;
         pBarItem->m_cpcViewName = cpcViewName;
         pBarItem->m_cpcEntity = cpcEntity;
         pBarItem->m_cpcAttribute = cpcAttribute;
         return;
      }
   }

   pBarItem = new ZBarItem( ulId, ulType, cpcTag, cpcText,
                            cpcTipText, cpcAction,
                            cpcViewName, cpcEntity,
                            cpcAttribute );
   m_BarItemList.SetAtGrow( m_nBtnCnt++, pBarItem );
}

zLONG
ZEditBar::GetItemText( zCPCHAR cpcItemTag, zPCHAR pchText, zLONG lMaxTextLth )
{
   ZBarItem *pBarItem = LocateBarItem( cpcItemTag );
   if ( pBarItem )
   {
      if ( pBarItem->m_pBarCombo )
      {
         CString cs;

         CWnd *pWnd = pBarItem->m_pBarCombo->GetWindow( GW_CHILD );
         if ( pWnd )
         {
            pWnd->GetWindowText( cs );
            strncat_s( pchText, lMaxTextLth, cs, lMaxTextLth - 1 );
         }

      // zLONG lIdx = pBarItem->m_pBarCombo->GetCurSel( );
      // pBarItem->m_pBarCombo->GetLBText( lIdx, cs );
      }
      else
         strcpy_s( pchText, lMaxTextLth, pBarItem->m_pchText );

      return( 0 );
   }

   return( -1 );
}

zLONG
ZEditBar::SetItemText( zCPCHAR cpcItemTag, zCPCHAR cpcText )
{
   ZBarItem *pBarItem = LocateBarItem( cpcItemTag );
   if ( pBarItem )
   {
      if ( pBarItem->m_pBarCombo )
         pBarItem->m_pBarCombo->SetWindowText( cpcText );
      else
         strcpy_s( pBarItem->m_pchText, pBarItem->m_lTextLth, cpcText );

      return( 0 );
   }

   return( -1 );
}

ZBarItem *
ZEditBar::LocateBarItem( zCPCHAR cpcItemTag )
{
   ZBarItem *pBarItem;
   zSHORT nPos = m_nBtnCnt;

   while ( nPos-- )
   {
      pBarItem = m_BarItemList[ nPos ];
      if ( zstrcmp( cpcItemTag, pBarItem->m_cpcTag ) == 0 )
         return( pBarItem );
   }

   return( 0 );
}

ZBarItem *
ZEditBar::LocateBarItem( zULONG ulId )
{
   ZBarItem *pBarItem;
   zSHORT nPos = m_nBtnCnt;

   while ( nPos-- )
   {
      pBarItem = m_BarItemList[ nPos ];
      if ( ulId == pBarItem->m_ulId )
         return( pBarItem );
   }

   return( 0 );
}

void
ZEditBar::CleanupBarItems( )
{
   ZBarItem *pBarItem;
   while ( m_nBtnCnt > 0 )
   {
      m_nBtnCnt--;
      pBarItem = m_BarItemList[ m_nBtnCnt ];
      mDeleteInit( pBarItem );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
ToolBar( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
// if ( pCtrlDef->Subtype & zTOOLBAR_PALETTE )
//    return( new ZPaletteBar( pZSubtask, pWndParent,
//                             pzmaComposite, vDialog,
//                             nOffsetX, nOffsetY, pCtrlDef ) );
// else
      return( new ZEditBar( pZSubtask, pWndParent,
                            pzmaComposite, vDialog,
                            nOffsetX, nOffsetY, pCtrlDef ) );
}

//./ ADD NAME=TB_GetLastSelection
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TB_GetLastSelection
//                                              02/25/02 Modified: 02/25/02
//
//  PURPOSE:    This operation gets the last selection for a toolbar item
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the toolbar.
//              cpcCtrlTag  - The unique name of the control.
//              pchReturnText - The last selected text.
//              lMaxTextLth - The maximum length for the return text
//                            (without the null terminator).
//
//  RETURNS:   >= 0 - Last selection successfully returned
//             -1 - Error locating control or no selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zLONG  OPERATION
TB_GetLastSelection( zVIEW   vSubtask,
                     zCPCHAR cpcCtrlTag,
                     zPCHAR  pchText,
                     zLONG   lMaxTextLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEditBar *pEditBar = DYNAMIC_DOWNCAST( ZEditBar, pzma->m_pCtrl );
      if ( pEditBar )
         return( pEditBar->GetLastSelection( pchText, lMaxTextLth ) );

      TraceLineS( "drvr - Invalid control type for TB_GetLastSelection ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TB_GetItemText
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TB_GetItemText
//                                          2003/10/24 Modified: 2003/10/24
//
//  PURPOSE:    This operation gets the text for a toolbar item
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the toolbar.
//              cpcCtrlTag  - The unique name of the control.
//              cpcItemTag - The unique name of the toolbar item.
//              pchReturnText - The toolbar item text.
//              lMaxTextLth - The maximum length for the return text
//                            (without the null terminator).
//
//  RETURNS:    0 - Text successfully returned
//             -1 - Error locating control or no selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zLONG  OPERATION
TB_GetItemText( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zCPCHAR cpcItemTag,
                zPCHAR  pchText,
                zLONG   lMaxTextLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   pchText[ 0 ] = 0;
   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEditBar *pEditBar = DYNAMIC_DOWNCAST( ZEditBar, pzma->m_pCtrl );
      if ( pEditBar )
         return( pEditBar->GetItemText( cpcItemTag, pchText, lMaxTextLth ) );

      TraceLineS( "drvr - Invalid control type for TB_GetItemText ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TB_SetItemText
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TB_SetItemText
//                                          2003/10/24 Modified: 2003/10/24
//
//  PURPOSE:    This operation sets the text for a toolbar item
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the toolbar.
//              cpcCtrlTag  - The unique name of the control.
//              cpcSubCtrlTag - The unique name of the toolbar item.
//              cpcText     - The toolbar item text.
//
//  RETURNS:    0 - Text successfully set
//             -1 - Error locating control or no selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zLONG  OPERATION
TB_SetItemText( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zCPCHAR cpcItemTag,
                zCPCHAR cpcText )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEditBar *pEditBar = DYNAMIC_DOWNCAST( ZEditBar, pzma->m_pCtrl );
      if ( pEditBar )
         return( pEditBar->SetItemText( cpcItemTag, cpcText ) );

      TraceLineS( "drvr - Invalid control type for TB_SetItemText ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TB_GetTextMode
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TB_GetTextMode
//                                              02/25/02 Modified: 02/25/02
//
//  PURPOSE:    This operation gets the last selection for a toolbar item
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the toolbar.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Text mode is off.
//              1 - Text mode is on.
//             -1 - Error locating control or no selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
TB_GetTextMode( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEditBar *pEditBar = DYNAMIC_DOWNCAST( ZEditBar, pzma->m_pCtrl );
      if ( pEditBar )
         return( pEditBar->GetTextMode( ) );

      TraceLineS( "drvr - Invalid control type for TB_GetTextMode ", cpcCtrlTag );
   }

   return( -1 );
}

//./ ADD NAME=TB_SetTextMode
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: TB_SetTextMode
//                                              02/25/02 Modified: 02/25/02
//
//  PURPOSE:    This operation gets the last selection for a toolbar item
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the toolbar.
//              cpcCtrlTag  - The unique name of the control.
//              bTextMode   - TRUE - set text mode on.
//                            FALSE - set text mode off.
//
//  RETURNS:    0 - Previous text mode is off.
//              1 - Previous text mode is on.
//             -1 - Error locating control or no selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 3
zOPER_EXPORT zSHORT OPERATION
TB_SetTextMode( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zSHORT  bTextMode )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZEditBar *pEditBar = DYNAMIC_DOWNCAST( ZEditBar, pzma->m_pCtrl );
      if ( pEditBar )
         return( pEditBar->SetTextMode( (zBOOL) bTextMode ) );

      TraceLineS( "drvr - Invalid control type for TB_SetTextMode ", cpcCtrlTag );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZClientRect : public CRect
{
public:
   ZClientRect( HWND hWnd )
   {
      ::GetClientRect( hWnd, this );
   };

   ZClientRect( const CWnd *pWnd )
   {
      ::GetClientRect( pWnd->GetSafeHwnd( ), this );
   };
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class ZWindowRect : public CRect
{
public:
   ZWindowRect( HWND hWnd )
   {
      ::GetWindowRect( hWnd, this );
   };

   ZWindowRect( const CWnd *pWnd )
   {
      ::GetWindowRect( pWnd->GetSafeHwnd( ), this );
   };
};

/////////////////////////////////////////////////////////////////////////////
ZTB_Popup::ZTB_Popup( )
{
   m_pCaller = 0;
   m_pControl = 0;
   m_pEvents = 0;
   m_hPrevParentCtrl = 0;
   m_bDestroy = FALSE;
}

ZTB_Popup::~ZTB_Popup( )
{
   m_pCaller = 0;
   m_pControl = 0;
   m_pEvents = 0;
}

/////////////////////////////////////////////////////////////////////////////
zBOOL
ZTB_Popup::Display( CWnd *pControl, CWnd *pCaller,
                    CRect& rectCaller, IPopupCtrlEvent *pEvents )
{
   if ( pControl == 0 )
   {
      delete( this );
      return( FALSE );
   }

   m_pControl = pControl;
   m_pCaller = pCaller;
   m_pEvents = pEvents;

   if ( m_pEvents && m_pEvents->OnInit( ) == FALSE )
   {
      delete( this );
      return( FALSE );
   }
   // Calculate the most appropriate position for the left-top corner of control
   // By default, at left and bottom of the caller
   CPoint pt( rectCaller.left, rectCaller.bottom );
   ZWindowRect rectCtrl( m_pControl );

   // Alignment at right if necessary
   if ( pt.x + rectCtrl.Width( ) > ::GetSystemMetrics( SM_CXSCREEN ) )
   {
      pt.x = rectCaller.right-rectCtrl.Width( );
   }

   // Alignment at top if necessary
   if ( pt.y + rectCtrl.Height( ) > ::GetSystemMetrics( SM_CYSCREEN ) )
   {
      pt.y = rectCaller.top-rectCtrl.Height( );
   }

   // Adjustments to keep control into screen
   if ( pt.x + rectCtrl.Width( ) > ::GetSystemMetrics( SM_CXSCREEN ) )
   {
      pt.x = ::GetSystemMetrics( SM_CXSCREEN )-rectCtrl.Width( );
   }

   if ( pt.y + rectCtrl.Height( ) > ::GetSystemMetrics( SM_CYSCREEN ) )
   {
      pt.y = ::GetSystemMetrics( SM_CYSCREEN )-rectCtrl.Height( );
   }

   if ( pt.x < 0 )
      pt.x = 0;

   if ( pt.y < 0 )
      pt.y = 0;

   if ( !CMiniFrameWnd::Create( 0, _T(""),
                                WS_POPUP | (m_pCaller ? MFS_SYNCACTIVE : 0),
                                CRect( pt.x, pt.y,
                                       pt.x + rectCtrl.Width( ),
                                       pt.y+rectCtrl.Height( ) ),
                                m_pControl->GetParentFrame( ) ) )
   {
      return( FALSE );
   }

   m_hPrevParentCtrl = m_pControl->SetParent( this )->GetSafeHwnd( );
   m_pControl->UpdateWindow( );
   m_pControl->SetWindowPos( 0, 0, 0, 0, 0,
                             SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW );
   ShowWindow( SW_SHOW );

   if ( m_pEvents )
   {
      m_pEvents->OnShow( );
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
void
ZTB_Popup::EndPopup( bool bAbort, bool bSetFocus )
{
   zBOOL bDestroy = m_bDestroy;

   m_bDestroy = TRUE;
   if ( m_pEvents )
      m_pEvents->OnHide( bAbort );

   ASSERT( m_pControl->m_hWnd );

   m_pControl->ShowWindow( SW_HIDE );

   ASSERT( m_hPrevParentCtrl );
   ::SetParent( m_pControl->m_hWnd, m_hPrevParentCtrl );

   // Do a local copy of the pointer because DestroyWindow deletes the object
   // and m_pCaller will not be valid after the delete.
   CWnd *pCaller = m_pCaller;

   if ( bDestroy == FALSE )
      DestroyWindow( );

   if ( bSetFocus && pCaller )
      pCaller->SetFocus( );
}

/////////////////////////////////////////////////////////////////////////////
BOOL
ZTB_Popup::PreTranslateMessage( MSG *pMsg )
{
   bool bEnd = false;
   bool bAbort = false;
   bool bResult = false;
   int  nCode = eDoNothing;

   switch ( pMsg->message )
   {
      case WM_KEYDOWN:
         // Default action for <Escape> key
         if ( pMsg->wParam == VK_ESCAPE )
         {
            bEnd = bAbort = bResult = true;
            break;
         }

         if ( m_pEvents )
         {
            IPopupCtrlEvent *pEvents = m_pEvents;

            // Search for the interface for the correct object (referenced
            // by pMsg->hwnd).
            if ( pMsg->hwnd != m_pControl->m_hWnd )
            {
               pEvents = m_pEvents->GetInterfaceOf( pMsg->hwnd );
            }

            if ( pEvents )
            {
               nCode = pEvents->OnKeyDown( pMsg->wParam,
                                           LOWORD( pMsg->lParam ),
                                           HIWORD( pMsg->lParam ) );
            }
         }

         break;

      case WM_LBUTTONDOWN:
         if ( m_pEvents )
         {
            IPopupCtrlEvent *pEvents = m_pEvents;

            // Search for the interface for the correct object
            if ( pMsg->hwnd != m_pControl->m_hWnd )
            {
               pEvents = m_pEvents->GetInterfaceOf( pMsg->hwnd );
            }

            if ( pEvents )
            {
               nCode = pEvents->OnLButtonDown( pMsg->wParam, CPoint( (zSHORT) LOWORD( pMsg->lParam ),
                                                                     (zSHORT) HIWORD( pMsg->lParam ) ) );
            }
         }

         break;

      case WM_LBUTTONUP:
         if ( m_pEvents )
         {
            IPopupCtrlEvent *pEvents = m_pEvents;

            // Search for the interface for the correct object.
            if ( pMsg->hwnd != m_pControl->m_hWnd )
            {
               pEvents = m_pEvents->GetInterfaceOf( pMsg->hwnd );
            }

            if ( pEvents )
            {
               nCode = pEvents->OnLButtonUp( pMsg->wParam, CPoint( (zSHORT) LOWORD( pMsg->lParam ),
                                                                   (zSHORT) HIWORD( pMsg->lParam ) ) );
            }
         }

         break;
   }

   switch ( nCode )
   {
      case eNoSend:
         bResult = true;
         break;

      case eEnd:
         bEnd = bResult = true;
         break;

      case eAbort:
         bEnd = bAbort = bResult = true;
         break;
   }

   if ( bResult == FALSE )
   {
      bResult = (CMiniFrameWnd::PreTranslateMessage( pMsg ) != 0 );
   }

   if ( bEnd )
   {
      EndPopup( bAbort );
   }

   return( bResult );
}

///////////////////////////////////////////////////////////////////////////////
void
ZTB_Popup::PostNcDestroy( )
{
   delete( this );
}

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP( ZTB_Popup, CMiniFrameWnd )
   //{{AFX_MSG_MAP( ZTB_Popup )
   ON_WM_ACTIVATE( )
   ON_WM_CANCELMODE( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

IMPLEMENT_DYNCREATE( ZTB_Popup, CMiniFrameWnd );

/////////////////////////////////////////////////////////////////////////////
void
ZTB_Popup::OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized )
{
   CMiniFrameWnd::OnActivate( nState, pWndOther, bMinimized );

   if ( nState == WA_INACTIVE && m_bDestroy == FALSE )
   {
      CFrameWnd *pFrame = 0;
      bool      bSetFocus = false;

      if ( m_pCaller &&
           (pFrame = m_pCaller->GetParentFrame( )) != 0 &&
           pWndOther )
      {
         bSetFocus = (pWndOther->m_hWnd == pFrame->m_hWnd);
      }

      EndPopup( true, bSetFocus );
   }
}

/////////////////////////////////////////////////////////////////////////////
void
ZTB_Popup::OnCancelMode( )
{
   PostMessage( WM_DESTROY );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ZTB_List

ZTB_List::ZTB_List( )
{
}

ZTB_List::~ZTB_List( )
{
}

int
ZTB_List::GetSelectedItem( ) const
{
   zSHORT nCount = GetItemCount( );

   for ( zSHORT k = 0; k < nCount; k++ )
   {
      if ( GetItemState( k, LVIS_SELECTED ) & LVIS_SELECTED )
      {
         return( k );
      }
   }

   return( -1 );
}

BEGIN_MESSAGE_MAP( ZTB_List, CListCtrl )
   //{{AFX_MSG_MAP( ZTB_List )
      // NOTE - the ClassWizard will add and remove mapping macros here.
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool
ZTB_List::XPopupCtrlEvent::OnInit( )
{
   METHOD_PROLOGUE_( ZTB_List, PopupCtrlEvent );

// TRACE( "ZTB_List::OnInit( )\n" );
   pThis->ModifyStyle( LVS_EDITLABELS, 0 ); // does not work correctly!
   pThis->ModifyStyle( 0, WS_BORDER, SWP_DRAWFRAME );

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
void
ZTB_List::XPopupCtrlEvent::OnShow( )
{
   METHOD_PROLOGUE_( ZTB_List, PopupCtrlEvent );

// TRACE( "ZTB_List::OnShow( )\n" );
   pThis->SetFocus( );
   pThis->Scroll( CSize( 20,0 ) ); // to correctly display the calendar
}

/////////////////////////////////////////////////////////////////////////////
void
ZTB_List::XPopupCtrlEvent::OnHide( bool bAbort )
{
   METHOD_PROLOGUE_( ZTB_List, PopupCtrlEvent );

#if 0
   TRACE( "ZTB_List::OnHide( Abort = %s )\n", bAbort ? "TRUE" : "FALSE" );

   CEditView *pView = (CEditView *)
     ((CMDIFrameWnd *) AfxGetMainWnd( ))->GetActiveFrame( )->GetActiveView( );

   CString csText;
   if ( pView && bAbort == FALSE )
   {
      int nLength = pView->GetWindowTextLength( );

      csText.Format( "ZTB_List::Selection = \"%s\"\r\n",
                     pThis->GetItemText( pThis->GetSelectedItem( ), 0 ) );

      pView->GetEditCtrl( ).SetSel( nLength + 1, nLength + 1, TRUE );
      pView->GetEditCtrl( ).ReplaceSel( csText );
      nLength = pView->GetWindowTextLength( );
      pView->GetEditCtrl( ).SetSel( nLength + 1, nLength + 1 );
   }
#else
   zLONG   lSelectedItem = -1;

   pThis->m_csSendText = "";
   if ( bAbort == FALSE )
   {
      lSelectedItem = pThis->GetSelectedItem( );
      if ( lSelectedItem >= 0 )
         pThis->m_csSendText = pThis->GetItemText( lSelectedItem, 0 );
   }

   ZTB_Popup *pPopup = (ZTB_Popup *) pThis->GetParent( );
   if ( pPopup && pPopup->m_pCaller &&
        mIs_hWnd( pPopup->m_pCaller->m_hWnd ) )
   {
      pPopup->m_pCaller->SendMessage( zZEIDON_TOOLBAR_SET_SELECT,
                                      (WPARAM) lSelectedItem, (LPARAM) &(pThis->m_csSendText) );
   }
#endif
}

/////////////////////////////////////////////////////////////////////////////
int
ZTB_List::XPopupCtrlEvent::OnKeyDown( UINT nChar, UINT, UINT )
{
   if ( nChar == VK_RETURN )
   {
      return( ZTB_Popup::eEnd );
   }

   return( ZTB_Popup::eDoNothing );
}

/////////////////////////////////////////////////////////////////////////////
int
ZTB_List::XPopupCtrlEvent::OnLButtonDown( UINT, CPoint pt )
{
   // Uncomment next line to close by double click instead of simple click
   // return( ZTB_Popup::doNothing );

   METHOD_PROLOGUE_( ZTB_List, PopupCtrlEvent );

   int  hItem;
   UINT uHitFlags = LVHT_ONITEM;

   if ( ( hItem = pThis->HitTest( pt, &uHitFlags ) ) != -1 )
   {
      pThis->SetItemState( hItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
   }

   return( ZTB_Popup::eNoSend );
}

/////////////////////////////////////////////////////////////////////////////
int
ZTB_List::XPopupCtrlEvent::OnLButtonUp( UINT, CPoint pt )
{
   METHOD_PROLOGUE_( ZTB_List, PopupCtrlEvent );

   int  hItem;
   UINT uHitFlags = LVHT_ONITEM;

   if ( (hItem = pThis->HitTest( pt, &uHitFlags )) != -1 )
   {
      return( ZTB_Popup::eEnd );
   }

   return( ZTB_Popup::eDoNothing );
}

/////////////////////////////////////////////////////////////////////////////
IPopupCtrlEvent *
ZTB_List::XPopupCtrlEvent::GetInterfaceOf( HWND )
{
   // Not used here because this control doesn't include other controls
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZTB_Combo

BEGIN_MESSAGE_MAP( ZTB_Combo, CComboBox )
   //{{AFX_MSG_MAP( ZTB_Combo )
   // NOTE - the ClassWizard will add and remove mapping macros here.
   ON_WM_CREATE( )
   ON_CONTROL_REFLECT( CBN_DROPDOWN, CBNDropdown )
   ON_CONTROL_REFLECT( CBN_CLOSEUP, CBNCloseup )
   ON_CONTROL_REFLECT( CBN_SELCHANGE, CBNSelchange )
   ON_CONTROL_REFLECT( CBN_SELENDOK, CBNSelendok )
   ON_CONTROL_REFLECT( CBN_SELENDCANCEL, CBNSelendcancel )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

ZTB_Combo::ZTB_Combo( zULONG ulId )
{
   m_ulId = ulId;
   m_bIsDroppedDown = FALSE;
}

ZTB_Combo::~ZTB_Combo( )
{
}

int
ZTB_Combo::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = CComboBox::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   // Set the style to flat.
   // There is a bug when docking vertically.  The size of the separator
   // is not calculated correctly by MFC.  Only in flat style.
   ModifyStyle( CBS_DROPDOWNLIST, CBS_DROPDOWN );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
// ZTB_Combo message handlers

void
ZTB_Combo::CBNDropdown( )
{
// TraceLineS( "ZTB_Combo::CBNDropdown", "" );
   m_bIsDroppedDown = TRUE;
}

void
ZTB_Combo::CBNCloseup( )
{
// TraceLineS( "ZTB_Combo::CBNCloseup", "" );
   if ( m_bIsDroppedDown )
   {
      m_bIsDroppedDown = FALSE;

      zLONG   lSelectedItem = GetCurSel( );

      m_csSendText = "";
      if ( lSelectedItem >= 0 )
         GetLBText( lSelectedItem, m_csSendText ) ;

      ZEditBar *pBar = (ZEditBar *) GetParent( );
      if ( pBar && mIs_hWnd( pBar->m_hWnd ) )
      {
         pBar->m_uLastDropDownId = m_ulId - 0x0100;
         pBar->SendMessage( zZEIDON_TOOLBAR_SET_SELECT, (WPARAM) lSelectedItem, (LPARAM) &m_csSendText );
      }
   }
}

void
ZTB_Combo::CBNSelchange( )
{
   zLONG   lSelectedItem = GetCurSel( );

#ifdef DEBUG_ALL
   TraceLineI( "ZTB_Combo::CBNSelchange Selected Item = ", lSelectedItem );
#endif

   m_csSendText = "";
   if ( lSelectedItem >= 0 )
      GetLBText( lSelectedItem, m_csSendText );

   ZEditBar *pBar = (ZEditBar *) GetParent( );
   if ( pBar && mIs_hWnd( pBar->m_hWnd ) )
   {
      pBar->m_uLastDropDownId = m_ulId - 0x0100;
      pBar->PostMessage( zZEIDON_TOOLBAR_SET_SELECT, (WPARAM) lSelectedItem, (LPARAM) &m_csSendText );
   }
}

void
ZTB_Combo::CBNSelendok( )
{
// TraceLineS( "ZTB_Combo::CBNSelendok", "" );
   if ( m_bIsDroppedDown )
   {
      m_bIsDroppedDown = FALSE;

#if 0  // this gives a double event (along with CBNSelchange)
      zLONG   lSelectedItem = GetCurSel( );

      m_csSendText = "";
      if ( lSelectedItem >= 0 )
         GetLBText( lSelectedItem, m_csSendText ) ;

      ZEditBar *pBar = (ZEditBar *) GetParent( );
      if ( pBar && mIs_hWnd( pBar->m_hWnd ) )
      {
         pBar->m_uLastDropDownId = m_ulId - 0x0100;
         pBar->SendMessage( zZEIDON_TOOLBAR_SET_SELECT,
                            (WPARAM) lSelectedItem,
                            (LPARAM) &m_csSendText );
      }
#endif
   }
}

void
ZTB_Combo::CBNSelendcancel( )
{
// TraceLineS( "ZTB_Combo::CBNSelendcancel", "" );
   m_bIsDroppedDown = FALSE;
}

#if 0
BOOL
ZTB_Combo::PreTranslateMessage( MSG *pMsg )
{
   if ( (pMsg->message != WM_KEYDOWN) || (pMsg->wParam != VK_RETURN) )
      return( CComboBox::PreTranslateMessage( pMsg ) );

   // When the enter key is hit in the ComboBox we want to add the string
   // to the top of the list and hilight it.  We also want to limit the
   // list to the last 15 entries.
   if ( (pMsg->lParam & 0x40000000) == 0 )   // not a repeat
   {
      CString csText;
      GetWindowText( csText );
      InsertString( 0, csText );
      SetCurSel( 0 );
      if ( GetCount( ) > 15 )
         DeleteString( GetCount( ) - 1 );
   }

   return( TRUE );
}
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// ZSizingControlBar            Version 2.44
//
// Created: Jan 24, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
//
// The sources and a short version of the docs are also available at
// www.codeproject.com . Look for a "Docking Windows" section and check
// the version to be sure you get the latest one ;)
//
// Hint: These classes are intended to be used as base classes. Do not
// simply add your code to these files - instead create a new class
// derived from one of ZSizingControlBarXX classes and put there what
// you need. See CMyBar classes in the demo projects for examples.
// Modify this file only to fix bugs, and don't forget to send me a copy.
/////////////////////////////////////////////////////////////////////////
// Acknowledgements:
//  o   Thanks to Harlan R. Seymour for his continuous support during
//      development of this code.
//  o   Thanks to Dundas Software for the opportunity
//      to test this code on real-life applications.
//  o   Some ideas for the gripper came from the CToolBarEx flat toolbar
//      by Joerg Koenig. Thanks, Joerg!
//  o   Thanks to Robert Wolpow for the code on which CDockContext based
//      dialgonal resizing is based.
//  o   Thanks to the following people for various bug fixes and/or
//      enhancements: Chris Maunder, Jakawan Ratiwanich, Udo Schaefer,
//      Anatoly Ivasyuk, Peter Hauptmann, DJ(?), Pat Kusbel, Aleksey
//      Malyshev.
//  o   And, of course, many thanks to all of you who used this code,
//      for the invaluable feedback I received.
/////////////////////////////////////////////////////////////////////////

// sizecbar.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar

IMPLEMENT_DYNAMIC( ZSizingControlBar, CControlBar );
BEGIN_MESSAGE_MAP( ZSizingControlBar, CControlBar )
   //{{AFX_MSG_MAP(ZSizingControlBar)
   ON_WM_CREATE()
   ON_WM_PAINT()
   ON_WM_NCPAINT()
   ON_WM_NCCALCSIZE()
   ON_WM_WINDOWPOSCHANGING()
   ON_WM_CAPTURECHANGED()
   ON_WM_SETTINGCHANGE()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_NCLBUTTONDOWN()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_RBUTTONDOWN()
   ON_WM_NCMOUSEMOVE()
   ON_WM_NCHITTEST()
   ON_WM_CLOSE()
   ON_WM_SIZE()
   ON_WM_WINDOWPOSCHANGED( )
   //}}AFX_MSG_MAP
   ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

ZSizingControlBar::ZSizingControlBar()
{
    m_szMinHorz = CSize(zTAG_LTH, 32);
    m_szMinVert = CSize(zTAG_LTH, 32);
    m_szMinFloat = CSize(37, 32);
    m_szHorz = CSize(200, 200);
    m_szVert = CSize(200, 200);
    m_szFloat = CSize(200, 200);
    m_bTracking = FALSE;
    m_bKeepSize = FALSE;
    m_bParentSizing = FALSE;
    m_cxEdge = 5;
    m_bDragShowContent = FALSE;
    m_nDockBarID = 0;
    m_dwSCBStyle = 0;
}

ZSizingControlBar::~ZSizingControlBar()
{
}

// old creation method, still here for compatibility reasons
BOOL ZSizingControlBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
                               CSize sizeDefault, BOOL bHasGripper,
                               UINT nID, DWORD dwStyle)
{
    UNUSED_ALWAYS(bHasGripper);

    m_szHorz = m_szVert = m_szFloat = sizeDefault;
    return Create(lpszWindowName, pParentWnd, nID, dwStyle);
}

// preferred creation method
BOOL ZSizingControlBar::Create(LPCTSTR lpszWindowName,
                               CWnd* pParentWnd, UINT nID,
                               DWORD dwStyle)
{
    // must have a parent
    ASSERT_VALID(pParentWnd);
    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    ASSERT (!((dwStyle & CBRS_SIZE_FIXED) &&
              (dwStyle & CBRS_SIZE_DYNAMIC)));

    m_dwStyle = dwStyle & CBRS_ALL; // save the control bar styles

    // register and create the window - skip CControlBar::Create()
    CString wndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        ::GetSysColorBrush(COLOR_BTNFACE), 0);

    dwStyle &= ~CBRS_ALL; // keep only the generic window styles
    dwStyle |= WS_CLIPCHILDREN; // prevents flashing
    if (!CWnd::Create(wndclass, lpszWindowName, dwStyle,
        CRect(0, 0, 0, 0), pParentWnd, nID))
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar operations
#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
void ZSizingControlBar::EnableDocking(DWORD dwDockStyle)
{
    // must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
    // cannot have the CBRS_FLOAT_MULTI style
    ASSERT((dwDockStyle & CBRS_FLOAT_MULTI) == 0);
    // the bar must have CBRS_SIZE_DYNAMIC style
    ASSERT((m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    m_dwDockStyle = dwDockStyle;
    if (m_pDockContext == NULL)
        m_pDockContext = new ZSCBDockContext(this);

    // permanently wire the bar's owner to its current parent
    if (m_hWndOwner == NULL)
        m_hWndOwner = ::GetParent(m_hWnd);
}
#endif

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar message handlers

void
ZSizingControlBar::OnWindowPosChanged( WINDOWPOS *lpWndPos )
{
// TraceLineS( "ZSizingControlBar::OnWindowPosChanged: ", "" );

   // Control Bar notification needs to be added
   CControlBar::OnWindowPosChanged( lpWndPos );
}

int ZSizingControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CControlBar::OnCreate(lpCreateStruct) == -1)
        return( -1 );

    // query SPI_GETDRAGFULLWINDOWS system parameter
    // OnSettingChange() will update m_bDragShowContent
    m_bDragShowContent = FALSE;
    ::SystemParametersInfo( SPI_GETDRAGFULLWINDOWS, 0,
                            &m_bDragShowContent, 0 );

    // uncomment this line if you want raised borders
//  m_dwSCBStyle |= SCBS_SHOWEDGES;

    return( 0 );
}

LRESULT ZSizingControlBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
    UNUSED_ALWAYS(wParam);

    LRESULT lResult = CWnd::Default();

    if (IsFloating() &&
        GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
    {
        m_pDockBar->SetWindowText((LPCTSTR) lParam); // update dockbar
        GetParentFrame()->DelayRecalcLayout(); // refresh miniframe
    }

    return lResult;
}

const BOOL ZSizingControlBar::IsFloating() const
{
    return !IsHorzDocked() && !IsVertDocked();
}

const BOOL ZSizingControlBar::IsHorzDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

const BOOL ZSizingControlBar::IsVertDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT ||
        m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT);
}

const BOOL ZSizingControlBar::IsSideTracking() const
{
    // don't call this when not tracking
    ASSERT(m_bTracking && !IsFloating());

    return (m_htEdge == HTLEFT || m_htEdge == HTRIGHT) ?
        IsHorzDocked() : IsVertDocked();
}

CSize ZSizingControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    if (bStretch) // the bar is stretched (is not the child of a dockbar)
        if (bHorz)
            return CSize(32767, m_szHorz.cy);
        else
            return CSize(m_szVert.cx, 32767);

    // dirty cast - we need access to protected CDockBar members
    ZSCBDockBar* pDockBar = (ZSCBDockBar*) m_pDockBar;

    // force imediate RecalcDelayShow() for all sizing bars on the row
    // with delayShow/delayHide flags set to avoid IsVisible() problems
    ZSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = pDockBar->m_bLayoutQuery ?
        NULL : ::BeginDeferWindowPos(arrSCBars.GetSize());
    for (int i = 0; i < arrSCBars.GetSize(); i++)
        if (arrSCBars[i]->m_nStateFlags & (delayHide|delayShow))
            arrSCBars[i]->RecalcDelayShow(&layout);
    if (layout.hDWP != NULL)
        ::EndDeferWindowPos(layout.hDWP);

    // get available length
    CRect rc = pDockBar->m_rectLayout;
    if (rc.IsRectEmpty())
        m_pDockSite->GetClientRect(&rc);
    int nLengthTotal = bHorz ? rc.Width() + 2 : rc.Height() - 2;

    if (IsVisible() && !IsFloating() &&
        m_bParentSizing && arrSCBars[0] == this)
        if (NegotiateSpace(nLengthTotal, (bHorz != FALSE)))
            AlignControlBars();

    m_bParentSizing = FALSE;

    if (bHorz)
        return CSize(max(m_szMinHorz.cx, m_szHorz.cx),
                     max(m_szMinHorz.cy, m_szHorz.cy));

    return CSize(max(m_szMinVert.cx, m_szVert.cx),
                 max(m_szMinVert.cy, m_szVert.cy));
}

CSize ZSizingControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) // docked ?
    {
        if (nLength == -1)
            m_bParentSizing = TRUE;

        return CControlBar::CalcDynamicLayout(nLength, dwMode);
    }

    if (dwMode & LM_MRUWIDTH) return m_szFloat;
    if (dwMode & LM_COMMIT) return m_szFloat; // already committed

#ifndef _SCB_REPLACE_MINIFRAME
    // check for dialgonal resizing hit test
    int nHitTest = m_pDockContext->m_nHitTest;
    if (IsFloating() &&
        (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT ||
        nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT))
    {
        CPoint ptCursor;
        ::GetCursorPos(&ptCursor);

        CRect rFrame, rBar;
        GetParentFrame()->GetWindowRect(&rFrame);
        GetWindowRect(&rBar);

        if (nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT)
        {
            m_szFloat.cx = rFrame.left + rBar.Width() - ptCursor.x;
            m_pDockContext->m_rectFrameDragHorz.left =
                min(ptCursor.x, rFrame.left + rBar.Width() - m_szMinFloat.cx);
        }

        if (nHitTest == HTTOPLEFT || nHitTest == HTTOPRIGHT)
        {
            m_szFloat.cy = rFrame.top + rBar.Height() - ptCursor.y;
            m_pDockContext->m_rectFrameDragHorz.top =
                min(ptCursor.y, rFrame.top + rBar.Height() - m_szMinFloat.cy);
        }

        if (nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT)
            m_szFloat.cx = rBar.Width() + ptCursor.x - rFrame.right;

        if (nHitTest == HTBOTTOMLEFT || nHitTest == HTBOTTOMRIGHT)
            m_szFloat.cy = rBar.Height() + ptCursor.y - rFrame.bottom;
    }
    else
#endif //_SCB_REPLACE_MINIFRAME
        ((dwMode & LM_LENGTHY) ? m_szFloat.cy : m_szFloat.cx) = nLength;

    m_szFloat.cx = max( m_szFloat.cx, m_szMinFloat.cx );
    m_szFloat.cy = max( m_szFloat.cy, m_szMinFloat.cy );

    return m_szFloat;
}

void ZSizingControlBar::OnWindowPosChanging( WINDOWPOS FAR* lpWndPos )
{
// TraceLineS( "ZSizingControlBarOnWindowPosChanging", "" );

    // force non-client recalc if moved or resized
    lpWndPos->flags |= SWP_FRAMECHANGED;

    CControlBar::OnWindowPosChanging( lpWndPos );

    // find on which side are we docked
    m_nDockBarID = GetParent()->GetDlgCtrlID( );

    if ( !IsFloating( ) )
    {
       if ( lpWndPos->flags & SWP_SHOWWINDOW )
          m_bKeepSize = TRUE;
    }
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void ZSizingControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
        CWnd::OnLButtonDown(nFlags, point);
}

void ZSizingControlBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_pDockBar != NULL)
    {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
        CWnd::OnLButtonDblClk(nFlags, point);
}

void ZSizingControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    UNUSED_ALWAYS(point);

    if (m_bTracking || IsFloating())
        return;

    if ((nHitTest >= HTSIZEFIRST) && (nHitTest <= HTSIZELAST))
        StartTracking(nHitTest, point); // sizing edge hit
}

void ZSizingControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    CControlBar::OnLButtonUp(nFlags, point);
}

void ZSizingControlBar::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (m_bTracking)
        StopTracking();

    CControlBar::OnRButtonDown(nFlags, point);
}

void ZSizingControlBar::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bTracking)
    {
        CPoint ptScreen = point;
        ClientToScreen(&ptScreen);

        OnTrackUpdateSize(ptScreen);
    }

    CControlBar::OnMouseMove(nFlags, point);
}

void ZSizingControlBar::OnCaptureChanged(CWnd *pWnd)
{
    if (m_bTracking && (pWnd != this))
        StopTracking();

    CControlBar::OnCaptureChanged(pWnd);
}

void ZSizingControlBar::OnNcCalcSize(BOOL bCalcValidRects,
                                     NCCALCSIZE_PARAMS FAR* lpncsp)
{
    UNUSED_ALWAYS(bCalcValidRects);

#ifndef _SCB_REPLACE_MINIFRAME
    // Enable diagonal resizing for floating miniframe
    if (IsFloating())
    {
        CFrameWnd* pFrame = GetParentFrame();
        if (pFrame != NULL &&
            pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
        {
            DWORD dwStyle = ::GetWindowLong(pFrame->m_hWnd, GWL_STYLE);
            if ((dwStyle & MFS_4THICKFRAME) != 0)
            {
                pFrame->ModifyStyle(MFS_4THICKFRAME, 0); // clear
                GetParent()->ModifyStyle(0, WS_CLIPCHILDREN);
            }
        }
    }
#endif _SCB_REPLACE_MINIFRAME

    // compute the the client area
    m_dwSCBStyle &= ~SCBS_EDGEALL;

    // add resizing edges between bars on the same row
    if (!IsFloating() && m_pDockBar != NULL)
    {
        ZSCBArray arrSCBars;
        int nThis;
        GetRowSizingBars(arrSCBars, nThis);

        BOOL bHorz = IsHorzDocked();
        if (nThis > 0)
            m_dwSCBStyle |= bHorz ? SCBS_EDGELEFT : SCBS_EDGETOP;

        if (nThis < arrSCBars.GetUpperBound())
            m_dwSCBStyle |= bHorz ? SCBS_EDGERIGHT : SCBS_EDGEBOTTOM;
    }

    NcCalcClient(&lpncsp->rgrc[0], m_nDockBarID);
}

void ZSizingControlBar::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    CRect rc(pRc);

    rc.DeflateRect(3, 5, 3, 3);
    if (nDockBarID != AFX_IDW_DOCKBAR_FLOAT)
        rc.DeflateRect(2, 0, 2, 2);

    switch (nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        m_dwSCBStyle |= SCBS_EDGEBOTTOM;
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        m_dwSCBStyle |= SCBS_EDGETOP;
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        m_dwSCBStyle |= SCBS_EDGERIGHT;
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        m_dwSCBStyle |= SCBS_EDGELEFT;
        break;
    }

    // make room for edges only if they will be painted
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rc.DeflateRect(
            (m_dwSCBStyle & SCBS_EDGELEFT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGETOP) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGERIGHT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGEBOTTOM) ? m_cxEdge : 0);

    *pRc = rc;
}

void ZSizingControlBar::OnNcPaint()
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this); // the HDC will be released by the destructor

    CRect rcClient, rcBar;
    GetClientRect(rcClient);
    ClientToScreen(rcClient);
    GetWindowRect(rcBar);
    rcClient.OffsetRect(-rcBar.TopLeft());
    rcBar.OffsetRect(-rcBar.TopLeft());

    CDC mdc;
    mdc.CreateCompatibleDC(&dc);

    CBitmap bm;
    bm.CreateCompatibleBitmap(&dc, rcBar.Width(), rcBar.Height());
    CBitmap* pOldBm = mdc.SelectObject(&bm);

    // draw borders in non-client area
    CRect rcDraw = rcBar;
    DrawBorders(&mdc, rcDraw);

    // erase the NC background
    mdc.FillRect(rcDraw, CBrush::FromHandle(
        (HBRUSH) GetClassLong(m_hWnd, GCL_HBRBACKGROUND)));

    if (m_dwSCBStyle & SCBS_SHOWEDGES)
    {
        CRect rcEdge; // paint the sizing edges
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                mdc.Draw3dRect(rcEdge, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                    ::GetSysColor(COLOR_BTNSHADOW));
    }

    NcPaintGripper(&mdc, rcClient);

    // client area is not our bussiness :)
    dc.IntersectClipRect(rcBar);
    dc.ExcludeClipRect(rcClient);

    dc.BitBlt(0, 0, rcBar.Width(), rcBar.Height(), &mdc, 0, 0, SRCCOPY);

    mdc.SelectObject(pOldBm);
    bm.DeleteObject();
    mdc.DeleteDC();
}

void ZSizingControlBar::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    UNUSED_ALWAYS(pDC);
    UNUSED_ALWAYS(rcClient);
}

void ZSizingControlBar::OnPaint()
{
    // overridden to skip border painting based on clientrect
    CPaintDC dc(this);
}

LRESULT ZSizingControlBar::OnNcHitTest(CPoint point)
{
    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);

    if (!IsFloating())
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                if (rcEdge.PtInRect(point))
                    return GetEdgeHTCode(i);

    return HTCLIENT;
}

void ZSizingControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CControlBar::OnSettingChange(uFlags, lpszSection);

    m_bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
        &m_bDragShowContent, 0); // update
}

void ZSizingControlBar::OnSize(UINT nType, int cx, int cy)
{
    UNUSED_ALWAYS(nType);

    if ((m_dwSCBStyle & SCBS_SIZECHILD) != 0)
    {
        // automatic child resizing - only one child is allowed
        CWnd* pWnd = GetWindow(GW_CHILD);
        if (pWnd != NULL)
        {
            pWnd->MoveWindow(0, 0, cx, cy);
            ASSERT(pWnd->GetWindow(GW_HWNDNEXT) == NULL);
        }
    }
}

void ZSizingControlBar::OnClose()
{
    // do nothing: protection against accidentally destruction by the
    //   child control (i.e. if user hits Esc in a child editctrl)
}

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar implementation helpers

void ZSizingControlBar::StartTracking(UINT nHitTest, CPoint point)
{
    SetCapture();

    // make sure no updates are pending
    if (!m_bDragShowContent)
        RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

    m_htEdge = nHitTest;
    m_bTracking = TRUE;

    BOOL bHorz = IsHorzDocked();
    BOOL bHorzTracking = m_htEdge == HTLEFT || m_htEdge == HTRIGHT;

    m_nTrackPosOld = bHorzTracking ? point.x : point.y;

    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);
    GetEdgeRect(rcBar, m_htEdge, rcEdge);
    m_nTrackEdgeOfs = m_nTrackPosOld -
        (bHorzTracking ? rcEdge.CenterPoint().x : rcEdge.CenterPoint().y);

    ZSCBArray arrSCBars;
    int nThis;
    GetRowSizingBars(arrSCBars, nThis);

    m_nTrackPosMin = m_nTrackPosMax = m_nTrackPosOld;
    if (!IsSideTracking())
    {
        // calc minwidth as the max minwidth of the sizing bars on row
        int nMinWidth = bHorz ? m_szMinHorz.cy : m_szMinVert.cx;
        for (int i = 0; i < arrSCBars.GetSize(); i++)
            nMinWidth = max(nMinWidth, bHorz ?
                arrSCBars[i]->m_szMinHorz.cy :
                arrSCBars[i]->m_szMinVert.cx);
        int nExcessWidth = (bHorz ? m_szHorz.cy : m_szVert.cx) - nMinWidth;

        // the control bar cannot grow with more than the width of
        // remaining client area of the mainframe
        CRect rcT;
        m_pDockSite->RepositionBars(0, 0xFFFF, AFX_IDW_PANE_FIRST,
            reposQuery, &rcT, NULL, TRUE);
        int nMaxWidth = bHorz ? rcT.Height() - 2 : rcT.Width() - 2;

        BOOL bTopOrLeft = m_htEdge == HTTOP || m_htEdge == HTLEFT;

        m_nTrackPosMin -= bTopOrLeft ? nMaxWidth : nExcessWidth;
        m_nTrackPosMax += bTopOrLeft ? nExcessWidth : nMaxWidth;
    }
    else
    {
        // side tracking:
        // max size is the actual size plus the amount the other
        // sizing bars can be decreased until they reach their minsize
        if (m_htEdge == HTBOTTOM || m_htEdge == HTRIGHT)
            nThis++;

        for (int i = 0; i < arrSCBars.GetSize(); i++)
        {
            ZSizingControlBar* pBar = arrSCBars[i];

            int nExcessWidth = bHorz ?
                pBar->m_szHorz.cx - pBar->m_szMinHorz.cx :
                pBar->m_szVert.cy - pBar->m_szMinVert.cy;

            if (i < nThis)
                m_nTrackPosMin -= nExcessWidth;
            else
                m_nTrackPosMax += nExcessWidth;
        }
    }

    OnTrackInvertTracker(); // draw tracker
}

void ZSizingControlBar::StopTracking()
{
    OnTrackInvertTracker(); // erase tracker

    m_bTracking = FALSE;
    ReleaseCapture();

    m_pDockSite->DelayRecalcLayout();
}

void ZSizingControlBar::OnTrackUpdateSize(CPoint& point)
{
    ASSERT(!IsFloating());

    BOOL bHorzTrack = m_htEdge == HTLEFT || m_htEdge == HTRIGHT;

    int nTrackPos = bHorzTrack ? point.x : point.y;
    nTrackPos = max(m_nTrackPosMin, min(m_nTrackPosMax, nTrackPos));

    int nDelta = nTrackPos - m_nTrackPosOld;

    if (nDelta == 0)
        return; // no pos change

    OnTrackInvertTracker(); // erase tracker

    m_nTrackPosOld = nTrackPos;

    BOOL bHorz = IsHorzDocked();

    CSize sizeNew = bHorz ? m_szHorz : m_szVert;
    switch (m_htEdge)
    {
    case HTLEFT:    sizeNew -= CSize(nDelta, 0); break;
    case HTTOP:     sizeNew -= CSize(0, nDelta); break;
    case HTRIGHT:   sizeNew += CSize(nDelta, 0); break;
    case HTBOTTOM:  sizeNew += CSize(0, nDelta); break;
    }

    ZSCBArray arrSCBars;
    int nThis;
    GetRowSizingBars(arrSCBars, nThis);

    if (!IsSideTracking())
        for (int i = 0; i < arrSCBars.GetSize(); i++)
        {
            ZSizingControlBar* pBar = arrSCBars[i];
            // make same width (or height)
            (bHorz ? pBar->m_szHorz.cy : pBar->m_szVert.cx) =
                bHorz ? sizeNew.cy : sizeNew.cx;
        }
    else
    {
        int nGrowingBar = nThis;
        BOOL bBefore = m_htEdge == HTTOP || m_htEdge == HTLEFT;
        if (bBefore && nDelta > 0)
            nGrowingBar--;
        if (!bBefore && nDelta < 0)
            nGrowingBar++;
        if (nGrowingBar != nThis)
            bBefore = !bBefore;

        // nGrowing is growing
        nDelta = abs(nDelta);
        ZSizingControlBar* pBar = arrSCBars[nGrowingBar];
        (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) += nDelta;

        // the others are shrinking
        int nFirst = bBefore ? nGrowingBar - 1 : nGrowingBar + 1;
        int nLimit = bBefore ? -1 : arrSCBars.GetSize();

        for (int i = nFirst; nDelta != 0 && i != nLimit; i += (bBefore ? -1 : 1))
        {
            ZSizingControlBar* pBar = arrSCBars[i];

            int nDeltaT = min(nDelta,
                (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) -
                (bHorz ? pBar->m_szMinHorz.cx : pBar->m_szMinVert.cy));

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) -= nDeltaT;
            nDelta -= nDeltaT;
        }
    }

    OnTrackInvertTracker(); // redraw tracker at new pos

    if (m_bDragShowContent)
        m_pDockSite->DelayRecalcLayout();
}

void ZSizingControlBar::OnTrackInvertTracker()
{
    ASSERT(m_bTracking);

    if (m_bDragShowContent)
        return; // don't show tracker if DragFullWindows is on

    BOOL bHorz = IsHorzDocked();
    CRect rc, rcBar, rcDock, rcFrame;
    GetWindowRect(rcBar);
    m_pDockBar->GetWindowRect(rcDock);
    m_pDockSite->GetWindowRect(rcFrame);
    VERIFY(GetEdgeRect(rcBar, m_htEdge, rc));
    if (!IsSideTracking())
        rc = bHorz ?
            CRect(rcDock.left + 1, rc.top, rcDock.right - 1, rc.bottom) :
            CRect(rc.left, rcDock.top + 1, rc.right, rcDock.bottom - 1);

    BOOL bHorzTracking = m_htEdge == HTLEFT || m_htEdge == HTRIGHT;
    int nOfs = m_nTrackPosOld - m_nTrackEdgeOfs;
    nOfs -= bHorzTracking ? rc.CenterPoint().x : rc.CenterPoint().y;
    rc.OffsetRect(bHorzTracking ? nOfs : 0, bHorzTracking ? 0 : nOfs);
    rc.OffsetRect(-rcFrame.TopLeft());

    CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    CBrush* pBrush = CDC::GetHalftoneBrush();
    CBrush* pBrushOld = pDC->SelectObject(pBrush);

    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATINVERT);

    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

BOOL ZSizingControlBar::GetEdgeRect(CRect rcWnd, UINT nHitTest,
                                    CRect& rcEdge)
{
    rcEdge = rcWnd;
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rcEdge.DeflateRect(1, 1);
    BOOL bHorz = IsHorzDocked();

    switch (nHitTest)
    {
    case HTLEFT:
        if (!(m_dwSCBStyle & SCBS_EDGELEFT)) return FALSE;
        rcEdge.right = rcEdge.left + m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTTOP:
        if (!(m_dwSCBStyle & SCBS_EDGETOP)) return FALSE;
        rcEdge.bottom = rcEdge.top + m_cxEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cxEdge, 0);
        break;
    case HTRIGHT:
        if (!(m_dwSCBStyle & SCBS_EDGERIGHT)) return FALSE;
        rcEdge.left = rcEdge.right - m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTBOTTOM:
        if (!(m_dwSCBStyle & SCBS_EDGEBOTTOM)) return FALSE;
        rcEdge.top = rcEdge.bottom - m_cxEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cxEdge, 0);
        break;
    default:
        ASSERT(FALSE); // invalid hit test code
    }
    return TRUE;
}

UINT ZSizingControlBar::GetEdgeHTCode(int nEdge)
{
    if (nEdge == 0) return HTLEFT;
    if (nEdge == 1) return HTTOP;
    if (nEdge == 2) return HTRIGHT;
    if (nEdge == 3) return HTBOTTOM;
    ASSERT(FALSE); // invalid edge code
    return HTNOWHERE;
}

void ZSizingControlBar::GetRowInfo(int& nFirst, int& nLast, int& nThis)
{
    ASSERT_VALID(m_pDockBar); // verify bounds

    nThis = m_pDockBar->FindBar(this);
    ASSERT(nThis != -1);

    int i, nBars = m_pDockBar->m_arrBars.GetSize();

    // find the first and the last bar in row
    for (nFirst = -1, i = nThis - 1; i >= 0 && nFirst == -1; i--)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nFirst = i + 1;
    for (nLast = -1, i = nThis + 1; i < nBars && nLast == -1; i++)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nLast = i - 1;

    ASSERT((nLast != -1) && (nFirst != -1));
}

void ZSizingControlBar::GetRowSizingBars(ZSCBArray& arrSCBars)
{
    int nThis; // dummy
    GetRowSizingBars(arrSCBars, nThis);
}

void ZSizingControlBar::GetRowSizingBars(ZSCBArray& arrSCBars, int& nThis)
{
    arrSCBars.RemoveAll();

    int nFirstT, nLastT, nThisT;
    GetRowInfo(nFirstT, nLastT, nThisT);

    nThis = -1;
    for (int i = nFirstT; i <= nLastT; i++)
    {
        ZSizingControlBar* pBar =
            (ZSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        if (pBar->IsKindOf(RUNTIME_CLASS(ZSizingControlBar)))
        {
            if (pBar == this)
                nThis = arrSCBars.GetSize();

            arrSCBars.Add(pBar);
        }
    }
}

BOOL ZSizingControlBar::NegotiateSpace(int nLengthTotal, BOOL bHorz)
{
    ASSERT(bHorz == IsHorzDocked());

    int k;
    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    int nLengthAvail = nLengthTotal;
    int nLengthActual = 0;
    int nLengthMin = 2;
    int nWidthMax = 0;
    ZSizingControlBar* pBar;

    for (int i = nFirst; i <= nLast; i++)
    {
        pBar = (ZSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        BOOL bIsSizingBar =
            pBar->IsKindOf(RUNTIME_CLASS(ZSizingControlBar));

        int nLengthBar; // minimum length of the bar
        if (bIsSizingBar)
            nLengthBar = bHorz ? pBar->m_szMinHorz.cx - 2 :
                pBar->m_szMinVert.cy - 2;
        else
        {
            CRect rcBar;
            pBar->GetWindowRect(&rcBar);
            nLengthBar = bHorz ? rcBar.Width() - 2 : rcBar.Height() - 2;
        }

        nLengthMin += nLengthBar;
        if (nLengthMin > nLengthTotal)
        {
            // split the row after fixed bar
            if (i < nThis)
            {
                m_pDockBar->m_arrBars.InsertAt(i + 1,
                    (CControlBar*) NULL);
                return FALSE;
            }

            // only this sizebar remains on the row, adjust it to minsize
            if (i == nThis)
            {
                if (bHorz)
                    m_szHorz.cx = m_szMinHorz.cx;
                else
                    m_szVert.cy = m_szMinVert.cy;

                return TRUE; // the dockbar will split the row for us
            }

            // we have enough bars - go negotiate with them
            m_pDockBar->m_arrBars.InsertAt(i, (CControlBar*) NULL);
            nLast = i - 1;
            break;
        }

        if (bIsSizingBar)
        {
            nLengthActual += bHorz ? pBar->m_szHorz.cx - 2 :
                pBar->m_szVert.cy - 2;
            nWidthMax = max(nWidthMax, bHorz ? pBar->m_szHorz.cy :
                pBar->m_szVert.cx);
        }
        else
            nLengthAvail -= nLengthBar;
    }

    ZSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    int nNumBars = arrSCBars.GetSize();
    int nDelta = nLengthAvail - nLengthActual;

    // return faster when there is only one sizing bar per row (this one)
    if (nNumBars == 1)
    {
        ASSERT(arrSCBars[0] == this);

        if (nDelta == 0)
            return TRUE;

        m_bKeepSize = FALSE;
        (bHorz ? m_szHorz.cx : m_szVert.cy) += nDelta;

        return TRUE;
    }

    // make all the bars the same width
    for ( k = 0; k < nNumBars; k++)
        if (bHorz)
            arrSCBars[k]->m_szHorz.cy = nWidthMax;
        else
            arrSCBars[k]->m_szVert.cx = nWidthMax;

    // distribute the difference between the bars,
    // but don't shrink them below their minsizes
    while (nDelta != 0)
    {
        int nDeltaOld = nDelta;
        for ( k = 0; k < nNumBars; k++)
        {
            pBar = arrSCBars[k];
            int nLMin = bHorz ?
                pBar->m_szMinHorz.cx : pBar->m_szMinVert.cy;
            int nL = bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy;

            if ((nL == nLMin) && (nDelta < 0) || // already at min length
                pBar->m_bKeepSize) // or wants to keep its size
                continue;

            // sign of nDelta
            int nDelta2 = (nDelta == 0) ? 0 : ((nDelta < 0) ? -1 : 1);

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) += nDelta2;
            nDelta -= nDelta2;
            if (nDelta == 0) break;
        }

        // clear m_bKeepSize flags
        if ((nDeltaOld == nDelta) || (nDelta == 0))
            for ( k = 0; k < nNumBars; k++)
                arrSCBars[k]->m_bKeepSize = FALSE;
    }

    return TRUE;
}

void ZSizingControlBar::AlignControlBars()
{
    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    BOOL bHorz = IsHorzDocked();
    BOOL bNeedRecalc = FALSE;
    int nAlign = bHorz ? -2 : 0;

    CRect rc, rcDock;
    m_pDockBar->GetWindowRect(&rcDock);

    for (int i = nFirst; i <= nLast; i++)
    {
        ZSizingControlBar* pBar =
            (ZSizingControlBar*) m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;

        pBar->GetWindowRect(&rc);
        rc.OffsetRect(-rcDock.TopLeft());

        if (pBar->IsKindOf(RUNTIME_CLASS(ZSizingControlBar)))
            rc = CRect(rc.TopLeft(),
                bHorz ? pBar->m_szHorz : pBar->m_szVert);

        if ((bHorz ? rc.left : rc.top) != nAlign)
        {
            if (!bHorz)
                rc.OffsetRect(0, nAlign - rc.top - 2);
            else if (m_nDockBarID == AFX_IDW_DOCKBAR_TOP)
                rc.OffsetRect(nAlign - rc.left, -2);
            else
                rc.OffsetRect(nAlign - rc.left, 0);
            pBar->MoveWindow(rc);
            bNeedRecalc = TRUE;
        }
        nAlign += (bHorz ? rc.Width() : rc.Height()) - 2;
    }

    if (bNeedRecalc)
        m_pDockSite->DelayRecalcLayout();
}

void ZSizingControlBar::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);
}

void ZSizingControlBar::LoadState(LPCTSTR lpszProfileName)
{
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd()); // must be called after Create()

#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
    // compensate the caption miscalculation in CFrameWnd::SetDockState()
    CDockState state;
    state.LoadState(lpszProfileName);

    UINT nID = GetDlgCtrlID();
    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        if (!pInfo->m_bFloating)
            continue;

        // this is a floating dockbar - check the ID array
        for (int j = 0; j < pInfo->m_arrBarID.GetSize(); j++)
            if ((DWORD) pInfo->m_arrBarID[j] == nID)
            {
                // found this bar - offset origin and save settings
                pInfo->m_pointPos.x++;
                pInfo->m_pointPos.y +=
                    ::GetSystemMetrics(SM_CYSMCAPTION) + 1;
                pInfo->SaveState(lpszProfileName, i);
            }
    }
#endif //_SCB_REPLACE_MINIFRAME && !_SCB_MINIFRAME_CAPTION

    CWinApp* pApp = AfxGetApp();

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-SCBar-%d"), lpszProfileName,
        GetDlgCtrlID());

    m_szHorz.cx = max(m_szMinHorz.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeHorzCX"), m_szHorz.cx));
    m_szHorz.cy = max(m_szMinHorz.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeHorzCY"), m_szHorz.cy));

    m_szVert.cx = max(m_szMinVert.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeVertCX"), m_szVert.cx));
    m_szVert.cy = max(m_szMinVert.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeVertCY"), m_szVert.cy));

    m_szFloat.cx = max(m_szMinFloat.cx, (int) pApp->GetProfileInt(
        szSection, _T("sizeFloatCX"), m_szFloat.cx));
    m_szFloat.cy = max(m_szMinFloat.cy, (int) pApp->GetProfileInt(
        szSection, _T("sizeFloatCY"), m_szFloat.cy));
}

void ZSizingControlBar::SaveState(LPCTSTR lpszProfileName)
{
    // place your SaveState or GlobalSaveState call in
    // CMainFrame's OnClose() or DestroyWindow(), not in OnDestroy()
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd());

    CWinApp* pApp = AfxGetApp();

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-SCBar-%d"), lpszProfileName,
        GetDlgCtrlID());

    pApp->WriteProfileInt(szSection, _T("sizeHorzCX"), m_szHorz.cx);
    pApp->WriteProfileInt(szSection, _T("sizeHorzCY"), m_szHorz.cy);

    pApp->WriteProfileInt(szSection, _T("sizeVertCX"), m_szVert.cx);
    pApp->WriteProfileInt(szSection, _T("sizeVertCY"), m_szVert.cy);

    pApp->WriteProfileInt(szSection, _T("sizeFloatCX"), m_szFloat.cx);
    pApp->WriteProfileInt(szSection, _T("sizeFloatCY"), m_szFloat.cy);
}

void ZSizingControlBar::GlobalLoadState(CFrameWnd* pFrame,
                                        LPCTSTR lpszProfileName)
{
    POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        ZSizingControlBar* pBar =
            (ZSizingControlBar*) pFrame->m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
        if (pBar->IsKindOf(RUNTIME_CLASS(ZSizingControlBar)))
            pBar->LoadState(lpszProfileName);
    }
}

void ZSizingControlBar::GlobalSaveState(CFrameWnd* pFrame,
                                        LPCTSTR lpszProfileName)
{
    POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        ZSizingControlBar* pBar =
            (ZSizingControlBar*) pFrame->m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
        if (pBar->IsKindOf(RUNTIME_CLASS(ZSizingControlBar)))
            pBar->SaveState(lpszProfileName);
    }
}

#ifdef _SCB_REPLACE_MINIFRAME
#ifndef _SCB_MINIFRAME_CAPTION
/////////////////////////////////////////////////////////////////////////////
// ZSCBDockContext Drag Operations

static void AdjustRectangle(CRect& rect, CPoint pt)
{
    int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
                    (pt.x > rect.right) ? (pt.x - rect.right) : 0;
    int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
                    (pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
    rect.OffsetRect(nXOffset, nYOffset);
}

void ZSCBDockContext::StartDrag(CPoint pt)
{
    ASSERT_VALID(m_pBar);
    m_bDragging = TRUE;

    InitLoop();

    ASSERT((m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) != 0);

    // get true bar size (including borders)
    CRect rect;
    m_pBar->GetWindowRect(rect);
    m_ptLast = pt;
    CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
    CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
    CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

    m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
    m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

    // calculate frame dragging rectangle
    m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);

#ifdef _MAC
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz,
        WS_THICKFRAME, WS_EX_FORCESIZEBOX);
#else
    CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz, WS_THICKFRAME);
#endif
    m_rectFrameDragHorz.DeflateRect(2, 2);
    m_rectFrameDragVert = m_rectFrameDragHorz;

    // adjust rectangles so that point is inside
    AdjustRectangle(m_rectDragHorz, pt);
    AdjustRectangle(m_rectDragVert, pt);
    AdjustRectangle(m_rectFrameDragHorz, pt);
    AdjustRectangle(m_rectFrameDragVert, pt);

    // initialize tracking state and enter tracking loop
    m_dwOverDockStyle = CanDock();
    Move(pt);   // call it here to handle special keys
    Track();
}
#endif //_SCB_MINIFRAME_CAPTION

/////////////////////////////////////////////////////////////////////////////
// ZSCBMiniDockFrameWnd

IMPLEMENT_DYNCREATE(ZSCBMiniDockFrameWnd, baseZSCBMiniDockFrameWnd);

BEGIN_MESSAGE_MAP(ZSCBMiniDockFrameWnd, baseZSCBMiniDockFrameWnd)
    //{{AFX_MSG_MAP(ZSCBMiniDockFrameWnd)
    ON_WM_NCLBUTTONDOWN()
    ON_WM_GETMINMAXINFO()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL ZSCBMiniDockFrameWnd::Create(CWnd* pParent, DWORD dwBarStyle)
{
    // set m_bInRecalcLayout to avoid flashing during creation
    // RecalcLayout will be called once something is docked
    m_bInRecalcLayout = TRUE;

    DWORD dwStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|MFS_MOVEFRAME|
        MFS_4THICKFRAME|MFS_SYNCACTIVE|MFS_BLOCKSYSMENU|
        FWS_SNAPTOBARS;

    if (dwBarStyle & CBRS_SIZE_DYNAMIC)
        dwStyle &= ~MFS_MOVEFRAME;

    DWORD dwExStyle = 0;
#ifdef _MAC
    if (dwBarStyle & CBRS_SIZE_DYNAMIC)
        dwExStyle |= WS_EX_FORCESIZEBOX;
    else
        dwStyle &= ~(MFS_MOVEFRAME|MFS_4THICKFRAME);
#endif

    if (!CMiniFrameWnd::CreateEx(dwExStyle,
        NULL, _T(""), dwStyle, rectDefault, pParent))
    {
        m_bInRecalcLayout = FALSE;
        return FALSE;
    }
    dwStyle = dwBarStyle & (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT) ?
        CBRS_ALIGN_LEFT : CBRS_ALIGN_TOP;
    dwStyle |= dwBarStyle & CBRS_FLOAT_MULTI;
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    //pSysMenu->DeleteMenu(SC_SIZE, MF_BYCOMMAND);
    CString strHide;
    if (strHide.LoadString(AFX_IDS_HIDE))
    {
        pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
        pSysMenu->AppendMenu(MF_STRING|MF_ENABLED, SC_CLOSE, strHide);
    }

    // must initially create with parent frame as parent
    if (!m_wndDockBar.Create(pParent, WS_CHILD | WS_VISIBLE | dwStyle,
        AFX_IDW_DOCKBAR_FLOAT))
    {
        m_bInRecalcLayout = FALSE;
        return FALSE;
    }

    // set parent to CMiniDockFrameWnd
    m_wndDockBar.SetParent(this);
    m_bInRecalcLayout = FALSE;

    return TRUE;
}

void ZSCBMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    if (nHitTest == HTCAPTION || nHitTest == HTCLOSE)
    {
        baseZSCBMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
        return;
    }

    if (GetSizingControlBar() != NULL)
        CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
    else
        baseZSCBMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
}

ZSizingControlBar* ZSCBMiniDockFrameWnd::GetSizingControlBar()
{
    CWnd* pWnd = GetWindow(GW_CHILD); // get the dockbar
    if (pWnd == NULL)
        return NULL;

    pWnd = pWnd->GetWindow(GW_CHILD); // get the controlbar
    if (pWnd == NULL)
        return NULL;

    if (!pWnd->IsKindOf(RUNTIME_CLASS(ZSizingControlBar)))
        return NULL;

    return (ZSizingControlBar*) pWnd;
}

void ZSCBMiniDockFrameWnd::OnSize(UINT nType, int cx, int cy)
{
    ZSizingControlBar* pBar = GetSizingControlBar();
    if ((pBar != NULL) && (GetStyle() & MFS_4THICKFRAME) == 0
        && pBar->IsVisible() &&
        cx + 4 >= pBar->m_szMinFloat.cx &&
        cy + 4 >= pBar->m_szMinFloat.cy)
        pBar->m_szFloat = CSize(cx + 4, cy + 4);

    baseZSCBMiniDockFrameWnd::OnSize(nType, cx, cy);
}

void ZSCBMiniDockFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    baseZSCBMiniDockFrameWnd::OnGetMinMaxInfo(lpMMI);

    ZSizingControlBar* pBar = GetSizingControlBar();
    if (pBar != NULL)
    {
        CRect r(CPoint(0, 0), pBar->m_szMinFloat - CSize(4, 4));
#ifndef _SCB_MINIFRAME_CAPTION
        CMiniFrameWnd::CalcBorders(&r, WS_THICKFRAME);
#else
        CMiniFrameWnd::CalcBorders(&r, WS_THICKFRAME|WS_CAPTION);
#endif //_SCB_MINIFRAME_CAPTION
        lpMMI->ptMinTrackSize.x = r.Width();
        lpMMI->ptMinTrackSize.y = r.Height();
    }
}

void ZSCBMiniDockFrameWnd::OnWindowPosChanging(WINDOWPOS FAR* lpWndPos)
{
    if ((GetStyle() & MFS_4THICKFRAME) != 0)
    {
        ZSizingControlBar *pBar = GetSizingControlBar( );
        if ( pBar )
        {
            lpWndPos->flags |= SWP_NOSIZE; // don't size this time
            // prevents flicker
            pBar->m_pDockBar->ModifyStyle(0, WS_CLIPCHILDREN);

            // enable diagonal resizing
            DWORD dwStyleRemove = MFS_4THICKFRAME;
#ifndef _SCB_MINIFRAME_CAPTION
            // remove caption
            dwStyleRemove |= WS_SYSMENU|WS_CAPTION;
#endif
            ModifyStyle(dwStyleRemove, 0);

            DelayRecalcLayout();
            pBar->PostMessage(WM_NCPAINT);
        }
    }

    CMiniFrameWnd::OnWindowPosChanging(lpWndPos);
}

#endif //_SCB_REPLACE_MINIFRAME

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// ZSizingControlBarG           Version 2.44
//
// Created: Jan 24, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

// sizecbar.cpp : implementation file
//

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBarG

IMPLEMENT_DYNAMIC(ZSizingControlBarG, ZSizingControlBar);

ZSizingControlBarG::ZSizingControlBarG()
{
    m_cyGripper = 12;
}

ZSizingControlBarG::~ZSizingControlBarG()
{
}

BEGIN_MESSAGE_MAP(ZSizingControlBarG, ZSizingControlBar)
   //{{AFX_MSG_MAP(ZSizingControlBarG)
   ON_WM_NCLBUTTONUP()
   ON_WM_NCHITTEST()
   ON_WM_WINDOWPOSCHANGED( )
   //}}AFX_MSG_MAP
   ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBarG message handlers

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//

void
ZSizingControlBarG::OnWindowPosChanged( WINDOWPOS *lpWndPos )
{
// TraceLineS( "ZSizingControlBarG::OnWindowPosChanged", "" );

   // Control Bar notification needs to be added
   ZSizingControlBar::OnWindowPosChanged( lpWndPos );
}

void ZSizingControlBarG::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    if (nHitTest == HTCLOSE)
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide

    ZSizingControlBar::OnNcLButtonUp(nHitTest, point);
}

void ZSizingControlBarG::NcCalcClient(LPRECT pRc, UINT nDockBarID)
{
    CRect rcBar(pRc); // save the bar rect

    // subtract edges
    ZSizingControlBar::NcCalcClient(pRc, nDockBarID);

    if (!HasGripper())
        return;

    CRect rc(pRc); // the client rect as calculated by the base class

    BOOL bHorz = (nDockBarID == AFX_IDW_DOCKBAR_TOP) ||
                 (nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);

    if (bHorz)
        rc.DeflateRect(m_cyGripper, 0, 0, 0);
    else
        rc.DeflateRect(0, m_cyGripper, 0, 0);

    // set position for the "x" (hide bar) button
    CPoint ptOrgBtn;
    if (bHorz)
        ptOrgBtn = CPoint(rc.left - 13, rc.top);
    else
        ptOrgBtn = CPoint(rc.right - 12, rc.top - 13);

    m_biHide.Move(ptOrgBtn - rcBar.TopLeft());

    *pRc = rc;
}

void ZSizingControlBarG::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if (!HasGripper())
        return;

    // paints a simple "two raised lines" gripper
    // override this if you want a more sophisticated gripper
    CRect gripper = rcClient;
    CRect rcbtn = m_biHide.GetRect();
    BOOL bHorz = IsHorzDocked();

    gripper.DeflateRect(1, 1);
    if (bHorz)
    {   // gripper at left
        gripper.left -= m_cyGripper;
        gripper.right = gripper.left + 3;
        gripper.top = rcbtn.bottom + 3;
    }
    else
    {   // gripper at top
        gripper.top -= m_cyGripper;
        gripper.bottom = gripper.top + 3;
        gripper.right = rcbtn.left - 3;
    }

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    gripper.OffsetRect(bHorz ? 3 : 0, bHorz ? 0 : 3);

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    m_biHide.Paint(pDC);
}

LRESULT ZSizingControlBarG::OnNcHitTest(CPoint point)
{
    CRect rcBar;
    GetWindowRect(rcBar);

    UINT nRet = ZSizingControlBar::OnNcHitTest(point);
    if (nRet != HTCLIENT)
        return nRet;

    CRect rc = m_biHide.GetRect();
    rc.OffsetRect(rcBar.TopLeft());
    if (rc.PtInRect(point))
        return HTCLOSE;

    return HTCLIENT;
}

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBarG implementation helpers

void ZSizingControlBarG::OnUpdateCmdUI(CFrameWnd* pTarget,
                                      BOOL bDisableIfNoHndler)
{
    UNUSED_ALWAYS(bDisableIfNoHndler);
    UNUSED_ALWAYS(pTarget);

    if (!HasGripper())
        return;

    BOOL bNeedPaint = FALSE;

    CPoint pt;
    ::GetCursorPos(&pt);
    BOOL bHit = (OnNcHitTest(pt) == HTCLOSE);
    BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

    BOOL bWasPushed = m_biHide.bPushed;
    m_biHide.bPushed = bHit && bLButtonDown;

    BOOL bWasRaised = m_biHide.bRaised;
    m_biHide.bRaised = bHit && !bLButtonDown;

    bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) ||
                  (m_biHide.bRaised ^ bWasRaised);

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

/////////////////////////////////////////////////////////////////////////
// ZSCBButton

ZSCBButton::ZSCBButton()
{
    bRaised = FALSE;
    bPushed = FALSE;
}

void ZSCBButton::Paint(CDC* pDC)
{
    CRect rc = GetRect();

    if (bPushed)
        pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW),
            ::GetSysColor(COLOR_BTNHIGHLIGHT));
    else
        if (bRaised)
            pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                ::GetSysColor(COLOR_BTNSHADOW));

    COLORREF clrOldTextColor = pDC->GetTextColor();
    pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
    font.CreatePointFont(pointsize, _T("Marlett"));
    CFont* oldfont = pDC->SelectObject(&font);

    pDC->TextOut(ptOrg.x + 2, ptOrg.y + 2, CString(_T("r"))); // x-like

    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
}

BOOL ZSizingControlBarG::HasGripper() const
{
#if defined(_SCB_MINIFRAME_CAPTION) || !defined(_SCB_REPLACE_MINIFRAME)
    // if the miniframe has a caption, don't display the gripper
    if (IsFloating())
        return FALSE;
#endif //_SCB_MINIFRAME_CAPTION

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// ZSizingControlBarCF          Version 2.44
//
// Created: Dec 21, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBarCF

IMPLEMENT_DYNAMIC(ZSizingControlBarCF, ZSizingControlBarG);

int CALLBACK EnumFontFamProc(ENUMLOGFONT FAR *lpelf,
                             NEWTEXTMETRIC FAR *lpntm,
                             int FontType,
                             LPARAM lParam)
{
    UNUSED_ALWAYS(lpelf);
    UNUSED_ALWAYS(lpntm);
    UNUSED_ALWAYS(FontType);
    UNUSED_ALWAYS(lParam);

    return( 0 );
}

ZSizingControlBarCF::ZSizingControlBarCF()
{
    m_bActive = FALSE;

    CDC dc;
    dc.CreateCompatibleDC(NULL);

    m_csFontFace = (::EnumFontFamilies(dc.m_hDC,
        _T("Tahoma"), (FONTENUMPROC) EnumFontFamProc, 0) == 0) ?
        _T("Tahoma") : _T("Arial");

    dc.DeleteDC();

}

BEGIN_MESSAGE_MAP(ZSizingControlBarCF, ZSizingControlBarG)
   //{{AFX_MSG_MAP(ZSizingControlBarCF)
   //}}AFX_MSG_MAP
   ON_WM_WINDOWPOSCHANGED( )
   ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

void
ZSizingControlBarCF::OnWindowPosChanged( WINDOWPOS *lpWndPos )
{
// TraceLineS( "ZSizingControlBarCF::OnWindowPosChanged", "" );

   // Control Bar notification needs to be added
   ZSizingControlBarG::OnWindowPosChanged( lpWndPos );
}

void ZSizingControlBarCF::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    ZSizingControlBarG::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

    if (!HasGripper())
        return;

    BOOL bNeedPaint = FALSE;

    CWnd* pFocus = GetFocus();
    BOOL bActiveOld = m_bActive;

    m_bActive = (pFocus->GetSafeHwnd() && IsChild(pFocus));

    if (m_bActive != bActiveOld)
        bNeedPaint = TRUE;

    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

// gradient defines (if not already defined)
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION     27
#define COLOR_GRADIENTINACTIVECAPTION   28
#define SPI_GETGRADIENTCAPTIONS         0x1008
#endif

void ZSizingControlBarCF::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if (!HasGripper())
        return;

    // compute the caption rectangle
    BOOL bHorz = IsHorzDocked();
    CRect rcGrip = rcClient;
    CRect rcBtn = m_biHide.GetRect();
    if (bHorz)
    {   // right side gripper
        rcGrip.left -= m_cyGripper + 1;
        rcGrip.right = rcGrip.left + 11;
        rcGrip.top = rcBtn.bottom + 3;
    }
    else
    {   // gripper at top
        rcGrip.top -= m_cyGripper + 1;
        rcGrip.bottom = rcGrip.top + 11;
        rcGrip.right = rcBtn.left - 3;
    }
    rcGrip.InflateRect(bHorz ? 1 : 0, bHorz ? 0 : 1);

    // draw the caption background
    //CBrush br;
    COLORREF clrCptn = m_bActive ?
        ::GetSysColor(COLOR_ACTIVECAPTION) :
        ::GetSysColor(COLOR_INACTIVECAPTION);

    // query gradient info (usually TRUE for Win98/Win2k)
    BOOL bGradient = FALSE;
    ::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS, 0, &bGradient, 0);

    if (!bGradient)
        pDC->FillSolidRect(&rcGrip, clrCptn); // solid color
    else
    {
        // gradient from left to right or from bottom to top
        // get second gradient color (the right end)
        COLORREF clrCptnRight = m_bActive ?
            ::GetSysColor(COLOR_GRADIENTACTIVECAPTION) :
            ::GetSysColor(COLOR_GRADIENTINACTIVECAPTION);

        // this will make 2^6 = 64 fountain steps
        int nShift = 6;
        int nSteps = 1 << nShift;

        for (int i = 0; i < nSteps; i++)
        {
            // do a little alpha blending
            int nR = (GetRValue(clrCptn) * (nSteps - i) +
                      GetRValue(clrCptnRight) * i) >> nShift;
            int nG = (GetGValue(clrCptn) * (nSteps - i) +
                      GetGValue(clrCptnRight) * i) >> nShift;
            int nB = (GetBValue(clrCptn) * (nSteps - i) +
                      GetBValue(clrCptnRight) * i) >> nShift;

            COLORREF cr = RGB(nR, nG, nB);

            // then paint with the resulting color
            CRect r2 = rcGrip;
            if (bHorz)
            {
                r2.bottom = rcGrip.bottom -
                    ((i * rcGrip.Height()) >> nShift);
                r2.top = rcGrip.bottom -
                    (((i + 1) * rcGrip.Height()) >> nShift);
                if (r2.Height() > 0)
                    pDC->FillSolidRect(r2, cr);
            }
            else
            {
                r2.left = rcGrip.left +
                    ((i * rcGrip.Width()) >> nShift);
                r2.right = rcGrip.left +
                    (((i + 1) * rcGrip.Width()) >> nShift);
                if (r2.Width() > 0)
                    pDC->FillSolidRect(r2, cr);
            }
        }
    }

    // draw the caption text - first select a font
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(85, 96, ppi); // 8.5 points at 96 ppi

    LOGFONT lf;
    BOOL bFont = font.CreatePointFont(pointsize, m_csFontFace);
    if (bFont)
    {
        // get the text color
        COLORREF clrCptnText = m_bActive ?
            ::GetSysColor(COLOR_CAPTIONTEXT) :
            ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);

        int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
        COLORREF clrOldText = pDC->SetTextColor(clrCptnText);

        if (bHorz)
        {
            // rotate text 90 degrees CCW if horizontally docked
            font.GetLogFont(&lf);
            font.DeleteObject();
            lf.lfEscapement = 900;
            font.CreateFontIndirect(&lf);
        }

        CFont* pOldFont = pDC->SelectObject(&font);
        CString sTitle;
        GetWindowText(sTitle);

        CPoint ptOrg = bHorz ?
            CPoint(rcGrip.left - 1, rcGrip.bottom - 3) :
            CPoint(rcGrip.left + 3, rcGrip.top - 1);

        pDC->ExtTextOut(ptOrg.x, ptOrg.y,
            ETO_CLIPPED, rcGrip, sTitle, NULL);

        pDC->SelectObject(pOldFont);
        pDC->SetBkMode(nOldBkMode);
        pDC->SetTextColor(clrOldText);
    }

    // draw the button
    m_biHide.Paint(pDC);
}

LRESULT ZSizingControlBarCF::OnSetText(WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = ZSizingControlBarG::OnSetText(wParam, lParam);

    SendMessage(WM_NCPAINT);

    return lResult;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zCONTROLBAR_GRIPPERS          zCONTROL_RESERVED_UNUSABLE
#define zCONTROLBAR_LEFT              zCONTROL_FULL_CLIENT
#define zCONTROLBAR_BOTTOM            zCONTROL_CENTERJUSTIFY
#define zCONTROLBAR_RIGHT             zCONTROL_RIGHTJUSTIFY
#define zCONTROLBAR_TOP               zCONTROL_SPECIFIC_1
#define zCONTROLBAR_XX2               zCONTROL_SPECIFIC_2
#define zCONTROLBAR_XX3               zCONTROL_SPECIFIC_3

IMPLEMENT_DYNAMIC( ZControlBarG, ZSizingControlBarG )
BEGIN_MESSAGE_MAP( ZControlBarG, ZSizingControlBarG )
   //{{AFX_MSG_MAP( ZControlBarG )
   ON_WM_CREATE( )
   ON_WM_SETFOCUS( )
   ON_WM_SIZE( )
   ON_WM_WINDOWPOSCHANGED( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZControlBarG
//
// The child window ID of a control bar is important.  The default layout of
// control bar only works for control bars with IDs in the range of
// AFX_IDW_CONTROLBAR_FIRST to AFX_IDW_CONTROLBAR_LAST.  Note that even
// though there is a range of 256 control bar IDs, the first 32 of these
// control bar IDs are special since they are directly supported by the print
// preview architecture.
#define ID_VIEW_MYTOOLBAR               32771
#define ID_VIEW_1                       32772
#define ID_VIEW_2                       32773
#define ID_VIEW_3                       32774
#define ID_VIEW_4                       32775
#define ID_VIEW_INSTANT                 32776

ZControlBarG::ZControlBarG( ZSubtask *pZSubtask,        // ctor
                            CWnd     *pWndParent,
                            ZMapAct  *pzmaComposite,
                            zVIEW    vDialog,
                            zSHORT   nOffsetX,
                            zSHORT   nOffsetY,
                            zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
              ZSizingControlBarG( ),
              ZMapAct( pZSubtask,          // base class ctor
                       pzmaComposite,
                       pWndParent,
                       this,
                       vDialog,
                       nOffsetX,
                       nOffsetY,
                       pCtrlDef,
                       "ControlBar" )
{
   zULONG ulControlInfoLth;

   Attr.Style = WS_CHILD | WS_VISIBLE;
   m_lSubtype = pCtrlDef->Subtype;

   Attr.Style &= ~WS_TABSTOP;
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   m_ulMapActFlags |= zMAPACT_IGNORE_RESIZE | zMAPACT_CLIENT_TYPE;

   GetAttributeLength( &ulControlInfoLth, vDialog, szlCtrl, szlCtrlBOI );
   if ( ulControlInfoLth )
   {
      m_pchControlInfo = new char[ ulControlInfoLth ];
      GetVariableFromAttribute( m_pchControlInfo, &ulControlInfoLth, zTYPE_BLOB,
                                ulControlInfoLth, vDialog, szlCtrl, szlCtrlBOI,
                                0, 0 );
   }
   else
      m_pchControlInfo = 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 ||
        (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }

   nOffsetX = (zSHORT) pCtrlDef->PSDLG_X;
   nOffsetY = (zSHORT) pCtrlDef->PSDLG_Y;

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );

   CreateZeidonCtrls( vSubobject, pZSubtask, this, this, 0, 0, TRUE );
   DropView( vSubobject );
}

void
ZControlBarG::OnWindowPosChanged( WINDOWPOS *lpWndPos )
{
// TraceLineS( "ZControlBarG::OnWindowPosChanged: ", *m_pzsTag );

// #define SWP_SHOWWINDOW      0x0040
// #define SWP_HIDEWINDOW      0x0080

   if ( lpWndPos->flags & (SWP_SHOWWINDOW | SWP_HIDEWINDOW) )
   {
      zSHORT lFlags = lpWndPos->flags & SWP_SHOWWINDOW ? 1 : 2;
      ProcessImmediateEvent( this, lFlags, (zPVOID) lFlags );
   }

   // Control Bar notification needs to be added
   ZSizingControlBarG::OnWindowPosChanged( lpWndPos );
}

void
ZControlBarG::CreateZ( )
{
   if ( Create( *m_pzsText, m_pZSubtask->m_pZFWnd, m_nIdNbr ) )
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
}

int
ZControlBarG::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = ZSizingControlBar::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetSCBStyle( GetSCBStyle( ) | SCBS_SIZECHILD );
   SetBarStyle( GetBarStyle( ) |
                 CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

   m_pZSubtask->m_pZFWnd->EnableDocking( CBRS_ALIGN_ANY );

   // Enable docking only on specified sides (or floating).
   DWORD dwStyle = 0;
   if ( m_lSubtype & zCONTROLBAR_LEFT )
   {
      dwStyle |= CBRS_ALIGN_LEFT;
   }

   if ( m_lSubtype & zCONTROLBAR_BOTTOM )
   {
      dwStyle |= CBRS_ALIGN_BOTTOM;
   }

   if ( m_lSubtype & zCONTROLBAR_RIGHT )
   {
      dwStyle |= CBRS_ALIGN_RIGHT;
   }

   if ( m_lSubtype & zCONTROLBAR_TOP )
   {
      dwStyle |= CBRS_ALIGN_TOP;
   }

   EnableDocking( dwStyle );
   m_pZSubtask->m_pZFWnd->DockControlBar( this, AFX_IDW_DOCKBAR_LEFT );

   return( nRC );  // ControlBar is now created
}

BOOL
ZControlBarG::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZControlBarG::DestroyWindow ", *m_pzsTag );
#endif
   zCHAR szBuffer[ 40 ]; // tag length + 7
   zLONG lLth = zsizeof( "-2147483647," ) * 4;
   zPCHAR pch = new char[ lLth ];
   zLONG  lStyle = GetBarStyle( );
   zSHORT nStyleLth;

   if ( (lStyle & CBRS_TOP) == CBRS_TOP )
   {
      lStyle = CBRS_TOP;
   }
   else
   if ( (lStyle & CBRS_LEFT) == CBRS_LEFT )
   {
      lStyle = CBRS_LEFT;
   }
   else
   if ( (lStyle & CBRS_BOTTOM) == CBRS_BOTTOM )
   {
      lStyle = CBRS_BOTTOM;
   }
   else
   if ( (lStyle & CBRS_RIGHT) == CBRS_RIGHT )
   {
      lStyle = CBRS_RIGHT;
   }
   else
   {
      lStyle = 0;
   }

   _ltoa_s( lStyle, pch, lLth, 10 );
   nStyleLth = (zSHORT) zstrlen( pch );
   pch[ nStyleLth++ ] = ',';
   pch[ nStyleLth ] = 0;
   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );
   mDeleteInitA( pch );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( ZSizingControlBar::DestroyWindow( ) );
}

ZControlBarG::~ZControlBarG( )  // dtor
{
   mDeleteInitA( m_pchControlInfo );
}

zSHORT
ZControlBarG::MapFromOI( WPARAM wFlag )
{
   return( 0 );
}

zLONG
ZControlBarG::GetZCtrlState( zLONG lStatusType )
{
   if ( lStatusType == zCONTROL_STATUS_VISIBLE )
   {
      if ( mIs_hWnd( m_hWnd ) )
      {
         zBOOL bVisible;
         bVisible = (GetStyle( ) & WS_VISIBLE) ? TRUE : FALSE;
         return( bVisible );
      }
   }

   return( ZMapAct::GetZCtrlState( lStatusType ) );
}

zLONG
ZControlBarG::SetZCtrlState( zLONG  lStatusType,
                             zULONG ulValue )
{
   zLONG lRC = 0;

   switch ( lStatusType )
   {
      case zCONTROL_STATUS_VISIBLE:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) && m_pDockSite )
         {
            if ( ulValue )
            {
               m_ulMapActFlags |= zMAPACT_VISIBLE;
               m_pDockSite->ShowControlBar( this, TRUE, FALSE );
            }
            else
            {
               m_ulMapActFlags &= ~zMAPACT_VISIBLE;
               m_pDockSite->ShowControlBar( this, FALSE, FALSE );
            }
         }

         break;
      }

      default:
      {
         lRC = ZMapAct::SetZCtrlState( lStatusType, ulValue );
         break;
      }
   }

   return( lRC );
}

#if 0 // the default method should be just fine
zBOOL
ZControlBarG::IsEventHandled( zLONG lEvent )
{
   return( TRUE );
}
#endif

zLONG
ZControlBarG::HandleEvent( zLONG lEvent )
{
#ifdef DEBUG_ALL
      TraceLineS( "ZControlBarG::HandleEvent Action: ", "" );
#endif
   return( 0 );   // inform CmdUI that event does not have a handler
}

void
ZControlBarG::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZControlBarG::OnSetFocus ", *m_pzsTag );
#endif

   if ( m_hWndFullClient )
      ::SetFocus( m_hWndFullClient );
   else
   if ( m_pZSubtask->m_pzmaWithFocus && m_pZSubtask->m_pzmaWithFocus->m_pCtrl )
   {
      m_pZSubtask->m_pzmaWithFocus->m_pCtrl->SetFocus( );
   }
}

void
ZControlBarG::OnSize( UINT uType, int cx, int cy )
{
   if ( m_pZSubtask->m_pZMIXCtrl && (m_ulMapActFlag2 & zMAPACT_CREATED) )
   {
      CSize size( cx, cy );

      if ( mIs_hWnd( m_hWndFullClient ) )
      {
         ::SetWindowPos( m_hWndFullClient, HWND_TOP, 0, 0, cx, cy, SWP_SHOWWINDOW );
      }
      else
      {
         m_pZSubtask->SendParentResize( this, &size );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( ZControlBarC, ZSizingControlBarCF )
BEGIN_MESSAGE_MAP( ZControlBarC, ZSizingControlBarCF )
   //{{AFX_MSG_MAP( ZControlBarC )
   ON_WM_CREATE( )
   ON_WM_SETFOCUS( )
   ON_WM_SIZE( )
   ON_WM_WINDOWPOSCHANGED( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZControlBarC
//
// The child window ID of a control bar is important.  The default layout of
// control bar only works for control bars with IDs in the range of
// AFX_IDW_CONTROLBAR_FIRST to AFX_IDW_CONTROLBAR_LAST.  Note that even
// though there is a range of 256 control bar IDs, the first 32 of these
// control bar IDs are special since they are directly supported by the print
// preview architecture.
#define ID_VIEW_MYTOOLBAR               32771
#define ID_VIEW_1                       32772
#define ID_VIEW_2                       32773
#define ID_VIEW_3                       32774
#define ID_VIEW_4                       32775
#define ID_VIEW_INSTANT                 32776

ZControlBarC::ZControlBarC( ZSubtask *pZSubtask,        // ctor
                            CWnd     *pWndParent,
                            ZMapAct  *pzmaComposite,
                            zVIEW    vDialog,
                            zSHORT   nOffsetX,
                            zSHORT   nOffsetY,
                            zKZWDLGXO_Ctrl_DEF *pCtrlDef ) :
              ZSizingControlBarCF( ),
              ZMapAct( pZSubtask,          // base class ctor
                       pzmaComposite,
                       pWndParent,
                       this,
                       vDialog,
                       nOffsetX,
                       nOffsetY,
                       pCtrlDef,
                       "ControlBar" )
{
   zULONG ulControlInfoLth;

   Attr.Style = WS_CHILD | WS_VISIBLE;
   m_lSubtype = pCtrlDef->Subtype;

   Attr.Style &= ~WS_TABSTOP;
   m_ulMapActFlags &= ~zMAPACT_FOCUS_ENABLED;
   m_ulMapActFlags |= zMAPACT_IGNORE_RESIZE | zMAPACT_CLIENT_TYPE;

   GetAttributeLength( &ulControlInfoLth, vDialog, szlCtrl, szlCtrlBOI );
   if ( ulControlInfoLth )
   {
      m_pchControlInfo = new char[ ulControlInfoLth ];
      GetVariableFromAttribute( m_pchControlInfo, &ulControlInfoLth, zTYPE_BLOB,
                                ulControlInfoLth, vDialog, szlCtrl, szlCtrlBOI, 0, 0 );
   }
   else
      m_pchControlInfo = 0;

#ifndef zREMOTE_SERVER
   if ( pzmaComposite == 0 || (pzmaComposite->m_ulMapActFlag2 & zMAPACT_CREATE_CHILDREN) )
#endif
   {
      CreateZ( );
   }

   nOffsetX = (zSHORT) pCtrlDef->PSDLG_X;
   nOffsetY = (zSHORT) pCtrlDef->PSDLG_Y;

   zVIEW vSubobject;

   CreateViewFromViewForTask( &vSubobject, vDialog, m_pZSubtask->m_vDialog );
   SetViewToSubobject( vSubobject, szlCtrlCtrl );

   CreateZeidonCtrls( vSubobject, pZSubtask, this, this, 0, 0, TRUE );
   DropView( vSubobject );
}

void
ZControlBarC::OnWindowPosChanged( WINDOWPOS *lpWndPos )
{
// TraceLineS( "ZControlBarC::OnWindowPosChanged: ", *m_pzsTag );

   // Control Bar notification needs to be added
   ZSizingControlBarCF::OnWindowPosChanged( lpWndPos );
}

void
ZControlBarC::CreateZ( )
{
   if ( Create( *m_pzsText, m_pZSubtask->m_pZFWnd, m_nIdNbr ) )
      m_ulMapActFlag2 |= zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED1;
}

int
ZControlBarC::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
   int nRC = ZSizingControlBar::OnCreate( lpCreateStruct );
   if ( nRC == -1 )
      return( -1 );

   SetSCBStyle( GetSCBStyle( ) | SCBS_SIZECHILD );
   SetBarStyle( GetBarStyle( ) | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

   m_pZSubtask->m_pZFWnd->EnableDocking( CBRS_ALIGN_ANY );

   // Enable docking only on specified sides (or floating).
   DWORD dwStyle = 0;
   if ( m_lSubtype & zCONTROLBAR_LEFT )
   {
      dwStyle |= CBRS_ALIGN_LEFT;
   }

   if ( m_lSubtype & zCONTROLBAR_BOTTOM )
   {
      dwStyle |= CBRS_ALIGN_BOTTOM;
   }

   if ( m_lSubtype & zCONTROLBAR_RIGHT )
   {
      dwStyle |= CBRS_ALIGN_RIGHT;
   }

   if ( m_lSubtype & zCONTROLBAR_TOP )
   {
      dwStyle |= CBRS_ALIGN_TOP;
   }

   EnableDocking( dwStyle );
   m_pZSubtask->m_pZFWnd->DockControlBar( this, AFX_IDW_DOCKBAR_LEFT );

   return( nRC );  // ControlBar is now created
}

BOOL
ZControlBarC::DestroyWindow( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZControlBarC::DestroyWindow ", *m_pzsTag );
#endif
   zCHAR szBuffer[ 40 ]; // tag length + 7
   zLONG lLth = zsizeof( "-2147483647," ) * 4;
   zPCHAR pch = new char[ lLth ];
   zLONG  lStyle = GetBarStyle( );
   zSHORT nStyleLth;

   if ( (lStyle & CBRS_TOP) == CBRS_TOP )
   {
      lStyle = CBRS_TOP;
   }
   else
   if ( (lStyle & CBRS_LEFT) == CBRS_LEFT )
   {
      lStyle = CBRS_LEFT;
   }
   else
   if ( (lStyle & CBRS_BOTTOM) == CBRS_BOTTOM )
   {
      lStyle = CBRS_BOTTOM;
   }
   else
   if ( (lStyle & CBRS_RIGHT) == CBRS_RIGHT )
   {
      lStyle = CBRS_RIGHT;
   }
   else
   {
      lStyle = 0;
   }

   _ltoa_s( lStyle, pch, lLth, 10 );
   nStyleLth = (zSHORT) zstrlen( pch );
   pch[ nStyleLth++ ] = ',';
   pch[ nStyleLth ] = 0;
   strcpy_s( szBuffer, zsizeof( szBuffer ), "Ctrl:" );
   strcpy_s( szBuffer + 5, zsizeof( szBuffer ) - 5, *m_pzsTag );
   SetWindowPreferenceString( m_pZSubtask->m_vDialog, szBuffer, pch );
   mDeleteInitA( pch );
   m_ulMapActFlag2 &= ~(zMAPACT_MAPPED_FROM_OI | zMAPACT_CREATE_CHILDREN | zMAPACT_CREATED);
   return( ZSizingControlBar::DestroyWindow( ) );
}

ZControlBarC::~ZControlBarC( )  // dtor
{
   mDeleteInitA( m_pchControlInfo );
}

zSHORT
ZControlBarC::MapFromOI( WPARAM wFlag )
{
   return( 0 );
}

zLONG
ZControlBarC::GetZCtrlState( zLONG lStatusType )
{
   if ( lStatusType == zCONTROL_STATUS_VISIBLE )
   {
      if ( mIs_hWnd( m_hWnd ) )
      {
         zBOOL bVisible;
         bVisible = (GetStyle( ) & WS_VISIBLE) ? TRUE : FALSE;
         return( bVisible );
      }
   }

   return( ZMapAct::GetZCtrlState( lStatusType ) );
}

zLONG
ZControlBarC::SetZCtrlState( zLONG  lStatusType,
                             zULONG ulValue )
{
   zLONG lRC = 0;

   switch ( lStatusType )
   {
      case zCONTROL_STATUS_VISIBLE:
      {
         if ( mIs_hWnd( m_pCtrl->m_hWnd ) && m_pDockSite )
         {
            if ( ulValue )
            {
               m_ulMapActFlags |= zMAPACT_VISIBLE;
               m_pDockSite->ShowControlBar( this, TRUE, FALSE );
            }
            else
            {
               m_ulMapActFlags &= ~zMAPACT_VISIBLE;
               m_pDockSite->ShowControlBar( this, FALSE, FALSE );
            }
         }

         break;
      }

      default:
      {
         lRC = ZMapAct::SetZCtrlState( lStatusType, ulValue );
         break;
      }
   }

   return( lRC );
}

#if 0 // the default method should be just fine
zBOOL
ZControlBarC::IsEventHandled( zLONG lEvent )
{
   return( TRUE );
}
#endif

zLONG
ZControlBarC::HandleEvent( zLONG lEvent )
{
#ifdef DEBUG_ALL
      TraceLineS( "ZControlBarC::HandleEvent Action: ", "" );
#endif
   return( 0 );   // inform CmdUI that event does not have a handler
}

void
ZControlBarC::OnSetFocus( CWnd *pWndLostFocus )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZControlBarC::OnSetFocus ", *m_pzsTag );
#endif

   if ( m_hWndFullClient )
      ::SetFocus( m_hWndFullClient );
   else
   if ( m_pZSubtask->m_pzmaWithFocus && m_pZSubtask->m_pzmaWithFocus->m_pCtrl )
   {
      m_pZSubtask->m_pzmaWithFocus->m_pCtrl->SetFocus( );
   }
}

void
ZControlBarC::OnSize( UINT uType, int cx, int cy )
{
   if ( m_pZSubtask->m_pZMIXCtrl && (m_ulMapActFlag2 & zMAPACT_CREATED) )
   {
      CSize size( cx, cy );
      m_pZSubtask->SendParentResize( this, &size );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

AFX_EXT_API
CWnd * OPERATION
ControlBar( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef )
{
   if ( pCtrlDef->Subtype & zCONTROLBAR_GRIPPERS )
      return( new ZControlBarG( pZSubtask, pWndParent, pzmaComposite,
                                vDialog, nOffsetX, nOffsetY, pCtrlDef ) );
   else
      return( new ZControlBarC( pZSubtask, pWndParent, pzmaComposite,
                                vDialog, nOffsetX, nOffsetY, pCtrlDef ) );
}

#if 0
//./ ADD NAME=CBarShowOrHide
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CBarShowOrHide
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation shows or hides the specified ControlBar.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control to be shown/hidden.
//              cpcCtrlTag  - The unique name of the control.
//              nVisible    - 0 - Hide ControlBar ... 1 - Show ControlBar
//
//  RETURNS:    0 - ControlBar was previously hidden
//              1 - ControlBar was previously visible
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
CBarShowOrHide( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zSHORT  nVisible )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      CControlBar *pControlBar = DYNAMIC_DOWNCAST( CControlBar, pzma->m_pCtrl );
      if ( pControlBar && pControlBar->m_pDockSite && mIs_hWnd( pControlBar->m_hWnd ) )
      {
         zBOOL bVisible;
         bVisible = (pControlBar->GetStyle( ) & WS_VISIBLE) ? TRUE : FALSE;
         pControlBar->m_pDockSite->ShowControlBar( pControlBar, nVisible, FALSE );
         return( bVisible );
      }

      TraceLineS( "drvr - Invalid control type for CBarShowOrHide ", cpcCtrlTag );
   }

   return( -1 );
}
#endif

//./ ADD NAME=CBarLoadSaveState
// Source Module=zdctlbar.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CBarLoadSaveState
//                                          05/28/2002 Modified: 05/28/2002
//
//  PURPOSE:    This operation maintains ControlBar state information.
//
//  PARAMETERS: pFrameWnd   - Pointer to the frame window.
//              cpcProfile  - The unique name under which to store the
//                            ControlBar information.
//              bSave       - TRUE ==> Save state ... FALSE ==> Load state
//
//  RETURNS:    0 - ControlBar state information loaded/saved.
//             -1 - ControlBar state information not loaded/saved.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
CBarLoadSaveState( CFrameWnd *pFrameWnd,
                   zCPCHAR   cpcProfile,
                   zBOOL     bSave )
{
   if ( pFrameWnd && mIs_hWnd( pFrameWnd->m_hWnd ) )
   {
      if ( bSave )
         ZSizingControlBar::GlobalSaveState( pFrameWnd, cpcProfile );
      else
         ZSizingControlBar::GlobalLoadState( pFrameWnd, cpcProfile );

      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
