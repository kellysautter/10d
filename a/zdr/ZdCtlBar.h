/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlbar.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlbar_h__  // Sentry, use file only if it's not already included
#define __zdctlbar_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

#define ID_MENU_BUTTONS   zZEIDON_CTRL_MSG + 1
#define ID_MENU_CAPTION   zZEIDON_CTRL_MSG + 2
#define ID_MENU_GRADIENT  zZEIDON_CTRL_MSG + 3

#define IDR_MAINBAR                     128
#define IDR_EXTOOLTYPE                  129
#define IDB_SMALL_COLOR                 130
#define IDB_SMALL_BW                    131
#define IDB_LARGE_COLOR                 136
#define IDB_LARGE_BW                    137
#define ID_FILENEW                      140
#define IDB_SM_BW                       146
#define IDB_LG_BW                       147
#define IBD_SM                          148
#define IDB_LG                          149
#define IDB_LG_DB                       150
#define IDB_SM_DB                       151
#define IDB_SM_DB_16                    152
#define IDB_LG_16                       153
#define IDB_LG_DB_16                    154
#define IDB_SM_16                       155
#define IDB_SM_BW_16                    156
#define IDB_LG_BW_16                    157
#define IDB_WND_SM_DB                   158
#define IDB_WND_LG                      159
#define IDB_WND_LG_DB                   160
#define IDB_WND_SM_BW                   161
#define IDB_WND_SM                      162
#define IDB_WND_LG_BW                   163
#define IDR_WND_TOOLBAR                 164
#define IDR_ICONTOOLBAR                 168
#define IDR_COLOR_EDITBAR               170
#define IDR_MONO_EDITBAR                171

typedef struct tagToolButton
{
   zULONG  ulId;      // Separators have an Id of 0
   zSHORT  nTabPos;   // Position of tab (first is always 0)
   zSHORT  nColLth;   // Length of list column
   zSHORT  nExtent;   // Extent of list column
   zSHORT  nColWidth; // Width of list column in 256ths of an inch
   zSHORT  nJustify;  // Right/Left/Center justification
   zPCHAR  pchEntity;
   zPCHAR  pchAttrib;
   zPCHAR  pchContext;
   zCPCHAR cpcTag;
   zPCHAR  pchText;
   zPCHAR  pchMB_Text;

} zTB_BTN, *zLPTB_BTN;

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ToolBar( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

/////////////////////////////////////////////////////////////////////////////
// ZToolBar window

#if 1

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDCTL_CLASS ZEditBar;
interface IPopupCtrlEvent;

///////////////////////////////////////////////////////////////////////////////
class ZTB_Popup : public CMiniFrameWnd
{
public:
   // Return value of OnKeyDown, OnLButtonDown and OnLButtonUp callbacks
   enum
   {
      eDoNothing = 0,  // No action
      eNoSend    = 1,  // Do not send the current message to the control
      eEnd       = 2,  // Hide popup normally
      eAbort     = 3,  // Hide popup with flag 'abort'
   };

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZTB_Popup )
public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual void PostNcDestroy( );
   //}}AFX_VIRTUAL

    zBOOL Display( CWnd *pControl, CWnd *pCaller,
                   CRect& rectCaller, IPopupCtrlEvent *pEvents = 0 );

// Implementation
public:
   CWnd  *m_pCaller;
   CWnd  *m_pControl;

protected:
   IPopupCtrlEvent *m_pEvents;
   HWND  m_hPrevParentCtrl;
   zBOOL m_bDestroy;

protected:
   ZTB_Popup( );
   virtual ~ZTB_Popup( );

   void EndPopup( bool bAbort = false, bool bSetFocus = true );

   // Generated message map functions
protected:
   //{{AFX_MSG( ZTB_Popup )
   afx_msg void OnActivate( UINT, CWnd*, BOOL );
   afx_msg void OnCancelMode( );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
   DECLARE_DYNCREATE( ZTB_Popup )
};

///////////////////////////////////////////////////////////////////////////////
// {600029F0-14EE-11D3-941C-00105A66E1E2}
DEFINE_GUID( IID_IPopupCtrlEvent, 0x600029F0, 0x14EE, 0x11D3, 0x94,
             0x1C, 0x00, 0x10, 0x5A, 0x66, 0xE1, 0xE2 );

interface IPopupCtrlEvent
{
   virtual bool OnInit( ) = 0;
   virtual void OnShow( ) = 0;
   virtual void OnHide( bool bAbort ) = 0;
   virtual int  OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) = 0;
   virtual int  OnLButtonDown( UINT nFlags, CPoint point ) = 0;
   virtual int  OnLButtonUp( UINT nFlags, CPoint point ) = 0;
   virtual IPopupCtrlEvent * GetInterfaceOf( HWND hWnd ) = 0;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZTB_List window

class ZTB_List : public CListCtrl
{
friend class ZEditBar;
private:
   // ctor
   ZTB_List( );

// Operations
public:
    int GetSelectedItem( ) const;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZTB_List )
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZTB_List( );

   CString m_csSendText;

   // Generated message map functions
protected:
   //{{AFX_MSG( ZTB_List )
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG


   DECLARE_MESSAGE_MAP( )

    /////////////////////////////////////////////////////////////////////////
    // ZTB_Popup events
    //
public:
    class XPopupCtrlEvent : public IPopupCtrlEvent
    {
       public:
          virtual bool OnInit( );
          virtual void OnShow( );
          virtual void OnHide( bool bAbort );
          virtual int  OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
          virtual int  OnLButtonDown( UINT nFlags, CPoint point );
          virtual int  OnLButtonUp( UINT nFlags, CPoint point );
          virtual IPopupCtrlEvent * GetInterfaceOf( HWND hWnd );

    } m_xPopupCtrlEvent;
};

/////////////////////////////////////////////////////////////////////////////
// ZTB_Combo window

class ZTB_Combo : public CComboBox
{
friend class ZEditBar;
friend class ZBarItem;

private:
   // ctor
   ZTB_Combo( zULONG ulId );
   virtual ~ZTB_Combo( );

// Overrides
protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZToolCombo )
//   virtual BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG(ZToolCombo)
      // NOTE - the ClassWizard will add and remove member functions here.
   void CBNDropdown( );
   void CBNCloseup( );
   void CBNSelchange( );
   void CBNSelendok( );
   void CBNSelendcancel( );
   //}}AFX_MSG

private:
   zULONG  m_ulId;
   zBOOL   m_bIsDroppedDown;
   CString m_csSendText;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZEditBar control

class ZDCTL_CLASS ZBarItem
{
friend class ZEditBar;
private:
   // ctor
   ZBarItem( zULONG ulId, zULONG ulType,
             zCPCHAR cpcTag, zCPCHAR cpcText,
             zCPCHAR cpcTipText, zCPCHAR cpcAction,
             zCPCHAR cpcViewName, zCPCHAR cpcEntity,
             zCPCHAR cpcAttribute );
   virtual ~ZBarItem( );

   zULONG    m_ulId;
   zULONG    m_ulType;
   zCPCHAR   m_cpcTag;
   zPCHAR    m_pchText;
   zLONG     m_lTextLth;
   zPCHAR    m_pchTipText;
   zLONG     m_lTipLth;
   zCPCHAR   m_cpcAction;
   zCPCHAR   m_cpcViewName;
   zCPCHAR   m_cpcEntity;
   zCPCHAR   m_cpcAttribute;
   ZTB_Combo *m_pBarCombo;
};

#define zMAX_IMAGE_LIST 6

class CCoolCmdUI : public CCmdUI  // class private to this file!
{
public: // re-implementations only
   virtual void Enable( BOOL bOn );
   virtual void SetCheck( int nCheck );
};

class ZDCTL_CLASS ZEditBar : public CToolBar, public ZMapAct
{
   DECLARE_DYNAMIC( ZEditBar )

public:

   // ctor
   ZEditBar( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   virtual ~ZEditBar( );   // dtor

   virtual zSHORT MapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual zSHORT PostMapFromOI( WPARAM wFlag );
   virtual void   CreateZ( );
// virtual zBOOL  IsEventHandled( zLONG lEvent ); use default method
   virtual zLONG  HandleEvent( zLONG lEvent );

   zBOOL  Init( CWnd *pParentWnd, zBOOL bColor );
   zBOOL  SetColor( zBOOL bColor );
   zBOOL  SetHorizontal( );
   zBOOL  SetVertical( );
   void   DrawGripper( CDC& dc ) const;
   CBrush * GetBackgroundBrush( HWND hWnd );
   void   HideSeparators( CDC *pDC );

   CSize  CalcButtonSize( );
   void   SetButtonDropDown( int nID );

   // Mode related method.
   void   SetIconMode( zBOOL bShowIconMode ); // set (large or small) icon mode
   zBOOL  GetIconMode( ) const;               // get (large or small) icon mode
   zBOOL  SetTextMode( zBOOL bShowIconText ); // set text mode (on or off)
   zBOOL  GetTextMode( ) const;               // get text mode (on or off)
   void   ResizeToolBar( );                   // set size of the toolbar
   virtual CSize GetButtonSize( TBBUTTON *pData, int iButton );
   void   SizeToolBar( TBBUTTON *pData, int nCnt, int nLth,
                       zBOOL bVert = FALSE );
   void   GetButton( int nIndex, TBBUTTON *pButton ) const;
   void   SetButton( int nIndex, TBBUTTON *pButton );

   int    WrapToolBar( TBBUTTON *pData, int nCount, int nWidth );
   CSize  CalcSize( TBBUTTON *pData, int nCount );
   CSize  CalcLayout( DWORD nMode, int nLength = -1 );

   zLONG  GetLastSelection( zPCHAR pchText, zLONG lMaxTextLth );
   void   AddBarItem( zULONG ulId, zULONG ulType,
                      zCPCHAR cpcTag, zCPCHAR cpcText,
                      zCPCHAR cpcTipText, zCPCHAR cpcAction,
                      zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                      zCPCHAR cpcAttribute, zCPCHAR cpcScope );
   ZBarItem * LocateBarItem( zULONG ulId );
   ZBarItem * LocateBarItem( zCPCHAR cpcItemTag );
   zLONG  GetItemText( zCPCHAR cpcItemTag, zPCHAR pchText, zLONG lMaxTextLth );
   zLONG  SetItemText( zCPCHAR cpcItemTag, zCPCHAR cpcText );
   void   CleanupBarItems( );
   zSHORT SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn );
   virtual BOOL DestroyWindow( );

protected:
// Overrides

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZEditBar )
   virtual void OnUpdateCmdUI( CFrameWnd *pTarget,
                               BOOL bDisableIfNoHndler );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );
   afx_msg void OnNcPaint( );
   afx_msg void OnNcCalcSize( BOOL bCalcValidRects,
                              NCCALCSIZE_PARAMS *pncsp );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
// afx_msg void OnButtonDropDown( );
   afx_msg void OnControlReflectCommand( );
   afx_msg void OnSysColorChange( );
   afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
   afx_msg void OnWindowPosChanging( WINDOWPOS *pWndPos );
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   afx_msg LRESULT OnGetToolTipText( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnToolBarDropDown( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnToolBarSetSelect( WPARAM wParam, LPARAM lParam );
   afx_msg BOOL OnCmdMsg( UINT uId, int nCode, void *pExtra,
                          AFX_CMDHANDLERINFO *pHandlerInfo );
// afx_msg void ParentNotify( UINT message, LPARAM lParam );
   //}}AFX_VIRTUAL

protected:
// Implementation
   virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
   virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );

public:
   zBOOL    m_bColor;
   zBOOL    m_bVertical;
   zSHORT   m_nBtnCnt;
   zPCHAR   m_pchButtonInfo;
   UINT     *m_puButtons;
   zULONG   m_ulFirstBarCmdId;
   zSHORT   m_nBarItemCnt;
   zLONG    m_uLastDropDownId;
   zLONG    m_lLastSelect;
   CString  m_csLastSelect;
   CArray<ZBarItem *, ZBarItem *> m_BarItemList;

protected:

   CString  m_csDLL_Name;
   CString  m_csResName;
   CString  m_csBitmapFile;
   zBOOL    m_bMenuRemoved;
   zBOOL    m_bShowIconMode;     // small or large icon
   zBOOL    m_bShowIconText;     // to enable or disable the icon text

   zSHORT   m_nDropButtonSize;   // drop button width
   zSHORT   m_nTextHeight;       // text height
   zSHORT   m_nTextWidth;        // text width
   CSize    m_sizeSmallIcon;     // size of icon in small mode
   CSize    m_sizeLargeIcon;     // size of icon in large mode
   zBOOL    m_bOldFloating;      // patch for drawing vertical
   ZTB_List m_lstPopup;


   //{{AFX_MSG( ZEditBar )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// palette.h : interface of the ZPaletteBar class
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

class ZDCTL_CLASS ZPaletteBar : public CToolBar, public ZMapAct
{
   DECLARE_DYNAMIC( ZPaletteBar )

public:
   ZPaletteBar( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   virtual ~ZPaletteBar( );   // dtor

   void SetColumns( UINT nColumns );
   UINT GetColumns( ) { return( m_nColumns ); };

// Attributes
public:

// Operations
public:

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

protected:
   UINT    m_nColumns;

// Generated message map functions
protected:
   //{{AFX_MSG( ZPaletteBar )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#else

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef TB_SETEXTENDEDSTYLE
#define TB_SETEXTENDEDSTYLE     (WM_USER + 84) // For TBSTYLE_EX_*
#define TBSTYLE_EX_DRAWDDARROWS 0x00000001
#endif

// Class declaration.
#define NB_POSSIBLE_MODE 6

// This class manages an extended toolbar.
class ZDCTL_CLASS ZToolBar : public CToolBar, public ZMapAct
{
   DECLARE_DYNAMIC( ZToolBar )
public:
   // ctor
   ZToolBar( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

public:
   virtual ~ZToolBar( );

// virtual void   CreateZ( );
// virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
// virtual zSHORT MapToOI( zLONG lFlag = 0 );

   void OnGradientSelect( );
   enum eImageMode { SmallHot, SmallStandard, SmallDisable,
                     LargeHot, LargeStandard, LargeDisable };
protected:

// Data definition.
private:
   // Other useful needed flag.
   zBOOL m_bOldFloating;         // patch for drawing vertical

public:
protected:
   // ToolBar attribute.
   UINT  m_nShowIconMode;        // small or large icon
   zBOOL m_bShowIconText;        // to enable or disable the icon text

   // Size data.
   int   m_nDropButtonSize;      // drop button width
   int   m_nTextHeight;          // text height
   int   m_nTextWidth;           // text width
   CSize m_SmallIconSize;        // size of the icon in small mode
   CSize m_LargeIconSize;        // size of the icon in large mode

   // The image list.
   CImageList m_ImageList[ NB_POSSIBLE_MODE ];  // list of images

// Implementation Method definition.
private:
    // Method to calculate the current size of the button.
    CSize CalcButtonSize( );

public:

protected:
   void DrawGripper( CDC& dc ) const;              // draw gripper

   // Protected methods to set image information.
   void SetDisabledImageList( CImageList *pList ); // set disabled image list
   void SetHotImageList( CImageList *pList );      // set hover image list
   void SetStandardImageList( CImageList *pList ); // set normal image list

   virtual void AssignImageList( );                // set correct image list

// Functional Method definition.
private:
public:
   // Loading information.
   void ResizeToolBar( );                        // set size of the toolbar
   virtual zBOOL LoadToolBar( UINT ResourceId ); // load toolbar information

   // To set the image information.
   void SetImageList( eImageMode ImageMode,
                      CImageList& rList );   // set small/disable image list

   // Image list initialization method.
   virtual void InitImageList( );            // set correct image list

   // Mode related method.
   void SetIconMode( UINT nShowIconMode );   // set ( large or small ) icon mode
   UINT GetIconMode( ) const;                // get ( large or small ) icon mode
   void SetTextMode( zBOOL bShowIconText );  // set text mode ( on or off )
   zBOOL GetTextMode( ) const;               // get text mode ( on or off )

   // Useful button method.
   void SetButtonDropDown( int nID );     // set a drop arrow for a given button

protected:

// MFC Definition.
// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZToolBar )
public:
      virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
      virtual CSize GetButtonSize( TBBUTTON *pData, int iButton );
      virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
protected:
      virtual void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );
   //}}AFX_VIRTUAL

// Generated message map functions.
protected:
   //{{AFX_MSG( ZToolBar )
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS *pncsp );
   afx_msg void OnNcPaint( );
   afx_msg void OnSysColorChange( );
   afx_msg void OnWindowPosChanging( WINDOWPOS *pWndPos );
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

class CCoolCmdUI : public CCmdUI  // class private to this file!
{
public: // re-implementations only
   virtual void Enable( BOOL bOn );
   virtual void SetCheck( int nCheck );
};

// Manages an extended toolbar based on bitmap.
class ZBmpToolBar : public ZToolBar
{
   DECLARE_DYNAMIC( ZBmpToolBar )
public:
   // ctor
   ZBmpToolBar( ZSubtask *pZSubtask,        // ctor
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   ~ZBmpToolBar( );  // dtor

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
// virtual zSHORT MapToOI( zLONG lFlag = 0 );

   // Structure definition.
public:
   class CExBitmap
   {
   public:
      UINT m_nResourceId;
      COLORREF m_clrTransparent;
   };

protected:

   // Data definition.
private:
   // The bitmap image.
   CExBitmap m_256Image[ NB_POSSIBLE_MODE ];  // The 256 color bitmap id list.
   CExBitmap m_16Image[ NB_POSSIBLE_MODE ];   // The 16 color bitmap id list.

   // Implementation Method definition.
private:
   // Generic method to build an image list.
   void BuildImageList( CImageList& rImageList, const CExBitmap& rBitmap );


   // Functional Method definition.
public:
   // Bitmap adding method.
   // To set the small disable bitmap.
   void SetBitmap( UINT nBmpId, eImageMode ImageMode,
                   COLORREF clrBk, zBOOL b256 = TRUE );

   // Image list initialization method.
   virtual void InitImageList( );  // Set correct image list in the control.

protected:

   // MFC Definition.
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZBmpToolBar )
      public:
protected:
   //}}AFX_VIRTUAL

   // Generated message map functions.
protected:
   //{{AFX_MSG( ZBmpToolBar )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

// This class manage an extended toolbar based on bitmap.
class ZIconToolBar : public ZToolBar
{
   DECLARE_DYNAMIC( ZIconToolBar )
public:
   // ctor
   ZIconToolBar( ZSubtask *pZSubtask,        // ctor
                 CWnd     *pWndParent,
                 ZMapAct  *pzmaComposite,
                 zVIEW    vDialog,
                 zSHORT   nOffsetX,
                 zSHORT   nOffsetY,
                 zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   ~ZIconToolBar( );  // dtor

   // Implementation Method definition.
private:
   // Private useful method.
   HICON ConvertIconToGrayScale( HICON hIcon, CSize sizeImage ); // To convert the given icon into a gray scale icon.

protected:

   // Functionnal Method definition.
private:
public:
   // Icon adding method.
   // To add icon.
   void SetIcon( UINT nStandardIcon, UINT nDisableIcon = 0, UINT nHotIcon = 0 );

protected:
   // MFC Definition.
   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZIconToolBar )
public:
protected:
   //}}AFX_VIRTUAL

   // Generated message map functions.
protected:
   //{{AFX_MSG( ZIconToolBar )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDCTL_CLASS ZToolBar : public CToolBar, public ZMapAct
{
   DECLARE_DYNAMIC( ZToolBar )
public:
   // ctor
   ZToolBar( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

public:
   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
// virtual zSHORT MapToOI( zLONG lFlag = 0 );

   void OnGradientSelect( );
   virtual ~ZToolBar( );

   // Generated message map functions.
   // Function to dock bars next to one another (used typically on startup).
   void DockBarNextTo( CControlBar *pDockedBar, UINT nDockBarID = 0 );

   //  Self Explanatory
   zBOOL IsBarVisible( );

   // Three functions to change the contents of a button.
   // First one takes a pointer to a bitmap and a 0 based position
   void OnChangeButton( CBitmap  *pButton, zSHORT nPosition );

   // Second used a resource ID, position, and transparent color.
   // Transparency does not work with gradient, this is the fault of the
   // gradient code.
   void OnChangeButton( zSHORT nBitmapID, zSHORT nPosition,
                        COLORREF clrTransparent = RGB( 255, 0, 255 ) );

   // Last one changes to a solid color ( this was for a paint program ;0 )
   void OnChangeButton( COLORREF color, zSHORT nPosition );

   // Sets a button to have a drop down arrow next to it.
   // See code for further instructions on how to use this, or the MSDN
   void SetDropdown( zSHORT nPosition );

   // Push button mode means the last button pushed stays 'down' ... useful
   // if toolbar selects different 'modes'.

   // Set global behavior.
   void SetPushButton( zBOOL bPush );

   // Set specific individual buttons to group together this way.
   void SetPushButton( zBOOL bPush, zSHORT nPos );

   // Base function to turn the caption bar on and off.  When it is off, the
   // floating toolbar has a gripper and resizes slightly to allow dragging.
   void SetShowCaption( zBOOL bCaption );

   // Essentially calls SetWindowText.
   void SetCaption( CString csCaption );

   // Sets the colors for the gradient.
   void SetBackground( COLORREF clr1, COLORREF clr2 );

   // The gradient is drawn in OnEraseBackground - a solid color or bitmap
   // could just as easily be used.
   void SetUseGradient( zBOOL bUseGradient );

   // This allows you to pass in a menu to be shown as the context menu.
   void SetMenu( zSHORT nID );

   // These four turn on the extra menu items available, to allow the user
   // to turn pushbuttons on and off, the caption bar and the gradient fill.
   void SetMenuAll( zBOOL menu );
   void SetMenuPushbutton( zBOOL bPush );
   void SetMenuCaption( zBOOL bCaption );
   void SetMenuGradient( zBOOL bGradient );

   // Fairly obvious stuff
   zBOOL GetShowCaption( );
   zBOOL GetUseGradient( );
   zBOOL GetPushbutton( );

   void DockControlBarLeftOf( CToolBar *pBar, CToolBar *pLeftOf );
   void OnControlReflectCommand( );
   void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );
   LRESULT OnGetToolTipText( WPARAM wParam, LPARAM lParam );
   BOOL OnCmdMsg( UINT uId, int nCode, void *pExtra,
                  AFX_CMDHANDLERINFO *pHandlerInfo );
   void ParentNotify( UINT message, LPARAM lParam );

protected:
   //{{AFX_MSG( ZToolBar )
   afx_msg void OnWindowPosChanged( WINDOWPOS *pWndPos );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
   afx_msg void OnContextMenu( CWnd *pWnd, CPoint pt );
   afx_msg void OnTimer( UINT nIDEvent );
   afx_msg void OnLButtonDown( UINT nFlags, CPoint pt );
   afx_msg void ChangeButtons( );
   afx_msg void ChangeCaption( );
   afx_msg void ChangeGradient( );
   //}}AFX_MSG

   zBOOL     m_bMenuRemoved;   // Used to ensure our menu remove function is only called once
   zBOOL     m_bPushButton;    // Flag for push button behaviour
   zBOOL     m_bShowCaption;   // Caption visible/invisible flag
   zBOOL     m_bUseGradient;   // Gradient flag
   COLORREF  m_RGB1;           // Colors for gradient fill
   COLORREF  m_RGB2;
   zSHORT    m_nMenuID;        // ID of menu to display on right click.

   // These booleans determine which of these three options get added to
   // our context menu.
   zBOOL     m_bMenuButtons;
   zBOOL     m_bMenuCaption;
   zBOOL     m_bMenuGradient;
   zBOOL     *m_pButtonIdList; // array of zBOOLs to permit setting of push
                               // button behaviour for individual buttons
   zBOOL     m_bResizeBox;     // Used to keep track of the focus rect status in caption off mode
   zBOOL     m_bResizeDock;    // Used to keep track of focus rect when docking
   DECLARE_MESSAGE_MAP( )
};

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ControlBar( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define _SCB_MINIFRAME_CAPTION
#define _SCB_REPLACE_MINIFRAME

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
/////////////////////////////////////////////////////////////////////////

//#if !defined(__SIZECBAR_H__)
//#define __SIZECBAR_H__

//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000

#if defined(_SCB_MINIFRAME_CAPTION) && !defined(_SCB_REPLACE_MINIFRAME)
    #error "_SCB_MINIFRAME_CAPTION requires _SCB_REPLACE_MINIFRAME"
#endif

/////////////////////////////////////////////////////////////////////////
// ZSCBDockBar dummy class for access to protected members

class ZDCTL_CLASS ZSCBDockBar : public CDockBar
{
    friend class ZSizingControlBar;
};

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar control bar styles

#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010
#define SCBS_SIZECHILD      0x00000020

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar control bar

//#ifndef baseZSizingControlBar
//#define baseZSizingControlBar CControlBar
//#endif

class ZDCTL_CLASS ZSizingControlBar;
typedef CTypedPtrArray <CPtrArray, ZSizingControlBar*> ZSCBArray;

class ZDCTL_CLASS ZSizingControlBar : public CControlBar
{
   DECLARE_DYNAMIC(ZSizingControlBar);

// Construction
public:
   ZSizingControlBar();

   virtual BOOL Create( LPCTSTR lpszWindowName, CWnd* pParentWnd,
                        CSize sizeDefault, BOOL bHasGripper,
                        UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP );
   virtual BOOL Create( LPCTSTR lpszWindowName, CWnd* pParentWnd,
                        UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP );

// Attributes
public:
   const BOOL IsFloating() const;
   const BOOL IsHorzDocked() const;
   const BOOL IsVertDocked() const;
   const BOOL IsSideTracking() const;
   const BOOL GetSCBStyle() const {return m_dwSCBStyle;}

// Operations
public:
#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
   void EnableDocking(DWORD dwDockStyle);
#endif
   virtual void LoadState(LPCTSTR lpszProfileName);
   virtual void SaveState(LPCTSTR lpszProfileName);
   static void GlobalLoadState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
   static void GlobalSaveState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
   void SetSCBStyle(DWORD dwSCBStyle)
        {m_dwSCBStyle = (dwSCBStyle & ~SCBS_EDGEALL);}

// Overridables
   virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZSizingControlBar)
   public:
   virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
   virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZSizingControlBar();

protected:
   // implementation helpers
   UINT GetEdgeHTCode(int nEdge);
   BOOL GetEdgeRect(CRect rcWnd, UINT nHitTest, CRect& rcEdge);
   virtual void StartTracking(UINT nHitTest, CPoint point);
   virtual void StopTracking();
   virtual void OnTrackUpdateSize(CPoint& point);
   virtual void OnTrackInvertTracker();
   virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
   virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

   virtual void AlignControlBars();
   void GetRowInfo(int& nFirst, int& nLast, int& nThis);
   void GetRowSizingBars(ZSCBArray& arrSCBars);
   void GetRowSizingBars(ZSCBArray& arrSCBars, int& nThis);
   BOOL NegotiateSpace(int nLengthTotal, BOOL bHorz);

protected:
   DWORD   m_dwSCBStyle;
   UINT    m_htEdge;

   CSize   m_szHorz;
   CSize   m_szVert;
   CSize   m_szFloat;
   CSize   m_szMinHorz;
   CSize   m_szMinVert;
   CSize   m_szMinFloat;
   int     m_nTrackPosMin;
   int     m_nTrackPosMax;
   int     m_nTrackPosOld;
   int     m_nTrackEdgeOfs;
   BOOL    m_bTracking;
   BOOL    m_bKeepSize;
   BOOL    m_bParentSizing;
   BOOL    m_bDragShowContent;
   UINT    m_nDockBarID;
   int     m_cxEdge;

// Generated message map functions
protected:
   //{{AFX_MSG(ZSizingControlBar)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnNcPaint();
   afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
   afx_msg LRESULT OnNcHitTest(CPoint point);
   afx_msg void OnCaptureChanged(CWnd *pWnd);
   afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   afx_msg void OnPaint();
   afx_msg void OnClose();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   //}}AFX_MSG
   afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

   DECLARE_MESSAGE_MAP()

#ifdef _SCB_REPLACE_MINIFRAME
   friend class ZSCBMiniDockFrameWnd;
#endif //_SCB_REPLACE_MINIFRAME
};

#ifdef _SCB_REPLACE_MINIFRAME
#ifndef _SCB_MINIFRAME_CAPTION
/////////////////////////////////////////////////////////////////////////
// ZSCBDockContext dockcontext

class ZDCTL_CLASS ZSCBDockContext : public CDockContext
{
public:
// Construction
   ZSCBDockContext(CControlBar* pBar) : CDockContext(pBar) {}

// Drag Operations
   virtual void StartDrag(CPoint pt);
};
#endif //_SCB_MINIFRAME_CAPTION

/////////////////////////////////////////////////////////////////////////
// ZSCBMiniDockFrameWnd miniframe

#ifndef baseZSCBMiniDockFrameWnd
#define baseZSCBMiniDockFrameWnd CMiniDockFrameWnd
#endif

class ZDCTL_CLASS ZSCBMiniDockFrameWnd : public baseZSCBMiniDockFrameWnd
{
   DECLARE_DYNCREATE(ZSCBMiniDockFrameWnd)

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZSCBMiniDockFrameWnd)
   public:
   virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
   //}}AFX_VIRTUAL

// Implementation
public:
   ZSizingControlBar* GetSizingControlBar();

   //{{AFX_MSG(ZSCBMiniDockFrameWnd)
   afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
   afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
   afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};
#endif //_SCB_REPLACE_MINIFRAME

//#endif // !defined(__SIZECBAR_H__)

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

//#if !defined(__SCBARG_H__)
//#define __SCBARG_H__

//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////
// ZSCBButton (button info) helper class

class ZDCTL_CLASS ZSCBButton
{
public:
   ZSCBButton();

   void Move(CPoint ptTo) {ptOrg = ptTo; };
   CRect GetRect() { return CRect(ptOrg, CSize(11, 11)); };
   void Paint(CDC* pDC);

   BOOL    bPushed;
   BOOL    bRaised;

protected:
   CPoint  ptOrg;
};

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBar control bar

//#ifndef baseZSizingControlBarG
//#define baseZSizingControlBarG ZSizingControlBar
//#endif

class ZDCTL_CLASS ZSizingControlBarG : public ZSizingControlBar
{
   DECLARE_DYNAMIC(ZSizingControlBarG);

// Construction
public:
   ZSizingControlBarG();

// Attributes
public:
   virtual BOOL HasGripper() const;

// Operations
public:

// Overridables
   virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZSizingControlBarG)
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZSizingControlBarG();

protected:
   // implementation helpers
   virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
   virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

protected:
   int     m_cyGripper;

   ZSCBButton m_biHide;

// Generated message map functions
protected:
   //{{AFX_MSG(ZSizingControlBarG)
   afx_msg LRESULT OnNcHitTest(CPoint point);
   afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

//#endif // !defined(__SCBARG_H__)

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

//#if !defined(__SCBARCF_H__)
//#define __SCBARCF_H__

//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000
// scbarcf.h : header file
//

/////////////////////////////////////////////////////////////////////////
// ZSizingControlBarCF

//#ifndef baseZSizingControlBarCF
//#define baseZSizingControlBarCF ZSizingControlBarG
//#endif

class ZDCTL_CLASS ZSizingControlBarCF : public ZSizingControlBarG
{
   DECLARE_DYNAMIC(ZSizingControlBarCF)

// Construction
public:
   ZSizingControlBarCF();

// Overridables
   virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Implementation
protected:
   // implementation helpers
   virtual void NcPaintGripper(CDC* pDC, CRect rcClient);

protected:
   BOOL    m_bActive; // a child has focus
   CString m_csFontFace;

// Generated message map functions
protected:
   //{{AFX_MSG(ZSizingControlBarCF)
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   //}}AFX_MSG
   afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////

//#endif // !defined(__SCBARCF_H__)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//#define baseZControlBarG ZSizingControlBarG

class ZDCTL_CLASS ZControlBarG : public ZSizingControlBarG, public ZMapAct
{
   DECLARE_DYNAMIC( ZControlBarG )

public:

   // ctor
   ZControlBarG( ZSubtask *pZSubtask,
                 CWnd     *pWndParent,
                 ZMapAct  *pzmaComposite,
                 zVIEW    vDialog,
                 zSHORT   nOffsetX,
                 zSHORT   nOffsetY,
                 zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   virtual ~ZControlBarG( );   // dtor

   virtual zSHORT MapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual void   CreateZ( );
// virtual zBOOL  IsEventHandled( zLONG lEvent ); use default method
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   virtual BOOL DestroyWindow( );

protected:
// Overrides

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZControlBar )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpwndpos );
   //}}AFX_VIRTUAL

public:
   zPCHAR m_pchControlInfo;
   zLONG  m_lSubtype;

protected:

   //{{AFX_MSG( ZControlBar )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

//#define baseZControlBarC ZSizingControlBarCF

class ZDCTL_CLASS ZControlBarC : public ZSizingControlBarCF, public ZMapAct
{
   DECLARE_DYNAMIC( ZControlBarC )

public:

   // ctor
   ZControlBarC( ZSubtask *pZSubtask,
                 CWnd     *pWndParent,
                 ZMapAct  *pzmaComposite,
                 zVIEW    vDialog,
                 zSHORT   nOffsetX,
                 zSHORT   nOffsetY,
                 zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   virtual ~ZControlBarC( );   // dtor

   virtual zSHORT MapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual void   CreateZ( );
// virtual zBOOL  IsEventHandled( zLONG lEvent ); use default method
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual BOOL DestroyWindow( );

protected:
// Overrides

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZControlBar )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   //}}AFX_VIRTUAL

public:
   zPCHAR m_pchControlInfo;
   zLONG  m_lSubtype;

protected:

   //{{AFX_MSG( ZControlBar )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlbar_h__ sentry.
