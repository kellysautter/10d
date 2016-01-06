/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctltim.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctltim_h__  // Sentry, use file only if it's not already included
#define __zdctltim_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Timer( ZSubtask *pZSubtask,
       CWnd     *pWndParent,
       ZMapAct  *pzmaComposite,
       zVIEW    vDialog,
       zSHORT   nOffsetX,
       zSHORT   nOffsetY,
       zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZTimer standard control
class ZDCTL_CLASS ZTimer : public CStatic, public ZMapAct
{
   DECLARE_DYNAMIC( ZTimer )

public:
friend UINT
fnTimerThreadProc( zPVOID p );

   // ctor
   ZTimer( ZSubtask *pZSubtask,
           CWnd     *pWndParent,
           ZMapAct  *pzmaComposite,
           zVIEW    vDialog,
           zSHORT   nOffsetX,
           zSHORT   nOffsetY,
           zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZTimer( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // transfer data (OI to ctrl)
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

private:

   zLONG      m_lInterval;
   zBOOL      m_bEnabled;
   CWinThread *m_pThread;

   DECLARE_MESSAGE_MAP( )
};

// Zeidon ZTime standard control
class ZDCTL_CLASS ZTime : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZTime )
public:

   // ctor
   ZTime( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZTime( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // transfer data (OI to ctrl)
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );

private:
   CDateTimeCtrl m_DateTime;

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctltim_h__ sentry.