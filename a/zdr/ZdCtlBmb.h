/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctl.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
// 2002.03.06   TMV
//    add code to load bitmaps from files if DLL load fails R056363

#ifndef __zdctlbmb_h__  // Sentry, use file only if it's not already included
#define __zdctlbmb_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
BitmapButton( ZSubtask *pZSubtask,
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

// Zeidon ZBitmapButton standard control
class ZDCTL_CLASS ZBitmapButton : public CBitmapButton, public ZMapAct
{
   DECLARE_DYNAMIC( ZBitmapButton )
public:
   // ctor
   ZBitmapButton( ZSubtask *pZSubtask,
                  CWnd     *pWndParent,
                  ZMapAct  *pzmaComposite,
                  zVIEW    vDialog,
                  zSHORT   nOffsetX,
                  zSHORT   nOffsetY,
                  zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZBitmapButton( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );

   HINSTANCE GetCorrectResourceHandle( zPCHAR *ppch );
   void   InitToolTip( );                // hover type
   BOOL   LoadBitmap( );                 // hover type
   void   LoadBitmaps( );
   zPCHAR LoadDib( HINSTANCE hInstDLL, zPCHAR pchBitmap );

   void SetToolTipText( CString *pcsTipText, BOOL bActivate = TRUE );
   void SetToolTipText( int nId, BOOL bActivate = TRUE );

   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );

   void BMClicked( );
   void BMDisable( );
   void BMDoubleClicked( );
   void BMHilite( );
   void BMPaint( );
   void BMUnhilite( );

   virtual BOOL DestroyWindow( );

protected:
   void ActivateToolTip( BOOL bActivate = TRUE );
   CToolTipCtrl m_ToolTip;

   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg LRESULT OnMouseHover( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnMouseLeave( WPARAM wParam, LPARAM lParam );

private:
   void SplitCtrlBOI_BMP( ); //used to split the CtrlBOI bitmaps

   zPCHAR  m_pchMsgText;  // Text when image does not exist
   zPCHAR  m_pchCtrlBOI;  // Hold the CtrlBOI
   zULONG  m_ulLth;       // length of CtrlBOI
   ZDib    *m_pDibUP;     // Normal image
   ZDib    *m_pDibDN;     // Selected image
   ZDib    *m_pDibFO;     // Focused image (non-modal only)
   ZDib    *m_pDibDA;     // Disabled image
   ZDib    *m_pDib;       // Current bitmap (Normal/Selected/Focused/Disabled)
   zCTRLBOI_BMP *m_pBMPINFO; // bitmaps for various states

   zBOOL   m_bHoverType;  // Indicates the button is a special hover type
   zBOOL   m_bHover;      // indicates if mouse is over the button
   zBOOL   m_bTracking;
   CSize   m_ButtonSize;  // width and height of the button
   CBitmap m_bitmap;

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlbmb_h__ sentry.