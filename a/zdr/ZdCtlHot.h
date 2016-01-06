// HyperLink.h : header file
//
//
// HyperLink static control.
//
// Copyright Giancarlo Iovino, 1997 (giancarlo@saria.com)
// This code is based on CHyperlink by Chris Maunder.
// Feel free to use and distribute. May not be sold for profit.

#ifndef __zdctlhot_h__  // Sentry, use file only if it's not already included
#define __zdctlhot_h__

// Structure used to get/set hyperlink colors
typedef struct tagHYPERLINKCOLORS
{
   COLORREF crLink;
   COLORREF crActive;
   COLORREF crVisited;
   COLORREF crHover;

} HYPERLINKCOLORS;

/////////////////////////////////////////////////////////////////////////////
// CHyperLink window

class CHyperLink : public CStatic
{
   DECLARE_DYNAMIC( CHyperLink )

public:
// Link styles
   static const DWORD StyleUnderline;
   static const DWORD StyleUseHover;
   static const DWORD StyleAutoSize;
   static const DWORD StyleDownClick;
   static const DWORD StyleGetFocusOnClick;
   static const DWORD StyleNoHandCursor;
   static const DWORD StyleNoActiveColor;

// Construction/destruction
   CHyperLink( );
   virtual ~CHyperLink( );

// Attributes
public:

// Operations
public:
   static void GetColors( HYPERLINKCOLORS& linkColors );

   static HCURSOR GetLinkCursor( );
   static void SetLinkCursor( HCURSOR hCursor );

   static void SetColors( COLORREF crLinkColor, COLORREF crActiveColor,
                          COLORREF crVisitedColor, COLORREF crHoverColor = -1 );
   static void SetColors( HYPERLINKCOLORS& colors );

   void SetURL( CString csURL );
   CString GetURL( ) const;

   DWORD GetLinkStyle( ) const;
   BOOL ModifyLinkStyle( DWORD dwRemove, DWORD dwAdd, BOOL bApply = TRUE );

   void SetWindowText( zCPCHAR cpcText );
   void SetFont( CFont *pFont );

   BOOL IsVisited( ) const;
   void SetVisited( BOOL bVisited = TRUE );

   // Use this if you want to subclass and also set different URL
   BOOL SubclassDlgItem( UINT uID, CWnd *pParent, zCPCHAR cpcURL = 0 );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CHyperLink )
public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );

protected:
   virtual void PreSubclassWindow( );
   //}}AFX_VIRTUAL

// Implementation
protected:
   static void SetDefaultCursor( );
   static LONG GetRegKey( HKEY hKey, zCPCHAR cpcSubkey, zPCHAR pchReturnData );
   static void ReportError( int nError );
   static HINSTANCE GotoURL( zCPCHAR cpcURL, int nShowCmd );

   void AdjustWindow( );
   void FollowLink( );
   inline void SwitchUnderline( );

// Protected attributes
protected:
   static COLORREF g_crLinkColor;      // Link normal color
   static COLORREF g_crActiveColor;    // Link active color
   static COLORREF g_crVisitedColor;   // Link visited color
   static COLORREF g_crHoverColor;     // Hover color
   static HCURSOR  g_hLinkCursor;      // Hyperlink mouse cursor

   BOOL     m_bLinkActive;             // Is the link active?
   BOOL     m_bOverControl;            // Is cursor over control?
   BOOL     m_bVisited;                // Has link been visited?
   DWORD    m_dwStyle;                 // Link styles
   CString  m_csURL;                   // Hyperlink URL string
   CFont    m_Font;                    // Underlined font (if required)
   CToolTipCtrl m_ToolTip;             // The link tooltip

   // Generated message map functions
protected:
   //{{AFX_MSG( CHyperLink )
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT uCtlColor );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage );
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pOldWnd );
   afx_msg void OnKillFocus( CWnd* pNewWnd );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepeatCnt, UINT uFlags );
   afx_msg LRESULT OnNcHitTest( CPoint pt );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctlhot_h__ sentry.