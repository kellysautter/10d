/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlmle.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlmle_h__  // Sentry, use file only if it's not already included
#define __zdctlmle_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
MLE( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZMLE standard control
class ZDCTL_CLASS ZMLE : public CEdit, public ZMapAct
{
   DECLARE_DYNAMIC( ZMLE )
public:
   // ctor
   ZMLE( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZMLE( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );

   virtual zLONG  SelectAll( );
   zSHORT SelectText( zSHORT nTextStartPos, zSHORT nTextEndPos );
   zSHORT LimitTextLength( zULONG ulTextLth );

   afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSysChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
// afx_msg BOOL PreTranslateMessage( MSG *pMsg );

   //
   // child id notifications handled at the child
   //
   void MLENChange( );
   void MLENErrspace( );
   void MLENHscroll( );
   void MLENMaxtext( );
   void MLENSetFocus( );
   void MLENKillFocus( );
   void MLENUpdate( );
   void MLENVscroll( );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

private:

   zULONG  m_ulLth;

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlmle_h__ sentry.
