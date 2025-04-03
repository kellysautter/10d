/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Application
// FILE:         tzctl.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for Zeidon design time controls.
//

//
// CHANGE LOG - most recent first order
//
// 2001.09.24    DKS   Bianca
//    NoteBook CtrlMapping to remap a Control.
//
// 2001.06.21    TMV   TBEDIT
//    Introduced new method to TBEDIT.OCX so wrapper class ZDrTbEdt has
//    got a new method
//       long   GetSelectedText( BSTR *pBuffer, long lLength );
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.03.26    TMV   2000   ActiveX
//    Store license key in pwd and create control using this
//    new method:
//       ZActiveX::BuildLicKeyString (for debugging only)
//    new Members
//       CString m_strLicenseKey;
//
// 2001.03.23    TMV   2000   ActiveX
//    add two members to TZActiveX to handle License information
//
// 2001.03.05    BL   TrueGrid
//    Grid: changed  TZActiveX::WritePersistProperties to public
//
// 2000.07.20    TMV 10
//    Introduced new method to TBEDIT.OCX so wrapper class ZDrTbEdt has
//    got a new method
//       zLONG   GetTextFromRange( BSTR *pBuffer, zLONG lStart, zLONG lEnd );
//       zLONG   GetObjectName( BSTR *pBuffer );
//
// 2000.03.29    TMV    Z10
//    move
//    const VARIANT_BOOL VAR_TRUE = ~VARIANT_FALSE;
//    const VARIANT_BOOL VAR_FALSE = VARIANT_FALSE;
//    to zdr.h to make it accessible in ActiveX DLLs
//
// 2000.03.16    TMV 10
//    Introduced new method to TBEDIT.OCX so wrapper class ZDrTbEdt has
//    got a new method
//       BOOL   SelectRange( zLONG lIndec, zLONG lLength );
//
// 1999.11.30    TMV 10
//    avoid memory leaks
//
// 1999.04.19  RS
//    Commentlines required for Rose.
//

// Don't remove the following commentlines. They are required for Rose.
//#include <afxtempl.h> //required for Rose

#ifndef __tzctl_h__  // Sentry, use file only if it's not already included
#define __tzctl_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

/////////////////////////////////////////////////////////////////////////////
// Note: Use AFX_EXT_CLASS to export an entire class.  This avoids
// creating a .DEF file with all the decorated names for the class.
// Creating a .DEF file is more efficient since the names can be
// exported by ordinal.
//
// Here we use that method of exporting, much like MFC does itself, by
// implementing the following two lines and adding the appropriate lines
// to our .DEF file. All the public interfaces are exported in ZdCtl.DEF.
//

#ifdef TZCTL_CLASS
// #undef  AFX_DATA
// #define AFX_DATA AFX_EXT_DATA
// #define TZCTL_CLASS_PREDEFINED
#else
   #define TZCTL_CLASS
#endif

// #define DEBUG_ALL

extern "C" void WINAPI
ZeidonInitTZCTL( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                 zLONG lMapDlgCharX, zLONG lMapDlgCharY );

#include "zdctl.h"

extern "C"
{
#include <tzctlopr.h>

#pragma pack( push, __tzctl_h )
#pragma pack( 1 )

#include <tzwdlgso.hst>

#pragma pack( pop, __tzctl_h )
}

/////////////////////////////////////////////////////////////////////////////

#define zCTRL_VALID_PAINT        1
#define zCTRL_VALID_MOVE         2
#define zCTRL_VALID_CHANGE       3

#define zCTRL_MODE_DIRECT_EDIT   0x00000001
#define zCTRL_MODE_CLIENT        0x00000002

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zER_NODE_CREATE          1
#define zER_NODE_LCLICK          2
#define zER_NODE_LDBLCLICK       3
#define zER_NODE_RCLICK          4
#define zER_NODE_RDBLCLICK       5
#define zER_ARC_CREATE           6
#define zER_ARC_LCLICK           7
#define zER_ARC_LDBLCLICK        8
#define zER_ARC_RCLICK           9
#define zER_ARC_RDBLCLICK       10
#define zER_SPACE_LCLICK        11
#define zER_SPACE_LDBLCLICK     12
#define zER_SPACE_RCLICK        13
#define zER_SPACE_RDBLCLICK     14

// There must be at least on cell separating each entity where each cell
// is comprised of three vertical lines and three horizontal lines.
#define  tzERCELLSIZE           m_pER_Diagram->m_lCellSize
#define  tzERPRINTCELLSIZE      21   // number of pixels per cell @ 100%
#define  tzERARCSIZE            tzERENTITYX
#define  tzERDIAGRAMX           1200 // 600    now a 1-meg array!!!
#define  tzERDIAGRAMY            900 // 300
#define  tzERDISPLAYPOS         m_pER_Diagram->m_lDisplayPos
#define  tzERMAXGRID            (zLONG) (((zLONG) tzERDIAGRAMX * (zLONG) tzERDIAGRAMY) - 1)
#define  tzERENTITYX            9  // # of cells in width
#define  tzERENTITYY            5  // # of cells in height
#define  tzERMAXPORTX           (tzERENTITYX / 2)
#define  tzERMAXPORTY           (tzERENTITYY / 2)
#define  tzERBADPATH            1000000
#define  tzERRECURSSZ           3
#define  tzERLINEABUTSLINE      1
#define  tzERLINEABUTSTEXT      1
#define  tzERLINEBENDS          3
#define  tzERLINECROSSESTEXT    100
#define  tzERLINECROSSESLINE    100
#define  tzERLINEABUTSENTITY    200
#define  tzERLINECROSSESENTITY  300
#define  tzERLINETEXTOFFSET     2
#define  tzERFONTHEIGHT         (((tzERCELLSIZE + (tzERCELLSIZE / 2 )) - \
                                  tzERLINETEXTOFFSET) * (tzERENTITYX / 9))
//#define  tzERFONTWIDTH          ((tzFONTHEIGHT * 2) / 3)
#define  tzERFONTWIDTH          0
#define  tzERFONTWEIGHT         100
#define  tzERFONTFF             VARIABLE_PITCH | FF_DONTCARE
#define  tzERFONTNAME           "Arial" // "Times New Roman"

#define  tzERCOLORBACKGROUND ::GetSysColor( COLOR_BTNFACE )
#define  tzERCOLORNORMAL     m_pER_Diagram->m_clrNormal
#define  tzERCOLORSELECTED   m_pER_Diagram->m_clrSelected
#define  tzERCOLORLINES      m_pER_Diagram->m_clrLines
#define  tzERCOLORBOX        m_pER_Diagram->m_clrBox
#define  tzERCOLORTEXT       m_pER_Diagram->m_clrText
#define  tzERCOLORBLACK      RGB( 0, 0, 0 )
#define  tzERCOLORWHITE      RGB( 255, 255, 255 )
#define  tzERCOLORGRAY       RGB( 190, 190, 190 )
#define  tzERCOLORSHADE      RGB( 228, 228, 228 )

// Indicators for what is in a cell
#define  tziENTITY      0x01
#define  tziLINE        0x02
#define  tziARC         0x04
#define  tziTEXT        0x08

// Indicators for where the arc exists
#define  tziULEFT       0x10
#define  tziURIGHT      0x20
#define  tziBLEFT       0x40
#define  tziBRIGHT      0x80

// Indicators for where the line runs
#define  tziHORIZ       0x10          // Horizontal
#define  tziVERT        0x20          // Vertical
#define  tziTEXTHORIZ   0x40          // Terminated OR Text horizontal
#define  tziTEXTVERT    0x80          // Terminated OR Text vertical

#define  tzLEFT         1
#define  tzUP           2
#define  tzDOWN         3
#define  tzRIGHT        4
#define  tzHORIZONTAL   5
#define  tzVERTICAL     6

// defines for TZAmbientProperty
//
#define VTI_EMPTY    0
#define VTI_BOOL     1
#define VTI_UI1      2
#define VTI_UI2      3
#define VTI_UI4      4
#define VTI_UI8      5
#define VTI_I1       6
#define VTI_I2       7
#define VTI_I4       8
#define VTI_I8       9
#define VTI_R4       10
#define VTI_R8       11
#define VTI_BSTR     12
#define VTI_CY       13
#define VTI_DATE     14
#define VTI_ERROR    15
#define VTI_COLOR    16
#define VTI_FONT     17
#define VTI_UNKNOWN  18
#define NUM_VARTYPES 19

// #define _SCB_REPLACE_MINIFRAME

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// TZSizingControlBar           Version 2.44
//
// Created: Jan 24, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

//#if !defined(__SIZECBAR_H__)
//#define __SIZECBAR_H__

//#include <afxpriv.h>    // for CDockContext
//#include <afxtempl.h>   // for CTypedPtrArray

//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000

#if defined(_SCB_MINIFRAME_CAPTION) && !defined(_SCB_REPLACE_MINIFRAME)
    #error "_SCB_MINIFRAME_CAPTION requires _SCB_REPLACE_MINIFRAME"
#endif

/////////////////////////////////////////////////////////////////////////
// TZSCBDockBar dummy class for access to protected members

class TZCTL_CLASS TZSCBDockBar : public CDockBar
{
    friend class TZSizingControlBar;
};

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar control bar styles

#define SCBS_EDGELEFT       0x00000001
#define SCBS_EDGERIGHT      0x00000002
#define SCBS_EDGETOP        0x00000004
#define SCBS_EDGEBOTTOM     0x00000008
#define SCBS_EDGEALL        0x0000000F
#define SCBS_SHOWEDGES      0x00000010
#define SCBS_SIZECHILD      0x00000020

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar control bar

#ifndef baseTZSizingControlBar
#define baseTZSizingControlBar CControlBar
#endif

class TZCTL_CLASS TZSizingControlBar;
typedef CTypedPtrArray <CPtrArray, TZSizingControlBar*> TZSCBArray;

class TZCTL_CLASS TZSizingControlBar : public baseTZSizingControlBar
{
    DECLARE_DYNAMIC(TZSizingControlBar);

// Construction
public:
    TZSizingControlBar();

    virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
        CSize sizeDefault, BOOL bHasGripper,
        UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);
    virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
        UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP);

// Attributes
public:
    const BOOL IsFloating() const;
    const BOOL IsHorzDocked() const;
    const BOOL IsVertDocked() const;
    const BOOL IsSideTracking() const;
    const BOOL GetSCBStyle() const {return m_dwSCBStyle;}

// Operations
public:
#if defined(_SCB_REPLACE_MINIFRAME) && !defined(_SCB_MINIFRAME_CAPTION)
    void EnableDocking(DWORD dwDockStyle);
#endif
    virtual void LoadState(LPCTSTR lpszProfileName);
    virtual void SaveState(LPCTSTR lpszProfileName);
    static void GlobalLoadState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
    static void GlobalSaveState(CFrameWnd* pFrame, LPCTSTR lpszProfileName);
    void SetSCBStyle(DWORD dwSCBStyle)
        {m_dwSCBStyle = (dwSCBStyle & ~SCBS_EDGEALL);}

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TZSizingControlBar)
    public:
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~TZSizingControlBar();

protected:
    // implementation helpers
    UINT GetEdgeHTCode(int nEdge);
    BOOL GetEdgeRect(CRect rcWnd, UINT nHitTest, CRect& rcEdge);
    virtual void StartTracking(UINT nHitTest, CPoint point);
    virtual void StopTracking();
    virtual void OnTrackUpdateSize(CPoint& point);
    virtual void OnTrackInvertTracker();
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
    virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

    virtual void AlignControlBars();
    void GetRowInfo(int& nFirst, int& nLast, int& nThis);
    void GetRowSizingBars(TZSCBArray& arrSCBars);
    void GetRowSizingBars(TZSCBArray& arrSCBars, int& nThis);
    BOOL NegotiateSpace(int nLengthTotal, BOOL bHorz);

protected:
    DWORD   m_dwSCBStyle;
    UINT    m_htEdge;

    CSize   m_szHorz;
    CSize   m_szVert;
    CSize   m_szFloat;
    CSize   m_szMinHorz;
    CSize   m_szMinVert;
    CSize   m_szMinFloat;
    int     m_nTrackPosMin;
    int     m_nTrackPosMax;
    int     m_nTrackPosOld;
    int     m_nTrackEdgeOfs;
    BOOL    m_bTracking;
    BOOL    m_bKeepSize;
    BOOL    m_bParentSizing;
    BOOL    m_bDragShowContent;
    UINT    m_nDockBarID;
    int     m_cxEdge;

// Generated message map functions
protected:
    //{{AFX_MSG(TZSizingControlBar)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnPaint();
    afx_msg void OnNcPaint();
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

#ifdef _SCB_REPLACE_MINIFRAME
    friend class TZSCBMiniDockFrameWnd;
#endif //_SCB_REPLACE_MINIFRAME
};

#ifdef _SCB_REPLACE_MINIFRAME
#ifndef _SCB_MINIFRAME_CAPTION
/////////////////////////////////////////////////////////////////////////
// TZSCBDockContext dockcontext

class TZCTL_CLASS TZSCBDockContext : public CDockContext
{
public:
// Construction
    TZSCBDockContext(CControlBar* pBar) : CDockContext(pBar) {}

// Drag Operations
    virtual void StartDrag(CPoint pt);
};
#endif //_SCB_MINIFRAME_CAPTION

/////////////////////////////////////////////////////////////////////////
// TZSCBMiniDockFrameWnd miniframe

#ifndef baseTZSCBMiniDockFrameWnd
#define baseTZSCBMiniDockFrameWnd CMiniDockFrameWnd
#endif

class TZCTL_CLASS TZSCBMiniDockFrameWnd : public baseTZSCBMiniDockFrameWnd
{
    DECLARE_DYNCREATE(TZSCBMiniDockFrameWnd)

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TZSCBMiniDockFrameWnd)
    public:
    virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
    //}}AFX_VIRTUAL

// Implementation
public:
    TZSizingControlBar* GetSizingControlBar();

    //{{AFX_MSG(TZSCBMiniDockFrameWnd)
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
#endif //_SCB_REPLACE_MINIFRAME

//#endif // !defined(__SIZECBAR_H__)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// TZSizingControlBarG           Version 2.44
//
// Created: Jan 24, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

//#if !defined(__SCBARG_H__)
//#define __SCBARG_H__

//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////
// TZSCBButton (button info) helper class

class TZCTL_CLASS TZSCBButton
{
public:
    TZSCBButton();

    void Move(CPoint ptTo) {ptOrg = ptTo; };
    CRect GetRect() { return CRect(ptOrg, CSize(11, 11)); };
    void Paint(CDC* pDC);

    BOOL    bPushed;
    BOOL    bRaised;

protected:
    CPoint  ptOrg;
};

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBar control bar

#ifndef baseTZSizingControlBarG
#define baseTZSizingControlBarG TZSizingControlBar
#endif

class TZCTL_CLASS TZSizingControlBarG : public baseTZSizingControlBarG
{
    DECLARE_DYNAMIC(TZSizingControlBarG);

// Construction
public:
    TZSizingControlBarG();

// Attributes
public:
    virtual BOOL HasGripper() const;

// Operations
public:

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(TZSizingControlBarG)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~TZSizingControlBarG();

protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);
    virtual void NcCalcClient(LPRECT pRc, UINT nDockBarID);

protected:
    int     m_cyGripper;

    TZSCBButton m_biHide;

// Generated message map functions
protected:
    //{{AFX_MSG(TZSizingControlBarG)
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//#endif // !defined(__SCBARG_H__)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// TZSizingControlBarCF          Version 2.44
//
// Created: Dec 21, 1998        Last Modified: March 31, 2002
//
// See the official site at www.datamekanix.com for documentation and
// the latest news.
//
/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-2002 by Cristi Posea. All rights reserved.
//
// This code is free for personal and commercial use, providing this
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// You must obtain the author's consent before you can include this code
// in a software library.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc. to
// cristi@datamekanix.com or post them at the message board at the site.
/////////////////////////////////////////////////////////////////////////

//#if !defined(__SCBARCF_H__)
//#define __SCBARCF_H__

//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000
// scbarcf.h : header file
//

/////////////////////////////////////////////////////////////////////////
// TZSizingControlBarCF

#ifndef baseTZSizingControlBarCF
#define baseTZSizingControlBarCF TZSizingControlBarG
#endif

class TZCTL_CLASS TZSizingControlBarCF : public baseTZSizingControlBarCF
{
    DECLARE_DYNAMIC(TZSizingControlBarCF)

// Construction
public:
    TZSizingControlBarCF();

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Implementation
protected:
    // implementation helpers
    virtual void NcPaintGripper(CDC* pDC, CRect rcClient);

protected:
    BOOL    m_bActive; // a child has focus
    CString m_sFontFace;

// Generated message map functions
protected:
    //{{AFX_MSG(TZSizingControlBarCF)
    //}}AFX_MSG
    afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////

//#endif // !defined(__SCBARCF_H__)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK
ZeidonTZCTL_WndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam );

#if 0
void
RegisterNullWindow( HINSTANCE hInstance );
void
UnregisterNullWindow( HINSTANCE hInstance );
#endif

class TZCTL_CLASS ZER_Entity;
class TZCTL_CLASS ZER_Relationship;

// Container class for Entities in diagram
class TZCTL_CLASS ZER_Diagram : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZER_Diagram )
public:
   ZER_Diagram( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~ZER_Diagram( );

   ZER_Diagram     *m_pER_Diagram;  // pointer to ourself for defines

   CArray<ZER_Entity *, ZER_Entity *> m_EntityList;
   CArray<ZER_Relationship *, ZER_Relationship *> m_RelationshipList;

// virtual BOOL DestroyWindow( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
   afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnWindowPosChanged( WINDOWPOS *pWindowPos );
   afx_msg void OnPaint( );

   zBOOL  IsDoubleClick( zUSHORT uMessage, CPoint ptStart );
   void   AnalyzeGrid( );
   void   AddRelationship( ZER_Relationship *pNewRelationship );
   zSHORT PosX( zLONG lIdx );
   zSHORT PosY( zLONG lIdx );
   zSHORT ZoomDiagram( zSHORT nPercent, zSHORT nOptions );
   void   CalculateMinimumScale( CRect& rect );

   void   CalculateDisplaySize( zLONG& lDisplaySizeX,
                                zLONG& lDisplaySizeY,
                                CRect& rect,
                                zLONG  lScale );

   zSHORT ExpandContractDiagram( zSHORT nPercent, zUSHORT uOptions );
   void   GetDeviceContext( );
   void   ReleaseDeviceContext( );
   void   MakeLogicalPoint( CPoint& pt );
   void   MakePhysicalLParam( CPoint& pt );
   void   DrawDiagram( );
   zSHORT PrintDiagram( zSHORT nPercent = 0, zSHORT nOptions = 0 );
   zLONG  AnalyzePages( zLONG& lHorizPages, zLONG& lScale,
                        zLONG& lPageSizeX, zLONG& lPageSizeY,
                        zSHORT nPercent, zSHORT nOptions,
                        CPoint ptDiagramSize, CPoint ptPageSize );
   void   RedrawDiagram( );
   void   ColorCell( LONG lIdx, COLORREF color );
   void   DisplayCells( zCPCHAR cpcText,
                        zLONG lX1, zLONG lY1, zLONG lX2, zLONG lY2 );
   void   DrawDiagramCell( zLONG lIdx,
                           zBOOL bErase = FALSE,
                           ZER_Relationship *pRelationship = 0 );
   zBOOL  PositionVisible( zLONG lStartIdx, zLONG lEndIdx );
   void   SetScrollBars( );
   void   SetDisplayBoundaries( CRect& rect, zBOOL bUpdate = FALSE );
   void   SetMaxPositions( );
   zLONG  MousePositionToIndex( CPoint pt );

   zSHORT CountEntitiesAtIndex( zLONG lIdx );
   ZER_Entity * IndexOnEntity( zLONG lIdx );
   ZER_Relationship * IndexOnRelationship( zLONG lIdx );
   zBOOL  PositionOnZeidonEntity( ZER_Entity *pEntity,
                                  zVIEW      vDiagram = 0,
                                  zBOOL      bSelect = FALSE,
                                  zBOOL      bCtrl = TRUE );

   zBOOL  PositionOnZeidonRelationship( ZER_Relationship *pRelationship,
                                        zVIEW            vDiagram = 0,
                                        zBOOL            bSelect = FALSE,
                                        zBOOL            bCtrl = TRUE );
   virtual void  CreateZ( );
   zSHORT MapFromOI( WPARAM wFlag = 0 );
   zSHORT MapToOI( zLONG lFlag = 0 );

   static CString m_csClassName;

   zSHORT          m_nEntCnt;
   zSHORT          m_nRelCnt;
   CDC             *m_hDC;
   int             m_nOldMapMode;
   CSize           m_sizeOldWExt;
   CSize           m_sizeOldVExt;
   zLONG           m_lMinScale;  // Minimum Scaling factor
   zLONG           m_lScale;     // Scaling factor
   zBOOL           m_bPrintDC;
   zBOOL           m_bShadeEnts; // Print - Shade Selected Entities in Gray
   zBOOL           m_bShadeRels; // Print - Shade Selected Entities in Gray
   zBOOL           m_bUpdateView;
   zBOOL           m_bMultMap;   // Allow multiple map from OI
   zBOOL           m_nLDblClk;   // Left  button double click occured
   zBOOL           m_nRDblClk;   // Right button double click occured
   zBOOL           m_bShowLines; // Show relationship lines
   zBOOL           m_bShowText;  // Show relationship text
   zBOOL           m_bShowCard;  // Show cardinality with text
   zBOOL           m_bRefreshERD;
   zLONG           m_hGrid;
   zPCHAR          m_pchGrid;
   zLONG           m_lCellSize;
   zLONG           m_lMinPosX;   // Minimum Entity Pos X
   zLONG           m_lMinPosY;   // Minimum Entity Pos Y
   zLONG           m_lMaxPosX;   // Maximum Entity Pos X
   zLONG           m_lMaxPosY;   // Maximum Entity Pos Y
   zSHORT          m_nCapture;
   zBOOL           m_bScrolledWindow;
   CPoint          m_ptMouseSelect;
   CPoint          m_ptLastMouseMove;
   zLONG           m_lMouseOffsetX;
   zLONG           m_lMouseOffsetY;
   zLONG           m_lDisplayPos;
   zLONG           m_lDisplayPosMax;
   zLONG           m_lUpdateDisplayPos;
   zLONG           m_lUpdateDisplayPosMax;
   ZER_Entity       *m_pPendingEntity;
   ZER_Relationship *m_pPendingRelationship;
   COLORREF        m_clrNormal;
   COLORREF        m_clrSelected;
   COLORREF        m_clrNoUpdate;
   COLORREF        m_clrBoxU;
   COLORREF        m_clrLinesU;
   COLORREF        m_clrTextU;
   COLORREF        m_clrBox;
   COLORREF        m_clrLines;
   COLORREF        m_clrText;
   ZER_Entity      *m_pMoveEntity;

   DECLARE_MESSAGE_MAP( );
};

class TZCTL_CLASS ZER_Entity
{
public:

   ZER_Entity( ZER_Diagram *pER_Diagram,
               zLONG       lXPos = 0,
               zLONG       lYPos = 0,
               zPCHAR      pchText = 0,
               zCHAR       chType  = 'F',
               zLONG       lPosZKey  = 0,
               zLONG       lZKey  = 0,
               zLONG       lPortFactor = 1,
               zBOOL       bSelect = FALSE );
   virtual ~ZER_Entity( );

   ZER_Diagram *m_pER_Diagram; // pointer to diagram class
   zLONG   m_lZKey;           // ZKey of Entity
   zLONG   m_lPosZKey;        // ZKey of Positioning Entity
   zLONG   m_lPosIdx;         // Grid position of Entity
   zLONG   m_lPortFactor;     // factor by which to multiply std nbr of ports
   zLONG   m_lEntPortX;       // number of X ports (std 9)
   zLONG   m_lEntPortY;       // number of Y ports (std 5)
   CString m_csText;          // Entity Text
   zBOOL   m_bSelected;       // Entity has been selected
   zBOOL   m_bUpdated;        // Entity has been updated (moved)
   zCHAR   m_chType;          // Entity type

   void   MarkEntity( zBOOL bErase = FALSE );
   zSHORT DrawEntity( zBOOL bErase = FALSE );
   void   Erase( zBOOL bUnmark = TRUE );
   void   CenterEntity( );
   void   Move( zLONG lNewPosX, zLONG lNewPosY, zBOOL bRecalcRels = TRUE );
   void   Select( zBOOL bToggle = FALSE, zBOOL bClear = TRUE );
   void   Deselect( );
   void   SelectNeighbors( zBOOL bSelect = TRUE,
                           zSHORT nSelectRels = 0,
                           zBOOL bClear  = TRUE );
   zBOOL  IndexInEntity( zLONG lIdx );
   zSHORT PosX( zLONG lIdx = -1 );
   zSHORT PosY( zLONG lIdx = -1 );
};

class TZCTL_CLASS ZER_Relationship
{
private:
public:
   ZER_Relationship( ZER_Diagram *pER_Diagram,
                     ZER_Entity  *pSrcEntity,
                     ZER_Entity  *pTgtEntity,
                     zPCHAR      pchForwardText = 0,
                     zPCHAR      pchReverseText = 0,
                     zBOOL       bSrcReq = FALSE,
                     zBOOL       bSrcMany = TRUE,
                     zBOOL       bTgtReq = FALSE,
                     zBOOL       bTgtMany = TRUE,
                     zLONG       lPosZKey = 0,
                     zLONG       lStartIdx = 0,
                     zLONG       lEndIdx = 0,
                     zLONG       lArcIdx = 0,
                     zBOOL       bSelect = FALSE,
                     zLONG       lFlags = 0 );
   virtual ~ZER_Relationship( );

   ZER_Diagram   *m_pER_Diagram; // pointer to diagram class
   ZER_Entity    *m_pSrcEntity;  // class pointer of source entity
   ZER_Entity    *m_pTgtEntity;  // class pointer of target entity
   zLONG         m_lPosZKey;     // ZKey of Positioning relationship entity
   zSHORT        m_nArcs;        // Number of arcs in the relationship line
   zLONG         m_lStartIdx;    // Starting cell of line
   zLONG         m_lArcIdx1;     // Position of first bend
   zLONG         m_lArcIdx2;     // Position of second bend
   zLONG         m_lArcIdx3;     // Position of third bend
   zLONG         m_lEndIdx;      // Ending Cell of line
   zLONG         m_lER_Flags;    // Relationship auto vs. user specified
   zBOOL         m_bLineDrawn;   // Line is currently drawn
   zLONG         m_lTextIdx;     // Index to text starting point
   zSHORT        m_nTextLth;     // Number of cells available for text
   zSHORT        m_nTextDir;     // Direction for the text
   zBOOL         m_bTextDrawn;   // Line is currently drawn
   CString       m_csTextF;      // Forward direction text
   zUSHORT       m_uLPOffsetF;   // Offset to LParen forward
   CString       m_csTextR;      // Reverse direction text
   zUSHORT       m_uLPOffsetR;   // Offset to LParen reverse
   zBOOL         m_bSelected;    // Relationship is selected
   zBOOL         m_bUpdated;     // Relationship has been updated (calc'd)
   zBOOL         m_bSrcReq;      // Relationship is required at Src end
   zBOOL         m_bSrcMany;     // Relationship supports many at Src end
   zBOOL         m_bTgtReq;      // Relationship is required at Tgt end
   zBOOL         m_bTgtMany;     // Relationship supports many at Tgt end

   zSHORT CalculateLine( zBOOL bDisplayMsgBox = FALSE, zSHORT nErrCnt = 0 );
   zLONG  AnalyzeLine( zPLONG  lSourcePortIdx,
                       zPLONG  lTargetPortIdx,
                       zPLONG  lArcIdx1,
                       zPLONG  lArcIdx2,
                       zPLONG  lArcIdx3,
                       zSHORT  uSourcePort,
                       zSHORT  uTargetPort,
                       zSHORT  uSourceAxis,
                       zLONG   lPortX,
                       zLONG   lPortY );

   void   CalculateText( );
   zSHORT DrawRelationship( zBOOL  bErase = FALSE,
                            zBOOL  bDisplayMsgBox = FALSE,
                            zSHORT nCnt = 0 );
   void   MarkRelationship( zBOOL bErase = FALSE );
   zBOOL  RelationshipTextVisible( );
   void   DrawRelationshipText( );
   void   EraseText( );
   void   Erase( zBOOL bUnmark = TRUE );
   void   Select( zBOOL bToggle = FALSE, zBOOL bClear = TRUE );
   void   Deselect( );
   void   ToggleSelect( );
   zSHORT PosX( zLONG lIdx );
   zSHORT PosY( zLONG lIdx );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//   Painter classes
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define  BUTTON_SIZE 28

#define   zALIGN_LEFT         1
#define   zALIGN_TOP          2
#define   zALIGN_RIGHT        4
#define   zALIGN_BOTTOM       8
#define   zSIZE_WIDTH         1
#define   zSIZE_HEIGHT        2
#define   zSIZE_BOTH          zSIZE_HEIGHT + zSIZE_WIDTH
#define   zEQUAL_HSPACE       1
#define   zEQUAL_VSPACE       2
#define   zABUT_HORIZONTAL    1
#define   zABUT_VERTICAL      2

class TZCTL_CLASS TZPainterBar;
class TZCTL_CLASS TZToolBox;
class TZCTL_CLASS TZPainterWindow;
class TZCTL_CLASS TZPainterClient;
class TZCTL_CLASS TZClientCover;
class TZCTL_CLASS TZPainterCtrl;
class TZCTL_CLASS TZCtrlCover;
class TZCTL_CLASS TZActiveX;
class TZCTL_CLASS TZRuler;
class TZCTL_CLASS TZGroupSet;
class TZCTL_CLASS TZGroup;
class TZCTL_CLASS TZWebCtrl;
class TZCTL_CLASS TZSelectCover;

typedef zLONG (POPERATION zCONTROLCALL)( zSHORT, TZPainterWindow *,
                                         zLONG, CWnd *,
                                         zVIEW, CRect *,
                                         TZPainterCtrl *,
                                         zLONG, zBOOL, zLONG, zLONG, zLONG,
                                         zPCHAR, zPCHAR );

/////////////////////////////////////////////////////////////////////////////
//
// The TZPalette presents the available design-time
// controls which can be added to a window.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZPalette : public CToolBar
{
   DECLARE_DYNAMIC( TZPalette )
public:
   // ctor
   TZPalette( TZPainterBar *pControlBar );

   virtual ~TZPalette( );

   void SetColumns( zUSHORT uColumns );
   UINT GetColumns( ) { return( m_uColumns ); };

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnControlReflectCommand( );
   afx_msg void ParentNotify( UINT message, LPARAM lParam );
   afx_msg LRESULT OnGetToolTipText( WPARAM wParam, LPARAM lParam );

   virtual void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );
   virtual BOOL OnCmdMsg( UINT nID, int nCode, void *pExtra,
                          AFX_CMDHANDLERINFO *pHandlerInfo );
   virtual BOOL DestroyWindow( );

// void CreateButtons( zBOOL bShow = TRUE );
// void DeleteButtons( );
// void DeselectModal( );
// void SelectModalPointer( );
// zBOOL PaintToolBoxCtrl( TZPainterWindow *pPainterWindow,
//                         CPoint ptStart, CPoint ptEnd );

   TZPainterBar *m_pBar;                // point up the chain
   zVIEW  m_vSubtask;

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

protected:
   zUSHORT  m_uColumns;

   DECLARE_MESSAGE_MAP()
};

typedef struct tagPALETTE_ITEM
{
   zCHAR  chState;
   zLONG  lKey;
   UINT   uCmdId;
   UINT   uBitmapIdx;
   zCHAR  szTag[ 33 ];

} zPALETTE_ITEM;

/////////////////////////////////////////////////////////////////////////////
//
// The TZPainterBar class is a driver custom control which
// is the painter. This class builds a toolbox in the
// main painter window for painting and editing controls.
// Subsequent TZPainterWindow class instances are built
// each time the user wants to edit a Zeidon window
// (1 instance for each window).
//
/////////////////////////////////////////////////////////////////////////////
#define zTZCB_INITIALIZED        0x01
#define zTZCB_SCROLLBARS         0x02
#define zTZCB_GRID               0x04
#define zTZCB_SHOWTABS           0x08
#define zTZCB_REPORT             0x10
#define zTZCB_XSLT               0x20

class TZCTL_CLASS TZPainterBar : public CToolBar
{
   DECLARE_DYNAMIC( TZPainterBar )
public:
   TZPainterBar( ZSubtask *pZSubtask,
                 CWnd     *pWndParent,
                 ZMapAct  *pzmaComposite,
                 zVIEW    vSubtask,
                 zSHORT   nOffsetX,
                 zSHORT   nOffsetY,
                 zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~TZPainterBar( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnClose( );
   afx_msg void OnControlReflectCommand( );
   afx_msg void ParentNotify( UINT message, LPARAM lParam );
   afx_msg LRESULT OnGetToolTipText( WPARAM wParam, LPARAM lParam );

// virtual BOOL OnCreateBands( );
   virtual void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );
   virtual BOOL OnCmdMsg( UINT nID, int nCode, void *pExtra,
                          AFX_CMDHANDLERINFO *pHandlerInfo );
   virtual BOOL DestroyWindow( );

   void   Initialization( );
   void   DeleteAllPainterWindows( );
   void   EmptyAllPainterWindows( zPCHAR pchParm );
   zSHORT DeleteCurrentPainterWindow( );
   void   EnablePainterWindows( zBOOL bEnable );
   void   SetScrollbars( zBOOL bEnable );
   void   SetGrid( zBOOL bEnable );
   void   DockControlBarLeftOf( CToolBar *pBar, CToolBar *pLeftOf );
   void   TZPainterBar::CreatePaletteButtons( );
   void   LoadActionList( zCPCHAR cpcWndTag );
   void   LoadWindowList( );
   void   DeselectModal( );
   void   SelectModalPointer( );
   zBOOL  PaintToolBoxCtrl( TZPainterWindow *pPainterWindow,
                            CPoint ptStart, CPoint ptEnd );
   void   CreateDefaultFont( );
   void   CreateAppView( zVIEW *pvApp );
   void   ResetSelectedMouseReleaseSwap( zBOOL bDeselect = TRUE );
   zLONG  CreateFlatCtrl( zVIEW vDialog, CRect& rectParent,
                          zLONG lMultX, zLONG lDivX,
                          zLONG lMultY, zLONG lDivY );
   void   DeleteWebCtrls( );

   ZSubtask *m_pZSubtask;
   zVIEW   m_vSubtask;
   CArray<TZPainterWindow *, TZPainterWindow *> m_PainterWindowList;
   zSHORT  m_nWndCnt;
   TZPainterWindow *m_pActivePainterWindow;
   ZActiveX_CtrlList *m_pActiveX_CtrlList;

   UINT    m_uPaletteItemCnt;
   zPALETTE_ITEM *m_PaletteBarItems;
   UINT    m_uSelectedPaletteItem;

   zCHAR   m_chDialogReport;
   zUCHAR  m_fState;             // Initialized Scrollbars Grid ShowTabs
   CComboBox m_wndListOperations;
   CComboBox m_wndListWindows;
   CComboBox m_wndListActions;
   TZPalette m_wndPaletteBar;
   HFONT   m_hFontTab;
   CFont   *m_pFontWS;           // non-zero ==> override font [Workstation]
   CFont   *m_pFontCtrl;         // non-zero ==> override control fonts
   zVIEW   m_vTaskLPLR;
   CString m_csLPLR_Name;
   zCHAR   m_szReportFaceName[ 32 ];  // LF_FACENAME == 32
   zLONG   m_lReportFontSize;
   ZMultiIdxSet *m_pZMIXWebCtrl; // pointer to indexed ctrl list
   zBOOL   m_bMouseSwapped;
   zBOOL   m_bUseMapDlgUnits;

   DECLARE_MESSAGE_MAP( );
};

class TZCTL_CLASS TZToolBox : public CToolBar
{
   DECLARE_DYNAMIC( TZToolBox )
public:

   TZToolBox( ZSubtask *pZSubtask,
              TZPainterBar pPainterBar );
   virtual ~TZToolBox( );

   virtual BOOL DestroyWindow( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   void CreateButtons( zBOOL bShow = TRUE );
   void DeleteButtons( );
   void DeselectModal( );
   void SelectModalPointer( );
   zBOOL PaintToolBoxCtrl( TZPainterWindow *pPainterWindow,
                           CPoint ptStart, CPoint ptEnd );

   TZPainterBar *m_pBar;                // point up the chain
   zVIEW  m_vSubtask;
};

class TZOrderCtrl
{
public:
   TZOrderCtrl( TZPainterCtrl *pCtrl, zSHORT nArrayPos, CRect& rectCtrl );
   ~TZOrderCtrl( );

   TZPainterCtrl   *m_pCtrl;
   zSHORT          m_nArrayPos;
   CRect           m_rectCtrl;
   TZOrderCtrl     *m_pNext;
   TZOrderCtrl     *m_pPrev;
};

class TZUndoRedo
{
public:

   enum eUNDO_REDO_TYPE { eDelete, eMove, eSize, eCopy, eCreate, eClone };

   TZUndoRedo( TZPainterCtrl *pCtrl, zSHORT nIdNbr, zSHORT nArrayPos,
               zBOOL bSelected, eUNDO_REDO_TYPE eType, zCPCHAR cpcLabel = 0 );
   ~TZUndoRedo( );

   TZPainterCtrl   *m_pCtrl;
   zSHORT          m_nIdNbr;
   eUNDO_REDO_TYPE m_eType;
   zSHORT          m_nArrayPos;
   zCHAR           m_szLabel[ 64 ];
   TZUndoRedo      *m_pNext;
   TZUndoRedo      *m_pPrev;
   zUSHORT         m_uVisible;   // original Visibility value
   zBOOL           m_bSelected;  // original Selection state
   CRect           m_rect;       // original rect (only for Move or Resize)
   CRect           m_rectPainter; // original Painter rect
   TZPainterCtrl   *m_pParent;   // original parent (only for Move or Resize)
};

/////////////////////////////////////////////////////////////////////////////
//
// The TZPainterWindow represents the application window in the Zeidon
// object.  This window is totally unknown by the driver.  It is created
// each time a user desires to update a window.  The only control the
// driver "knows about" is the painter TZPainterBar class described above.
//
/////////////////////////////////////////////////////////////////////////////
#define zTZPW_UPDATEABLE         0x01
#define zTZPW_SHIFT_KEY          0x10
#define zTZPW_CTRL_KEY           0x20
#define zTZPW_INITIALIZING       0x40
#define zTZPW_SCROLLADJUSTED     0x80

#define zTZPTR_16THS_INCH        0x00000000
#define zTZPTR_256THS_INCH       0x00000001
#define zTZPTR_10THS_MM          0x00000002

class TZCTL_CLASS TZPainterWindow : public CFrameWnd
{
   DECLARE_DYNAMIC( TZPainterWindow )
public:

   TZPainterWindow( TZPainterBar *pPainterBar,
                    zVIEW vDialog = 0, zBOOL bPaintWindow = TRUE );
   virtual ~TZPainterWindow( );

   BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   virtual BOOL DestroyWindow( );
   virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg BOOL OnCreateClient( LPCREATESTRUCT lpcs,
                                CCreateContext *pContext );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnWindowPosChanged( WINDOWPOS *pWindowPos );
   afx_msg void OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnInitMenu( CMenu *pMenu );
   afx_msg void OnInitMenuPopup( CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnClose( );
   afx_msg void OnPaint( );
   afx_msg void OnNcPaint( );
   afx_msg BOOL OnNcActivate( BOOL bActive );
// afx_msg void OnNcDestroy( );

   void OnMenuSelection( );

   void GenerateTag( zPCHAR pchTagReturn, zLONG lType = 0,
                     zBOOL bUnique = TRUE );
   void MoveAllSelectedFramesToTop( );
   void RepaintSelectedCtrls( CWindowDC *pDC );
   void SnapRect( CRect *lpNewRect,
                  CRect *lpOldRect = 0,
                  zSHORT nForce = 0 );
   void AbutSelectedCtrls( zSHORT nAbut );
   void AlignSelectedCtrls( zSHORT nAlign );
   void EqualSpaceSelectedCtrls( zSHORT nSize );
   void SizeSelectedCtrls( zSHORT nSize );
   void DeleteSelectedCtrls( );
   void EqualSizePosClassCtrls( );
   void GroupBoxSelectedCtrls( );
   void UngroupSelectedGroupCtrls( );
   zSHORT DeleteCtrl( TZPainterCtrl *pCtrlParent,
                      zSHORT nIdNbr, zSHORT nTotalDeleteCnt );
   zBOOL DeselectAllCtrls( );
   void AddToUndo( TZPainterCtrl *pCtrl, zSHORT nIdNbr, zSHORT nArrayPos,
                   zBOOL bSelected, TZUndoRedo::eUNDO_REDO_TYPE eType,
                   zCPCHAR cpcLabel );
   void MoveToUndoRedo( zBOOL bToRedo );
   void FinalizeUndoRedo( );
   void EmptyPainterWindow( );
   void PaintZeidonWindow( zBOOL bReset = TRUE );
   void UpdateZeidonWindow( );
   void PaintActionBar( );
   void PaintZeidonCtrls( );
   void RepaintZeidonCtrl( );
   void SetZeidonWindowPosition( );
   void ShowZeidonWindowPosition( );
   void SetDIL_Text( TZPainterCtrl *pTZPainterCtrl = 0,
                     zBOOL bPosOnly = FALSE );
   void SetOverlapVisibility( );
   void SetChildOverlapVisibility( TZPainterCtrl *pCtrlParent,
                                   zBOOL bRemove );
   TZPainterCtrl * GetChildCtrl( TZPainterCtrl *pCtrlParent,
                                 TZPainterCtrl *pCtrl );

   zLONG SendCtrlMsg( zSHORT  nMessage,
                      zVIEW   vCtrl,
                      zLONG   lType,
                      zLONG   lParentType,
                      zBOOL   bPlaceHolder,
                      CRect   *rectNew,
                      zLONG   lZKey = 0,
                      TZPainterCtrl *pCtrl = 0,
                      zLONG   lCtrlID = 0,
                      zLONG   lTabStopNbr = 0,
                      zPCHAR  pchTag = 0,
                      zPCHAR  pchText = 0,
                      CWnd *pParentTWnd = 0 );
   TZPainterCtrl * PaintCtrl( TZPainterCtrl *pCtrlParent,
                              zCPCHAR cpcEntity,
                              zVIEW   vCtrl,
                              zLONG   lType,
                              CRect   *rectPaint,
                              zPCHAR  pchTag,
                              zPCHAR  pchText,
                              zLONG   lZKey = 0,
                              zBOOL   bSelect = FALSE );
   void ValidateCtrlTags( );
   void BuildTZPNEVWO( );
   zSHORT CreateMetaEntity( zVIEW vDlg, zPCHAR pchEntity, zSHORT nPos );

   zBOOL Select( TZPainterCtrl *pTZPainterCtrl, zBOOL bClear = TRUE );
   zBOOL Deselect( TZPainterCtrl *pTZPainterCtrl );
   zBOOL Remove( TZPainterCtrl *pPainterCtrl );

public:
   TZPainterBar    *m_pBar;            // pointer to control bar
   zLONG           m_lZKey;            // ZKey of window
   CString         m_csDlgTag;         // Tag for dialog/report
   CString         m_csWndTag;         // Tag for window

   // a linked list class of all edit controls which are currently
   // defined to this painter window.
   CArray<TZPainterCtrl *, TZPainterCtrl *> m_CtrlList;
   zSHORT          m_nCtrlCnt;
   TZPainterClient *m_pPainterClient;
   zSHORT          m_nSelectCnt;
   TZPainterCtrl   *m_pCurrCtrl;
   zVIEW           m_vDialog;          // View to window we are painting
   zSHORT          m_nSnap;            // 1=character height, 2=1/2 char
   TZPainterCtrl   *m_pLastSelected;   // Last selected ctrl
   TZPainterCtrl   *m_pDirectModeCtrl; // Mode to be edited directly
   zSHORT          m_nMaxTabIdNbr;     // Maximum Tab id set by painter
   TZPainterCtrl   *m_pPaintParent;    // Parent control for paint
   zLONG           m_lMaxZKey;         // Sync key assigned by painter
   CMenu           *m_pMenu;           // Menu for window being painted
   zUCHAR          m_fWndState;        // ShiftKey CtrlKey Updateable
   zUSHORT         m_uSizeX;           // Current Size of window
   zUSHORT         m_uSizeY;           // ----------"-----------
   zULONG          m_ulZOrder;         // ZOrder value for next selected ctrl

   zLONG           m_lTopMargin;       // ruler margin (in pixels);
   zLONG           m_lLeftMargin;      // report margin (in pixels);
   zLONG           m_lRptSizeX;        // X report size in units
   zLONG           m_lRptSizeY;        // Y report size in units
   zLONG           m_lReportTextSize;
   zLONG           m_lScrollUnitsX;
   zLONG           m_lScrollUnitsY;
   CPoint          m_pt;               // pt from last mouse button down

   TZPainterCtrl   *m_pLastGroupSet;
   TZActiveX       *m_pActiveX;        // ActiveX ctrl target for cmd
   zLONG           m_lHScrollMax;
   zLONG           m_lVScrollMax;
   zLONG           m_lUnit;
   zSHORT          m_nGranularity;
   zLONG           m_lPrintFlags;
   zULONG          m_ulCommonFlags;    // Common attributes of selected ctrls
   ZAttr           Attr;

   TZUndoRedo      *m_pUndoRedo;
   TZUndoRedo      *m_pUndoCurr;
   TZUndoRedo      *m_pRedoCurr;

   DECLARE_MESSAGE_MAP( );
};

class TZAmbientProperty : public CObject
{
   DECLARE_SERIAL( TZAmbientProperty );

public:
   TZAmbientProperty();
   TZAmbientProperty( DISPID dispid, LPCTSTR pszName, const VARIANT& var,
     int vti, BOOL tStock = FALSE );

   void Enable( BOOL tEnabled );
   DISPID GetID() const;
   CString GetName() const;
   const COleVariant& GetValue() const;
   int GetVTI() const;
   BOOL IsEnabled() const;
   BOOL IsStock() const;
   void SetValue( const VARIANT& varValue, int vti = -1 );

   void Serialize( CArchive& ar );

   #ifdef _DEBUG
   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }
   #endif


protected:
   DISPID m_dispid;
   CString m_csName;
   COleVariant m_varValue;
   int m_vti;
   BOOL m_tEnabled;
   BOOL m_tStock;
};

/////////////////////////////////////////////////////////////////////////////
//
// Client window within the TZPainterWindow
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZPainterClient : public CFormView
{
   DECLARE_DYNAMIC( TZPainterClient )
public:
   TZPainterClient( TZPainterWindow *pPainterWindow,
                    CCreateContext  *pContext );
   virtual ~TZPainterClient( );

   void DragOtherSelected( TZPainterCtrl *pCtrl,
                           CPoint        pt,
                           zBOOL         bShift = FALSE,
                           zBOOL         bCtrl = FALSE );
   void MoveOtherSelected( TZPainterCtrl *pCtrl, CPoint pt,
                           zSHORT nIdNbr = 0 );
   zBOOL OnlyCtrlContainsPt( TZPainterCtrl *pCtrl, CRect& rectBand );
   zBOOL ShowTabbing( zSHORT nShow = -1 );  //default swaps Tabbing flag
   void  RemoveTabbing( );
   zBOOL AdjustGroupSets( );
   void  ShowContextMenu( CPoint& pt );
   TZPainterCtrl * FindCtrlAtPt( CRect& rect, CPoint &pt );
   TZPainterCtrl * FindCtrlByCWnd( CWnd *pWnd );

   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   virtual BOOL OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                                   VARIANT *pVariant );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSize( UINT sizeType, int cx, int cy );
   afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos,
                           CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos,
                           CScrollBar *pScrollBar );
   afx_msg void OnPaint( );
   afx_msg void OnNcPaint();
   afx_msg void OnDraw( CDC *pDC );

   TZPainterWindow   *m_pPainterWindow;
   TZSelectCover     *m_pHeadSelect;

   CDC               *m_hDC;              // Device context for dragging
   zBOOL             m_bCapture;          // Capture indicator
   CPoint            m_ptMouseSelect;     // Position of mouse for selection
   CPoint            m_ptLastMouseMove;   // Last position of mouse for move
   zBOOL             m_bRButtonUp;
// zBOOL             m_bMouseSwap;

   zBOOL             m_bPaintWindow;
   TZRuler           *m_pRuler;
   TZCtrlCover       *m_pPageHeader;
   TZCtrlCover       *m_pPageFooter;
   zSHORT            m_nRecurse;
   zSHORT            m_nMultiMoveSize;
   ZAttr             Attr;

   CBitmap           *m_pBitmap;
   CBrush            *m_pBrush;

   TZAmbientProperty * FindAmbientProperty( DISPID dispid );
   TZAmbientProperty * FindAmbientProperty( LPCOLESTR pszName );

protected:
   zBOOL m_bTraceAmbientPropertyRequest;
   static LPCSTR m_cpcZPtrClassName;

   // members for Ambient Properties
   CArray<TZAmbientProperty *, TZAmbientProperty *> m_apList; // Ambient properties for control interaction at designtime
   CFontHolder* m_pFontHolder;
   FONTDESC m_descFont;

   void InitAmbientProperties();
   void RemoveAmbientProperties();

   DECLARE_MESSAGE_MAP( );
};

#if 0
/////////////////////////////////////////////////////////////////////////////
//
// Covering window for the TZPainterClient window to present moving of ctrls
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZClientCover : public CWnd
{
   DECLARE_DYNAMIC( TZClientCover )
public:

   TZClientCover( TZPainterWindow *pPainterWindow );

   virtual ~TZClientCover( );

   void EstablishCapture( UINT uModKeys, CPoint& pt,
                          TZCtrlCover *pCtrlCover = 0 );
   zPCHAR GetClassName( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnPaint( );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );

   TZPainterWindow   *m_pPainterWindow;

   DECLARE_MESSAGE_MAP( );
};
#endif

/////////////////////////////////////////////////////////////////////////////
//
// The TZTracker class is a frame placed on top of the CtrlCover for
// editing (selecting, moving, etc.) each ctrl as needed.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZTracker : public CRectTracker
{
public:
   TZTracker( LPCRECT pSrcRect, UINT nStyle,
              CPoint& ptSrc, TZPainterCtrl *pCtrl );
   ~TZTracker( );

   virtual void OnChangedRect( const CRect& rectOld );
   virtual void DrawTrackerRect( LPCRECT lpRect, CWnd *pWndClipTo,
                                 CDC *pDC, CWnd *pWnd );
// virtual UINT GetHandleMask( ) const;
// zBOOL   m_bHide;

   TZPainterCtrl *m_pCtrl;
   UINT    m_uStyle;
   CPoint  m_ptSrc;
   CPoint  m_ptPrev;
   CRect   m_rectSrc;
   CDC     *m_pDC;
   zSHORT  m_nHit;
};

/////////////////////////////////////////////////////////////////////////////
//
// The TZSelectCover class is a window placed on top of each CoverCtrl when
// the corresponding ctrl is selected.  Its size is slightly larger than
// that of the CoverCtrl to permit external sizing handles.  It also provides
// the capability to completely erase tracker sizing handles.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZSelectCover : public CWnd
{
   DECLARE_DYNAMIC( TZSelectCover )
public:
   TZSelectCover( TZPainterCtrl *pCtrl );
   virtual ~TZSelectCover( );

   void   TrackSelection( UINT uModKeys, CPoint pt,
                          zBOOL bFromMouseMove = FALSE );
   zBOOL  IsDoubleClick( zUSHORT uMessage, CPoint ptStart );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );

public:

   TZPainterCtrl  *m_pCtrl;
   TZTracker      *m_pTracker;
   TZSelectCover  *m_pNextSelect;
   zSHORT         m_nTrackerHandleSize;
   zBOOL          m_bFromMouseMove;
   zBOOL          m_bFirstPaint;

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// The TZCtrlCover class is a window placed on top of each
// control for editing (moving, selecting etc.) each control
// in the painter.  If a control is made not selectable and
// not displayable, for resource utilization reasons, this
// is deleted ... and re-created as necessary.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZCtrlCover : public CWnd
{
   DECLARE_DYNAMIC( TZCtrlCover )
public:
   TZCtrlCover( TZPainterWindow *pTZPainterWindow,
                TZPainterCtrl   *pCtrl,
                zCPCHAR         cpcTag );

   virtual ~TZCtrlCover( );

   void MoveCtrl( CRect& rectNew,
                  TZPainterCtrl *pNewParent = (TZPainterCtrl *) -1,
                  zSHORT nIdNbr = 0, zCPCHAR cpcLabel = 0 );
   void GetAdjustedWindowRect( CRect& rect, zBOOL bIncludeSelect );
   TZPainterCtrl * FindChildmostUnselected( TZPainterCtrl *pCtrlParent,
                                            CPoint& pt,
                                            zBOOL   bIgnoreSelected );
   zBOOL  IsDoubleClick( zUSHORT uMessage, CPoint ptStart );

   virtual BOOL DestroyWindow( );

   afx_msg void OnWindowPosChanged( WINDOWPOS *pWindowPos );
   afx_msg void OnPaint( );
   afx_msg void OnNcPaint( );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );

   TZPainterWindow *m_pPainterWindow;  // Pointer to painter window
   TZPainterCtrl   *m_pCtrl;           // Base class for all painter ctrls
   TZSelectCover   *m_pSelectCover;
   ZAttr           Attr;

   zBOOL           m_bMoving;
   static zSHORT   m_nFrameSizeX;
   static zSHORT   m_nFrameSizeY;
   static zSHORT   m_nMinWindowX;
   static zSHORT   m_nMinWindowY;
   static zSHORT   m_nDragX;
   static zSHORT   m_nDragY;

   DECLARE_MESSAGE_MAP( );
};

#define zDEBUG_CTRL
#ifdef zDEBUG_CTRL
void
TraceCtrl( zCPCHAR pchText,
           TZPainterCtrl *pCtrl );
#else
#define TraceCtrl
#endif

/////////////////////////////////////////////////////////////////////////////
//
// class TZPainterCtrl
//
// DESCRIPTION: The TZPainterCtrl class is a base class for all painter
//              controls.  It permits common access for events such as
//              CtlColor and SetVisibility
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZPainterCtrl
{
public:
   TZPainterCtrl( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd     *pWndParent,
                  CRect&   rect,
                  zVIEW    vCtrl,
                  CWnd     *pCtrl,
                  zCPCHAR  cpcTag,
                  zCPCHAR  cpcText,
                  zLONG    lType,
                  zBOOL    bPlaceHolder,
                  zLONG    lZKey,
                  zLONG    lTabStopNbr,
                  zLONG    lCtrlID,
                  zCHAR    chEntity = 'C' );

   virtual ~TZPainterCtrl( );

   TZPainterWindow *m_pPainterWindow;
   TZCtrlCover     *m_pCtrlCover;    // Corresponding ctrl cover window
   CWnd            *m_pWndCtrl;      // CWnd pointer
   CWnd            *m_pWndParent;    // Parent CWnd pointer
   ZAttr    Attr;
   WINDOWPLACEMENT m_wp;
   zVIEW    m_vCtrl;
   zLONG    m_lSubtype;          // Subtype from Zeidon Ctrl
   COLORREF m_clrText;           // non-zero ==> override text color
   CFont    *m_pFont;            // non-zero ==> override font
   zUSHORT  m_uVisible;          // see doc on TZPainterCtrl::SetVisibility
   zLONG    m_lCtrlID;           // Ctrl ID
   zCHAR    m_chEntity;          // 'C' - Ctrl; 'G' - Group; 'S' - groupSet
   CString  m_csText;            // for use by an individual ctrl
   CString  m_csLabel;           // for use by an individual ctrl
   zCHAR    m_chType;            // for use by an individual ctrl (e.g.
                                 // GroupSet uses Header/Footer/Entity)

   TZPainterCtrl *m_pCtrlParent; // Parent PainterCtrl
   TZPainterCtrl *m_pOrigParent; // Original Parent PainterCtrl
   zSHORT   m_nChildren;         // Number of children
   CRect    m_rectCtrl;          // Current Pos/Size of ctrl
   CRect    m_rectPainter;       // Current Pos/Size of CtrlCover (may not exist)
   zLONG    m_lRangeTop;         // Top of range of overlapped ctrl within group
   zLONG    m_lRangeBottom;      // Bottom of range of overlapped ctrl within group
   zLONG    m_lZKey;             // Key to sync up with Zeidon Object
   zLONG    m_lType;             // Type of ctrl from PPE
   zBOOL    m_bPlaceHolder;      // Ctrl is a placeholder (from PPE)
   CString  m_csTag;             // Tag for ctrl
   zSHORT   m_nTabStopNbr;       // TabStop number set by painter
   zBOOL    m_bCapture;          // Capture indicator
   CPoint   m_ptMouseSelect;     // Position of mouse for selection
   CPoint   m_ptLastMouseMove;   // Last position of mouse for move
   zBOOL    m_bShiftKey;         // Shift key with mouse select
   zBOOL    m_bCtrlKey;          // Ctrl key with mouse select
   zBOOL    m_bBuildOnDemand;    // Build sub-ctrls on demand
   zBOOL    m_bCtrlCoverSelected; // Selected via CtrlCover
   zULONG   m_ulZOrder;          // ZOrder based on latest selected ctrl

   virtual void  CreateZ( ) = 0;
   virtual void  DestroyZ( );

   void  ParentToPainterRect( CRect& rectReturn );
   void  PainterToParentRect( CRect& rectReturn );
   zSHORT Bigger( TZPainterCtrl *pCmprCtrl, CRect *rectCmpr = 0 );
   TZPainterCtrl * ValidateParentObject( CRect& rect,
                                         zLONG& lNewID,
                                         zSHORT nValidate );
   TZPainterCtrl * FindParentObject( CRect&      rectPainter,
                                     TZPainterCtrl *pLastParent,
                                     zBOOL       bNewObject );
   zBOOL PositionOnParentCtrl( zVIEW vDialog = 0 );
   zBOOL PositionOnZeidonCtrl( zVIEW vDialog = 0 );
   zBOOL Select( );
   zBOOL Deselect( );
   zBOOL IsSelected( );
   void  InvalidateParent( );

   void  CopySubordinateEntities( zVIEW vOrigCtrl, zVIEW vNewCtrl );
   TZPainterCtrl * CopyZeidonCtrl( TZPainterCtrl *pParent, zSHORT nIdNbr,
                                   zCPCHAR cpcLabel, CRect *rectPos = 0 );
   void  MoveZeidonCtrl( TZPainterCtrl *pNewParent );
   TZPainterCtrl * Clone( TZPainterCtrl *pParent, CRect& rectNew,
                          zSHORT nIdNbr, zCPCHAR cpcLabel );

   void  MoveCoveringWindow( );
   void  MoveDescendentCoveringWindows( );
   zSHORT GetVisibility( );
   zBOOL SetVisibility( zUSHORT uVisible, zSHORT nRecurse, zSHORT nFlag );
// void  GetMoveRects( CPoint& pt, CRect& rectPainter,
//                     CRect& rectOld, CRect& rectNew );

   // these must be virtual
   virtual HBRUSH CtrlColor( CDC *pDC, HBRUSH hBrushDflt );
   virtual void   SelectPoint( CPoint pt );
   virtual TZPainterCtrl * IsDeletable( );
   virtual void   SetupCtrlCover( ); // first chance the ctrl has to know
                                     // its corresponding pCtrlCover
   virtual void  EndPaint( );
   virtual TZPainterCtrl * GetRealCtrl( );
   virtual zBOOL IsChildValid( zLONG lChildId );
   virtual zBOOL IsModeValid( zULONG ulMode );
   virtual void  HideForUndo( zBOOL bHide );
   virtual void  ReturnFromMaintenanceWindow( );
   virtual void  PreMoveCtrl( CRect& rect );
   virtual void  PostMoveCtrl( );
   virtual TZPainterCtrl * AdjustCtrlRect( CRect& rect, zBOOL bToPainterRect );
   virtual void  RealizeCtrl( zBOOL bCreate );
   virtual void  UndoRedo( zBOOL bToRedo );
   virtual zULONG SetCommon( zULONG ulCommonFlags );
   virtual zBOOL ShowContextMenu( CPoint& pt );
   virtual void  WritePersistProperties( );
   virtual zBOOL SetFontOverride( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZWebCtrl
//
// DESCRIPTION: The TZWebCtrl class is a base class for all painter
//              controls.  It permits common access for events such as
//              CtlColor and SetVisibility
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZWebCtrl
{
public:
   TZWebCtrl( CRect&   rect,
              zCPCHAR  cpcTag,
              zLONG    lType );

   virtual ~TZWebCtrl( );

   CString   m_csTag;          // for use by an individual ctrl
   CRect     m_rect;           // Current Pos/Size of ctrl
   zLONG     m_lType;          // Ctrl type (PPE)
   ZNameItem *m_pTop;
   ZNameItem *m_pLeft;
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZText
//
// DESCRIPTION: The TZText class is used for painting text and display
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZText : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZText )
public:

   TZText( TZPainterWindow *pPainterWindow,
           TZPainterCtrl   *pCtrlParent,
           CWnd *pWndParent, CRect& rect,
           zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
           zLONG lTabStopNbr, zLONG lCtrlID,
           zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZText( );
   virtual void CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg void OnPaint( );

   zLONG   m_lBorderStyle;
   zLONG   m_lBorderColor;
   zLONG   m_lEscapement;
   zLONG   m_lTypeText;
   zBOOL   m_bLeft;
   zBOOL   m_bCenter;
   zBOOL   m_bRight;
   zCHAR   m_chMultiLine;
   UINT    m_uFormat;

   zCHAR   m_szFaceName[ 32 ];  // LF_FACENAME == 32
   zBOOL   m_bBold;
   zBOOL   m_bItalic;
   zBOOL   m_bStrikeout;
   zBOOL   m_bUnderline;
   zLONG   m_lSize;

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZEdit
//
// DESCRIPTION: The TZEdit class is used for painting edit
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZEdit : public CEdit, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZEdit )
public:

   TZEdit( TZPainterWindow *pPainterWindow,
           TZPainterCtrl   *pCtrlParent,
           CWnd *pWndParent, CRect& rect,
           zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
           zLONG lTabStopNbr, zLONG lCtrlID,
           zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZEdit( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZEditor
//
// DESCRIPTION: The TZEditor class is used for painting editor
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZEditor : public CEdit, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZEditor )
public:

   TZEditor( TZPainterWindow *pPainterWindow,
             TZPainterCtrl   *pCtrlParent,
             CWnd *pWndParent, CRect& rect,
             zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
             zLONG lTabStopNbr, zLONG lCtrlID,
             zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZEditor( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZPush
//
// DESCRIPTION: The TZPush class is used for painting pushbutton
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZPush : public CButton, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZPush )
public:

   TZPush( TZPainterWindow *pPainterWindow,
           TZPainterCtrl   *pCtrlParent,
           CWnd *pWndParent, CRect& rect,
           zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
           zLONG lTabStopNbr, zLONG lCtrlID,
           zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZPush( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZCalendar
//
// DESCRIPTION: The TZCalendar class is used for painting calendar
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZCalendar : public CWnd, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZCalendar )
public:

   TZCalendar( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZCalendar( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZDayTimer
//
// DESCRIPTION: The TZDayTimer class is used for painting day timer
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZDayTimer : public CWnd, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZDayTimer )
public:

   TZDayTimer( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZDayTimer( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );

   CBrush m_brBack;
   CPen   m_penNull;

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZControlBarG
//
// DESCRIPTION: The TZControlBarG class is used for painting control bar
//              (with gripper) ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef baseTZControlBarG
#define baseTZControlBarG TZSizingControlBarG
#endif

class TZCTL_CLASS TZControlBarG : public baseTZControlBarG, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZControlBarG )
public:

   TZControlBarG( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd *pWndParent, CRect& rect,
                  zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
                  zLONG lTabStopNbr, zLONG lCtrlID,
                  zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZControlBarG( );
   virtual void  CreateZ( );
   virtual void  HideForUndo( zBOOL bHide );
   virtual zBOOL IsModeValid( zULONG ulMode );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnSize( UINT sizeType, int cx, int cy );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZControlBarC
//
// DESCRIPTION: The TZControlBarC class is used for painting control bar
//              (with caption) ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef baseTZControlBarC
#define baseTZControlBarC TZSizingControlBarCF
#endif

class TZCTL_CLASS TZControlBarC : public baseTZControlBarC, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZControlBarC )
public:

   TZControlBarC( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd *pWndParent, CRect& rect,
                  zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
                  zLONG lTabStopNbr, zLONG lCtrlID,
                  zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZControlBarC( );
   virtual void  CreateZ( );
   virtual void  HideForUndo( zBOOL bHide );
   virtual zBOOL IsModeValid( zULONG ulMode );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnSize( UINT sizeType, int cx, int cy );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZIcon
//
// DESCRIPTION: The TZIcon class is used for painting icon
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZIcon : public CWnd, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZIcon )
public:
   // ctor
   TZIcon( TZPainterWindow *pPainterWindow,
           TZPainterCtrl   *pCtrlParent,
           CWnd *pWndParent, CRect& rect,
           zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
           zLONG lTabStopNbr, zLONG lCtrlID,
           zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZIcon( );

   virtual void CreateZ( );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );

   HICON    m_hIcon;   // Normal image
   zSHORT   m_nIconX;
   zSHORT   m_nIconY;
   CRect    m_rectIconText;
   zPCHAR   m_pchCtrlBOI;        // Hold the CtrlBOI
   zPCHAR   m_pchMsgText;        // Text when image does not exist

   static zSHORT m_nIconPixelSizeX;    // x-size of icon
   static zSHORT m_nIconPixelSizeY;    // y-size of icon
   static zSHORT m_nIconEdgeX;
   static zSHORT m_nIconEdgeY;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZBitmap
//
// DESCRIPTION: The TZBitmap class is used for painting bitmap
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZBitmap : public CBitmapButton, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZBitmap )
public:

   TZBitmap( TZPainterWindow *pPainterWindow,
             TZPainterCtrl   *pCtrlParent,
             CWnd *pWndParent, CRect& rect,
             zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
             zLONG lTabStopNbr, zLONG lCtrlID,
             zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZBitmap( );
   virtual void  CreateZ( );

   zPCHAR LoadDib( HINSTANCE hInstDLL, zPCHAR pchBitmap );
   void  SetupFromDib( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void DrawItem( LPDRAWITEMSTRUCT pDrawInfo );
   virtual BOOL OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                                   VARIANT *pVariant );

   ZDib     *m_pDib;
   zPCHAR   m_pchCtrlBOI;        // Hold the CtrlBOI
   zPCHAR   m_pchMsgText;        // Text when image does not exist
   zSHORT   m_nPixelWidth;
   zSHORT   m_nPixelHeight;

   zSHORT m_nRealId;                   // id of button

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZTimer
//
// DESCRIPTION: The TZTimer class is used for painting timer
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZTimer : public CBitmapButton, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZTimer )
public:

   TZTimer( TZPainterWindow *pPainterWindow,
            TZPainterCtrl   *pCtrlParent,
            CWnd *pWndParent, CRect& rect,
            zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
            zLONG lTabStopNbr, zLONG lCtrlID,
            zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZTimer( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void DrawItem( LPDRAWITEMSTRUCT pDrawInfo );

   ZDib     *m_pDib;

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZRadio
//
// DESCRIPTION: The TZRadio class is used for painting radiobutton
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZRadio : public CButton, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZRadio )
public:
   TZRadio( TZPainterWindow *pPainterWindow,
            TZPainterCtrl   *pCtrlParent,
            CWnd *pWndParent, CRect& rect,
            zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
            zLONG lTabStopNbr, zLONG lCtrlID,
            zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZRadio( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZCheck
//
// DESCRIPTION: The TZCheck class is used for painting radiobutton
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZCheck : public CButton, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZCheck )
public:
   TZCheck( TZPainterWindow *pPainterWindow,
            TZPainterCtrl   *pCtrlParent,
            CWnd *pWndParent, CRect& rect,
            zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
            zLONG lTabStopNbr, zLONG lCtrlID,
            zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZCheck( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZComboBox
//
// DESCRIPTION: The TZComboBox class is used for painting radiobutton
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZComboBox : public CComboBox, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZComboBox )
public:

   TZComboBox( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZComboBox( );

   virtual void  CreateZ( );
   virtual void  EndPaint( );
// virtual BOOL DestroyWindow( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );

private:

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZListBox
//
// DESCRIPTION: The TZListBox class is used for painting listbox
//              ctrls in the painter. The standard listbox ctrl
//              is painted and a scroll bar for the WYSIWYG look is
//              created as a child of the list box because windows
//              does not like to show the scroll bar on empty listboxes.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZListBox : public CListBox, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZListBox )
public:

   TZListBox( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd *pWndParent, CRect& rect,
              zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
              zLONG lTabStopNbr, zLONG lCtrlID,
              zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZListBox( );
   virtual void  CreateZ( );
// virtual BOOL DestroyWindow( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

private:

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZGroupBox
//
// DESCRIPTION: The TZGroupBox class is used for painting groups of
//              ctrls (grouped for tabbing purposes as well as
//              visual logical grouping) in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZGroupBox : public CButton, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZGroupBox )
public:

   TZGroupBox( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZGroupBox( );
   virtual void  CreateZ( );
   virtual BOOL  OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                                    VARIANT *pVariant );
   virtual void  RealizeCtrl( zBOOL bCreate );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );
// afx_msg void OnDrawItem( int nCtrlId, LPDRAWITEMSTRUCT pDrawInfo );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZNoteBook
//
// DESCRIPTION: The TZNoteBook class is used for painting the tab and notebook
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZNotePage;

class TZCTL_CLASS TZNoteBook : public CTabCtrl, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZNoteBook )
public:

   TZNoteBook( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZNoteBook( );

   virtual void  CreateZ( );
   virtual void  PostMoveCtrl( );
   virtual void  EndPaint( );
   virtual TZPainterCtrl * IsDeletable( );
   virtual TZPainterCtrl * GetRealCtrl( );
   virtual void  RealizeCtrl( zBOOL bCreate );
   virtual void  SelectPoint( CPoint pt );

   TZNotePage * GetActivePage( );
   TZNotePage * GetPage( zSHORT nPageNbr );

   void ActivateTab( zSHORT nTabNbr );

   virtual BOOL DestroyWindow( );

protected:
   void SelChange( NMHDR *pNMHDR, LRESULT *pResult );
   void SelChanging( NMHDR *pNMHDR, LRESULT *pResult );
   BOOL OnChildNotify( UINT message, WPARAM wParam,
                       LPARAM lParam, LRESULT *pResult );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT pDrawInfo );
   afx_msg void DrawItem( LPDRAWITEMSTRUCT pDrawInfo );

public:
   TZNotePage *m_pZNotePage; // head of linked list of Tab dialogs (one per tab)
   zSHORT     m_nPageCnt;
   zSHORT     m_nTabNbrMax;
// zSHORT     m_nActiveTabNbr;
   zBOOL      m_bDeletable;
   zBOOL      m_bInitialized;

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZNotePage
//
// DESCRIPTION: The TZNotePage class is used for grouping ctrls for each
//              tab within a tab/notebook ctrl.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZNotePage : public CWnd, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZNotePage )
public:

   TZNotePage( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZNotePage( );

   virtual void  CreateZ( );
   virtual TZPainterCtrl * IsDeletable( );
   virtual TZPainterCtrl * GetRealCtrl( );
   virtual void  PreMoveCtrl( CRect& rect );
   virtual TZPainterCtrl * AdjustCtrlRect( CRect& rect, zBOOL bToPainterRect );
   virtual void  RealizeCtrl( zBOOL bCreate );
   virtual void  UndoRedo( zBOOL bToRedo );
   virtual void  SelectPoint( CPoint pt );
   virtual BOOL OnAmbientProperty( COleControlSite *pSite, DISPID dispid,
                                   VARIANT *pVariant );
   virtual BOOL DestroyWindow( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnDrawItem( int nCtrlId, LPDRAWITEMSTRUCT pDrawInfo );

// afx_msg void OnPaint( );

   TZNoteBook *m_pZNoteBook;// TZNoteBook ctrl parent of this tab dialog
   TZNotePage *m_pNext;     // Next tab in linked list (rooted at TZNoteBook)
   zPCHAR     m_pchCaption;
   zSHORT     m_nTabNbr;    // original tab order
   zSHORT     m_nTabIdx;    // current index for this tab

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZGraph
//
// DESCRIPTION: The TZGraph class is used for painting graphic ctrls
//              in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZGraph : public CEdit, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZGraph )
public:

   TZGraph( TZPainterWindow *pPainterWindow,
            TZPainterCtrl   *pCtrlParent,
            CWnd *pWndParent, CRect& rect,
            zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
            zLONG lTabStopNbr, zLONG lCtrlID,
            zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZGraph( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZMLEdit
//
// DESCRIPTION: The TZMLEdit class is used for painting edit
//              ctrls in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZMLEdit : public CEdit, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZMLEdit )
public:

   TZMLEdit( TZPainterWindow *pPainterWindow,
             TZPainterCtrl   *pCtrlParent,
             CWnd *pWndParent, CRect& rect,
             zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
             zLONG lTabStopNbr, zLONG lCtrlID,
             zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZMLEdit( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZDil
//
// DESCRIPTION: The TZDil class is used for painting DIL message lines
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZDil : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZDil )
public:

   TZDil( TZPainterWindow *pPainterWindow,
          TZPainterCtrl   *pCtrlParent,
          CWnd *pWndParent, CRect& rect,
          zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
          zLONG lTabStopNbr, zLONG lCtrlID,
          zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZDil( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZGrid
//
// DESCRIPTION: The TZDil class is used for painting spreadsheet ctrls
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZGrid : public CListBox, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZGrid )
public:

   TZGrid( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd *pWndParent, CRect& rect,
                  zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
                  zLONG lTabStopNbr, zLONG lCtrlID,
                  zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZGrid( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZTree
//
// DESCRIPTION: The TZTree class is used for painting an tree ctrl
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZTree : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZTree )
public:

   TZTree( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZTree( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZERD
//
// DESCRIPTION: The TZERD class is used for painting the ERD Ctrl
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZERD : public CEdit, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZERD )
public:

   TZERD( TZPainterWindow *pPainterWindow,
          TZPainterCtrl   *pCtrlParent,
          CWnd *pWndParent, CRect& rect,
          zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
          zLONG lTabStopNbr, zLONG lCtrlID,
          zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZERD( );
   virtual void  CreateZ( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZGroupSetRpt
//
// DESCRIPTION: The TZGroupSetRpt class is used for painting the
//              Report GroupSet
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZGroupSetRpt : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZGroupSetRpt )
public:

   TZGroupSetRpt( TZPainterWindow *pPainterWindow,
                  TZPainterCtrl   *pCtrlParent,
                  CWnd *pWndParent, CRect& rect,
                  zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
                  zLONG lTabStopNbr, zLONG lCtrlID,
                  zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZGroupSetRpt( );
   virtual void  CreateZ( );
   virtual TZPainterCtrl * IsDeletable( );
   virtual zBOOL IsChildValid( zLONG lChildId );
   virtual void  SetupCtrlCover( ); // first chance the ctrl has to know
                                    // its corresponding pCtrlCover
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );

   zLONG   m_lGrpSetFlags;

   DECLARE_MESSAGE_MAP( );
};


#define zGROUP_OVERLAY     1

/////////////////////////////////////////////////////////////////////////////
//
// class TZGroupRpt
//
// DESCRIPTION: The TZGroupRpt class is used for painting the Report Group
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZGroupRpt : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZGroupRpt )
public:

   TZGroupRpt( TZPainterWindow *pPainterWindow,
               TZPainterCtrl   *pCtrlParent,
               CWnd *pWndParent, CRect& rect,
               zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
               zLONG lTabStopNbr, zLONG lCtrlID,
               zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZGroupRpt( );
   virtual void  CreateZ( );
   virtual TZPainterCtrl * IsDeletable( );
   virtual void  SetupCtrlCover( ); // first chance the ctrl has to know
                                    // its corresponding pCtrlCover

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );

   zBOOL   m_bDeletable;
   zLONG   m_lGrpFlags;

   DECLARE_MESSAGE_MAP( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZShape
//
// DESCRIPTION: The TZShape class is used for painting rectangles, lines,
//              or ellipses in the painter.
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZShape : public CWnd, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZShape )
public:
   // ctor
   TZShape( TZPainterWindow *pPainterWindow,
            TZPainterCtrl   *pCtrlParent,
            CWnd *pWndParent, CRect& rect,
            zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
            zLONG lTabStopNbr, zLONG lCtrlID,
            zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZShape( );

   virtual void  CreateZ( );

   afx_msg void OnPaint( );

   zPCHAR m_pchCtrlBOI;        // Hold the CtrlBOI
   zBOOL  m_bRectangle;
   zBOOL  m_bRoundRect;
   zBOOL  m_bEllipse;
   zBOOL  m_bDiamond;
   zBOOL  m_bHorizontalLine;
   zBOOL  m_bVerticalLine;
   zBOOL  m_bDiagonalDescend;
   zBOOL  m_bDiagonalAscend;
   zLONG  m_lPenWidth;
   zLONG  m_lPenType;
   zLONG  m_lPenColor;
   zLONG  m_lRoundValue;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZRuler
//
// DESCRIPTION: The TZRuler class is used for painting the Report ruler
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZRuler : public CWnd
{
   DECLARE_DYNAMIC( TZRuler )
public:

   TZRuler( TZPainterWindow *pPainterWindow );
   virtual ~TZRuler( );

   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnPaint( );

   ZAttr           Attr;
   TZPainterWindow *m_pPainterWindow;
   zBOOL           m_bTrack;
   zBOOL           m_bCapture;
   zLONG           m_lLastX;
   HCURSOR         m_hCursor;

   DECLARE_MESSAGE_MAP( );
};


#if 0
/////////////////////////////////////////////////////////////////////////////
//
// class TZTextRpt
//
// DESCRIPTION: The TZTextRpt class is used for painting the ERD Ctrl
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZTextRpt : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZTextRpt )
public:

   TZTextRpt( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd *pWndParent, CRect& rect,
              zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
              zLONG lTabStopNbr, zLONG lCtrlID,
              zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZTextRpt( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZMLE_Rpt
//
// DESCRIPTION: The TZMLE_Rpt class is used for painting the ERD Ctrl
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZMLE_Rpt : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZMLE_Rpt )
public:

   TZMLE_Rpt( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd *pWndParent, CRect& rect,
              zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
              zLONG lTabStopNbr, zLONG lCtrlID,
              zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZMLE_Rpt( );
};

/////////////////////////////////////////////////////////////////////////////
//
// class TZBMP_Rpt
//
// DESCRIPTION: The TZBMP_Rpt class is used for painting the ERD Ctrl
//
/////////////////////////////////////////////////////////////////////////////
class TZCTL_CLASS TZBMP_Rpt : public CStatic, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZBMP_Rpt )
public:

   TZBMP_Rpt( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd *pWndParent, CRect& rect,
              zVIEW vCtrl, zLONG lType, zBOOL bPlaceHolder, zLONG lZKey,
              zLONG lTabStopNbr, zLONG lCtrlID,
              zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZBMP_Rpt( );
};
#endif


class TZCTL_CLASS TZAxMenuVerb : public CObject
{
public:
   // ctor
   TZAxMenuVerb( OLEVERB verb){
    m_lVerb = verb.lVerb;
    m_csVerbName = verb.lpszVerbName;
    m_flags = verb.fuFlags;
    m_Attribs = verb.grfAttribs;
    }

   TZAxMenuVerb( ){};

   void Set( OLEVERB verb )
   {
      m_lVerb = verb.lVerb;
      m_csVerbName = verb.lpszVerbName;
      m_flags = verb.fuFlags;
      m_Attribs = verb.grfAttribs;
   }

   TZAxMenuVerb* operator = ( OLEVERB verb )
   {
      Set( verb );
      return this;
   }

   long GetVerb () { return m_lVerb; }
   CString GetName(){ return m_csVerbName; }
   DWORD GetFlags() { return m_flags; }

#ifdef _DEBUG
   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

   // dtor
   ~TZAxMenuVerb( ){};

private:

   zLONG m_lVerb;
   CString m_csVerbName;
   DWORD m_flags;
   DWORD m_Attribs;
};

////////////////////////////////////////////////////////////////////////////////////////
//
// class TZActiveX
//
// DESCRIPTION: The TZActiveX class is used for creating and painting an
//              ActiveX-Ctrl
//
////////////////////////////////////////////////////////////////////////////////////////

// Implements ActiveX support for the dialog painter
//
class TZCTL_CLASS TZActiveX : public CWnd, public TZPainterCtrl
{
   DECLARE_DYNAMIC( TZActiveX )
public:
   TZActiveX( TZPainterWindow *pPainterWindow,
              TZPainterCtrl   *pCtrlParent,
              CWnd *pWndParent, CRect& rect,
              zVIEW vCtrl, zLONG lType,
              zBOOL bPlaceHolder, zLONG lZKey,
              zLONG lTabStopNbr, zLONG lCtrlID,
              zPCHAR pchTag = 0, zPCHAR pchText = 0 );
   virtual ~TZActiveX();

   virtual void  CreateZ( );
   virtual void  WritePersistProperties();

   // Function to Communicate with the Property Dialog in Designmode
   zSHORT   GetAttributeCount();
   zSHORT   CreateZeidonControl( );
   void     ShowProperties();

   LPOLEOBJECT GetIOleObjectInterfacePtr();
   POSITION  GetFirstEventPosition();
   ZFunction *GetNextEvent( POSITION &pos ); // An event is a dispatch method
                                             // so this method returns a
                                             // ZFunction pointer
   void RemoveVerbs();
   void OnMenuCommand( UINT uCmd );
   //{{AFX_VIRTUAL(TZActiveX)
   public:
   virtual BOOL DestroyWindow( );
   //}}AFX_VIRTUAL


   //{{AFX_MSG(TZActiveX)
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   virtual zBOOL IsModeValid( zULONG ulMode );

protected:
   TZPainterClient *GetPainterClient() { return m_pPainterWindow->m_pPainterClient; }
   DWORD GetRequiredMemoryLength( );

   BOOL CreateActiveXObjectWithPersistProperties();
   LPOLECONTROL GetIOleControlInterfacePtr();
   BOOL InterfaceConnect( LPUNKNOWN pobj, REFIID riid,
                          IUnknown *pIUnknownSink, LPDWORD pdwConn );
   BOOL InterfaceDisconnect( LPUNKNOWN pobj, REFIID riid,
                             LPDWORD pdwConn );
   BOOL ConnectPropertyNotifySink( );
   BOOL LoadPersistProperties( );
   BOOL QueryRequiredInterfaces();
   void DestroyActiveX();
   BOOL RequestLicenseKey( BSTR &bstrLicenseKey, CString strClsid );
   void BuildLicKeyString();
public:
   BOOL m_bPropertiesChanged;

   DECLARE_INTERFACE_MAP()
   BEGIN_INTERFACE_PART( PropertyNotifySink, IPropertyNotifySink )
     STDMETHOD( OnChanged )( DISPID dispid );
     STDMETHOD( OnRequestEdit )( DISPID dispid );
   END_INTERFACE_PART( PropertyNotifySink )

#if 0
   BEGIN_INTERFACE_PART( AmbientProperties, IDispatch )
      STDMETHOD( GetIDsOfNames )( REFIID iid, LPOLESTR *ppszNames,
                                  UINT nNames, LCID lcid, DISPID *pDispIDs );
      STDMETHOD( GetTypeInfo )( UINT iTypeInfo, LCID lcid,
                                ITypeInfo **ppTypeInfo );
      STDMETHOD( GetTypeInfoCount )( UINT *pnInfoCount );
      STDMETHOD( Invoke )( DISPID dispidMember, REFIID iid, LCID lcid,
                           WORD wFlags, DISPPARAMS *pdpParams,
                           VARIANT *pvarResult, EXCEPINFO *pExceptionInfo,
                           UINT *piArgError );
   END_INTERFACE_PART( AmbientProperties )
#endif

private:

   ZTypeLibrary      m_TypeLibrary; // for browsing events and methods
   LPOLEOBJECT       m_lpObject;
   LPOLECONTROL      m_lpOleControl;
   ZDrDoc            *m_pDocument;
   CString           m_csClassID;   // the controls GUID
   CArray<TZAxMenuVerb *, TZAxMenuVerb *> m_VerbList; // available Context Menus of the ActiveX

   DWORD m_dwPropNotifySink;
   BOOL  m_bSameAsLoaded;
   BSTR  m_bstrLicenseKey;
   CString m_strLicenseKey;
   BOOL  m_bLicensed;
   CString m_csStorageFileName;
   zBOOL m_bTraceAmbientPropertyRequest;
   zBOOL m_bTraceBlob;
   zBOOL m_bTraceDisplayEntityInstance;
#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CWnd::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }

public:
   static int m_iInstanceCount;
#endif

};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

extern "C"
{
// Painter call stub for driver

zOPER_EXPORT zLONG  OPERATION
PainterCall( zSHORT     nMessage,
             zVIEW      vTZPNTROO,
             TZPainterBar *pTZPainterBar,
             zVIEW      vDialog,
             zPCHAR     pchParm,
             zLONG      lMaxLth );

zOPER_EXPORT void OPERATION
PainterSnapRect( TZPainterBar *pPainterBar,
                 CRect *lpNewRect, CRect *lpOldRect,
                 zSHORT nCharSnapValueX, zSHORT nCharSnapValueY );

zOPER_EXPORT void OPERATION
GetContextFromSrcCtrl( zVIEW  vSrcDialog,
                       zPCHAR pchContext );

zOPER_EXPORT zSHORT OPERATION
ConvertBitmap( zVIEW   vSubtask,
               zVIEW   vTgtDialog,
               zVIEW   vTgtRemote,
               zVIEW   vSrcDialog,
               zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertBitmapBtn( zVIEW   vSubtask,
                  zVIEW   vTgtDialog,
                  zVIEW   vTgtRemote,
                  zVIEW   vSrcDialog,
                  zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertButton( zVIEW   vSubtask,
               zVIEW   vTgtDialog,
               zVIEW   vTgtRemote,
               zVIEW   vSrcDialog,
               zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertCalendar( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertCheckBox( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertControlBar( zVIEW   vSubtask,
                   zVIEW   vTgtDialog,
                   zVIEW   vTgtRemote,
                   zVIEW   vSrcDialog,
                   zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertComboBox( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertDayTimer( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertDIL( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertEditBox( zVIEW   vSubtask,
                zVIEW   vTgtDialog,
                zVIEW   vTgtRemote,
                zVIEW   vSrcDialog,
                zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertERD( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertGraph( zVIEW   vSubtask,
              zVIEW   vTgtDialog,
              zVIEW   vTgtRemote,
              zVIEW   vSrcDialog,
              zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertGroupBox( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertHD( zVIEW   vSubtask,
           zVIEW   vTgtDialog,
           zVIEW   vTgtRemote,
           zVIEW   vSrcDialog,
           zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertIcon( zVIEW   vSubtask,
             zVIEW   vTgtDialog,
             zVIEW   vTgtRemote,
             zVIEW   vSrcDialog,
             zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertListBox( zVIEW   vSubtask,
                zVIEW   vTgtDialog,
                zVIEW   vTgtRemote,
                zVIEW   vSrcDialog,
                zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertMLE( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertTree( zVIEW       vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertRadioBtn( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertRadioBtnGroup( zVIEW   vSubtask,
                      zVIEW   vTgtDialog,
                      zVIEW   vTgtRemote,
                      zVIEW   vSrcDialog,
                      zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertRTE( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertSS( zVIEW   vSubtask,
           zVIEW   vTgtDialog,
           zVIEW   vTgtRemote,
           zVIEW   vSrcDialog,
           zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertTab( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertTabCtl( zVIEW   vSubtask,
               zVIEW   vTgtDialog,
               zVIEW   vTgtRemote,
               zVIEW   vSrcDialog,
               zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertText( zVIEW   vSubtask,
             zVIEW   vTgtDialog,
             zVIEW   vTgtRemote,
             zVIEW   vSrcDialog,
             zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertTimer( zVIEW   vSubtask,
              zVIEW   vTgtDialog,
              zVIEW   vTgtRemote,
              zVIEW   vSrcDialog,
              zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertToolBar( zVIEW   vSubtask,
                zVIEW   vTgtDialog,
                zVIEW   vTgtRemote,
                zVIEW   vSrcDialog,
                zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertControlBar( zVIEW   vSubtask,
                   zVIEW   vTgtDialog,
                   zVIEW   vTgtRemote,
                   zVIEW   vSrcDialog,
                   zPSHORT pnCtrlIdNbr );
zOPER_EXPORT zSHORT OPERATION
ConvertTrueGrid( zVIEW   vSubtask,
           zVIEW   vTgtDialog,
           zVIEW   vTgtRemote,
           zVIEW   vSrcDialog,
           zPSHORT pnCtrlIdNbr );

zOPER_EXPORT zSHORT OPERATION
CheckDuplicateTags( zVIEW   vSubtask,
                    zVIEW   vSrcDialog,
                    zLONG   lFlags );
void
TraceCtrlCnt( zCPCHAR cpcMsg,
              TZPainterBar *pPainterBar );
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//  C interface prototypes to tools C++ ctrls
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ER_Diagram( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// not "C" interface
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zBOOL
CheckDuplicateTagForWindow( zVIEW vSrcDialog,
                            zCPCHAR cpcWndTag,
                            zCPCHAR cpcCtrlTag );
zBOOL
fnRectMostlyWithinRect( CRect& rect1,
                        CRect& rect2 );
void
fnDeleteUnselectedCtrls( zVIEW vDialog,
                         TZPainterWindow *pPainterWindow );

zSHORT
fnMergeCtrls( TZPainterWindow *pPainterWindow,
              zVIEW  vTgt,
              zVIEW  vSrc,
              zVIEW  vTgtCtrl,
              zVIEW  vSrcLPLR,
              zVIEW  vPE );
zSHORT
fnMergeMenu( zVIEW  vSubtask,
             TZPainterWindow *pPainterWindow,
             zVIEW  vTgt,
             zVIEW  vSrc,
             zVIEW  vSrcLPLR,
             zVIEW  vPE );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**************************************************************
   Generated wrapper for ActiveX library named: TBEDITLib
   Generated at:   Wednesday, April 28, 1999
**************************************************************/


/*
Dispatch interface for TBEdit Control
*/
class TZCTL_CLASS ZDrTBEdt : public  COleDispatchDriver
{
public:
   ZDrTBEdt() {}
   ZDrTBEdt(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ZDrTBEdt(const ZDrTBEdt& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

public: // methods for setting and getting poperties


public: // methods of the current Dispinterface

   BOOL   OpenObject( BSTR* FileName );
   BOOL   CloseObject( );
   long   DeleteText( );
   long   CutText( );
   long   CopyText( );
   long   PasteText( );
   void   SyntaxOff( );
   void   SironSyntaxOn( );
   void   ZeidonSyntaxOn( );
   BOOL   PropertyDialog( );
   void   GetCursorPosition( long *lLinie, long *lSpalte, long *lIndex );
   void   GetLineCount( long *lZeilen );
   long   InsertItem( BSTR *pDaten );
   void   IsObjectChanged( BOOL *bChanged );
   void   PrintObject( );
   void   ReadOnlyModus( BOOL bReadOnly );
   BOOL   SaveObject( );
   BOOL   SaveObjectAs( BSTR *pFileName );
   BOOL   SelectItem( long lZeile, BSTR *pDaten );
   void   SetCursorPositionByIndex( long lIndex );
   void   SetCursorPositionByLine( long lZeile, long lSpalte );
   BOOL   Undo( );
   BOOL   Redo( );
   BOOL   TempSaveObject( BSTR *pTempName );
   long   FindTextPosition( BSTR *pSearchString, long *lTextPosition, long lOptions );
   long   GetActualTextLine( BSTR *pBuffer, long lLength );
   BOOL   IsReadOnly( );
   BOOL   IsSubWindowActive( );
   void   WantKeystrokes( BOOL bWantKeys );
   BOOL   DeleteTextRange( long lStartIndex, long lEndIndex );
   BOOL   CloseSubWindow( );
   void   SearchForward( );
   void   SearchBackward( );
   void   Deselect( );
   long   GetTextFromLineOfIndex( BSTR *pBuffer, long lLength, long lIndex );
   long   SetSironLanguage( long lLanguage );
   void   GetPositionByIndex( long lIndexParam, long *lLine, long *lCol );
   BOOL   IsCommentAtIndex( long lIndex );
   void   SearchWindow( );
   void   ReplaceWindow( );
   void   SelectAll( );
   BOOL   GotoWindow( );
   BOOL   CanCopy( );
   BOOL   CanPaste( );
   BOOL   CanUndo( );
   BOOL   CanRedo( );
   BOOL   SelectRange( zLONG lIndec, zLONG lLength );
   long   GetTextFromRange( BSTR *pBuffer, long lStart, long lEnd );
   long   GetObjectName( BSTR *pBuffer );
   long   GetSelectedText( BSTR *pBuffer, long lLength );

/////////////////////////////////////////////////////////////////////////////
//
// Helper functions
//

   BOOL OpenObject(LPCSTR FileName);
   long InsertItem(LPCSTR pDaten);
   BOOL SaveObjectAs(LPCSTR pFileName);
   BOOL SelectItem(long lZeile, LPCSTR pDaten);
   BOOL TempSaveObject(LPCSTR pTempName);
   long FindTextPosition(LPCSTR pSearchString, long *lTextPosition, long lOptions);
   long GetActualTextLine(LPSTR pBuffer, long lLength);
   long GetTextFromLineOfIndex(LPSTR pBuffer, long lLength, long lIndex);
   long GetTextFromRange(LPSTR pBuffer, long lStart, long lEnd );
   long GetObjectName( LPSTR pBuffer );
   long GetSelectedText( LPSTR pBuffer, long lLength );
};

#if 0
/*
Event interface for TBEdit Control
*/
class _DTBEditEvents : public  COleDispatchDriver
{
public:
   _DTBEditEvents() {}
   _DTBEditEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   _DTBEditEvents(const _DTBEditEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

public: // methods for setting and getting poperties


public: // methods of the current Dispinterface

   void   SetInsertMode( );
   void   RightMouseClick( BSTR *pName );
   void   SetLine( long lZeile, long lSpalte );
   void   SetOverwriteMode( );
   void   Keystroke( short sKey, BOOL *__MIDL_0014 );
};

/*
TBEdit Control
*/
#endif


#endif  // __tzctl_h__ sentry.
