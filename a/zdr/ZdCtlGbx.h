/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlgbx.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlgbx_h__  // Sentry, use file only if it's not already included
#define __zdctlgbx_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
GroupBox( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZGroupBox standard control
class ZDCTL_CLASS ZGroupBox : public CButton, public ZMapAct
{
   DECLARE_DYNAMIC( ZGroupBox )
public:
   // ctor
   ZGroupBox( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZGroupBox( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zBOOL  InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus );
   virtual zBOOL  InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus );

   //
   // child id notifications handled at the child
   //
   void GNClicked( );
   void GNDisable( );
   void GNDoubleclicked( );
   void GNHilite( );
   void GNPaint( );
   void GNUnhilite( );

   virtual BOOL DestroyWindow( );

protected:
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

// afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnEnable( BOOL bEnable );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );

   static zCPCHAR g_cpcClassName;

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlgbx_h__ sentry.
