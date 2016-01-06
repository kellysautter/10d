/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctltab.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctltab_h__  // Sentry, use file only if it's not already included
#define __zdctltab_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Tab( ZSubtask *pZSubtask,
     CWnd     *pWndParent,
     ZMapAct  *pzmaComposite,
     zVIEW    vDialog,
     zSHORT   nOffsetX,
     zSHORT   nOffsetY,
     zKZWDLGXO_Ctrl_DEF *pCtrlDef );

AFX_EXT_API
CWnd * OPERATION
TabDlg( ZSubtask *pZSubtask,
        CWnd     *pWndParent,
        ZMapAct  *pzmaComposite,
        zVIEW    vDialog,
        zSHORT   nOffsetX,
        zSHORT   nOffsetY,
        zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

class ZDCTL_CLASS ZNotePage;

class ZDCTL_CLASS ZNoteBook : public CTabCtrl, public ZMapAct
{
   DECLARE_DYNAMIC( ZNoteBook )
public:
   ZNoteBook( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~ZNoteBook( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // get text from OI and transfer to entry control
   virtual zBOOL  InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus );
   virtual zBOOL  InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus );
   virtual void   ParentResize( ZMapAct *pParent, CSize *pCSize );

   zSHORT ActivateTab( zSHORT nTab );
   zSHORT ResetActiveTab( zSHORT nTabToActivate );
   zSHORT HideTab( zSHORT nTabToHide, zSHORT nHide );
   ZNotePage * GetActivePage( );
   ZNotePage * GetPage( zSHORT nPageNbr );
   virtual BOOL PreTranslateMessage( MSG *pMsg );

   void  RepositionTabDlgs( );

   virtual BOOL DestroyWindow( );

protected:
   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawInfo );
   void SelChange( NMHDR *pNMHDR, LRESULT *pResult );
   void SelChanging( NMHDR *pNMHDR, LRESULT *pResult );
// void KeyDown( NMHDR *pNMHDR, LRESULT *pResult );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
// afx_msg UINT OnGetDlgCode( );
// afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );

public:
   ZNotePage *m_pZNotePage; // head of linked list of Tab dialogs (one per tab)
   ZNotePage *m_pZFirstPage;
   zSHORT    m_nPageCnt;
   zLONG     m_lTabStyle;
   COLORREF  m_clrSelectedText;
   COLORREF  m_clrSelectedTab;
   CBrush    *m_pBrushSelectedTab;
// ZMapAct   *m_pzmaLosingFocus;

   DECLARE_MESSAGE_MAP( )
};

class ZDCTL_CLASS ZNotePage : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZNotePage )
public:
   ZNotePage( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~ZNotePage( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual zSHORT PostMapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual zBOOL  InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus );
   virtual zBOOL  InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus );
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual zLONG  SetZCtrlProperty( zLONG   lPropertyType,
                                    zULONG  ulNumericProperty,
                                    zCPCHAR cpcStringProperty );

// afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnPaint( );
// virtual BOOL PreTranslateMessage( MSG *pMsg );

// COLORREF  m_clrDlg;
// HBRUSH    m_hBrushDlg;

   ZNoteBook *m_pZNoteBook;// ZNoteBook control parent of this tab dialog
   ZNotePage *m_pNext;     // Next tab in linked list (rooted at ZNoteBook)
   zPCHAR    m_pchCaption;
   zSHORT    m_nTabIdx;    // current index for this tab
   zSHORT    m_nTabNbr;    // original order in which this tab was inserted ...
                           // used to identify the tab for user API calls
   zSHORT    m_nHidden;    // is tab hidden (1) or shown (0) or shown readonly (-1)
   COLORREF  m_clrText;
   COLORREF  m_clrTab;
   CBrush    *m_pBrushTab;

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

   LRESULT OnPostedInvalidate( WPARAM wParam, LPARAM lParam );

   DECLARE_MESSAGE_MAP( )
};

#endif   // __zdctltab_h__ sentry.
