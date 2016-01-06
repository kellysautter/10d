/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlico.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlico_h__  // Sentry, use file only if it's not already included
#define __zdctlico_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
IconCtl( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZIconCtl standard control
class ZDCTL_CLASS ZIconCtl : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZIconCtl )

public:
   // ctor
   ZIconCtl( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZIconCtl( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );

   afx_msg void OnPaint( );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT uModKeys, CPoint pt );

   zBOOL  IsDoubleClick( zUSHORT uMessage, CPoint ptStart );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

private:

   static zSHORT m_nIconPixelSizeX;    // x-size of icon
   static zSHORT m_nIconPixelSizeY;    // y-size of icon
   static zSHORT m_nIconEdgeX;
   static zSHORT m_nIconEdgeY;

   HICON   m_hIcon;   // Normal image
   CString m_csText;
   zSHORT  m_nIconX;
   zSHORT  m_nIconY;
   zBOOL   m_bFocus;
   CRect   m_rectIconText;
   zSHORT  m_nLDblClk;
   zSHORT  m_nRDblClk;

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlico_h__ sentry.
