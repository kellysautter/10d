/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlrb.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlrb_h__  // Sentry, use file only if it's not already included
#define __zdctlrb_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
RadioBtn( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
AFX_EXT_API
CWnd * OPERATION
RadioBtnGroup( ZSubtask *pZSubtask,
               CWnd     *pWndParent,
               ZMapAct  *pzmaComposite,
               zVIEW    vDialog,
               zSHORT   nOffsetX,
               zSHORT   nOffsetY,
               zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

class ZDCTL_CLASS ZRadioBtn;

// Zeidon ZRadioBtnGroup standard control
class ZDCTL_CLASS ZRadioBtnGroup : public CButton, public ZMapAct
{
friend class ZRadioBtn;
   DECLARE_DYNAMIC( ZRadioBtnGroup )
public:
   // ctor
   ZRadioBtnGroup( ZSubtask *pZSubtask,
                   CWnd     *pWndParent,
                   ZMapAct  *pzmaComposite,
                   zVIEW    vDialog,
                   zSHORT   nOffsetX,
                   zSHORT   nOffsetY,
                   zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZRadioBtnGroup( );

   virtual void   CreateZ( );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zBOOL  IsFocusOKForChild( ZMapAct *pzmaReceivingFocus );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   virtual BOOL DestroyWindow( );

protected:
// afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );

   static zCPCHAR g_cpcClassName;

private:

   zSHORT    m_nFirstChild;
   zSHORT    m_nLastChild;
   zSHORT    m_nDefault;
   zUSHORT   m_uMapState;
   ZRadioBtn *m_pZDfltRB;
   ZRadioBtn *m_pZCurrRB;
   zCHAR     *m_pchCurrRB_Value;

   DECLARE_MESSAGE_MAP( )
};

// Zeidon ZRadioBtn standard control
class ZDCTL_CLASS ZRadioBtn : public CButton, public ZMapAct
{
   DECLARE_DYNAMIC( ZRadioBtn )
public:
   // ctor
   ZRadioBtn( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZRadioBtn( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual void   ResetFocus( );
   virtual ZMapAct * GetRealFocus( );

   // Overridables for remote server support.
   void  SetCheck( zBOOL bCheck );
   zBOOL GetCheck( );

   CString *m_pzsValueOn;
   CString *m_pzsValueOff;

   //
   // child id notifications handled at the child
   //
   void RBClicked( );
   void RBDisable( );
   void RBDoubleclicked( );
   void RBHilite( );
   void RBPaint( );
   void RBUnhilite( );

   virtual BOOL DestroyWindow( );

protected:
   HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnPaint( );

private:

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlrb_h__ sentry.
