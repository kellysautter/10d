/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlcal.h
// AUTHOR:
// Copyright (c) Iuri Apollonio 1998
// Use & modify as you want & need, and leave those 3 lines.
// http://www.codeguru.com
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlolb_h__  // Sentry, use file only if it's not already included
#define __zdctlolb_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
OutlookBar( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

typedef zLONG (POPERATION zFARPROC_OLB_EVENT)
                                      ( zVIEW, zCPCHAR, WPARAM, LPARAM );

/////////////////////////////////////////////////////////////////////////////

#include <afxcoll.h>

#define WM_OUTBAR_NOTIFY      WM_USER + 1
#define NM_OB_ITEMCLICK       1
#define NM_OB_ONLABELENDEDIT  2
#define NM_OB_ONGROUPENDEDIT  3
#define NM_OB_DRAGITEM        4
#define NM_FOLDERCHANGE       5

struct OUTBAR_INFO
{
   int     nIdx;
   zCPCHAR cpcText;
   int     nDragFrom;
   int     nDragTo;
};

class ZDCTL_CLASS ZOutlookBar : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZOutlookBar )
public:
   static CString m_csClassName;

   // ctor
   ZOutlookBar( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZOutlookBar( );

   virtual void   CreateZ( );
// virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
// virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // transfer text from OI to calendar control
// virtual zSHORT MapToOI( zLONG lFlag = 0 ); // transfer text to OI from calendar control

   virtual BOOL DestroyWindow( );

   LRESULT SendMessageToParent( WPARAM wMsg, LPARAM lParam );

   enum { fSmallIcon = 1 << 0, fLargeIcon = 1 << 1, fEditGroups = 1 << 2,
          fEditItems = 1 << 3, fRemoveGroups = 1 << 4, fRemoveItems = 1 << 5,
          fAddGroups = 1 << 6, fDragItems = 1 << 7, fAnimation = 1 << 8,
          fSelHighlight = 1 << 9 };

   enum { ircIcon = 1, ircLabel = 2, ircAll = 3 };

// Attributes
public:

   zFARPROC_OLB_EVENT m_lpfnOLB_Event;
   CImageList m_imgLarge;
   CImageList m_imgSmall;

   COLORREF   m_clrBackGroundColor, m_clrBackGroundColor1;
   COLORREF   m_clrTextColor;
   COLORREF   m_clr3dFace, m_clrLightBorder, m_clrHilightBorder;
   COLORREF   m_clrShadowBorder, m_clrDkShadowBorder;
   int        m_nFolderHeight;

   int        m_nSmallIconLabelOffsetX;
   int        m_nLargeIconLabelOffsetY;
   int        m_nSmallIconSpacingY;
   int        m_nLargeIconSpacingY;

   int        m_nLeftMarginX;
   int        m_nTopMarginY;
   bool       m_bUpArrow, m_bDownArrow, m_bUpPressed, m_bDownPressed;
   CRect      m_rectUpArrow, m_rectDownArrow;
   bool       m_bLooping;

   int        m_nHitInternal1;
   int        m_nHitInternal2;

   long       m_lAnimationTickCount;

   int        m_nLastSel;
   int        m_nSelAnimTiming;
   int        m_nSelAnimCount;

   DWORD      m_dwFlags;

   CPtrArray  m_arFolder;

   int        m_nLastFolderHighlighted;
   int        m_nLastSelectedFolder;
   int        m_nFirstItem;

   int        m_nLastItemHighlighted;
   bool       m_bPressedHighlight;

   int        m_nLastDragItemDraw;
   int        m_nLastDragItemDrawType;

   class ZBarItem
   {
   public:
      ZBarItem( zCPCHAR cpcName, const int nImageIdx, DWORD dwData );
      virtual  ~ZBarItem( );
      int        m_nImageIdx;
      zCHAR      *m_pchItem;
      DWORD      m_dwData;
   };

   class ZBarFolder
   {
   public:
      ZBarFolder( zCPCHAR cpcName, DWORD dwData );
      virtual  ~ZBarFolder( );
      int GetItemCount( );
      int InsertItem( int nIdx, zCPCHAR cpcText,
                      const int nImage, const DWORD dwData );
      zCHAR      *m_pchName;
      DWORD      m_dwData;
      CImageList *m_pLargeImageList;
      CImageList *m_pSmallImageList;
      CPtrArray  m_arItems;
      CWnd       *m_pChild;
   };

   int        m_nSelFolder;

   CImageList *m_pLargeImageList;
   CImageList *m_pSmallImageList;

   HCURSOR    m_hHandCursor;
   HCURSOR    m_hDragCursor;
   HCURSOR    m_hNoDragCursor;

   CPen       *m_pBlackPen;

// Operations
public:
   enum { htNothing = -1, htFolder, htItem, htUpScroll, htDownScroll};

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZOutlookBar )
   //}}AFX_VIRTUAL

// Implementation
public:
   void DrawAnimItem( const int nOffsetX, const int nOffsetY, const int nIdx );
   void SetAnimSelHighlight( const int nTime );
   DWORD GetFolderData( int nFolder = -1 );
   CWnd * GetFolderChild( int nFolder = -1 );
   int  AddFolderBar( zCPCHAR pFolder, CWnd *pSon, const DWORD dwData = 0 );
   CString GetItemText( const int nIdx );
   void SetAnimationTickCount( const long lValue ) { m_lAnimationTickCount = lValue; };
   long GetAnimationTickCount( ) { return( m_lAnimationTickCount ); };

   void AnimateFolderScroll( const int iFrom, const int iTo );
   int  GetDragItemRect( const int nIdx, CRect &rect );
   void DrawDragArrow( CDC * pDC, const int iFrom, const int iTo );
   void SetItemImage( const int nIdx, const int iImage );
   void SetItemData( const int nIdx, const DWORD dwData );
   int  GetItemImage( const int nIdx ) const;
   DWORD GetItemData( const int nIdx ) const;
   bool IsValidItem( const int nIdx ) const;
   void RemoveItem( const int nIdx );
   void SetItemText( const int nIdx, zCPCHAR text );
   void StartItemEdit( const int nIdx );
   void SetFolderText( const int nIdx, zCPCHAR text );
   void StartGroupEdit( const int nIdx );
   void GetLabelRect( const int nFolder, const int nIdx, CRect &rect );
   void GetIconRect( const int nFolder, const int nIdx, CRect &rect );
   void HighlightItem( const int nIdx, const bool bPressed = false );
   void GetVisibleRange( const int nFolder, int &first, int &last );
   void DrawItem( CDC *pDC, const int nFolder, CRect rect, const int nIdx,
                  const bool bOnlyImage = false );
   CImageList * GetFolderImageList( const int nIdx, const bool bSmall ) const;
   CSize GetItemSize( const int nFolder, const int nIdx, const int type );
   void PaintItems( CDC *pDC, const int nFolder, CRect rect );
   CImageList * GetImageList( CImageList * pImageList, int nImageList );
   CImageList * SetFolderImageList( const int folder, CImageList * pImageList, int nImageList );
   CImageList * SetImageList( CImageList * pImageList, int nImageList );
   int  GetCountPerPage( ) const;
   void RemoveFolder( const int nIdx );
   int  GetSelFolder( ) const;
   int  GetFolderCount( ) const;
   void SetSelFolder( const int nIdx );
   int  GetItemCount( ) const;
   int  InsertItem( const int folder, const int nIdx, zCPCHAR text,
                    const int image = -1, const DWORD exData = 0 );
   void HighlightFolder( const int nIdx );
   int  HitTestEx( const CPoint &point, int &index );
   void GetInsideRect( CRect &rect ) const;
   int  AddFolder( zCPCHAR cFolderName, const DWORD exData );
   void GetItemRect( const int nFolder, const int nIdx, CRect &rect );
   bool GetFolderRect( const int nIdx, CRect &rect ) const;
   void ModifyFlag( const DWORD &dwRemove, const DWORD &dwAdd, const UINT redraw = 0 );
   DWORD GetFlag( ) const;
   void SetSmallIconView( const bool bSet );
   bool IsSmallIconView( ) const;
   BOOL Create( DWORD dwStyle, const RECT& rect, CWnd *pParentWnd, UINT nID,
                const DWORD dwFlag = fDragItems | fEditGroups | fEditItems |
                                     fRemoveGroups | fRemoveItems | fAddGroups |
                                     fAnimation | fSelHighlight );

   // Generated message map functions
protected:
   void DrawFolder( CDC * pDC, const int iIdx, CRect rect, const bool bSelected );
   //{{AFX_MSG( ZOutlookBar )
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnTimer( UINT nIDEvent );
   afx_msg void OnPaint( );
   afx_msg BOOL OnEraseBkgnd( CDC* pDC );
   afx_msg void OnMouseMove( UINT nFlags, CPoint point );
   afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
   afx_msg BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message );
   afx_msg void OnSize( UINT nType, int cx, int cy );
   afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
   afx_msg void OnGfxLargeicon( );
   afx_msg void OnUpdateGfxLargeicon( CCmdUI* pCmdUI );
   afx_msg void OnGfxSmallicon( );
   afx_msg void OnUpdateGfxSmallicon( CCmdUI* pCmdUI );
   afx_msg void OnGfxRemoveitem( );
   afx_msg void OnUpdateGfxRemoveitem( CCmdUI* pCmdUI );
   afx_msg void OnGfxRenameitem( );
   afx_msg void OnUpdateGfxRenameitem( CCmdUI* pCmdUI );
   //}}AFX_MSG
   afx_msg long OnEndLabelEdit( WPARAM wParam, LPARAM lParam );
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZOLB_Edit window

class ZOLB_Edit : public CEdit
{
// Construction
public:
   ZOLB_Edit( );

// Attributes
public:
   BOOL       m_bEscapeKey;
   CString    m_csText;
   WPARAM     m_msgSend;
   bool       m_bNoDown;

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZOLB_Edit )
   public:
   virtual BOOL PreTranslateMessage( MSG* pMsg );
   protected:
   virtual void PostNcDestroy( );
   //}}AFX_VIRTUAL

// Implementation
public:
   int        m_nIdx;
   virtual ~ZOLB_Edit( );

   // Generated message map functions
protected:
   //{{AFX_MSG( ZOLB_Edit )
   afx_msg void OnKillFocus( CWnd* pNewWnd );
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

class ZOLB_PopupMenu : public CMenu
{
public:
   struct ToolBarData
   { // struct for toolbar resource; guess you already know it ; )
      WORD    wVersion;
      WORD    wWidth;
      WORD    wHeight;
      WORD    wItemCount;
   };

   struct SpawnItem
   {
      int     nImageIdx;
      int     nCmd;
      zCHAR   szText[ 128 ];
   };

   struct ImageItem
   {
      int   nImageIdx;
      int   nCmd;
   };

   ZOLB_PopupMenu( );
   virtual ~ZOLB_PopupMenu( );

   bool  CreateBoldFont( );

   HFONT m_hMenuFont;
   HFONT m_hGuiFont;
   CSize m_sizeImage;
   int FindImageItem( const int nCmd );

   SpawnItem * AddSpawnItem( zCPCHAR cpcText, const int nCmd );
   void AddImageItem( const int ndx, WORD wCmd );

   void RemapMenu( CMenu *pMenu );

   HFONT      m_hMenuBoldFont;
   CImageList m_ilList;
   CImageList m_ilOther;

   COLORREF   m_clrMenuText;
   COLORREF   m_clrMenuTextSel;
   COLORREF   m_clr3dFace;
   COLORREF   m_clrMenu;
   COLORREF   m_clrHighlight;
   COLORREF   m_clr3dHilight;
   COLORREF   m_clr3dShadow;
   COLORREF   m_clrGrayText;
   COLORREF   m_clrBtnFace;
   COLORREF   m_clrBtnHilight;
   COLORREF   m_clrBtnShadow;

   int        m_nSpawnItem;
   SpawnItem  **m_pSpawnItem;

   int        m_nImageItem;
   ImageItem  *m_pImageItem;

   bool LoadToolBarResource( unsigned int resId );
   bool AddToolBarResource( unsigned int resId );

   void EnableMenuItems( CMenu *pMenu, CWnd *pParent );

   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
   virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined( AFX_ZOUTBARCTRL_H__28FA2CA4_11B7_11D2_8437_0000B43382FE__INCLUDED_ )
