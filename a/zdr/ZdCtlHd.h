/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlhd.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlhd_h__  // Sentry, use file only if it's not already included
#define __zdctlhd_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
HierDiagram( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

#define POSITION_NODE   1          // test Node Positioning
#define DEBUG_TRACE     0          // test Node Positioning
#define MAX_LEVELS      31

#define  tzHDCELLSIZE           m_pHD_Diagram->m_lCellSize
#define  tzHDPRINTCELLSIZE      21   // number of pixels per cell @ 100%
#define  tzHDARCSIZE            9
#define  tzHDDIAGRAMX           1000
#define  tzHDDIAGRAMY           1000
#define  tzHDDISPLAYPOS         m_pHD_Diagram->m_lDisplayPos
#define  tzHDMAXGRID            (zLONG) (((zLONG) tzHDDIAGRAMX * (zLONG) tzHDDIAGRAMY) - 1)
#define  tzHDMAXPORTX           (tzHDNODEX / 2)
#define  tzHDMAXPORTY           (tzHDNODEY / 2)
#define  tzHDLINETEXTOFFSET     2
#define  tzHDFONTHEIGHT         (((tzHDCELLSIZE + (tzHDCELLSIZE / 2 )) - \
                                  tzHDLINETEXTOFFSET))
//#define  tzHDFONTWIDTH          ((tzHDFONTHEIGHT * 2) / 3)
#define  tzHDFONTWIDTH          0
#define  tzHDFONTWEIGHT         100
#define  tzHDFONTFF             VARIABLE_PITCH | FF_DONTCARE
#define  tzHDFONTNAME           "Arial" // "Times New Roman"

#define  tzHDCOLORBACKGROUND ::GetSysColor( COLOR_BTNFACE )
#define  tzHDCOLORNORMAL     m_pHD_Diagram->m_clrNormal
#define  tzHDCOLORSELECTED   m_pHD_Diagram->m_clrSelected
#define  tzHDCOLORLINES      m_pHD_Diagram->m_clrLines
#define  tzHDCOLORBOX        m_pHD_Diagram->m_clrBox
#define  tzHDCOLORTEXT       m_pHD_Diagram->m_clrText
#define  tzHDCOLORBLACK      RGB( 0, 0, 0 )
#define  tzHDCOLORWHITE      RGB( 255, 255, 255 )
#define  tzHDCOLORGRAY       RGB( 190, 190, 190 )
#define  tzHDCOLORSHADE      RGB( 228, 228, 228 )

// Indicators for what is in a cell
#define  tziNODE        0x01
#define  tziLINE        0x02

LRESULT CALLBACK
ZeidonCC_WndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam );

class ZDCTL_CLASS ZHD_Node;

// Container class for Entities in diagram
class ZDCTL_CLASS ZHD_Diagram : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZHD_Diagram )
public:
   ZHD_Diagram( ZSubtask *pZSubtask,
                CWnd     *pWndParent,
                ZMapAct  *pzmaComposite,
                zVIEW    vDialog,
                zSHORT   nOffsetX,
                zSHORT   nOffsetY,
                zKZWDLGXO_Ctrl_DEF *pCtrlDef );
   virtual ~ZHD_Diagram( );


   ZHD_Diagram *m_pHD_Diagram;  // pointer to ourself for defines
   ZHD_Node *m_pLastPosNode;

   CArray<ZHD_Node *, ZHD_Node *> m_NodeList;
   zSHORT    m_nNodeCnt;
   CDC       *m_hDC;
   int       m_nOldMapMode;
   CSize     m_sizeOldWExt;
   CSize     m_sizeOldVExt;
   zLONG     m_lMinScale; // Minimum Scaling factor
   zLONG     m_lScale;    // Scaling factor
   zBOOL     m_bOrientation;
   zBOOL     m_bCollapsable;
   zBOOL     m_bPrintDC;
   zBOOL     m_bShadeEnts;  // Print - Shade Selected Entities in Gray
   zBOOL     m_bShadeRels;  // Print - Shade Selected Entities in Gray
   zBOOL     m_bUpdateView;
   zBOOL     m_nLDblClk;   // Left  button double click occured
   zBOOL     m_nRDblClk;   // Right button double click occured
   zLONG     m_lCellSize;
   zLONG     m_lMinPosX;  // Minimum Entity Pos X
   zLONG     m_lMinPosY;  // Minimum Entity Pos Y
   zLONG     m_lMaxPosX;  // Maximum Entity Pos X
   zLONG     m_lMaxPosY;  // Maximum Entity Pos Y
   ZHD_Node  *m_pMinPosXNode;
   ZHD_Node  *m_pMaxPosXNode;
   ZHD_Node  *m_pMinPosYNode;
   ZHD_Node  *m_pMaxPosYNode;
   zBOOL     m_bCapture;
   zBOOL     m_bMapped;
   zBOOL     m_bScrolledWindow;
   CPoint    m_ptMouseSelect;
   zLONG     m_lMouseOffsetX;
   zLONG     m_lMouseOffsetY;
   zLONG     m_ptLastMouseMove;
   zLONG     m_lDisplayPos;
   zLONG     m_lDisplayPosMax;
   zLONG     m_lUpdateDisplayPos;
   zLONG     m_lUpdateDisplayPosMax;
   COLORREF  m_clrNormal[ 16 ];
   COLORREF  m_clrSelected;
   COLORREF  m_clrNoUpdate;
   COLORREF  m_clrBoxU;
   COLORREF  m_clrLinesU;
   COLORREF  m_clrTextU;
   COLORREF  m_clrBox;
   COLORREF  m_clrLines;
// COLORREF  m_clrText;     from ZMapAct

   zSHORT  NODE_HEIGHT, NODE_WIDTH,
           X_SPACING, HALF_X_SPACING, Y_SPACING;
   zSHORT  m_nMaxNodeEdge[ MAX_LEVELS + 1 ];
   zSHORT  m_nLevel;

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );

   void   DrawDiagram( );
   zSHORT ZoomDiagram( zSHORT nPercent, zSHORT nOptions );
   zSHORT PrintDiagram( zSHORT nPercent = 0, zSHORT nOptions = 0 );
   zLONG  AnalyzePages( zLONG& lHorizPages, zLONG& lScale,
                        zLONG& lPageSizeX, zLONG& lPageSizeY,
                        zSHORT nPercent, zSHORT nOptions,
                        CPoint ptDiagramSize, CPoint ptPageSize );
   void   RecursDrawNodes( zVIEW vDiagram, ZHD_Node *pParentNode );
   void   SetNodeMetrics( );
   zSHORT PositionNodes( zVIEW vDiagram );
   void   CalculateMinimumScale( CRect& rect );
   void   CalculateDisplaySize( zLONG& lDisplaySizeX,
                                zLONG& lDisplaySizeY,
                                CRect&  rect,
                                zLONG   lScale );
   void   SetDisplayPosMax( zLONG lX, zLONG lY );
   void   SetScrollBars( );
   zSHORT SetProperties( zLONG lProperties );
   zSHORT PosX( zLONG lIdx );
   zSHORT PosY( zLONG lIdx );
   void   GetDeviceContext( );
   void   ReleaseDeviceContext( );
   zBOOL  PositionVisible( zLONG lStartIdx, zLONG lEndIdx );
   void   SetDisplayBoundaries( CRect& rect, zBOOL bUpdate = 0 );
   void   SetMaxPositions( );
   void   MakeLogicalPoint( CPoint& pt );
   ZHD_Node *PointInNode( CPoint pt );
   zVIEW  PositionOnDiagramNode( ZHD_Node *pZHD_Node, zVIEW vDiagram = 0 );
   zBOOL  IsDoubleClick( zUSHORT uMessage, CPoint ptStart );

   afx_msg void OnLButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );

   afx_msg void OnPaint( );

   virtual BOOL DestroyWindow( );

protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );

   DECLARE_MESSAGE_MAP( )
};

class ZDCTL_CLASS ZHD_Node
{
public:
   ZHD_Node( ZHD_Diagram *pHD_Diagram,
             ZHD_Node    *pParentNode,
             zLONG       lXPos = 0,
             zLONG       lYPos = 0,
             zPCHAR      szText = 0,
             zPCHAR      szTitle = 0,
             zPCHAR      szDIL  = 0,
             zPCHAR      cOutsideText1 = 0,
             zPCHAR      cOutsideText2 = 0,
             zPCHAR      cOutsideText3 = 0,
             zPCHAR      cOutsideText4 = 0,
             zPCHAR      cOutsideText5 = 0,
             zPCHAR      pchType = 0,
             zLONG       lZKey  = 0,
             zBOOL       bSelect = 0,
             zLONG       lColor = 0 );
   virtual ~ZHD_Node( );

   ZHD_Diagram *m_pHD_Diagram; // pointer to diagram class
   ZHD_Node *m_pParentNode;   // pointer to parent node
   zLONG   m_lZKey;           // ZKey of Node
   zLONG   m_lPosIdx;         // Grid position of Entity
   CString m_csText;          // Node Text
   CString m_csTitle;         // Node Title
   CString m_csDIL;           // Node DIL
   CString m_csOutsideText1;  // OutsideText1
   CString m_csOutsideText2;  // OutsideText2
   CString m_csOutsideText3;  // OutsideText3
   CString m_csOutsideText4;  // OutsideText4
   CString m_csOutsideText5;  // OutsideText5
   zCHAR   m_chType;          // Entity type
   zBOOL   m_bSelected;       // Node has been selected
   zBOOL   m_bCollapsed;      // Node has been collapsed
   zBOOL   m_bHidden;         // Node is hidden
   zLONG   m_lColor;          // Color for node

   void    Select( zVIEW vDiagram = 0 );
   zSHORT  DrawNode( );
   zSHORT  DrawNodeLine( zBOOL bErase = 0 );
   void    EraseNode( );
   void    CenterNode( );
   zSHORT  PosX( zLONG lIdx = -1 );
   zSHORT  PosY( zLONG lIdx = -1 );
};

#endif   // __zdctlhd_h__ sentry.
