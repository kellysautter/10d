/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctldib.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctldib_h__  // Sentry, use file only if it's not already included
#define __zdctldib_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

/////////////////////////////////////////////////////////////////////////////

// Handle to a DIB
DECLARE_HANDLE( HDIB );

// DIB constants
#define PALVERSION   0x300

#define WM_DOREALIZE   (WM_USER + 0)

// DIB Macros

// Function prototypes
BOOL      WINAPI  PaintDIB( HDC, LPRECT, HDIB, LPRECT, CPalette *pPal );
BOOL      WINAPI  CreateDIB_Palette( HDIB hDIB, CPalette *cPal );
HGLOBAL   WINAPI  CopyHandle( HGLOBAL h );

BOOL      WINAPI  SaveDIB ( HDIB hDib, CFile& file );
HDIB      WINAPI  ReadDIB_File( CFile& file );


class ZDCTL_CLASS ZDibView;

/////////////////////////////////////////////////////////////////////////////

// afx_msg void OnPaletteChanged( CWnd *pFocusWnd );
// afx_msg BOOL OnQueryNewPalette( );

/////////////////////////////////////////////////////////////////////////////

class ZDibDoc : public CDocument
{
   DECLARE_DYNAMIC( ZDibDoc )

public:
   ZDibDoc( ZDibView *pView );
   virtual ~ZDibDoc( );

// Attributes
public:
   ZDibView *m_pDibView;

// HDIB GetHDIB( ) const { return m_hDIB; }
// CPalette *GetDocPalette( ) const { return m_palDIB; }
// CSize GetDocSize( ) const { return m_sizeDoc; }

// Operations
public:
// void ReplaceHDIB( HDIB hDIB );
// void InitDIB_Data( );

// Implementation
protected:
// virtual BOOL OnSaveDocument( LPCTSTR lpszPathName );
// virtual BOOL OnOpenDocument( LPCTSTR lpszPathName );

protected:
// HDIB m_hDIB;
// CPalette *m_palDIB;
// CSize m_sizeDoc;

#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif
protected:
   virtual BOOL OnNewDocument( );

// Generated message map functions
protected:
   //{{AFX_MSG( ZDibDoc )
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

class ZDCTL_CLASS ZDibView : public CScrollView, public ZMapAct
{
   DECLARE_DYNAMIC( ZDibView )

public:
   // ctor
   ZDibView( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZDibView( );

// Attributes
public:
   ZDibDoc *m_pZDoc;
   zBOOL m_bModified;
   HDIB  m_hDIB;
   CPalette *m_palDIB;
   CSize m_sizeDIB;
   CString m_csBitmapFileName;

   int   m_nMapMode;
   CSize m_totalLog;           // total size in logical units (no rounding)
   CSize m_totalDev;           // total size in device units
   CSize m_pageDev;            // per page scroll size in device units
   CSize m_lineDev;            // per line scroll size in device units

// Operations
public:
   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );

   void ReplaceHDIB( HDIB hDIB );
   void InitDIB_Data( );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
// void SetScrollSizes( int nMapMode, CSize sizeTotal,
//                      const CSize& sizePage, const CSize& sizeLine );
// afx_msg zBOOL OnScroll( UINT uScrollCode, UINT uThumbPos, zBOOL bDoScroll = TRUE );
// afx_msg zBOOL OnScrollBy( CSize sizeScroll, zBOOL bDoScroll = TRUE );

   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
// afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
// afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnDraw( CDC *pDC );

// Implementation
public:

   virtual void OnInitialUpdate( );
   virtual void OnActivateView( BOOL bActivate, CView *pActivateView,
                                CView *pDeactiveView );

   // Printing support
protected:
// virtual BOOL OnPreparePrinting( CPrintInfo *pInfo );

// Generated message map functions
protected:
   //{{AFX_MSG( ZDibView )
// afx_msg void OnEditCopy( );
// afx_msg void OnUpdateEditCopy( CCmdUI *pCmdUI );
// afx_msg void OnEditPaste( );
// afx_msg void OnUpdateEditPaste( CCmdUI *pCmdUI );
// afx_msg LRESULT OnDoRealize( WPARAM wParam, LPARAM lParam ); // user message
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctldib_h__ sentry.

