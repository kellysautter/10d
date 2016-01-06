/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlmst.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlmst_h__  // Sentry, use file only if it's not already included
#define __zdctlmst_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Outliner( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

#define zTREE_RECURSIVE             0x00000001L

#define zTREES_SINGLEEXPAND         0x00000001L
#define zTREES_OPENALLCHILDREN      0x00000002L
#define zTREES_OPENROOTCHILDREN     0x00000004L
#define zTREES_FOLDER_OPEN          0x00000008L
#define zTREES_NOSHOW_ENTITY        0x00000010L
#define zTREES_NOSHOW_LABEL         0x00000020L
#define zALS_MULTISEL               0x00001000L
#define zTREES_MULTISEL             0x00002000L
#define zTREES_NOBITMAPS            0x00004000L
#define zDOS_DIRRIGHT               0x00008000L
#define zTREES_HASBUTTONS           0x00010000L
#define zTREES_HASLINES             0x00020000L
#define zTREES_LINESATROOT          0x00040000L

#define zTREE_CHILDREN_NONE         0
#define zTREE_CHILDREN_OPEN         1
#define zTREE_CHILDREN_CLOSED       2
#define zTREE_CHILDREN_OPENALL 3
#define zTREE_CHILDREN_OPENROOT 4
#define zTREE_CHILDREN_CLOSEALL 5
#define zTREE_CHILDREN_OPENCUR 6
#define zTREE_CHILDREN_CLOSECUR 7
#define zTREE_CHILDREN_FLIPCUR 8

// Tree type
#define zTREE_TYPE_MULTIROOT        0
#define zTREE_TYPE_SINGLEROOT       1

#pragma pack( push, __zdctlmst_h )
#pragma pack( 1 )

typedef struct tagTreeItemItemColorFontImage
{
   zLONG     lVersionsNumber;
   COLORREF  clrTextColor;
   LOGFONT   fntTextFont;
   zCHAR     szEntityName[ 33 ];  // Entity/attribute name (required)
   zLONG     lUseDefaultBitmap;
   zLONG     lUseSelectedBitmap;
   zCHAR     szDefaultBitmapFile[ 255 ];
   zCHAR     szSelectedBitmapFile[ 255 ];

} zTREE_COLOR_FONT, *zLPOL_COLOR_FONT;

#pragma pack( pop, __zdctlmst_h )

void
fnFormatString( zPCHAR pchString, zCPCHAR cpcFormat );

class ZDCTL_CLASS ZMSTree;

class ZOL_Node : public CObject
{
friend class ZMSTree;
friend zOPER_EXPORT zLONG OPERATION
OL_GetCurrentEntityName( zVIEW    vSubtask,
                         zCPCHAR  cpcCtrlTag,
                         zPVIEW   lpReturnView,
                         zPCHAR   lpReturnEntityName );

   DECLARE_DYNAMIC( ZOL_Node )
public:

   ZOL_Node( );
   ZOL_Node( ZOL_Node *pParentNode, zCPCHAR cpcEntityName );
   ~ZOL_Node( );

   zSHORT   GetNodeLevel( );          // Item level ... Root = 0.
   void     DeleteChildNodes( );
   ZOL_Node * GetParent( ) { return( m_pParent ); }
   void     SetParent( ZOL_Node *pParent );

   // To retrieve the values of all mapped attributes of the current entity
   // separated by cDelimiter.
   CString  GetValues( zCHAR cDelimiter = '\t' );

   // To set/get only a specific attribute value of current entity.
   CString  GetValue( int k = 0 );
   void     SetValue( CString csValue, int k = 0 );

   void     AddValue( CString csValue ) { m_cslValues.AddTail( csValue ); }
   POSITION GetFirstChildPosition( )
   {
      return( m_olChildren.GetHeadPosition( ) );
   }

   ZOL_Node * GetNextChild( POSITION &pos )
   {
      return( (ZOL_Node *) m_olChildren.GetNext( pos ) );
   }

   ZOL_Node * FindNodeByEntityKey( zULONG ulKey );
   ZOL_Node * FindNodeByRelativePosition( zULONG ulKey, zCPCHAR cpcEntityName );

public:

// zBOOL             m_bOpen;                // State of child entities
   zBOOL             m_bSelected;            // TRUE if item is selected
   zCHAR             m_chChildren;           // State of child entities
   zPCHAR            m_hAttributes;          // List of attributes
   HTREEITEM         m_hItem;                // Tree Item Handle
   zULONG            m_ulEntityKey;          // Entity Key
// LPENTITYINSTANCE  m_hEntityInstance;      // Entity Instance Handle

private:

   void     Init( );
   void     AttachChild( ZOL_Node *pChild );
   void     DetachChild( ZOL_Node *pChild );

   ZOL_Node    *m_pParent;             // pointer to parent entry
   CObList     m_olChildren;           // List of child nodes
   CStringList m_cslValues;            // List of values on this level
   CString     m_csEntityName;         // Entity name
   CString     m_csAttributeName;      // Attribute name
   CString     m_csContextName;        // Context name
   CString     m_csDefaultBitmapFile;
   CString     m_csSelectedBitmapFile;
   zLONG       m_lUseDefaultBitmap;
   zLONG       m_lUseSelectedBitmap;
   COLORREF    m_TextColor;
   LOGFONT     m_TextFont;
};

class ZDCTL_CLASS ZMSTree : public CTreeCtrl, public ZMapAct
{
   DECLARE_DYNAMIC( ZMSTree )
public:
   // ctor
   ZMSTree( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZMSTree( );

   enum Image
   {
      ImgDefault = 0,
      ImgExpanded = 1,
      ImgSelected = 2
   };

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT PostMapFromOI( WPARAM wFlag );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual void   ParentResize( ZMapAct *pParent, CSize *pSize );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zBOOL  PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle = "",
                              zLONG lFlags = 0,
                              zSHORT nFontSize = 80,
                              zCPCHAR cpcFontFace = "Arial" );
   virtual zSHORT PrintZPage( zLONG  lPageNbr,
                              zLONG  lPageWidth,
                              zLONG  lPageHeight );
   void    DrawItemText( CDC *pDC, ZColorItem *pColorItem,
                         CRect rectLabel, zSHORT nCol,
                         zCPCHAR cpcText, UINT uFormat, zLONG lFlags );

   void     ListAllItems( zCPCHAR cpcMsg, HTREEITEM hItem = 0 );
   zLONG    GetSelectedCount( );
   zULONG   GetSelectedNodeEntityKey( );  // of currently selected item
   zLONG    SetTxtFontForEntity( zCPCHAR cpcEntityName, LOGFONT fntTextFont );
   zLONG    GetTextFontForEntity( zCPCHAR cpcEntityName, zPLONG plSize, zPLONG plBold,
                                  zPLONG plItalic, zPLONG plUnderline, zPLONG plStrikeout,
                                  zPLONG plTextColor, zPLONG plEscapement, zPCHAR pcFaceName );
   zLONG    SetTextFontForEntity( zCPCHAR cpcEntityName, zLONG nSize, zLONG lBold,
                                  zLONG lItalic, zLONG lUnderline, zLONG lStrikeout,
                                  zLONG lTextColor, zLONG nEscapement, zPCHAR pcFaceName );
   zLONG    GetTextColorForEntity( zCPCHAR cpcEntityName );
   zLONG    SetTextColorForEntity( zCPCHAR cpcEntityName, COLORREF clrTextColor );
   zLONG    GetBackgroundColor( );
   zLONG    SetBackgroundColor( COLORREF clrBackgroundColor );
   zLONG    EditItem( zULONG ulTextLength );
   zLONG    ExpandEntity( zCPCHAR cpcEntityName, zULONG ulValue );
   zLONG    GetExpandState( zCPCHAR cpcEntityName );
   zLONG    SelectEntityItem( zCPCHAR cpcEntityName, zULONG ulEntityKey, zULONG ulValue );
   zLONG    GetDefaultBitmap( zCPCHAR cpcEntityName, zPCHAR szBitmapFileName,
                              zULONG ulMaxReturnBitmapNameLength );
   zLONG    SetDefaultBitmap( zCPCHAR cpcEntityName, zCPCHAR cpcBitmapFileName );
   zLONG    GetSelectedBitmap( zCPCHAR cpcEntityName, zPCHAR szBitmapFileName,
                               zULONG ulMaxReturnBitmapNameLength );
   zLONG    SetSelectedBitmap( zCPCHAR cpcEntityName, zCPCHAR cpcBitmapFileName );
   zSHORT   SelectItemAtPosForEntity( zCPCHAR cpcEntityName, zLONG lFlags );
   zLONG    SelectItemByAttribute( zCPCHAR cpcEntityName, zCPCHAR cpcAttribName,
                                   zCPCHAR cpcFindString, zULONG  ulEntityKey, zULONG  ulFlag );
   zLONG    SetTextForCurrentItem( zCPCHAR cpcItemText );
   zLONG    SetTextItemColorText( zCPCHAR cpcItemText, COLORREF clrTextColor,
                                  zBOOL bEnsureVisible );
   zVIEW    GetApplicationView( ) { return m_vApp; }
   void     RemoveAllTextColorsForItemText( );
   void     GetExpandingEntityName( zPCHAR pchEntityName );
   void     GetCollapsingEntityName( zPCHAR pchEntityName );
   void     GetLastExpandedEntityName( zPCHAR pchEntityName );
   void     GetLastCollapsedEntityName( zPCHAR pchEntityName );
   void     DeselectAllEntities( );
   void     CursorListener( zVIEW pView, zCPCHAR pchEntityName );
   CString  GetRootEntityName( );
   HTREEITEM GetNextItemPhysical( HTREEITEM hItem );
   HTREEITEM GetPrevItemPhysical( HTREEITEM hItem );
   HTREEITEM GetLastItemPhysical( HTREEITEM hItem );

   // MFC Overwritables
   afx_msg UINT OnGetDlgCode( );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
   afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
   afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

   //{{AFX_MSG(ZMSTree)
   afx_msg void OnDblclk( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnOutOfMemory( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnRdblclk( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnReturn( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnBegindrag( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnBeginLabelEdit( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnBeginrdrag( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnDeleteItem( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnEndLabelEdit( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnItemExpanded( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnItemExpanding( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnSelChanged( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnSelChanging( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   //}}AFX_MSG

   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

public:
   static CString m_csTreeNodeEntity;
   static CString m_csTreeNodeTagAttr;
   static CString m_csTreeNodeTextAttr;
   static CString m_csTreeNodeChildEntity;
   static CString m_csTreeSelectionEntity;
   static CString m_csTreeSelectionTag;
   static CString m_csTreeSelectionAttr;
   static CString m_csTree;

   ZOL_Node  *m_pRootEntry;
   zCHAR     m_szLastAccEntity[ 33 ];
   zULONG    m_ulKeyLast;         // Key of last tree item selected

private:  // variables
   zCHAR     m_szClass[ 10 ];
   zPCHAR    m_pchCtrlBOI;
   zSHORT    m_nFirstEntityOffset;
   zPCHAR    m_pchFontColorBitmapBlob;
   DWORD     m_dwStyle;
   zSHORT    m_nType;
   zLONG     m_lFontColorBitmapBlobSize;
   zLONG     m_lStyle;
   zULONG    m_ulCurrentKey;
   zLONG     m_lBitmapHeight;
   zLONG     m_lBitmapWidth;
   zLONG     m_clrItemBk;
   zULONG    m_lLabelEditTextLth;
   zBOOL     m_bCreateTree;
   zBOOL     m_bExpandInMapFromOI;
   zBOOL     m_bNoImage;
   zBOOL     m_bFontColor;
   zBOOL     m_bLabelEdit;
   zBOOL     m_bSetMultiSelection;
   zBOOL     m_bPreSelectChangeAlreadyDone;
   zBOOL     m_bLabelEditESC;
   zBOOL     m_bLabelEditRemoveFlag;
   COLORREF  m_clrBackground;
   COLORREF  m_clrBackgroundOld;
   zVIEW     m_vSubtask;          // Subtask view
   zVIEW     m_vApp;              // Application view
   CBitmap   *m_pBitmap1;
   CBitmap   *m_pBitmap2;
   CString   m_csTreeText;
   CString   m_csExpandingEntity;
   CString   m_csCollapsingEntity;
   CString   m_csLastExpandedEntity;
   CString   m_csLastCollapsedEntity;
   CString   m_csCurrentEntity;
   CString   m_csLabelEditOldValue;
   CString   m_csAppViewName;     // guess what it is
   CString   m_csTreeViewName;
   CString   m_csRecursiveView;   // label of recursive View
   CString   m_csRootItemLabel;   // if tree is type of zTREE_TYPE_MULTIROOT
                                  // this is the label of the root entry
   HTREEITEM m_hItemFirstSel;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

   // Report stuff.
   CString   m_csTitle;
   CRect     m_rectDraw;
   CFont     *m_pFontNormal;
   CFont     *m_pFontBold;
   CFont     *m_pFontItalic;
   CFont     *m_pFontUnderline;
   zSHORT    m_nLinesPerPage;
   zLONG     m_lMaxPage;
   zSHORT    m_nTextHeight;
   zSHORT    m_nTextHeightBold;
   zLONG     m_lPrintFlags;

   CRect     m_rectBounds;
   CSize     m_sizeChar;

   zLONG     m_lTreeItemCnt;
   zSHORT    m_nCharWidth;
   zSHORT    m_nRowHeight;
   HANDLE    m_hDIB;
   zBOOL     m_bPrintPreview;
// WINDOWPLACEMENT m_WndPlace;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

public:

   // This enum is used when printing the content of a tree control
   // it returns the style of lines/buttons at a given level.
   enum eDRAW_TYPES
   {
      eNone = 0,            // no lines/buttons
      ePlus,                // [+]-

      ePlusDrop,            // [+]-
                            //  |

      eMinus,               // [-]-

      eMinusDrop,           // [-]-
                            //  |

      eLink,                // |
                            // +-

      eLinkDrop,            // |
                            // +-
                            // |

      eDrop                 // |
   };

   HTREEITEM   GetNextTreeItem( HTREEITEM hItem );
   void        DrawTreeItem( ZPrintout *pZPrintout, HTREEITEM hPrintItem,
                             CDC *pDC, CRect rect, CWnd *pWnd );
   void        ExpandAll( );

private:
   zLONG       GetIndentLevelStyle( HTREEITEM hPrintItem, HTREEITEM hIndentItem );
   void        DrawTreeStyle( ZPrintout *pZPrintout, CDC *pDC,
                              CRect box, eDRAW_TYPES eDrawType );
   HANDLE      ImageToDIB( CImageList *pImageList, int iImageNumber, CWnd *pWnd );
   HANDLE      DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette *pPal );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

   void PrintHeadFoot( CDC *pDC, zLONG lPage );

   struct ItemColorFontImage
   {
      COLORREF color;
      LOGFONT  logfont;
      CString  csBitmapFile;
      zLONG    lImageListIdx;
   };

   CMap<void *, void *, ItemColorFontImage, ItemColorFontImage&> m_mapItemColorFontImage;
   CMap<void *, void *, ItemColorFontImage, ItemColorFontImage&> m_mapItemColorFontImageSelected;

   struct FileImage
   {
      zLONG    lImageListIdx;
   };

   CMap<CString, LPCTSTR, FileImage, FileImage&> m_mapFileImage;

   struct ItemColorText
   {
      CString  csItemText;
      COLORREF clrTextColor;
   };

   CArray<ItemColorText, ItemColorText&> m_arrItemColorText;

private:
   CImageList m_ImageList;

   // Member Functions.
   void     BuildTreeDataStruct( zVIEW vApp, ZOL_Node *pParentNode,
                                 zLPOL_ITEM OL_Item, zSHORT nRecurse );
   void     ToggleSelection( ZOL_Node *pNode );
   void     ToggleSelection( HTREEITEM hItem );
   zSHORT   MapTreeFromOI( );
   zSHORT   EndLabelEdit( ZOL_Node *pNode, HTREEITEM hItem,
                          LRESULT *pResult, zBOOL bError );

#ifdef zREMOTE_SERVER
   zLONG    PopulateTree( ZOL_Node *pNode = NULL, zVIEW vRoot = 0 );
   void     HandleRemoteState( );
#else
   zLONG    PopulateTree( ZOL_Node *pNode = NULL, HTREEITEM hRoot = 0 );
#endif

   ZOL_Node * GetNewNode( zCPCHAR cpcMsg, NM_TREEVIEW *pNMTreeView );
   void      ResetViewForDefaultSelectSet( zVIEW v );
   CString   TreeMakeFullName( zCPCHAR cpcEntity, zCPCHAR cpcEntityLabel );
   CString   GetCurrentEntityName( );
   HTREEITEM GetFirstSelectedItem( );
   HTREEITEM GetNextSelectedItem( HTREEITEM hItem );
   HTREEITEM GetPrevSelectedItem( HTREEITEM hItem );
   HTREEITEM GetLastVisibleItem( HTREEITEM hItem );
   HTREEITEM GetItemHandle( zVIEW vApp, zCPCHAR cpcEntityName );
   zBOOL     CanMultiSelect( );
   zBOOL     SelectItems( HTREEITEM hItemFrom, HTREEITEM hItemTo );
   zBOOL     SetItemSelState( HTREEITEM hItem, UINT nState, UINT nStateMask );
   void      ClearSelection( );
   void      ExpandCollapseAll( HTREEITEM hRoot, UINT flag );
   void      SetMultipleSelectionWithCtrl( UINT nFlags, CPoint point );
   void      SetMultipleSelectionWithShift( UINT nFlags, CPoint point );
   void      SetSelectionForSystemKey( UINT nChar );
   void      SetSelectionForSingleSelect( UINT nChar );
   void      SetSelectionForMultipleSelect( UINT nChar, UINT nRepCnt,
                                            UINT nFlags );
   void      SetSelectionForDesignTime( UINT nFlags, HTREEITEM  hItem );
   void      SaveCurrentEntity( );
   HTREEITEM SelectCurrentEntity( zBOOL bReset = FALSE );

   void      GetColorFontBitmap( zVIEW vDialog );
   void      SetItemFont( HTREEITEM hItem, LOGFONT& logfont );
   void      SetItemBold( HTREEITEM hItem, BOOL bBold );
   void      SetItemColor( HTREEITEM hItem, COLORREF color );
   void      ReadFontColorBitmap( ZOL_Node *OL_Node, zPCHAR pchEntityName );
   void      SetColorAndFont( CDC* memDC );
   void      SetItemBitmap( HTREEITEM hItem, ZOL_Node *pNode );
   void      CollapseForSingleExpand( HTREEITEM hItem, HTREEITEM hSelItem );
   void      LabelEditCleanUp( );
   void      LabelEditError( HTREEITEM hItem );
   void      InitBeginLabelEdit( CEdit *pEdit, ZOL_Node *pNode,
                                 HTREEITEM hItem );
   zULONG    SaveExpandState( CArray<zULONG, zULONG> &hOpenEnityKey );
   void      ItemExpanded( HTREEITEM hItem, UINT nCode );
   void      RemoveChildSelection( HTREEITEM hItem, zPBOOL bSetNewSelection );
   void      CheckChildSelection( HTREEITEM hItem );
   void      SetAllTextColorsForItemText( );
   zBOOL     ItemExpand( HTREEITEM hItem, UINT nCode );
   zBOOL     GetItemFont( HTREEITEM hItem, LOGFONT *plogfont );
   zBOOL     GetItemBold( HTREEITEM hItem );
   zBOOL     IsDesignTime( zBOOL bCheckWnd = TRUE );
   zBOOL     IsMappedEntity( CString csEntityName );
   zBOOL     IsCancelByPreSelChangeAction( HTREEITEM hItem );
   COLORREF  GetItemColor( HTREEITEM hItem );
   zLONG     ItemExpanding( HTREEITEM hItem, UINT nCode );
   zLONG     SetNewFontColorBitmapBlob( ZOL_Node *pNode, CString strItemText );
   zLONG     GetBitmapIdFromImageList( HTREEITEM hItem, CString csBitmapFile, zBOOL bSelected );
   zLONG     SetBitmapFileToImageList( CString csBitmapFile );
   zLONG     SetTextColorForCurrentItem( COLORREF clrTextColor );
   zLONG     SetTxtFontForCurrentItem( LOGFONT fntTextFont );
   zLONG     SetNewStyle( zLONG lStatusType, zULONG ulValue );
   zLONG     SetSingleExpand( zULONG ulValue );
   zLONG     SetExpand( zULONG ulValue );
   zLONG     SetExpandAll( zULONG ulValue );
   zLONG     SetShowEntity( zULONG ulValue );
   zLONG     SetMultipleRoot( zULONG ulValue );
   zLONG     SetMultiSelect( zULONG ulValue );
   zLONG     LoadBitmapWithResourcePath( HTREEITEM hItem,
                                         zCPCHAR   cpcBitmapFileName,
                                         zBOOL     bSelected );
   zLONG     SetExpandState( CArray<zULONG, zULONG> &hOpenEntityKey,
                             zULONG ulTopEntityKey );
   zLONG     SetExpandStatusForEntity( zCPCHAR cpcEntityName,
                                       zULONG ulValue );
   zLONG     SetTxtItemColorText( zCPCHAR cpcItemText,
                                     COLORREF clrTextColor,
                                     zBOOL bEnsureVisible );
   LPENTITYINSTANCE  GetEntityInstanceHandle( zVIEW vApp,
                                              zPCHAR pchEntityName );

   DECLARE_MESSAGE_MAP( )
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// VividTree.h : Definition of Class VividTree
///////////////////////////////////////////////////////////////////////////
//
// Copyright(C) 2005 Jim Alsup  All rights reserved
//              email: ja.alsup@gmail.com
//
// License: This code is provided "as is" with no expressed or implied
//          warranty. You may use, or derive works from this file without
//          any restrictions except those listed below.
//
//        - This original header must be kept in the derived work.
//
//        - If your derived work is distributed in any form, you must
//          notify the author via the email address above and provide a
//          short description of the product and intended audience.
//
//        - You may not sell this code or a derived version of it as part of
//          a commercial code library.
//
//        - Offering the author a free licensed copy of any end product
//          using this code is not required, but does endear you with a
//          bounty of good karma.
//
///////////////////////////////////////////////////////////////////////////

#pragma once

//#include "resource.h"

// Helper Routine
extern void
GradientFillRect( CDC *pDC, CRect &rect, COLORREF clrFrom, COLORREF clrTo, zBOOL bVertGrad );

class VividTree : public CTreeCtrl
{
   DECLARE_DYNAMIC( VividTree )

public:
   VividTree( );
   virtual ~VividTree( );

   // Attributes
   void SetBkGradients( COLORREF clrFrom, COLORREF clrTo ) { m_clrGradientBkgdFrom = clrFrom;
                                                             m_clrGradientBkgdTo = clrTo; }
   COLORREF GetBkGradientFrom( ) { return( m_clrGradientBkgdFrom ); }
   COLORREF GetBkGradientTo( ) { return( m_clrGradientBkgdTo ); }

   enum BkMode { BK_MODE_BMP = 0, BK_MODE_GRADIENT, BK_MODE_FILL };
   BkMode GetBkMode( ) { return( m_BkgdMode ); }
   void SetBkMode( BkMode bkgdMode ) { m_BkgdMode = bkgdMode; }

   zBOOL GetBitmapTiledMode( ) { return m_bBmpTiledMode; }
   void SetBitmapTiledMode( zBOOL bBmpTiledMode ) { m_bBmpTiledMode = bBmpTiledMode; }
   void SetBitmapID( UINT id );

   zBOOL GetGradientHorz( ) { return m_bGradientHorz; }
   void SetGradientHorz( zBOOL horz ) { m_bGradientHorz = horz; }

   virtual void  SetItemIcon( HICON icon ) { m_icon = icon; }; // overridable
   virtual HICON GetItemIcon( HTREEITEM item ); // overridable

   // Operations
   zBOOL IsItemVisible( HTREEITEM item );

private:
   COLORREF m_clrGradientBkgdFrom;  // gradient variables
   COLORREF m_clrGradientBkgdTo;
   COLORREF m_clrGradientBkgdSel;
   zBOOL    m_bGradientHorz;        // horz or vertical gradient

   BkMode  m_BkgdMode;              // current background mode
   CBitmap m_BmpBackGround;         // background bitmap image
   CBitmap m_BmpTreeClosed;         // tree open image (marker)
   CBitmap m_BmpTreeOpened;         // tree closed image (marker)
   zBOOL   m_bBmpTiledMode;         // tile background image

   CRect m_rect;                    // the client rect when drawing
   int   m_nOffsetX;                // 0... -x (scroll offset)
   int   m_nSizeX;                  // width of unclipped window
   int   m_nOffsetY;                // 0 ... -y (scroll offset)
   int   m_nSizeY;                  // height of unclipped window

   HICON m_icon;

   void DrawBackGround( CDC *pDC ); // background paint code
   void DrawItems( CDC *pDC );      // item paint code

protected:
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnPaint( );
   afx_msg void OnTvnItemExpanding( NMHDR *pNMHDR, LRESULT *pResult );
   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlmst_h__ sentry.
