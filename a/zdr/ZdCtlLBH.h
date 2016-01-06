/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctllbh.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctllbh_h__  // Sentry, use file only if it's not already included
#define __zdctllbh_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
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

#define zMINCOLWIDTH 10
#define zLB_BEFOREPASTECELL           0x0006
#define zLB_AFTERPASTECELL            0x0007

/////////////////////////////////////////////////////////////////////////////
// ZSortedHeaderCtrl window

class ZSortedHeaderCtrl : public CHeaderCtrl
{
   DECLARE_DYNAMIC( ZSortedHeaderCtrl )
public:
   // ctor
   ZSortedHeaderCtrl( );

   // dtor
   virtual ~ZSortedHeaderCtrl( );

protected:

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZSortedHeaderCtrl)
   //}}AFX_VIRTUAL

   //{{AFX_MSG(ZSortedHeaderCtrl)
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

public:
   zSHORT SetSortImage( zSHORT nCol, zBOOL bAsc );

private:
   zSHORT m_nSortCol;
   zBOOL  m_bSortAsc;

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Zeidon ZListCtrl standard control
class ZDCTL_CLASS ZListCtrl : public CListCtrl, public ZMapAct
{
   DECLARE_DYNAMIC( ZListCtrl )
public:
   // ctor
   ZListCtrl( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZListCtrl( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zLONG  SetZCtrlProperty( zLONG   lPropertyType,
                                    zULONG  ulNumericProperty,
                                    zCPCHAR cpcStringProperty );
   virtual zLONG  GetZCtrlProperty( zLONG   lPropertyType,
                                    zPULONG pulNumericProperty,
                                    zPCHAR  pchStringProperty,
                                    zLONG   lMaxLth );
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
   virtual zBOOL  PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle = "",
                              zLONG lFlags = 0,
                              zSHORT nFontSize = 80,
                              zCPCHAR cpcFontFace = "Arial" );
   virtual zSHORT PrintZPage( zLONG  lPageNbr,
                              zLONG  lPageWidth,
                              zLONG  lPageHeight );
   virtual zSHORT SetRowColText( zCPCHAR cpcText,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   virtual zSHORT GetRowColText( zPCHAR pchText, zUSHORT uLth,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   virtual void HiliteText( zBOOL bHilite, zLONG lType,
                            zLONG lRange = 0, zBOOL bHiliteItem = TRUE );
   virtual zLONG  SelectAll( );

// zSHORT GetSelectIdxFromPoint( CPoint pt );
   void   AddCol( zCPCHAR cpcText, zLONG lColumn );
   zLONG  HiliteAllSelectedEntities( );
   void   CorrelateZeidonSelectToListSelect( );
   void   SetLastSelectedZeidonEntity( zSHORT nPrevSelectSet );
   void   SortByColumn( zSHORT nColumn, zSHORT nAscending );
   zLONG  GetSortOrderForKey( zLONG lKey );
   void   SortByDisplayData( );
   void   SetDisplaySelSet( zSHORT nSelSet );
   void   SetSelectSelSet( zSHORT nSelSet );
   void   SetSelectedPosition( zLONG lDisplayPos );
// void   SelectHilitedEntity( zVIEW   vApp,
//                             zCPCHAR cpcEName,
//                             zCPCHAR cpcScope,
//                             zSHORT  nRelIdx );
// void   ProcessCommand( zUSHORT uNotificationCode );
// void   UpdateSelection( zSHORT nSelection, zUSHORT uFlags, CPoint pt );
   zBOOL  IsItemSelected( zLONG lOffset );

// CImageList * SetImageList( CImageList *pImageList,
//                            zSHORT nImageListType = TVSIL_NORMAL );
   zBOOL  AddColumn( zCPCHAR cpcItem, zSHORT nItem, zSHORT nSubItem = -1,
             zSHORT nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
             zSHORT nFmt = LVCFMT_LEFT );
   zBOOL  AddItem( zSHORT  nItem, zSHORT nSubItem,
                   zCPCHAR cpcItem, zSHORT nImageIndex = -1 );
   void   CheckItem( zSHORT nNewCheckedItem );

   zSHORT FormatTextAtPosition( zPCHAR pchText, zLONG lRow,
                                zSHORT nCol, zSHORT nMaxLth );
   zBOOL  SetFullRowSel( zBOOL bFillRowSel );
   zBOOL  GetFullRowSel( );

   zSHORT GetColumnCount( ) const;
   void   AutoSizeColumns( zLONG lCol, zLONG lTgtMinWidth, zLONG lTgtMaxWidth );
   void   GetPreferencesFromIni( );

   zBOOL  BeginDrag( ZDragDrop *pDragDrop );
   zBOOL  QueryDrop( ZDragDrop *pDragDrop );

   void   SetColorAttribute( zCPCHAR cpcAttributeName, zSHORT nCol );
   void   SetColorAssociation( zLONG lColumn, zCPCHAR cpcValue,
                               zLONG lClrText, zLONG lClrBack, zLONG lFlags );
   void   RemoveColorAssociations( );
   void   MeasureItem( LPMEASUREITEMSTRUCT lpMIS );
   void   DrawItemText( CDC *pDC, ZPairedList *pPairedList,
                        CRect& rectItem, CRect& rectLabel, zSHORT nCol,
                        zCPCHAR cpcText, UINT uFormat, zLONG lFlags );
   void   DrawSeparatorLines( );
   void   RepaintSelectedItems( );

   virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult );
   virtual BOOL DestroyWindow( );

protected:
   afx_msg void OnColumnClick( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnGetDispInfo( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnSetDispInfo( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnItemChanging( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnItemChanged( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnBeginDrag( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnBeginRDrag( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnItemActivate( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnOdStateChanged( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnBeginLabelEdit( LPNMHDR pnmhdr, LRESULT *pResult );
   afx_msg void OnEndLabelEdit( LPNMHDR pnmhdr, LRESULT *pResult );

   afx_msg void OnChar( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT uModKeys, CPoint pt );

   afx_msg UINT OnGetDlgCode( );
   afx_msg void OnEnable( BOOL bEnable );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnDeleteItem( NMHDR *pNMHDR, LRESULT *pResult );
   afx_msg void DrawItem( LPDRAWITEMSTRUCT pDrawInfo );
   afx_msg void OnMeasureItem( int nIdCtl, LPMEASUREITEMSTRUCT lpMIS );

#if 0
   LVN_INSERTITEM, LVNInsertItem
   LVN_DELETEITEM, LVNDeleteItem
   LVN_DELETEALLITEMS, LVNDeleteAllItems
   LVN_DELETEALLITEMS, LVNDeleteAllItems
#endif

   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

   // Implementation - state icon width
   afx_msg LRESULT OnSetImageList( WPARAM wParam, LPARAM lParam );

   // Implementation - list view colors
   afx_msg LRESULT OnSetTextColor( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnSetTextBkColor( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnSetBkColor( WPARAM wParam, LPARAM lParam );

private:

   void OnEditCopy( );
   void OnEditPaste( );
   COleDataSource * CopyTextFromListbox( );
   zBOOL PasteTextToListbox( ZCellId cellId, COleDataObject *pDataObject );
   void  WriteRow( CSharedFile& sf, zLONG lMinRow, zLONG lMaxRow,
                   int nMinCol, int nMaxCol );

public:

   zVIEW     m_vApp;
   zVIEW     m_vAppList;
   CString   m_csViewName;
   CString   m_csViewNameList;
   zSHORT    m_nRetryMapFromOI;  // flag to permit retry of MapFromOI
   zLONG     m_lEntityCnt;       // number of entities in list
   zLONG     m_lEntityNbr;
   zLONG     m_lLastSelected;
   zSHORT    m_nCols;
   zSHORT    m_nColsMax;
   zSHORT    m_nColLastSorted;
   zSHORT    m_nLinesPerRow;
   zCHAR     m_chAscendDescend;
   zBOOL     m_bAutoSort;
   zBOOL     m_bRememberSort;
   zBOOL     m_bSortDisplay;
   zBOOL     m_bInHilite;
   zLONG     m_lSelSrc;
   WNDPROC   m_lpfnHeaderButtonWndProc;
   CString   m_csSortBuffer;
   ZNumberList *m_pRowColColorList;
   ZColorList *m_pColorList;
   zLPLB_COL m_pCol;
   zLONG     m_lKey1;
   zLONG     m_lKey2;
   zVIEW     m_vApp1;
   zVIEW     m_vApp2;
   zULONG    m_ulFlag;
   ZNumberList *m_pSortColumnList;
   CArray<zLONG, zLONG> m_EntityKeyList;

protected:

   zPCHAR    m_pchListInfo;
   zULONG    m_ulListLth;
   zCHAR     m_chMultiSelect;
   zCHAR     m_chListOnlyWhenSelected;
   zCHAR     m_chHilitePosition;
   zSHORT    m_nDisplaySelSet;
   zSHORT    m_nSelectSelSet;
   zBOOL     m_bAutoDragSort;
   zBOOL     m_bLabelEdit;
   zPCHAR    m_pchLabelText;
   zSHORT    m_nCntPerPage;

   zBOOL     m_bFullRowSel;
   zBOOL     m_bClientWidthSel;
   zSHORT    m_nClientWidth;        // client area width
   zSHORT    m_nStateImageOffset;   // state icon width
   zBOOL     m_bStateIcons;
   zLONG     m_lCheckedItem;
   HICON     m_hSmallIcon;
// HICON     m_hLargeIcon;
   ZListTip  m_wndListTip;

   ZSortedHeaderCtrl m_SortedHeaderCtrl;
   zBOOL     m_bSeparatorLines;

   // List view colors.
   COLORREF  m_clrText;
   COLORREF  m_clrTextBk;
   COLORREF  m_clrBkgnd;
   COLORREF  m_clrTextSel;
   COLORREF  m_clrTextBkSel;

   // Report stuff.
   CRect     m_rectDraw;
   CFont     *m_pFontNormal;
   CFont     *m_pFontBold;
   CFont     *m_pFontItalic;
   CFont     *m_pFontUnderline;
   zSHORT    m_nLinesPerPage;
   zSHORT    m_nMaxPage;
   zSHORT    m_nTextHeight;
   zSHORT    m_nTextHeightBold;
   zLONG     m_lPrintFlags;

private:

   DECLARE_MESSAGE_MAP( )
};

#if 0
class CGfxListCtrl : public CListCtrl
{
// Construction
public:
   CGfxListCtrl();
   CGfxHeaderCtrl wndHeader;

   CGfxColumnManager * pManager;
   CGfxCategoryManager * pCategoryManager;

   COLORREF m_clrText, crBackSel, m_clrTextBk, m_clrBkgnd, crCategoryBck, cr3dFace;

   int iCtrlId;
   int iSubItemFocus;
   int m_cxClient;
   int m_cxStateImageOffset;

   int iItemHeight;
   int iAutoPreviewHeight;
   int iItemHeightCategory;

   void SetItemHeight(const int iHeight, const bool bCategory = false);
   void SetAutoPreviewHeight(const int iHeight = 0);
   int  GetAutoPreviewHeight() const;

   int iInternal1;

   CPen * gpn, *gcatpn;

   CFont ftNormal, ftUnderline, ftBold, ftItalic;

   CWnd * pReciper;

   CWnd * GetReciper() const { return pReciper ? pReciper : GetParent(); };

// Database Bind Mode
public:
   bool        bDaoBind;
   CDaoRecordset * pRecordSet;
   bool        BindDaoRecordset(CDaoRecordset * pSet);
   bool        FillRecordsetColumn();
   bool        FillDaoItems();
   void        OnDaoGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
   void        OnDaoGetTextCallback(int iIndex, int iSubItem, long lCode, CString& cs);
   long        GetExInfoCallback(LXHDREX * pLx);

// Printing support
public:
   void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
   void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
   void OnPrint(CDC* pDC, CPrintInfo* pInfo);
   CFont m_pFont;
   CFont m_pFontBold;
   CFont m_pFontItalic;
   CFont m_pFontUnderline;
   int m_nLinesPerPage;
   CRect rcMarginMM;
   DWORD dwPrintFlag;
   CString csPrintName;
   void SetPrintFlag(const DWORD dwf);
   void SetPrintName(zCPCHAR name);
   void SetMargins(const int left, const int top, const int right, const int bottom);

// Bitmap background
public:
   bool bBlendBitmap;
   CPalette m_pal;
   CBitmap m_bitmap;
   int m_cxBitmap, m_cyBitmap;
   CImageList cBackImage;
   bool SetBkImage(UINT nIDResource);
   bool SetBkImage(LPCTSTR lpszResourceName);
   void SetBlendBitmap(const bool bBlend);

// Attributes
public:
   enum { OFFSET_FIRST = 2, OFFSET_OTHER = 6 };

   long  (CWnd::*m_fpExInfoFun)(LXHDREX *);
   void  (CWnd::*m_fpGetTextCallback)(int , int, long, CString& );
   void  (CWnd::*m_fpCategoryFun)(CGfxCategoryManager * , long &);
   void  (CWnd::*m_fpDaoCategoryFun)(CGfxCategoryManager *);

   void  SetExInfoCallback(long (CWnd::*fpExInfoFun)(LXHDREX *));
   void  SetCategoryCallback(void (CWnd::*fpCategoryFun)(CGfxCategoryManager * , long &));
   void  SetGetTextCallback(void (CWnd::*fpGetTextCallback)(int , int, long, CString& ));
   void  SetDaoCategoryCallback(void (CWnd::*fpDaoCategoryFun)(CGfxCategoryManager *));

   DWORD dwFlag;
   DWORD dwhCombo, dwhComboEx, dwhEdit, dwhNumeric, dwhValute, dwhImages;
   DWORD wdhExInfoOnCol;

   void SetNumeric(const int iCol);
   void SetValute(const int iCol);
   void SetEditCol(const int iCol);
   void SetComboCol(const int iCol);
   void SetComboExCol(const int iCol);
   void SetExColRequest(const int iCol);
   void DontDrawSortArrow(const int iCol);
   bool NeedColExInfo(const int iCol) const;

   CImageList * pItemImageList;
   void SetItemImageList(CImageList * pList);


// Operations
public:
   CGfxListTip wndToolTip;
   int CellRectFromPoint(CPoint & point, RECT * cellrect, int * col) const;

   // Drag column
   void DragColumn(int &source, int &dest);


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CGfxListCtrl)
   public:
   virtual BOOL PreTranslateMessage(MSG* pMsg);
   protected:
   virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
   //}}AFX_VIRTUAL
   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

   virtual void DrawItemCategory(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implementation
public:
   bool LinkRecordset(zCPCHAR cMainRecord, CDaoRecordset * ps, zCPCHAR cLinkRecord, zCPCHAR cTextField);
   bool LinkRecordset(const int iColId, CDaoRecordset * ps, const int iLnk, const int iTxtLnk);
   bool EnableMoveNext();
   bool EnableMovePrev();
   bool EnableMoveLast();
   bool EnableMoveFirst();
   void OnMovePrevRecord();
   void OnMoveNextRecord();
   void OnMoveLastRecord();
   void OnMoveFirstRecord();
   void ModifyExStyle(DWORD dwRemove, DWORD dwAdd, UINT dwRedraw = 0);
   void DeleteAllColumns();

   void ModifyItemEx(const int iIndex, const int iSubItem, const long lData, CString& cs);
   void ModifyItemEx(const int iIndex, const int iSubItem, const long lData);
   void DeleteItemEx(const int iIndex, const long lData);
   void GetItemTextEx(const int idx, const int sub, CString& cs, const unsigned long *dwData = NULL);
   int InsertItemEx(const int iIndex, const long lData);
   void EnableCategoryView(const bool bEnab = true);
   CGfxCategoryManager * GetCategoryManager();
   CGfxCategoryManager * CreateCategoryManager();
   void RetrieveColumnData();
   void SetupColumnData();
   CGfxColumnManager * GetColumnManager();
   CGfxColumnManager * CreateColumnManager();
   void EnableColumnDrag(const bool bEnableDrag = true);
   bool IsColumnDragEnabled() const { return wndHeader.bEnableDrag; };
   int GetColumnIndex(const int iCol) const;
   bool DoCopyInClipboard(const bool bOnlySel = true);
   int GetColumnCount() const;
   void ResetScrollBar();
   void SetColumnImage(const int iCol, const int iImage);
   void SetHeaderImageList(CImageList * pIma);
   void SetSubItemImage(const int iItem, const int iSubItem, const int iIndex);
   void RemoveAllSelection();
   void GetSubItemRect(const int nItem, const int nCol, CRect &rcItem);
   bool EnsureSubItemVisible(const int iSubItem);
   int HitTestEx(CPoint & point, int * col) const;
   CGfxInCombo * ComboSubLabel(int nItem, int nCol, bool bEditable = false, bool bClick = true);
   CRichEditCtrl * EditSubLabel(int nItem, int nCol, bool bClick = true);
   void OnInit();
   void ModifyFlag(const DWORD dwRemove, const DWORD dwAdd, const UINT dwRedraw);
   DWORD GetFlag() const { return dwFlag; };
   LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen, int nOffset);
   void MakeShortString(CDC* pDC, CString& cs, int nColumnLen, int nOffset);
   int InsertColumnEx(int iPos, zCPCHAR cText, int iAlign, int iWidth, DWORD exData); // LVCFMT_LEFT
   virtual ~CGfxListCtrl();
   bool AskForInfo(LXHDREX * pLx, CDC *pDC);
   long SendInfoExMsg(LXHDREX * pLx);

   // Generated message map functions
   afx_msg void OnGfxCustomizehead();
   afx_msg void OnGfxFormatheader();
protected:
   //{{AFX_MSG(CGfxListCtrl)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnPaint();
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnUpdateGfxCustomizehead(CCmdUI* pCmdUI);
   afx_msg void OnUpdateGfxFormatheader(CCmdUI* pCmdUI);
   afx_msg void OnGfxSortasc();
   afx_msg void OnUpdateGfxSortasc(CCmdUI* pCmdUI);
   afx_msg void OnGfxSortdesc();
   afx_msg void OnUpdateGfxSortdesc(CCmdUI* pCmdUI);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg BOOL OnQueryNewPalette();
   afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   //}}AFX_MSG
   afx_msg LRESULT OnSetTextColor(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnSetTextBkColor(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnSetBkColor(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnSetImageList(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnUserTab(WPARAM wParam, LPARAM lParam);
   afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);

   DECLARE_MESSAGE_MAP()
#endif
#endif   // __zdctllbh_h__ sentry.
