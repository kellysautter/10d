/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlcmb.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlcmb_h__  // Sentry, use file only if it's not already included
#define __zdctlcmb_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

class ZDCTL_CLASS ZMCComboEdit;
class ZComboColumn;
//class ZComboEdit;

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ComboBox( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZComboBox standard control
#ifdef zREMOTE_SERVER
class ZDCTL_CLASS ZComboBox : public ZRmtDataSet, public ZMapAct
#else
class ZDCTL_CLASS ZComboBox : public CComboBox, public ZMapAct
#endif
{
   DECLARE_DYNAMIC( ZComboBox )
public:
   // ctor
   ZComboBox( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZComboBox( );

   virtual void   CreateZ( );
   virtual zBOOL  HandleAccel( zACCEL_DEF& ad );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zSHORT SetCtrlMapping( zCPCHAR cpcViewName,
                                  zCPCHAR cpcEntityName,
                                  zCPCHAR cpcAttributeName,
                                  zCPCHAR cpcContextName,
                                  zLONG   lColumn = -1 );
   virtual zSHORT GetCtrlMapping( zPCHAR pchViewName,
                                  zPCHAR pchEntityName,
                                  zPCHAR pchAttributeName,
                                  zPCHAR pchContextName,
                                  zLONG  lColumn = -1 );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual void   ParentMove( );

   void    SetDisplaySelSet( zSHORT nSelSet );
   void    SetSelectSelSet( zSHORT nSelSet );
   zSHORT  SetData( zCPCHAR cpcSelectItem, zCPCHAR cpcListData );
   zLONG   SelectExactString( zLONG lStartAfter, zCPCHAR cpcSearchString );
   zSHORT  GetSelectedString( zPCHAR pchSelectedString, zLONG lMaxLth );
   zSHORT  BuildListFromOI( zBOOL bRefresh = FALSE );
   void    ProcessCommand( zUSHORT uNotificationCode, zLONG lSelectedIdx = -1 );

   void    FormatComboBox( );
// void    SetColumnWidth( zSHORT nColumnWidth0, ... );
// void    SetColumnAlignStyle( zSHORT nColumnAlignStyle0, ... );
// zLONG   AddRow( zCPCHAR cpcString0, ... );

   void    AddColumn( zSHORT nCol, zSHORT nColWidth, zLONG lAlignStyle,
                      zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                      zCPCHAR cpcAttribute, zCPCHAR cpcContext );
   ZComboColumn * FindColumn( zSHORT nCol ) const;
   void    RemoveColumns( );

// void    ResetContent( );
   zLONG   GetCurrColumnItem( zPCHAR pchReturnText,
                              zSHORT nMaxLth, zSHORT nCol );
   zSHORT  FormatTextAtPosition( zPCHAR pchText, zLONG lRow,
                                 zSHORT nCol, zSHORT nMaxLth );
   zSHORT  SetDataFromOI( zCPCHAR cpcSelectItem,
                          zVIEW   vAppOI,
                          zCPCHAR cpcEName,
                          zCPCHAR cpcAName,
                          zCPCHAR cpcContext,
                          zVIEW   vAppBrowseOI,
                          zCPCHAR cpcBrowseEName,
                          zCPCHAR cpcBrowseAName,
                          zCPCHAR cpcBrowseScope,
                          zCPCHAR cpcBrowseContext,
                          zSHORT  nDisplaySelSet,
                          zULONG  ulFlag );
   zSHORT  SetDataFromDomain( zCPCHAR cpcSelectItem,
                              zCPCHAR cpcVName,
                              zCPCHAR cpcEName,
                              zCPCHAR cpcAName,
                              zCPCHAR cpcContext,
                              zULONG  ulFlag );

protected:

   virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
   virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
   virtual int  CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct );
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual LRESULT DefWindowProc( UINT uMessage, WPARAM wParam, LPARAM lParam );

   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnEnable( BOOL bEnable );
// afx_msg void OnSetFocus( CWnd *pWndLostFocus );
// afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
// afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlColor );
// afx_msg void OnPaint( );

   //
   // child id notifications handled at the child
   //
   LRESULT OnGetText( WPARAM wParam, LPARAM lParam );
   LRESULT OnSetText( WPARAM wParam, LPARAM lParam );

   void CBNDropdown( );
   void CBNCloseup( );
   void CBNDblclk( );
   void CBNEditchange( );
// void CBNEditupdate( );
   afx_msg void OnEditUpdate();
   void CBNErrspace( );
   void CBNSelchange( );
   void CBNSelendok( );
   void CBNSelendcancel( );

   void CBNSetFocus( );
   void CBNKillFocus( );

#ifdef zREMOTE_SERVER

   void  Clear( )
   {
      SetZCtrlText( this, "" );
   }

   void  MapToRemoteDataset( );
   void  MapFromRemoteDataset( );

#else

   virtual BOOL DestroyWindow( );

protected:

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
#endif

private:    // Methods:

#ifdef zREMOTE_SERVER

   zSHORT SelectStringByKeyAttributes( zVIEW vXRA );
   zSHORT GetArrayWithKeyAttribute( zVIEW vApp );
   zBOOL  SearchStringByKeyAttributes( zVIEW vApp, zVIEW vListApp );

#endif

private:    // Variables:

   zVIEW    m_vApp;
   zVIEW    m_vAppList;
   CString  m_csViewName;
   CString  m_csViewNameList;
   CString  *m_pzsScope;
   zLONG    m_lEntityCnt;
   zLONG    m_lEntityNbr;

   zSHORT   m_nIsDroppedDown;
   zBOOL    m_bWasSelected;
   zBOOL    m_bListLoaded;
   zULONG   m_ulFlag;
   zULONG   m_ulSubtype;
   zSHORT   m_nDisplaySelSet;
   zSHORT   m_nSelectSelSet;
   CString  *m_pzsListMapVName;
   CString  *m_pzsListMapEName;
   CString  *m_pzsListMapAName;
   CString  *m_pzsListMapContext;
   CString  *m_pzsListBrowseEName;
   CString  *m_pzsListBrowseScopeEName;
   zLONG    m_lExtentY;

   zPCHAR   m_pchSelectItem;
   zPCHAR   m_pchListData;
// ZComboEdit *m_pComboEdit;


// it appears that this is done for us in 32-bit Windows
// zLONG    m_lOrigSize;
// zBOOL    m_bFirstTime;
// zULONG   m_ulDelayInterval;
// zULONG   m_ulLastTime;
// zLONG    m_lKeystrokeCnt;
// zLONG    m_lItemNbr;
// zCHAR    m_szKeystroke[ 64 ];
// it appears that this is done for us in 32-bit Windows

   CArray<zPCHAR, zPCHAR> m_aKeyAttr;

   zBOOL        m_bMultiCol;
   ZMCComboEdit *m_pMCCEdit;
   ZComboColumn *m_pHeadCol;
   zSHORT       m_nTotalColWidth;
   zSHORT       m_nTotalCols;
   zSHORT       m_nBoundCol;
   zSHORT       m_nShowColumn;
   zSHORT       m_nTextItemHeight;
   CPen         m_penLightGray;
// zSHORT       *m_pnColumnWidth;
// zLONG        *m_plColumnAlignStyle;
// CStringList  *m_pcsColItemList;
   CEdit        *m_pReadOnlyEdit;
   zBOOL        m_bAutoComplete;

   DECLARE_MESSAGE_MAP( )
};

class ZComboColumn
{
public:

   ZComboColumn( zSHORT nCol, zSHORT nColWidth, zLONG lAlignStyle,
                 zCPCHAR cpcViewName, zCPCHAR cpcEntity,
                 zCPCHAR cpcAttribute, zCPCHAR cpcContext );
   ~ZComboColumn( );

   void SetColumnEAC( zCPCHAR cpcViewName,
                      zCPCHAR cpcEntity,
                      zCPCHAR cpcAttribute,
                      zCPCHAR cpcContext );

   zSHORT  m_nCol;
   zSHORT  m_nColWidth;
   zLONG   m_lAlignStyle;
   zPCHAR  m_pchViewName;
   zPCHAR  m_pchEntity;
   zPCHAR  m_pchAttribute;
   zPCHAR  m_pchContext;
   ZComboColumn *m_pNext;
};

#if 0
class ZComboEdit : public CEdit
{
public:

   ZComboEdit( ZComboBox *pComboBox, CWnd *pWnd );
   ~ZComboEdit( );

   ZComboBox *m_pComboBox;

protected:

   afx_msg void OnPaint( );

   DECLARE_MESSAGE_MAP( )
};
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// By Xiao Wu Guang

// MultiColumnComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ZMCComboBox window

#define LEFTMARGIN_OF_DROPDOWNLIST     2
#define LEFTMARGIN_OF_CONTROL          1
#define RIGHTMARGIN_OF_DROPDOWNLIST    4
#define SPACE_BETWEEN_COLUMN           1

/////////////////////////////////////////////////////////////////////////////
// ZMCComboEdit window

class ZDCTL_CLASS ZMCComboEdit : public CEdit
{
// Construction
public:
   ZMCComboEdit( );
   virtual ~ZMCComboEdit( );

// Attributes
public:
// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZMCComboEdit )
   protected:
   virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( ZMCComboEdit )
   afx_msg void OnPaint( );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctlcmb_h__ sentry.
