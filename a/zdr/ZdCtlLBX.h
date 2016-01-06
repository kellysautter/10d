/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctllbx.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctllbx_h__  // Sentry, use file only if it's not already included
#define __zdctllbx_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ListBox( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef );
AFX_EXT_API
CWnd * OPERATION
ListCtrl( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZListBox standard control
#ifdef zREMOTE_SERVER
class ZDCTL_CLASS ZListBox : public ZRmtDataSet, public ZMapAct
#else
class ZDCTL_CLASS ZListBox : public CListBox, public ZMapAct
#endif
{
   DECLARE_DYNAMIC( ZListBox )
public:
   // ctor
   ZListBox( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZListBox( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zLONG  SelectAll( );
   virtual zSHORT SetCtrlMapping( zCPCHAR cpcViewName,
                                  zCPCHAR cpcEntityName,
                                  zCPCHAR cpcAttributeName,
                                  zCPCHAR cpcContextName,
                                  zLONG   lColumn = -1 );
   virtual zSHORT GetCtrlMapping( zPCHAR pchViewName,
                                  zPCHAR pchEntityName,
                                  zPCHAR pchAttributeName,
                                  zPCHAR pchContextName,
                                  zLONG  lColumn = -1 );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual void HiliteText( zBOOL bHilite, zLONG lType,
                            zLONG lRange = 0, zBOOL bHiliteItem = TRUE );
   zSHORT GetSelectIdxFromPoint( CPoint pt );
   void   HiliteSelectedEntity( zVIEW   vApp,
                                zCPCHAR cpcEName,
                                zCPCHAR cpcScope );
   void   SetDisplaySelSet( zSHORT nSelSet );
   void   SetSelectSelSet( zSHORT nSelSet );
   void   SelectHilitedEntity( zVIEW   vApp,
                               zCPCHAR cpcEName,
                               zCPCHAR cpcScope,
                               zLONG   lRelIdx );
   void   ProcessCommand( zUSHORT uNotificationCode, zLONG lSelectedIdx = -1 );
   void   UpdateSelection( zLONG lSelection, zUSHORT uFlags, CPoint pt );

   zLONG  FormatTextAtPosition( zLONG lPos );
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawInfo );
   virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureInfo );
   virtual int  CompareItem( LPCOMPAREITEMSTRUCT lpCompareInfo );

   zBOOL  BeginDrag( ZDragDrop *pDragDrop );
   zBOOL  QueryDrop( ZDragDrop *pDragDrop );

#ifndef zREMOTE_SERVER

   virtual BOOL DestroyWindow( );

   afx_msg void OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
// afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg UINT OnGetDlgCode( );
   afx_msg void OnEnable( BOOL bEnable );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );
#endif

protected:
#ifndef zREMOTE_SERVER
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
#endif

public:

   zSHORT  m_nCols;
   zLPLB_COL m_pCol;
   zLONG   m_lSelSrc;

   zULONG  m_ulBkColor;

   zPLONG  m_plCharWidth;   // list of character widths in variable font
   zLONG   m_lCharHeight;
   zLONG   m_lCharWidth;

   zVIEW   m_vApp;
   zVIEW   m_vAppList;
   CString m_csViewName;
   CString m_csViewNameList;
   zLONG   m_lEntityCnt;
   zSHORT  m_nMaxExtent;

   //
   // child id notifications handled at the child
   //
   void LBNDblclk( );
   void LBNErrspace( );
   void LBNSelcancel( );
   void LBNSelchange( );
   void LBNSetFocus( );
   void LBNKillFocus( );

#ifdef zREMOTE_SERVER

   void   MapToRemoteDataset();
   void   MapFromRemoteDataset();
   zLONG  SetMultiSelection();
   virtual zSHORT SetRowColText( zCPCHAR cpcText,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   virtual zSHORT GetRowColText( zPCHAR pchText, zUSHORT uLth,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   void   SortByColumn( zSHORT nColumn, zSHORT nAscending );

#endif

protected:

   zPCHAR   m_pchListInfo;
   zULONG   m_ulListLth;
   zCHAR    m_chMultiSelect;
   zCHAR    m_chListOnlyWhenSelected;
   zCHAR    m_chHilitePosition;
   zSHORT   m_nDisplaySelSet;
   zSHORT   m_nSelectSelSet;
   zULONG   m_ulFlag;
   zSHORT   m_nFirstIndex;
   zSHORT   m_nSelCnt;
   zPCHAR   m_pchMap;
   zSHORT   m_pchMapLth;
   zBOOL    m_bAutoDragSort;

#ifdef zREMOTE_SERVER
   zBOOL    m_bShowHeaderButtons;
   CString  m_csSortBuffer;
   zSHORT   m_nColLastSorted;
   zCHAR    m_chAscendDescend;
   zBOOL    m_bAutoSort;
   ZNumberList *m_pSortColumnList;
#endif

private:

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define PIT_COMBO 0  //PIT = property item type
#define PIT_EDIT  1
#define PIT_COLOR 2
#define PIT_FONT  3
#define PIT_FILE  4

#define IDC_PROPCMBBOX   712
#define IDC_PROPEDITBOX  713
#define IDC_PROPBTNCTRL  714

/////////////////////////////////////////////////////////////////////////////
// ZPropertyItem
class ZPropertyItem
{
public:
   // ctor
   ZPropertyItem( zSHORT  nItemType, zCPCHAR cpcPropertyName,
                  zCPCHAR cpcInitialValue, zCPCHAR cpcComboItemList,
                  zCPCHAR cpcDescription );
   virtual ~ZPropertyItem( );

   // Attributes
   zSHORT  m_nItemType;
   CString m_csPropertyName;
   CString m_csInitialValue;
   CString m_csComboList;
   CString m_csDescription;
};

/////////////////////////////////////////////////////////////////////////////
// ZPropertyList window

class ZDCTL_CLASS ZPropertyList : public CListBox, public ZMapAct
{
   DECLARE_DYNAMIC( ZPropertyList )
// Construction
public:
   ZPropertyList( ZSubtask *pZSubtask,
                  CWnd     *pWndParent,
                  ZMapAct  *pzmaComposite,
                  zVIEW    vDialog,
                  zSHORT   nOffsetX,
                  zSHORT   nOffsetY,
                  zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~ZPropertyList( );

// Operations
public:
   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );

   zSHORT AddItem( CString cs );
   zSHORT AddPropItem( ZPropertyItem *pItem );
   void   ZPropertyList::CleanAll( );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZPropertyList )
public:
   virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
protected:
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   virtual void PreSubclassWindow( );
   //}}AFX_VIRTUAL

// Implementation
public:

   // Generated message map functions
protected:
   //{{AFX_MSG( ZPropertyList )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void LBNSelchange( );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   //}}AFX_MSG

   afx_msg void OnKillfocusCmbBox( );
   afx_msg void OnSelchangeCmbBox( );
   afx_msg void OnKillfocusEditBox( );
   afx_msg void OnChangeEditBox( );
   afx_msg void OnButton( );

   void InvertLine( CDC *pDC, CPoint ptFrom, CPoint ptTo );
   void DisplayButton( CRect region );

   CComboBox m_cmbBox;
   CEdit m_editBox;
   CButton m_btnCtrl;
   CFont m_SSerif8Font;

   int m_curSel,m_prevSel;
   int m_nDivider;
   int m_nDivTop;
   int m_nDivBtm;
   int m_nOldDivX;
   int m_nLastBox;
   BOOL m_bTracking;
   BOOL m_bDivIsSet;
   HCURSOR m_hCursorArrow;
   HCURSOR m_hCursorSize;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctllbx_h__ sentry.
