/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlchk.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlchk_h__  // Sentry, use file only if it's not already included
#define __zdctlchk_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
CheckBox( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZCheckBox standard control
class ZDCTL_CLASS ZCheckBox : public CButton, public ZMapAct
{
   DECLARE_DYNAMIC( ZCheckBox )
public:
   // ctor
   ZCheckBox( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );
// ZCheckBox( int rcID, int nDimension );  // rcID       : bitmap resource ID;
                                    // nDimension : bitmap dimension;
   // dtor
   virtual ~ZCheckBox( );

   static zBOOL m_bDrawFocus;    // draw focus rectangle?

protected:
   enum
   {
      BOX_ON = 0,          // Column 0 : checked state
      BOX_OFF = 1,         // Column 1 : unchecked state
      BOX_LDOWN_1 = 2,     // Column 2 : mouse down in checked state
      BOX_LDOWN_2 = 3,     // Column 3 : mouse down in unchecked state
      BOX_DISABLED_1 = 4,  // Column 4 : CB disabled in checked state
      BOX_DISABLED_2 = 5   // Column 5 : CB disabled in unchecked state
   };

public:

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   // Overridables for remote server support.
   void  SetCheck( zBOOL bCheck );
   zBOOL GetCheck( );

   //
   // child id notifications handled at the child
   //
   void CHKClicked( );
// void CHKDisable( );
// void CHKDoubleclicked( );
// void CHKHilite( );
// void CHKPaint( );
// void CHKUnhilite( );

   virtual BOOL DestroyWindow( );

protected:
   //{{AFX_VIRTUAL(CCheckBox)
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual void PreSubclassWindow( );
   //}}AFX_VIRTUAL

   //{{AFX_MSG(CCheckBox)
   afx_msg void OnPaint( );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnEnable( BOOL bEnable );
   afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
   afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
   afx_msg void OnLButtonDown( UINT nFlags, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT nFlags, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg LRESULT OnGetCheck( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnSetCheck( WPARAM wParam, LPARAM lParam );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   //}}AFX_MSG

private:

   CString *m_pzsValueOn;
   CString *m_pzsValueOff;

   zBOOL m_bLeftText;      // BS_LEFTTEXT || BS_RIGHTBUTTON || WS_EX_RIGHT ??
   zBOOL m_bDisabled;
   zBOOL m_bChecked;
   zLONG m_lState;         // state of check box: BOX_ON, BOX_OFF,...

   int  BOX_SIZE;          // size of bitmaps (BOX_SIZE X BOX_SIZE)
   int  m_resID;           // resource ID (m_bSrc)

   DWORD  m_clr;           // background color == COLOR_3DFACE
   HBRUSH m_brush;         // background brush

   HBITMAP m_bmpSrc;       // source bitmap
   HBITMAP m_bitmaps[ 6 ]; // bitmaps for all states BOX_ON, BOX_OFF,...BOX_DISABLED_2

   zBOOL m_bInRect;          // mouse cursor over CB?
   zBOOL m_bSpaceDown;       // space button pressed?
   zBOOL m_bMouseDown;       // mouse button pressed?

   int   m_nHeight;
   zBOOL m_bPrepareBitmaps;
   zBOOL m_bHasFocus;                 // has focus?
   RECT  m_rectFocus;                 // focus rectangle

   int  GetMinHeight( HDC ); // calculates the minimal height 'mHeight' based on m_font
   void DrawTextToDestDC( HDC, CRect&, int );
   void PrepareBitmaps( HDC );        // prepares bitmpas for all states
   void PrepareState( HDC, HBITMAP &, int );  // create CB bitmap for specified state
   void SetState( int nState );       // set state (BOX_ON,BOX_OFF,...BOX_DISABLED_2)
   void BlitToScreen( HDC hdc, int nState ); // blit m_bitmaps[nState] to screen
   void DrawFocus( zBOOL bForceDraw = FALSE );

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctlchk_h__ sentry.
