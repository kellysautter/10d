/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlrte.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlrte_h__  // Sentry, use file only if it's not already included
#define __zdctlrte_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
RTE( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZRichEdit standard control
class ZDCTL_CLASS ZRichEdit : public CRichEditCtrl, public ZMapAct
{
   DECLARE_DYNAMIC( ZRichEdit )
public:
   // ctor
   ZRichEdit( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZRichEdit( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   afx_msg void OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSysChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
// afx_msg void HScroll( UINT nSBCode, UINT nPos );
// afx_msg void VScroll( UINT nSBCode, UINT nPos );
   afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void ParentNotify( UINT uMessage, LPARAM lParam );
   afx_msg void RTENotifyChange( NMHDR *pNotifyStruct, LRESULT *result );
   afx_msg void RTENotifyUpdate( NMHDR *pNotifyStruct, LRESULT *result );
   afx_msg void OnPaint( );

   //
   // child id notifications handled at the child
   //
   void RTENChange( );
   void RTENErrspace( );
   void RTENMaxtext( );
   void RTENSetFocus( );
   void RTENKillFocus( );
   void RTENUpdate( );
   void RTENRequestResize( );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

private:

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlrte_h__ sentry.
