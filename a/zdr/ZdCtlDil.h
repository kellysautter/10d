//////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctldil.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctldil_h__  // Sentry, use file only if it's not already included
#define __zdctldil_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
DIL( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZStatus standard control
class ZDCTL_CLASS ZStatus : public CStatusBar, public ZMapAct
{
   DECLARE_DYNAMIC( ZStatus )
public:
   // ctor
   ZStatus( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZStatus( );

   virtual void   CreateZ( );
   virtual void   ParentResize( UINT uSizeType, CSize *pCSize );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg LRESULT OnIdleUpdateCmdUI( WPARAM wParam, LPARAM lParam );
   afx_msg void DrawItem( LPDRAWITEMSTRUCT pDrawItemStruct );

private:
   zPLONG  m_plSubtype;

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctldil_h__ sentry.

