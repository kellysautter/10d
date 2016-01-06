/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlbtn.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlbtn_h__  // Sentry, use file only if it's not already included
#define __zdctlbtn_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
PushButton( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

/////////////////////////////////////////////////////////////////////////////
// colorBtn.h : header file for the CColorButton class
//
// Written by Bob Ryan (ryan@cyberzone.net)
// Copyright (c) 1998.
//
// This code may be freely distributable in any application.  If
// you make any changes to the source, please let me know so that
// I might make a better version of the class.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////

// Zeidon ZPushButton standard control
class ZDCTL_CLASS ZPushButton : public CButton, public ZMapAct
{
   DECLARE_DYNAMIC( ZPushButton )
public:
   // ctor
   ZPushButton( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   // dtor
   virtual ~ZPushButton( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

   void BNClicked( );
   void BNDisable( );
   void BNDoubleClicked( );
   void BNHilite( );
   void BNPaint( );
   void BNUnhilite( );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
// afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
// HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlType );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZPushButton )
public:
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
// virtual BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

   CSize GetLargestTextSize( CDC *pDC, CStringArray& arrayText );
   void  FormatMultiLineText( CStringArray& arrayText,
                              CDC           *pDC,
                              zCPCHAR       cpcText,
                              CRect&        rectClient );
   void DrawFrame( CDC *pDC, CRect rect, UINT uState );
   void DrawFilledRect( CDC *pDC, CRect rect, COLORREF clr );
   void DrawLine( CDC *pDC, zLONG lLeft, zLONG lTop,
                  zLONG lRight, zLONG lBottom, COLORREF clr );
   void DrawLine( CDC *pDC, CRect rectEndPoints, COLORREF clr );
   void DrawButtonText( CDC *pDC, CRect rect,
                        zCPCHAR cpcBuffer, COLORREF clrText );
   void SetColor( COLORREF clrText, COLORREF clrBk,
                  COLORREF clrBkDisabled,
                  COLORREF clrBkDark );
   void ResetColor( );
   COLORREF GetTextColor( ) { return( m_clrText ); }
   COLORREF GetBkColor( ) { return( m_clrBk ); }
   COLORREF GetDisabledColor( ) { return( m_clrBkDisabled ); }
   UINT GetBevel( ) { return( m_uBevel ); }

private:
   COLORREF m_clrBkDisabled;
   COLORREF m_clrLight;
   COLORREF m_clrHighlight;
   COLORREF m_clrShadow;
   COLORREF m_clrDarkShadow;
   zUSHORT  m_uBevel;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define TRUETYPE_FONT      0x0001
#define PRINTER_FONT       0x0002
#define DEVICE_FONT        0x0004

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         //
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       //
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class CFontObj : public CObject
{
protected:
   zBOOL m_bInUse;   // Fonts in use
   DWORD m_dwFlags;  // Font flags
public:
   CFontObj( DWORD dwFlags )
   {
      m_dwFlags = dwFlags;
      m_bInUse = FALSE;
   }

   CFontObj( CFontObj *pFontObj )
   {
      m_dwFlags = pFontObj->GetFlags( );
      m_bInUse = pFontObj->GetFontInUse( );
   }

   int   GetFlags( ) const { return m_dwFlags; }
   zBOOL GetFontInUse( ) { return m_bInUse; }
   void  SetFontInUse( zBOOL bInUse ) { m_bInUse = bInUse; }
};

/////////////////////////////////////////////////////////////////////////////

// Zeidon ZFontPicker standard control
class ZDCTL_CLASS ZFontPicker : public CComboBox, public ZMapAct
{
   DECLARE_DYNAMIC( ZFontPicker )
public:
   // ctor
   ZFontPicker( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   // dtor
   virtual ~ZFontPicker( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );

   void Initialize( );

protected:
   BOOL EnumerateFonts( );
   void AddFont( CString csName, DWORD dwFlags );
   void SetCurrentFont( );
   void SetFontInUse( const CString& csFont );
public:
   static BOOL CALLBACK AFX_EXPORT
      EnumFamScreenCallBackEx( ENUMLOGFONTEX *pelf,
                               NEWTEXTMETRICEX * /*lpntm*/,
                               int FontType, LPVOID pThis );

   static BOOL CALLBACK AFX_EXPORT
      EnumFamPrinterCallBackEx( ENUMLOGFONTEX *pelf,
                                NEWTEXTMETRICEX * /*lpntm*/,
                                int FontType, LPVOID pThis );

// Attributes
public:

protected:
   ZToolTip   m_wndToolTip;
   CString    m_csFontSave;
   CString    m_csDefault;
   CImageList m_ImageList;
   ZCorrelatedList m_FontList;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZFontPicker )
public:
   virtual void DeleteItem( LPDELETEITEMSTRUCT lpDeleteItemStruct );
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
   virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
   //}}AFX_VIRTUAL

   virtual BOOL DestroyWindow( );

   // Generated message map functions
protected:
   //{{AFX_MSG( ZFontPicker )
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void CBNKillFocus( );
   afx_msg void CBNSetFocus( );
   afx_msg void CBNCloseUp( );
   afx_msg void CBNSelchange( );
   afx_msg void OnTimer( UINT nIDEvent );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctlbtn_h__ sentry.
