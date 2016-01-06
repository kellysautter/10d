/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctltxt.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctltxt_h__  // Sentry, use file only if it's not already included
#define __zdctltxt_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Text( ZSubtask *pZSubtask,
      CWnd     *pWndParent,
      ZMapAct  *pzmaComposite,
      zVIEW    vDialog,
      zSHORT   nOffsetX,
      zSHORT   nOffsetY,
      zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

class ZTextRotator;

/////////////////////////////////////////////////////////////////////////////
// CControlToolTip window

class CControlToolTip : public CWnd
{
public:
   CControlToolTip( );
   virtual ~CControlToolTip( );

   static void RegisterWindowClass( );

   zBOOL Attach( CWnd *pControl );
   zBOOL Detach( );

   void SetBkgndColor( zBOOL bUse = FALSE,
                       COLORREF clr = ::GetSysColor( COLOR_INFOBK ) );
   void SetPosition( int iPosition = 0 );
   void SetText( zCPCHAR cpcText = "" );
   void SetTextColor( COLORREF clr = ::GetSysColor( COLOR_INFOTEXT ) );
   void ShowOnlyOnMouseClickDown( zBOOL bShow );

   void UseLeftMouseButton( zBOOL bUse );

   // inline functions
   int GetPosition( ) { return m_iPosition; }
   CString GetText( ) { return m_csText; }
   BOOL ShowOnlyOnMouseClickDown( ) { return m_bShowOnlyOnMouseClickDown; }
   BOOL IsAttached( ) { return ( m_pControl != 0 ); }
   BOOL IsLeftMouseButtonUsed( ) { return m_bUseLeftButton; }

public:
   //{{AFX_VIRTUAL( CControlToolTip )
   //}}AFX_VIRTUAL

protected:
   //{{AFX_MSG( CControlToolTip )
   afx_msg void OnTimer( UINT nIDEvent );
   afx_msg void OnPaint( );
   //}}AFX_MSG

protected:
   CWnd     *m_pControl;
   CString   m_csText;
   CRect     m_rectText;
   int       m_iPosition; // 0 = OVER CONTROL
                          // 1 = UNDER CONTROL
                          // 2 = ABOVE CONTROL
   COLORREF  m_clrText;
   COLORREF  m_clrBkgnd;
   zBOOL      m_bUseBgndColor;
   zBOOL      m_bShowOnlyOnMouseClickDown;
   zBOOL      m_bUseLeftButton;

   CSize GetMultiLineTextExtent( CClientDC *pDC, zCPCHAR cpcText );
   int   GetNumberOfLines( zCPCHAR cpcText = NULL );
   zBOOL  IsMouseInControl( );
   void  Show( );

   DECLARE_MESSAGE_MAP( )
};

// Zeidon ZText standard control
class ZDCTL_CLASS ZText : public CStatic, public ZMapAct
{
friend class ZTextRotator;

   DECLARE_DYNAMIC( ZText )
public:
   // ctor
   ZText( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZText( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // get text from OI and transfer to entry control
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zLONG  SetZCtrlProperty( zLONG   lPropertyType,
                                    zULONG  ulNumericProperty,
                                    zCPCHAR cpcStringProperty );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );
   virtual void   ParentResize( ZMapAct * pParent, CSize *pSize );

public:
   virtual BOOL DestroyWindow( );
protected:
   virtual void PreSubclassWindow( );
   virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
protected:
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT uCtlColor );
// afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
   afx_msg void OnPaint( );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   afx_msg void OnEnable( BOOL bEnable );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMsg );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnTimer( UINT nIDEvent );

public:

   // Color functions.
   ZText &SetBkColor( COLORREF clr = ::GetSysColor( COLOR_3DFACE ) );
   ZText &SetDefaultBkColor( );
   ZText &SetDefaultTextBackColor( );
   ZText &SetDefaultTextColor( );
   ZText &SetHighLightColor( COLORREF clr = ::GetSysColor( COLOR_HIGHLIGHT ) );
   ZText &SetTextBackColor( COLORREF clr = ::GetSysColor( COLOR_3DFACE ) );
   ZText &SetTextColor( COLORREF clr = ::GetSysColor( COLOR_WINDOWTEXT ) );

   // Font functions.
   ZText &SetFontBold( zBOOL bBold );
   ZText &SetFontItalic( zBOOL bItalic );
   ZText &SetFontName( zCPCHAR szFaceName );
   ZText &SetFontSize( zLONG lSize );
   ZText &SetFontStrikeout( zBOOL bStrikeout );
   ZText &SetFontUnderline( zBOOL bUnderline );

   ZText &FlashBackground( zBOOL bActivate, zULONG ulTime = 500 );
   ZText &FlashText( zBOOL bActivate, zULONG ulTime = 500 );

   void ForceNoOwnerDraw( zBOOL bForce );

   zBOOL   GetBorder( );
   zULONG  GetHorzAlignment( );
   zULONG  GetHorzAlignmentIndex( );
   zBOOL   GetSunken( );
   CString GetText( );
   zULONG  GetVertAlignment( );
   zULONG  GetVertAlignmentIndex( );

   ZText &SetAlignment( zULONG ulAlign = HORZ_LEFT | VERT_TOP );
   ZText &SetAngle( zULONG ulAngle = 0 );
   ZText &SetBackImage( HINSTANCE hInst, zULONG ulID = -1 );
   ZText &SetBorder( zBOOL bSet );
   ZText &SetCursor( HCURSOR hCursor );
   ZText &SetCursor( zULONG ulCursorID );
   ZText &SetDisabled( zBOOL bSet );
   ZText &SetLink( zBOOL bSet );
   ZText &SetLinkCursor( HCURSOR hCursor );
   ZText &SetLinkCursor( zULONG ulCursorID );
   ZText &SetLowerCase( zBOOL bSet );
   ZText &SetText( zCPCHAR szText );
   ZText &SetSunken( zBOOL bSet );
   ZText &SetToolTipPosition( int iPosition = 0 );
   ZText &SetToolTipText( zCPCHAR cpcText = "" );
   ZText &SetUpperCase( zBOOL bSet );
   ZText &SetVerticalText( zBOOL bSet );
   ZText &ShowToolTipOnlyOnMouseClick( zBOOL bUse );
   ZText &StretchBitmap( zBOOL bStretch );
   ZText &UseBevelLine( zBOOL bUse );
   ZText &UseHighLighting( zBOOL bUse );
   ZText &UseHighLightingOnlyWithToolTip( zBOOL bUse );
   ZText &UseLeftMouseButtonForToolTip( zBOOL bUse );
   ZText &UseToolTip( zBOOL bUse );

   // inline functions.
   COLORREF GetBkColor( );
   COLORREF GetHighLightColor( );
   COLORREF GetTextBackColor( );
   COLORREF GetTextColor( );
   zBOOL    IsDefaultBkColor( );
   zBOOL    IsDefaultTextBackColor( );
   zBOOL    IsDefaultTextColor( );

   zBOOL    GetFontBold( );
   zBOOL    GetFontItalic( );
   CString  GetFontName( );
   zLONG    GetFontSize( );
   zBOOL    GetFontUnderline( );

   zULONG   GetAlignment( );
   zULONG   GetAngle( );
   zBOOL    GetBevelLine( );
   zBOOL    GetLink( );
   zBOOL    GetVerticalText( );
   zBOOL    IsBitmapStretched( );
   zBOOL    IsDisabled( );
   zBOOL    IsLowerCase( );
   zBOOL    IsToolTipUsed( );
   zBOOL    IsUpperCase( );
   zBOOL    UseHighLightingOnlyWithToolTip( );
   zBOOL    UseToolTipOnLeftMouseClick( );
   zBOOL    UseToolTipWithMouseClick( );
   zBOOL    SetRedrawLocal( zBOOL bRedraw );
   void     RedrawWindowLocal( );

public:
   enum
   {
      HORZ_LEFT = 0x01,
      HORZ_CENTER = 0x02,
      HORZ_RIGHT = 0x04,
      VERT_TOP = 0x08,
      VERT_CENTER = 0x10,
      VERT_BOTTOM = 0x20
   };

private:

// HBITMAP m_hEmptyBitmap1;
// HBITMAP m_hEmptyBitmap2;

public:

#define zTEXT_BOLD         (BYTE) 0x01
#define zTEXT_ITALIC       (BYTE) 0x02
#define zTEXT_STRIKEOUT    (BYTE) 0x04
#define zTEXT_UNDERLINE    (BYTE) 0x08
   BYTE     m_fTextFlags;

   zLONG    m_lColor;
   zLONG    m_lSize;
   zLONG    m_lBorderStyle;
   zLONG    m_lBorderColor;
   zLONG    m_lEscapement;
   zLONG    m_lType;
   zBOOL    m_bLeft;
   zBOOL    m_bCenter;
   zBOOL    m_bRight;
   zCHAR    m_chMultiLine;
   UINT     m_uFormat;
   zCHAR    m_szFaceName[ LF_FACESIZE ];

   COLORREF          m_clrBackColor;
   COLORREF          m_clrTextForeColor;
   COLORREF          m_clrTextBackColor;
   COLORREF          m_clrHighLight;
   COLORREF          m_clrSaveTextColor;
   HBRUSH            m_hBackBrush;
   zBOOL             m_bForceNoOwnerDraw;
   zBOOL             m_bUseDefaultBackColor;
   zBOOL             m_bUseDefaultTextBackColor;
   zBOOL             m_bUseDefaultTextColor;
   zBOOL             m_bLink;
   zBOOL             m_bUseBevelLine;
   zBOOL             m_bUseHighLighting;
   zBOOL             m_bVerticalText;
   zBOOL             m_bInside;
   zBOOL             m_bTextFlash;
   zBOOL             m_bBackFlash;
   zBOOL             m_bFlashTextState;
   zBOOL             m_bFlashBackState;
   zBOOL             m_bUseToolTip;
   zBOOL             m_bUseHighLightingOnlyWithToolTip;
   zBOOL             m_bBlockMessage;
   zBOOL             m_bLowerCase;
   zBOOL             m_bUpperCase;
   zBOOL             m_bStretchBitmap;
   zBOOL             m_bFixTopLeft;
   zBOOL             m_bRedrawLocal;
   LOGFONT           m_lf;
   HCURSOR           m_hCursor;
   HCURSOR           m_hCursorOld;
   HBITMAP           m_hBackImage;
   zULONG            m_ulAlignment;
   zULONG            m_ulAngle;
   CControlToolTip   m_ToolTipCtrl;

   void    DrawAlignedText( CDC *pDC, zCPCHAR cpcText,
                            RECT Rect, zULONG ulAlignment );
   int     GetWindowText( zPCHAR pchStringBuf, int nMaxCount );
   void    GetWindowText( CString& rString );
   zSHORT  IsMultiLineString( zCPCHAR cpcText );
   CString MakeVerticalText( zCPCHAR cpcText = 0 );
   zBOOL    ReconstructFont( );
   void    SetWindowText( zCPCHAR cpcString );
   void    TrackMouse( zBOOL bHover, zBOOL bLeave );

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CNewLabel.h
//
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Date of creation  : 20/07/2000
// Last modification : 07/08/2000
/////////////////////////////////////////////////////////////////////////////

// Remark: Rotation works only on Windows NT/2000 !!!!!

/////////////////////////////////////////////////////////////////////////////
// class ZTextRotator

class ZTextRotator
{
friend class ZText;

public:
   ZTextRotator( ZText *pText, HDC hSrcDC, zCPCHAR cpcText = "",
                 zULONG ulHorzAlignment = DT_LEFT,
                 zULONG ulVertAlignment = 0 );
   virtual ~ZTextRotator( );

// zBOOL BitBltText( int X, int Y );
   zBOOL BitBltText( HDC hRotatedMemDC, CRect rectSrc, zULONG ulAngle );
// void GetRotatedDC( CDC *pDC );
// void GetRotatedRect( RECT *pRect );
   HDC RotateText( CRect& rectClient, zULONG ulAngle = 0 );

// HDC   GetRotatedDC( );
// CRect GetRotatedRect( );

   void DrawBevelLine( zBOOL bDraw );
   void SetBackgroundColor( COLORREF clr );
   void SetDisabledText( zBOOL bDisable );
   void SetHorzAlignment( zULONG ulHorzAlignment = DT_LEFT );
   void SetText( zCPCHAR cpcText );
   void SetTextColor( COLORREF clr );
   void SetVertAlignment( zULONG ulVertAlignment = 0 );

protected:

   ZText    *m_pText;
   HDC      m_hSrcDC;
// HDC      m_hRotatedMemDC;
   HBITMAP  m_hRotatedBitmapMemDC;
   CRect    m_rectRotated;
   zULONG   m_ulHorzAlignment;
   zULONG   m_ulVertAlignment;
// zULONG   m_ulAngle;
   zBOOL    m_bDisabled;
   zBOOL    m_bDrawBevelLine;
   CString  m_csText;
   COLORREF m_clrBackground;
   COLORREF m_clrText;

   void  AngleBlt( HDC hSrcDC, HDC hTgtDC,
                   RECT rectSrc, zULONG ulAngle );
// void  Clear( );
   CSize GetLargestTextSize( HDC hDC, CStringArray *parrayText );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// ColorPicker.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included. If
// the source code in  this file is used in any commercial application
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZClrPicker window

#define CP_MODE_TEXT 1  // edit text color
#define CP_MODE_BK   2  // edit background color (default)

class ZDCTL_CLASS ZClrPicker : public CButton, public ZMapAct
{
   DECLARE_DYNAMIC( ZClrPicker )
public:
   // ctor
   ZClrPicker( ZSubtask *pZSubtask,
               CWnd     *pWndParent,
               ZMapAct  *pzmaComposite,
               zVIEW    vDialog,
               zSHORT   nOffsetX,
               zSHORT   nOffsetY,
               zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   // dtor
   virtual ~ZClrPicker( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  GetZCtrlProperty( zLONG   lPropertyType,
                                    zPULONG pulNumericProperty,
                                    zPCHAR  pchStringProperty,
                                    zLONG   lMaxLth );
   virtual zLONG  SetZCtrlProperty( zLONG   lPropertyType,
                                    zULONG  ulNumericProperty,
                                    zCPCHAR cpcStringProperty );

// Operations
public:
   COLORREF GetColor( );
   void     SetColor( COLORREF clrColor );

   void     SetDefaultText( zCPCHAR szDefaultText );
   void     SetCustomText( zCPCHAR szCustomText );

   void     SetTrackSelection( zBOOL bTracking = TRUE );
   zBOOL    GetTrackSelection( );

   void     SetSelectionMode( UINT nMode );
   UINT     GetSelectionMode( );

   void     SetBkColor( COLORREF clrColorBk );
   COLORREF GetBkColor( );

   void     SetTextColor( COLORREF clrColorText );
   COLORREF GetTextColor( );

public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZClrPicker )
public:
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
protected:
   virtual void PreSubclassWindow( );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( ZClrPicker )
   afx_msg BOOL OnClicked( );
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   //}}AFX_MSG
   afx_msg LONG OnSelEndOK( WPARAM wParam, LPARAM lParam );
   afx_msg LONG OnSelEndCancel( WPARAM wParam, LPARAM lParam );
   afx_msg LONG OnSelChange( WPARAM wParam, LPARAM lParam );

   void SetWindowSize( );

// Attributes.
protected:
   zSHORT   m_nNbrColors;
   zBOOL    m_bActive;               // is the dropdown active?
   zBOOL    m_bTrackSelection;       // track color changes?
   COLORREF m_clrColorBk;
   COLORREF m_clrColorText;
   UINT     m_nSelectionMode;
   CRect    m_rectArrow;
   CString  m_csDefaultText;
   CString  m_csCustomText;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

// ClrPopup.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included. If
// the source code in this file is used in any commercial application
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.


// ZClrPopup messages
#define CPN_SELCHANGE       WM_USER + 1001  // ClrPicker Selection change
#define CPN_DROPDOWN        WM_USER + 1002  // ClrPicker drop down
#define CPN_CLOSEUP         WM_USER + 1003  // ClrPicker close up
#define CPN_SELENDOK        WM_USER + 1004  // ClrPicker end OK
#define CPN_SELENDCANCEL    WM_USER + 1005  // ClrPicker end (cancelled)

// forward declaration
class ZClrPicker;

// To hold the colors and their names
typedef struct
{
   COLORREF clrColor;
   TCHAR    *pchName;
} ColorTableEntry;

/////////////////////////////////////////////////////////////////////////////
// ZClrPopup window

class ZClrPopup : public CWnd
{
// Construction
public:
   ZClrPopup( );
   ZClrPopup( CPoint p, COLORREF clrColor, CWnd *pParentWnd,
              zCPCHAR cpcDefaultText = 0, zCPCHAR cpcCustomText = 0 );
   void Initialize( );

// Attributes
public:

// Operations
public:
   BOOL Create( CPoint p, COLORREF clrColor, CWnd *pParentWnd,
                zCPCHAR cpcDefaultText = 0, zCPCHAR cpcCustomText = 0 );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZClrPopup )
   public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZClrPopup( );

protected:
   zBOOL GetCellRect( zLONG lIndex, const LPRECT& rect );
   void  FindCellFromColor( COLORREF clrColor );
   void  SetWindowSize( );
   void  CreateToolTips( );
   void  ChangeSelection( zLONG lIndex );
   void  EndSelection( zSHORT nMessage );
   void  DrawCell( CDC *pDC, zLONG lIndex );

   COLORREF GetColor( zLONG lIndex );
   zCPCHAR  GetColorName( zLONG lIndex );
   zLONG    GetIndex( zLONG lRow, zLONG lCol ) const;
   zLONG    GetRow( zLONG lIndex ) const;
   zLONG    GetColumn( zLONG lIndex ) const;

// protected attributes
protected:
// static ColorTableEntry m_clrColors[ ];
   zSHORT       m_nNbrColors;
   zLONG        m_lNbrCols;
   zLONG        m_lNbrRows;
   zLONG        m_lCurrentSel;
   zLONG        m_lChosenColorSel;
   zSHORT       m_nBoxSize;
   zSHORT       m_nMargin;
   CString      m_csDefaultText;
   CString      m_csCustomText;
   CRect        m_CustomTextRect;
   CRect        m_DefaultTextRect;
   CRect        m_WindowRect;
   CFont        m_Font;
   CPalette     m_Palette;
   COLORREF     m_clrInitialColor;
   COLORREF     m_clrColor;
   CToolTipCtrl m_ToolTip;
   CWnd         *m_pParent;

   // Generated message map functions
protected:
   //{{AFX_MSG( ZClrPopup )
   afx_msg void OnNcDestroy( );
   afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
   afx_msg void OnPaint( );
   afx_msg void OnMouseMove( UINT nFlags, CPoint pt );
   afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
   afx_msg BOOL OnQueryNewPalette( );
   afx_msg void OnPaletteChanged( CWnd *pFocusWnd );
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnActivateApp( BOOL bActive, DWORD hTask );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctltxt_h__ sentry.
