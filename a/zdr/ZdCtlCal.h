/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlcal.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlcal_h__  // Sentry, use file only if it's not already included
#define __zdctlcal_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Calendar( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

#define zDAYS_IN_WEEK         7
#define zMONTHS_IN_YEAR      12
#define zDAY_SPOTS_IN_MONTH  42  // 6 rows of days (max calendar weeks in month)
#define zNO_HIT              -1

#if 0
typedef struct _SYSTEMTIME
{
   WORD wYear;
   WORD wMonth;
   WORD wDayOfWeek;
   WORD wDay;
   WORD wHour;
   WORD wMinute;
   WORD wSecond;
   WORD wMilliseconds;

} SYSTEMTIME, *PSYSTEMTIME, FAR *LPSYSTEMTIME;

// DWORD DateTimePick_GetSystemtime(HWND hdp, LPSYSTEMTIME pst)
//   returns GDT_NONE if "none" is selected (DTS_SHOWNONE only)
//   returns GDT_VALID and modifies *pst to be the currently selected value
#define DTM_GETSYSTEMTIME   (DTM_FIRST + 1)
#define DateTime_GetSystemtime( hdp, pst ) \
                   (DWORD) SNDMSG( hdp, DTM_GETSYSTEMTIME, 0, (LPARAM)(pst) )

// BOOL DateTime_SetSystemtime(HWND hdp, DWORD gd, LPSYSTEMTIME pst)
//   if gd==GDT_NONE, sets datetimepick to None (DTS_SHOWNONE only)
//   if gd==GDT_VALID, sets datetimepick to *pst
//   returns TRUE on success, FALSE on error (such as bad params)
#define DTM_SETSYSTEMTIME   (DTM_FIRST + 2)
#define DateTime_SetSystemtime( hdp, gd, pst ) \
         (BOOL) SNDMSG( hdp, DTM_SETSYSTEMTIME, (LPARAM)(gd), (LPARAM)(pst) )
#endif

const static int g_nFullDayAppointmentHeight =
     ::GetSystemMetrics( SM_CXHSCROLL ) + ::GetSystemMetrics( SM_CXEDGE );
const static int g_nDragX = ::GetSystemMetrics( SM_CXDRAG );
const static int g_nDragY = ::GetSystemMetrics( SM_CYDRAG );

#define zDTN_DATETIMECHANGE  1
#define zDTN_USERSTRING      2
#define zDTN_WMKEYDOWN       4
#define zDTN_FORMAT          8
#define zDTN_FORMATQUERY    16
#define zDTN_DROPDOWN       32
#define zDTN_CLOSEUP        64

#define zCALENDAR_PERMIT_NULL           zCONTROL_SPECIFIC_1
#define zCALENDAR_CHECKBOX              zCONTROL_SPECIFIC_2
#define zCALENDAR_RECURSIVECLICK        zMAPACT_CTRL_SPECIFIC1

// Zeidon ZCalendar standard control
class ZDCTL_CLASS ZCalendar : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZCalendar )
public:
   // ctor
   ZCalendar( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZCalendar( );

   virtual void   CreateZ( );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // get text from OI and transfer to entry control
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zSHORT GetRowColText( zPCHAR pchText, zUSHORT uMaxLth,
                                 zLONG lRow, zLONG lColumn );
   virtual zSHORT SetRowColText( zCPCHAR cpcText, zLONG lRow, zLONG lColumn );
   virtual BOOL DestroyWindow( );

// Operations
public:
   CTime    GetDateTime( );
   BOOL     SetDateTime( const CTime& time );
   COleDateTime GetOleDateTime( );
   BOOL     SetDateTime( const COleDateTime& time );
   void     SetDefaultDate( DWORD& dwGDT );

// Attributes
public:
   COLORREF SetMonthCalColor( int nColIndex, COLORREF clr );
   COLORREF GetMonthCalColor( int nColIndex );
   BOOL     SetFormat( LPCTSTR szFmt );
   void     SetMonthCalFont( CFont& font, BOOL bRedraw = TRUE );
   CFont *  GetMonthCalFont( );
   BOOL     SetRange( CTime *pMinTime, CTime *pMaxTime );
   DWORD    GetRange( CTime *pMinTime, CTime *pMaxTime );
   BOOL     SetRange( COleDateTime *pMinTime, COleDateTime *pMaxTime );
   DWORD    GetRange( COleDateTime *pMinTime, COleDateTime *pMaxTime );


protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnEnable( BOOL bEnable );

   afx_msg void OnDateTimeChange( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnUserString( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnKeyDown( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnFormat( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnFormatQuery( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnDropDown( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnCloseUp( LPNMHDR pNMHDR, LRESULT *pResult );
   afx_msg void OnPaint( );
// afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT uCtlColor );

protected:
   void  SysTimeFromCTime( const CTime& time, SYSTEMTIME *pSysTime );
   BOOL  SetSystemTime( const SYSTEMTIME& time );
   BOOL  SetSystemTime( const CTime& time );
   void  SysTimeFromCOleDateTime( const COleDateTime& time, SYSTEMTIME *pSysTime );
   BOOL  SetSystemTime( const COleDateTime& time );

   COleDateTime GetSystemTime( );

private:

   COleDateTime m_CurrentTime;
// static BOOL bCommonControlsInitialized;
// BOOL   m_bDroppedDown;
   BOOL   m_bReportDateChangeWhileDropped;

   SYSTEMTIME m_SysTime;
   zBOOL m_bPermitNull;
   zBOOL m_bShowCheckBox;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMultiSelCal

class CMultiSelCal;

class CCalendarWeekdayWnd : public CWnd
{
// Construction
public:
   CCalendarWeekdayWnd( );

// Attributes
public:
   BOOL           m_bMultiEnabled;
   int            m_nFirstDayOfWeek;

protected:
   static CString m_csClassName;

   CMultiSelCal   *m_pParentCalendar;

   int            m_nTrackedWeekday;
   int            m_nPressedWeekday;

   CRect          m_rectWeekdays[ zDAYS_IN_WEEK ];
   zCHAR          *m_WeekdayLabels[ zDAYS_IN_WEEK ];

   COleDateTime   m_dateToday;

// Operations
public:
   void SetDate( const COleDateTime& date );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCalendarWeekdayWnd)
   public:
   virtual BOOL Create(const RECT& rect, CWnd *pParentWnd, UINT uID);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CCalendarWeekdayWnd();

   // Generated message map functions
protected:
   //{{AFX_MSG(CCalendarWeekdayWnd)
   afx_msg void OnLButtonUp(UINT uFlags, CPoint pt);
   afx_msg void OnLButtonDown(UINT uFlags, CPoint pt);
   afx_msg void OnMouseMove(UINT uFlags, CPoint pt);
   afx_msg void OnPaint();
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnSize(UINT uType, int cx, int cy);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   int  WeekdayHitTest( CPoint& pt ) const;
   void OnClickWeekday( int nWeekday );
   void InvalidateDay( int nDay );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCalendarScrollbarWnd

#define zMINICAL_BUTTON_IMAGE_CNT  5
class CMultiSelCal;

class CCalendarScrollbarWnd : public CWnd
{
// Construction
public:
   CCalendarScrollbarWnd( );

// Attributes
public:
   COLORREF    m_clrBack;
   COLORREF    m_clrLight;
   COLORREF    m_clrShadow;
   COLORREF    m_clrText;
   CString     m_csTodayLabel;

   enum eBUTTON_ID { ePrevYear, ePrevMonth, eToday, eNextMonth, eNextYear };

protected:
   static CString m_csClassName;

   CImageList     m_Images;
   CSize          m_ImageSize;

   CMultiSelCal   *m_pParentCalendar;

   CRect          m_rectScrollButtons[ zMINICAL_BUTTON_IMAGE_CNT ];
   int            m_nTrackedScrollButton;
   int            m_nPressedScrollButton;

   int            m_nButtonHeight;
   int            m_nWindowHeight;

   CToolTipCtrl   m_ToolTip;

   BOOL           m_bSlowTimerMode;
   int            m_nSlowTimerCount;

// Operations
public:
   void AlignInParentClientArea( BOOL bBottom = TRUE );
   int  GetHeight( ) const
   {
      return( m_nWindowHeight );
   }

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCalendarScrollbarWnd)
   public:
   virtual BOOL Create(const RECT& rect, CWnd *pParentWnd, UINT uID);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CCalendarScrollbarWnd( );

   // Generated message map functions
protected:
   //{{AFX_MSG(CCalendarScrollbarWnd)
   afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnLButtonUp(UINT uFlags, CPoint pt);
   afx_msg void OnLButtonDown(UINT uFlags, CPoint pt);
   afx_msg void OnMouseMove(UINT uFlags, CPoint pt);
   afx_msg void OnPaint();
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnSize(UINT uType, int cx, int cy);
   afx_msg void OnTimer(UINT uIDEvent);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   int  ScrollButtonHitTest( CPoint& pt ) const;
   void OnClickScrollButton( int nScrollButton );
   void RelayEvent( UINT uMessage, WPARAM wParam, LPARAM lParam );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMultiSelCal

class CMultiSelCal : public CWnd
{
   DECLARE_DYNAMIC(CMultiSelCal)

// Construction
public:
   CMultiSelCal( );
   virtual ~CMultiSelCal( );

// Attributes
public:
   COLORREF    m_clrBack;
   COLORREF    m_clrLight;
   COLORREF    m_clrShadow;
   COLORREF    m_clrText;
   COLORREF    m_clrSelBack;
   COLORREF    m_clrSelText;

protected:
   static CString m_csClassName;

   CCalendarScrollbarWnd  m_wndToolbar;
   CCalendarWeekdayWnd  m_wndCaption;
   CRect                m_rectDays;
   BOOL                 m_bIsDropCalendar;
   CWnd                 *m_pParent;

   COleDateTime         m_dateToday;
   int                  m_nTrackedDaySpot;
   int                  m_nPressedDaySpot;
   int                  m_nTitleHeight;
   CRect                m_rectDayLocations[ 31 ];
   int                  m_nDaysInMonth;
   int                  m_nFirstDayOfWeek;

   COleDateTime         m_dateMin;
   COleDateTime         m_dateMax;

   BOOL                 m_bMultipleSel;
   BOOL                 m_bAbsoluteSel;

   CList< int, int >    m_SelectedDays;
   CList< int, int >    m_MarkedDays;
   int                  m_nStartMultipleSelection;
   int                  m_nLastMultipleSelection;

   CFont                m_fontBold;

   enum eBUTTON_STATE { eFLAT, eHILIGHT, ePRESSED };

// Operations
public:
   BOOL SetDate( const COleDateTime& date );
   const COleDateTime& GetDate( ) const;

   void SetValidInterval( const COleDateTime& from, const COleDateTime& to );

   void EnableMultipleSelection( BOOL bMultipleSel = TRUE );

   void EnableAbsoluteSelection( BOOL bAbsoluteSel = TRUE )
   {
      m_bAbsoluteSel = bAbsoluteSel;
   }

   void OnToolbarNotifyClickButton( CCalendarScrollbarWnd::eBUTTON_ID nID );
   void SelectDayInWeek( int nDay, BOOL bClearOther );

   void UpdateColors( );

   void ClearSelectedDays( );
   void AddSelectedDay( int nDay );
   void RemoveSelectedDay( int nDay );
   BOOL IsDaySelected( int nDay ) const;

   BOOL MarkDay( int nDay, BOOL bMarked );
   void UnMarkAllDays( );

protected:
   int  DaySpotHitTest( CPoint& pt ) const;
   BOOL GetButtonRect( int nID, CRect& rect ) const;
   void RebuildLocations( );

   BOOL HideDropCalendar( const COleDateTime *pDateNew = NULL );

   BOOL IsDateValid( const COleDateTime& date ) const;

   BOOL IsLeftSelected( int nDay ) const;
   BOOL IsTopSelected( int nDay ) const;
   BOOL IsRightSelected( int nDay ) const;
   BOOL IsBottomSelected( int nDay ) const;

   void AlignSelectedDays( const COleDateTime& oldDate );

   void OnSelectionChanged( );
   void OnDateChanged( );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CMultiSelCal)
   public:
   virtual BOOL Create(DWORD dwStyle, int x, int y, CWnd *pParentWnd);
   //}}AFX_VIRTUAL

// Implementation
   // Generated message map functions
protected:
   //{{AFX_MSG(CMultiSelCal)
   afx_msg void OnLButtonDown(UINT uFlags, CPoint pt);
   afx_msg void OnLButtonUp(UINT uFlags, CPoint pt);
   afx_msg void OnMouseMove(UINT uFlags, CPoint pt);
   afx_msg void OnPaint();
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnKeyDown(UINT uChar, UINT uRepCnt, UINT uFlags);
   afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnKillFocus(CWnd *pNewWnd);
   afx_msg void OnSize(UINT uType, int cx, int cy);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZMultiCal

class ZDCTL_CLASS ZMultiCal : public CMultiSelCal, public ZMapAct
{
   DECLARE_DYNAMIC( ZMultiCal )
public:
   // ctor
   ZMultiCal( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZMultiCal( );

   virtual void   CreateZ( );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // transfer text from OI to calendar control
   virtual zSHORT MapToOI( zLONG lFlag = 0 ); // transfer text to OI from calendar control

   virtual BOOL DestroyWindow( );

private:

   SYSTEMTIME m_SysTime;
   zBOOL      m_bPermitNull;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZInPlaceEdit

class ZInPlaceEdit : public CEdit
{
public:
   ZInPlaceEdit( int nItem, int nSubItem, CString csInitText );
   virtual ~ZInPlaceEdit( );

   void CalculateSize( );

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZInPlaceEdit )
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( ZInPlaceEdit )
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnChar( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   //}}AFX_MSG

private:
   int     m_nItem;
   int     m_nSubItem;
   CString m_csInitText;
   BOOL    m_bESC;      // to indicate whether ESC key was pressed

   DECLARE_MESSAGE_MAP( )
};

class QInPlaceEdit : public CEdit
{
// Construction
public:
   QInPlaceEdit( int nItem, int nSubItem, CString csInitText );
   virtual ~QInPlaceEdit( );

// Implementation
public:

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( QInPlaceEdit )
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( QInPlaceEdit )
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnNcDestroy( );
   afx_msg void OnChar( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   //}}AFX_MSG

private:
   int     m_nItem;
   int     m_nSubItem;
   CString m_csInitText;
   BOOL    m_bESC;      // to indicate whether ESC key was pressed

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define WM_SPLITTER_MOVED   (WM_USER + 1000)

/////////////////////////////////////////////////////////////////////////////
// CxSplitterWnd window

class CxSplitterWnd : public CWnd
{
// Construction
public:
   CxSplitterWnd( );

// Attributes
public:

// Operations
public:
    BOOL BindWithControl( CWnd *pParent, DWORD ctrlId );
    void Unbind( );

    void SetMinWidth( int left, int right );

    BOOL AttachAsLeftPane( DWORD ctrlId );
    BOOL AttachAsRightPane( DWORD ctrlId );
    BOOL DetachAllPanes( );
    void RecalcLayout( );

protected:
    BOOL GetMouseClipRect( LPRECT rectClip, CPoint& pt );

    CWnd        *m_pParent;
    CDWordArray m_leftIds, m_rightIds;
    int         m_minLeft, m_minRight;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CxSplitterWnd )
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CxSplitterWnd( );

   // Generated message map functions
protected:
   //{{AFX_MSG( CxSplitterWnd )
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT message );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CySplitterWnd window

class CySplitterWnd : public CWnd
{
// Construction
public:
   CySplitterWnd( );

// Attributes
public:

// Operations
public:
    BOOL BindWithControl( CWnd *pParent, DWORD ctrlId );
    void Unbind( );

    void SetMinHeight( int above, int below );

    BOOL AttachAsAbovePane( DWORD ctrlId );
    BOOL AttachAsBelowPane( DWORD ctrlId );
    BOOL DetachAllPanes( );
    void RecalcLayout( );

protected:
    BOOL GetMouseClipRect( LPRECT rectClip, CPoint& pt );

    CWnd        *m_pParent;
    CDWordArray m_aboveIds, m_belowIds;
    int         m_minAbove, m_minBelow;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CySplitterWnd )
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CySplitterWnd( );

   // Generated message map functions
protected:
   //{{AFX_MSG( CySplitterWnd )
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT message );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctlcal_h__ sentry.
