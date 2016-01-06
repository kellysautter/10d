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
#ifndef __zdctldt_h__  // Sentry, use file only if it's not already included
#define __zdctldt_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
DayTimer( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

#define zDAYTIMER_SELECT_DATE               0x00000001
#define zDAYTIMER_SELECT_APPOINTMENT        0x00000002
#define zDAYTIMER_SELECT_CALENDAR           0x00000003
#define zDAYTIMER_SELECT_AGENDA             0x00000004
#define zDAYTIMER_SELECT_FULLDAY            0x00000005
#define zDAYTIMER_SELECT_FULLDAY_WND        0x00000006
#define zDAYTIMER_SELECT_DAYMONTHYEAR       0x00000007
#define zDAYTIMER_DBLCLK_DATE               0x00000010
#define zDAYTIMER_DBLCLK_APPOINTMENT        0x00000020
#define zDAYTIMER_DBLCLK_CALENDAR           0x00000030
#define zDAYTIMER_DBLCLK_AGENDA             0x00000040
#define zDAYTIMER_DBLCLK_FULLDAY            0x00000050
#define zDAYTIMER_DBLCLK_FULLDAY_WND        0x00000060
#define zDAYTIMER_DBLCLK_DAYMONTHYEAR       0x00000070
#define zDAYTIMER_RCLICK_DATE               0x00000100
#define zDAYTIMER_RCLICK_APPOINTMENT        0x00000200
#define zDAYTIMER_RCLICK_CALENDAR           0x00000300
#define zDAYTIMER_RCLICK_AGENDA             0x00000400
#define zDAYTIMER_RCLICK_FULLDAY            0x00000500
#define zDAYTIMER_RCLICK_FULLDAY_WND        0x00000600
#define zDAYTIMER_RCLICK_DAYMONTHYEAR       0x00000700
#define zDAYTIMER_APPOINTMENT_ALARM         0x00010000
#define zDAYTIMER_EDIT_CHANGE_APPOINTMENT   0x10000000
#define zMINICAL_SELECT_CHANGE              0x20000000

class ZDCTL_CLASS ZDayTimer;
class ZDayMonthYear;
class ZFullDayWnd;
class ZAgenda;
class ZMiniCalendar;

BOOL CALLBACK
IsSpecialDateCallback( ZDayTimer *pDayTimer, COleDateTime &date );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZAppointment

class ZAppointment : public CObject
{
friend class ZDayTimer;
friend class ZAgenda;
friend class ZMiniCalendar;
friend class ZFullDayWnd;
friend class ZEditAppointment;
friend zLONG OPERATION
EditAppointmentWndProc( HWND     hWnd,
                        WORD     uMsg,
                        WPARAM   wParam,
                        LPARAM   lParam );
   DECLARE_DYNAMIC( ZAppointment )

public:
   ZAppointment( int nIdNbr, unsigned int uKey );
   virtual ~ZAppointment( );

   BOOL DrawFullDay( COleDateTime& date, CDC *pDC, const CRect& rectArea,
                     ZAgenda *pView, BOOL bSelected, int nSelectedDay );
   void DrawAppointment( COleDateTime& date, CDC *pDC, const CRect& rectArea,
                         ZAgenda *pView, int nScrollPos );
   void UnlinkAppointment( ZAgenda *pAgenda );

protected:
   int          m_nSeqNbr;  // Id of appointment ... usually incremental
   unsigned int m_uKey;     // Zeidon key ... otw 0
   CRect        m_rectDraw;
   ZAppointment *m_pNext;
   ZAppointment *m_pPrev;
   ZAppointment *m_pStartOverlap;
   ZAppointment *m_pEndOverlap;
   int          m_nOverlap;
   int          m_nOverlapMax;
   int          m_nOverlapPos;

public:
   BOOL         m_bSelected;
   BOOL         m_bFullDay;
   int          m_nAlarm;
   int          m_nAppointmentType;
   CString      m_csSubject;
   CString      m_csMessage;
   COleDateTime m_date;
   COleDateTime m_timeTo;
   COleDateTime m_timeFrom;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZAutoFitLabel

class ZAutoFitLabel : public CStatic
{
// Construction
public:
   ZAutoFitLabel( );

// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZAutoFitLabel )
   protected:
   virtual void PreSubclassWindow( );
   //}}AFX_VIRTUAL

// Implementation
public:
   void Update( );
   virtual ~ZAutoFitLabel( );

   // Generated message map functions
protected:
   CFont *m_pFont;
   //{{AFX_MSG( ZAutoFitLabel )
   afx_msg void OnSize( UINT uType, int cx, int cy );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZEditAppointment

class ZEditAppointment : public CEdit
{
friend class ZDayTimer;
friend class ZAgenda;
friend class ZMiniCalendar;
friend zLONG OPERATION
EditAppointmentWndProc( HWND     hWnd,
                        WORD     uMsg,
                        WPARAM   wParam,
                        LPARAM   lParam );
public:
   ZEditAppointment( ZAgenda *pAgenda );
   virtual ~ZEditAppointment( );

   void CalculateSize( );

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZEditAppointment )
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( ZEditAppointment )
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnChar( UINT uChar, UINT uRepeatCnt, UINT uFlags );
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSizing( UINT uSide, LPRECT pRect );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnMove( int x, int y );
   afx_msg LRESULT OnNcHitTest( CPoint pt );
   afx_msg void OnNcPaint( );
   afx_msg void OnWindowPosChanging( WINDOWPOS *pWndPos );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   //}}AFX_MSG

protected:
   BOOL         m_bESC;      // to indicate whether ESC key was pressed
   BOOL         m_bSizeMove; // to indicate Sizing/Moving is in progress
   ZAgenda      *m_pAgenda;
   ZAppointment *m_pA;
   WNDPROC      m_OldWndProc;
   COleDateTime m_timeFrom;
   COleDateTime m_timeTo;
   CRect        m_rectSizing;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZAgenda

const static int    g_nTimeGutterBorder = 5;
const static int    g_nGutterBandWidth = 6;
const static double g_dHalfHourDivider = 0.7;

inline int INTFRAC( int k, double dFraction )
{
   return( int( k * double( dFraction ) ) );
}

class ZDCTL_CLASS ZAgenda : public CWnd
{
friend class ZDayTimer;
friend class ZMiniCalendar;
friend class ZMiniCalendarMonthCell;
friend class ZDayMonthYear;
friend class ZFullDayWnd;
friend class ZAppointment;
friend class ZEditAppointment;
friend zLONG OPERATION
EditAppointmentWndProc( HWND     hWnd,
                        WORD     uMsg,
                        WPARAM   wParam,
                        LPARAM   lParam );
   DECLARE_DYNAMIC( ZAgenda )
public:
   // ctor
   ZAgenda( ZDayTimer *pDayTimer );
   virtual ~ZAgenda( );

   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL DestroyWindow( );

// Operations
   void ShowSelectedDays( );
   int  GetAppointmentCountOnDate( COleDateTime& date );
   int  GetDayCount( );
   int  GetShownDayCount( );
   BOOL DropAppointment( ZAppointment *pA, COleDateTime& date );
   int  GetDateFromPoint( COleDateTime& date, CPoint& pt );
   void SetDrawRect( CRect& rectDraw, COleDateTime& timeFrom,
                     COleDateTime& timeTo,
                     const CRect& rectArea, int nScrollPos );
   int  GetNumberOfFulldayAppointments( );
   void SortAppointments( );
   void SetAppointmentOverlap( );
   void UpdatePositionOfControls( );
   void SetWorkday( COleDateTime& timeWorkdayStart,
                    COleDateTime& timeWorkdayEnd,
                    zLONG lWorkdayFlag );
   ZAppointment * AppointmentHitTest( CPoint& pt );
   void TrackEditAppointment( ZAppointment *pA, CPoint& pt );
   void EditAppointment( ZAppointment *pA );
   ZAppointment * GetNextShownAppointment( BOOL bBackwards );
   void TraceAppointmentList( zCPCHAR cpcText );
// void MoveTop( ZAppointment *pA );
   void MoveBefore( ZAppointment *pA, ZAppointment *pNext );

   ZAppointment * AddAppointment( COleDateTime& date,
                                  COleDateTime& timeFrom,
                                  COleDateTime& timeTo,
                                  zCPCHAR cpcSubject,
                                  zCPCHAR cpcMessage,
                                  zLONG lAlarm,
                                  zLONG lAppointmentType,
                                  zBOOL bFullDay = FALSE );
   void AddAppointment( ZAppointment *pA );
   int  DeleteAppointment( ZAppointment *pA );
   void RemoveAllAppointments( );
   BOOL IsDoubleClick( UINT uMessage, CPoint ptStart );
   void UpdateScrollSize( BOOL bResetPos = FALSE );
   void UpdateScrollPosition( );
   void GoToTime( COleDateTime& Time, BOOL bMoveToTop = TRUE );

// Overrides
protected:
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZAgenda )
protected:
// virtual void OnDraw( CDC *pDC );     // overridden to draw this view
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   //}}AFX_VIRTUAL

   // Generated message map functions
   //{{AFX_MSG( ZAgenda )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnPaint( );
   afx_msg void OnVScroll( UINT uSBCode, UINT uPos, CScrollBar *pScrollBar );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uFlags, CPoint pt );
   afx_msg void OnRButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnContextMenu( CWnd *pWnd, CPoint pt );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   //}}AFX_MSG

// Attributes
protected:
   ZDayTimer        *m_pDayTimer;
   ZDayMonthYear    *m_pDayMonthYear;
   ZFullDayWnd      *m_pFullDayWnd;
   ZAppointment     *m_pAppointmentHead;
   ZAppointment     *m_pAppointmentLastFocus;
   BOOL             m_bFocusRect;
   int              m_nSeqNbr;
   int              m_nHourHeight;
   int              m_nTimeGutterWidth;
   int              m_nEditBorder;
   int              m_nHeightOfDayMonthYear;
   CBrush           m_brushAppointmentBack;
   CBrush           m_brushWorkdayBack;
   COleDateTime     m_timeWorkdayStart;
   COleDateTime     m_timeWorkdayEnd;
   zLONG            m_lWorkdayFlag;
   CPen             m_penShadow;
   CPen             m_penDivider;
   CPen             m_penThinBlack;
   CFont            m_fontAppointmentSubject;

   CImageList       m_Images;
   CSize            m_ImageSize;

   BOOL             m_bTrackingSelected;
   COleDateTime     m_dateLastSelected;
   COleDateTime     m_timeFirstSelected;
   COleDateTime     m_timeLastSelected;
   ZAppointment     *m_pTrackedAppointmentBand;
   ZAppointment     *m_pTrackedAppointment;
   CPoint           m_ptTracked;

// Implementation
   BOOL             m_bEditAppointment;
   ZEditAppointment m_editAppointment;
   ZToolTip         m_wndToolTip;
   CFont            *m_pHourFont;
   CFont            *m_pSmallHourFont;
   CPen             m_penNull;
   CBrush           m_brushBack;
   int              m_nClientX;
   int              m_nClientY;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDayMonthYear window

class ZDayMonthYear : public CWnd
{
friend class ZDayTimer;
friend class ZAgenda;
// Construction
public:
   ZDayMonthYear( );
   virtual ~ZDayMonthYear( );

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZDayMonthYear )
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( ZFullDayWnd )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );
   afx_msg void OnDestroy( );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uFlags, CPoint pt );
   afx_msg void OnRButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnRButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnContextMenu( CWnd *pWnd, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   //}}AFX_MSG

protected:
   ZAgenda  *m_pAgenda;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZFullDayWnd window

class ZFullDayWnd : public CWnd
{
friend class ZDayTimer;
friend class ZAgenda;
// Construction
public:
   ZFullDayWnd( );
   virtual ~ZFullDayWnd( );

// Operations
public:
   ZAppointment * AppointmentHitTest( CPoint& pt );
   BOOL IsDoubleClick( UINT uMessage, CPoint ptStart );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZFullDayWnd )
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:

   //{{AFX_MSG( ZFullDayWnd )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaint( );
   afx_msg void OnDestroy( );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uFlags, CPoint pt );
   afx_msg void OnRButtonDown( UINT uFlags, CPoint pt );
// afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnContextMenu( CWnd *pWnd, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   //}}AFX_MSG

protected:
   ZAgenda      *m_pAgenda;
   CPoint       m_ptTracked;
   ZAppointment *m_pHilightAppointment;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDayTimer

class ZDCTL_CLASS ZDayTimer : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZDayTimer )
public:
   static CString m_csClassName;

   // ctor
   ZDayTimer( ZSubtask *pZSubtask,
              CWnd     *pWndParent,
              ZMapAct  *pzmaComposite,
              zVIEW    vDialog,
              zSHORT   nOffsetX,
              zSHORT   nOffsetY,
              zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZDayTimer( );

   virtual void   CreateZ( );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // transfer text from OI to calendar control
   virtual zSHORT MapToOI( zLONG lFlag = 0 ); // transfer text to OI from calendar control

   virtual BOOL DestroyWindow( );

   BOOL PreTranslateMessage( MSG *pMsg );
   void UpdatePositionOfControls( );

   // Generated message map functions
protected:
   //{{AFX_MSG( ZMiniCalendar )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   //}}AFX_MSG

public:
   int  GoToTime( COleDateTime& Time, BOOL bMoveToTop = TRUE );
   int  RemoveAllAppointments( );
   int  ZDayTimer::SignalAlarms( );
   void OnTimer( UINT uIDEvent );
   ZAppointment * AddFulldayAppointment( COleDateTime& date,
                                         zCPCHAR cpcSubject,
                                         zCPCHAR cpcMessage,
                                         zLONG   lAlarm,
                                         zLONG   lAppointmentType,
                                         zULONG  ulKey );
   ZAppointment * AddAppointment( COleDateTime& date,
                                  COleDateTime& timeFrom,
                                  COleDateTime& timeTo,
                                  zCPCHAR cpcSubject,
                                  zCPCHAR cpcMessage,
                                  zLONG lAlarm,
                                  zLONG lAppointmentType,
                                  zULONG ulKey );
   ZAppointment * NewAppointment( CPoint& pt,
                                  LPCSTR  cpcSubject,
                                  LPCSTR  cpcMessage,
                                  LONG    lAppointmentType,
                                  BOOL    bFullDay );
   int  DeleteAppointment( ZAppointment *pA );
   int  UpdateAppointment( ZAppointment *pA );
   int  SetColorAssociation( COLORREF clrText,
                             COLORREF clrBack,
                             int      lFlag,
                             LPCSTR   cpcAttributeValue );
   int  SetTimeEntityAttributes( LPCSTR cpcTimeEntity,
                                 LPCSTR cpcSubjectAttribute,
                                 LPCSTR cpcMessageAttribute,
                                 LPCSTR cpcTimeFromAttribute,
                                 LPCSTR cpcTimeToAttribute,
                                 LPCSTR cpcTimeAlarmAttribute,
                                 LPCSTR cpcTimeAppointmentTypeAttribute,
                                 LPCSTR cpcFullDayAttribute );

   int  GetHourHeight( /*[out, retval]*/ int *plVal );
   int  SetHourHeight( /*[in]*/ int lNewVal );

   CString       m_csTimeEntity;
   CString       m_csSubjectAttribute;
   CString       m_csMessageAttribute;
   CString       m_csTimeFromAttribute;
   CString       m_csTimeToAttribute;
   CString       m_csAlarmAttribute;
   CString       m_csAppointmentTypeAttribute;
   CString       m_csFullDayAttribute;
   CRect         m_rectCtrl;
   ZAgenda       *m_pAgenda;
   ZMiniCalendar *m_pMiniCal;
   zCHAR         m_chStyle;
   int           m_nRows;
   int           m_nCols;
   int           m_nMiniCalSize;
   int           m_nMaxShowAppointment;
   BOOL          m_bMultipleSel;
   BOOL          m_bPermitNull;
   BOOL          m_bToday;
   zCHAR         m_chAlignment;
   COLORREF      m_clrSelBack;
   COLORREF      m_clrBackground;
   zCHAR         m_chScope;
   zCHAR         m_szScopeEntity[ 33 ];
   UINT          m_uAppointmentTimerId;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

// ZMiniCalendar
// Author:  Matt Gullett
//       gullettm@yahoo.com
// Copyright © 2001
//
// This is a user-interface componenet similar to the MS outlook mini
// calendar control.  (The one used in date picker control and the
// appointment (day view)).
//
// You may freely use this source code in personal, freeware, shareware
// or commercial applications provided that 1) my name is recognized in the
// code and if this code represents a substantial portion of the application
// that my name be included in the credits for the application (about box, etc)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class
// http://www.codeproject.com/gdi/flickerfree.asp
//
// *******************************************************************
// TECHNICAL NOTES:
//
// See .cpp file for tech notes
//////////////////////////////////////////////////////////////////////

// Custom window styles.
#define FMC_MULTISELECT       0x0040L
#define FMC_NOHIGHLIGHTTODAY  0x0080L
#define FMC_TODAYBUTTON       0x0100L
#define FMC_NONEBUTTON        0x0200L
#define FMC_AUTOSETTINGS      0x0400L
#define FMC_NO3DBORDER        0x0800L
#define FMC_NOSHOWNONMONTHDAYS   0x1000L

// I usually don't use preprocessor macros like this
// but it makes sense here.
#define FMC_MAX_SIZE_NONE        CSize( -2, -2 )
#define FMC_MAX_SIZE_PARENT      CSize( -1, -1 )

// Default font settings.
#define FMC_DEFAULT_FONT         "Tahoma"
#define FMC_DEFAULT_FONT_SIZE       8
#define FMC_DEFAULT_FONT_SIZE_640   7
#define FMC_DEFAULT_FONT_SIZE_800   8
#define FMC_DEFAULT_FONT_SIZE_1024  10
#define FMC_DEFAULT_FONT_SIZE_1280  12
#define FMC_DEFAULT_MIN_FONT_SIZE   5

// Used to disable Maximum selection day rules.
#define FMC_NO_MAX_SEL_DAYS         -1

// Callback function (definition) for the IsSpecialDate function.
typedef BOOL (CALLBACK *fnSPECIALDATE)( ZDayTimer *pDayTimer, COleDateTime& );

// Forward declaration for list popup.
class ZMiniCalendarList;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendar window

class ZMiniCalendarCtrlFontInfo
{
public:
   ZMiniCalendarCtrlFontInfo( );
   ~ZMiniCalendarCtrlFontInfo( );

   virtual void CreateFont( CDC *pDC );

   CString  m_csFontName;
   int      m_nFontSize;
   BOOL     m_bBold;
   BOOL     m_bItalic;
   BOOL     m_bUnderline;
   COLORREF m_cColor;
   CFont    *m_pFont;
};

class ZMiniCalendarDaySpot
{
friend class ZDayTimer;
friend class ZAgenda;
friend class ZMiniCalendar;
friend class ZMiniCalendarMonthCell;
public:
   ZMiniCalendarDaySpot( ) { };
   ~ZMiniCalendarDaySpot( ) { };

protected:
   CRect        m_rect;
   COleDateTime m_date;
   int          m_nDaySpot;
   BOOL         m_bSelected;
   BOOL         m_bNonMonth;
};

class ZMiniCalendar;

class ZMiniCalendarMonthCell
{
friend class ZDayTimer;
friend class ZAgenda;
friend class ZMiniCalendar;
public:
   ZMiniCalendarMonthCell( );
   ~ZMiniCalendarMonthCell( );

   BOOL  IsDaySelected( int nDaySpot );
   BOOL  IsLeftSelected( int nDay );
   BOOL  IsTopSelected( int nDay );
   BOOL  IsRightSelected( int nDay );
   BOOL  IsBottomSelected( int nDay );
   ZMiniCalendarDaySpot * DaySpotHitTest( CPoint& pt ) const;
   int   WeekdayHitTest( CPoint& pt ) const;
   void  InvalidateWeekday( int nWeekday );

protected:
   ZMiniCalendar             *m_pMiniCal;
   CRect                     m_rectHeader;
   CRect                     m_rectWeekdayHeader;
   CRect                     m_rect;

   int                       m_nRow;
   int                       m_nCol;
   int                       m_nYear;
   int                       m_nMonth;

   ZMiniCalendarDaySpot      *m_pDaySpots;
   int                       m_nDaysInMonth;
   int                       m_nDayOfWeekMonthBegins;
   CRect                     m_rectWeekdays[ zDAYS_IN_WEEK ];
// CRect                     m_rectDayLocations[ zDAY_SPOTS_IN_MONTH ];
};

class ZMiniCalendarSelSpot
{
friend class ZDayTimer;
friend class ZAgenda;
friend class ZMiniCalendar;
public:
   ZMiniCalendarSelSpot( COleDateTime& date )
   {
      m_date = date;
   };

   ~ZMiniCalendarSelSpot( ) { };

private:
   COleDateTime m_date;
};

class ZMiniCalendar : public CWnd
{
friend class ZDayTimer;
friend class ZAgenda;
friend class ZMiniCalendarMonthCell;
friend class ZDayMonthYear;
friend class ZFullDayWnd;
friend class ZMiniCalendarList;
friend zOPER_EXPORT zSHORT OPERATION
DT_SetStartDateRowsCols( zVIEW   vSubtask,
                         zCPCHAR cpcCtrlTag,
                         zSHORT  nYear,
                         zSHORT  nMonth,
                         zSHORT  nRows,
                         zSHORT  nCols,
                         zLONG   lFlag );

   DECLARE_DYNAMIC( ZMiniCalendar )

public:
   ZMiniCalendar( ZDayTimer *pDayTimer ); // ctor
   virtual ~ZMiniCalendar( );             // dtor

// Attributes
   static CString m_csClassName;
   static CString m_csHeaderClassName;
   enum eBUTTON_STATE { eFLAT, eHILIGHT, ePRESSED };

// Operations
   BOOL  Create( const RECT& rect, CWnd *pParentWnd, UINT uID );

   void  SetHeaderFont( zCPCHAR cpcFont, int nSize, BOOL bBold, BOOL bItalic,
                        BOOL bUnderline, COLORREF clr );
   ZMiniCalendarCtrlFontInfo& GetHeaderFont( ) {return m_HeaderFontInfo;}

   void  SetDaysOfWeekFont( zCPCHAR cpcFont, int nSize, BOOL bBold,
                            BOOL bItalic, BOOL bUnderline, COLORREF clr );
   ZMiniCalendarCtrlFontInfo& GetDaysOfWeekFont( ) { return m_DaysOfWeekFontInfo; }

   void  SetSpecialDaysFont( zCPCHAR cpcFont, int nSize, BOOL bBold,
                             BOOL bItalic, BOOL bUnderline, COLORREF clr );
   ZMiniCalendarCtrlFontInfo& GetSpecialDaysFont( ) { return m_SpecialDaysFontInfo; }

   void  SetDaysFont( zCPCHAR cpcFont, int nSize, BOOL bBold, BOOL bItalic,
                      BOOL bUnderline, COLORREF clr );
   ZMiniCalendarCtrlFontInfo& GetDaysFont( ) { return m_DaysFontInfo; }

protected:
   void  AutoConfigure( );
   void  ReinitializeCells( );
   void  AutoSize( );
   int   ComputeTodayNoneHeight( );
   CSize ComputeTotalSize( );
   CSize ComputeSize( );
   CSize Compute3DBorderSize( );
   void  FireNoneButton( );
   void  FireNotifyDblClick( );
   void  FireNotifyClick( );
   void  FireTodayButton( );
   void  ScrollRight( int nCnt = 1 );
   void  ScrollLeft( int nCnt = 1 );

   BOOL  GetSelectedDateByOrder( COleDateTime& date,
                                 int nSelectedOrder = 0 );
   void  SetTracking( ZMiniCalendarMonthCell **ppMonthHeader,
                      ZMiniCalendarMonthCell **ppMonthWeekdays,
                      ZMiniCalendarMonthCell **ppMonthCell,
                      CPoint& pt );
   int   MonthCellFromRowCol( int nMonthRow, int nMonthCol );
   int   ScrollButtonHitTest( CPoint& pt );
   void  RelayEvent( UINT uMessage, WPARAM wParam, LPARAM lParam );
   ZMiniCalendarMonthCell * MonthHeaderHitTest( CPoint& pt );
   ZMiniCalendarMonthCell * MonthWeekdayHeaderHitTest( CPoint& pt );
   ZMiniCalendarMonthCell * MonthCellHitTest( CPoint& pt );

   void  SetCurrentDate( COleDateTime date );
// COleDateTime GetDate( );

   void  SetRowsAndColumns( int nRows, int nCols );
   int   GetRows( ) { return m_nRows; }
   int   GetCols( ) { return m_nCols; }

   void  SetShowNonMonthDays( BOOL bValue ) { m_bShowNonMonthDays = bValue; }
   BOOL  GetShowNonMonthDays( ) { return m_bShowNonMonthDays; }

   void  SetDefaultMinFontSize( int nDefaultMinFontSize );
   int   GetDefaultMinFontSize( ) { return m_nDefaultMinFontSize; }

   void  SetDefaultFont( LPCTSTR lpszValue );
   CString GetDefaultFont( ) { return m_csDefaultFontName; }

   void  SetMonthName( int nMonth, LPCTSTR cpcMonthName );
   CString GetMonthName( int nMonth );

   void  SetDayOfWeekName( int nDayOfWeek, LPCTSTR cpcDayName );
   CString GetDayOfWeekName( int nDayOfWeek );

   void  SetFirstDayOfWeek( int nDayOfWeek );
   int   GetFirstDayOfWeek( );

   void  SetCurrentMonthAndYear( int nMonth, int nYear );
   int   GetCurrentMonth( ) { return m_nCurrentMonth; }
   int   GetCurrentYear( ) { return m_nCurrentYear; }

   void  EnableMultipleSelection( BOOL bMultiSel ) { m_bMultipleSel = bMultiSel; }
   BOOL  IsMultipleSelectionEnabled( ) { return m_bMultipleSel; }

   void  EnablePermitNull( BOOL bPermitNull ) { m_bPermitNull = bPermitNull; }
   BOOL  IsPermitNullEnabled( ) { return m_bPermitNull; }

   void  EnableToday( BOOL bToday ) { m_bToday = bToday; }
   BOOL  IsTodayEnabled( ) { return m_bToday; }

   void  SetBackColor( COLORREF clrBack ) { m_clrBack = clrBack; }
   COLORREF GetBackColor( ) { return m_clrBack; }

   void  SetHighlightToday( BOOL bHighlightToday ) { m_bHighlightToday = bHighlightToday; }
   BOOL  GetHighlightToday( ) { return m_bHighlightToday; }

   void  SetShowTodayButton( BOOL bShowTodayButton ) { m_bShowTodayButton = bShowTodayButton; }
   BOOL  GetShowTodayButton( ) { return m_bShowTodayButton; }

   void  SetShowNoneButton( BOOL bShowNoneButton ) { m_bShowNoneButton = bShowNoneButton; }
   BOOL  GetShowNoneButton( ) { return m_bShowNoneButton; }

   void  SetNonMonthDayColor( COLORREF clr ) { m_clrNonMonthDay = clr; }
   COLORREF GetNonMonthDayColor( COLORREF clr ) { m_clrNonMonthDay = clr; }

   void  SetShow3DBorder( BOOL bShow3dBorder ) { m_bShow3dBorder = bShow3dBorder; }
   BOOL  GetShow3DBorder( ) { return m_bShow3dBorder; }

   void  SetMaxSize( SIZE sizeMax );
   CSize GetMaxSize( ) { return m_sizeMax; }

   void  SetUseAutoSettings( BOOL bUseAutoSettings ) { m_bUseAutoSettings = bUseAutoSettings; }
   BOOL  GetUseAutoSettings( ) { return m_bUseAutoSettings; }

   void  SetMaxSelDays( int nMaxSelDays ) { m_nMaxSelDays = nMaxSelDays; }
   int   GetMaxSelDays( ) { return m_nMaxSelDays; }

   void  SetSpecialDaysCallback( fnSPECIALDATE pfnSpecialDaysCallback );
   fnSPECIALDATE GetSpecialDaysCallback( ) { return m_pfnSpecialDaysCallback; }

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZMiniCalendar )
public:
   BOOL PreTranslateMessage( MSG *pMsg );
   //}}AFX_VIRTUAL

   // Generated message map functions
protected:
   //{{AFX_MSG( ZMiniCalendar )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnPaint( );
   afx_msg void OnDevModeChange( LPTSTR lpDeviceName );
   afx_msg void OnFontChange( );
   afx_msg void OnPaletteChanged( CWnd *pFocusWnd );
   afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );
   afx_msg void OnTimeChange( );
   afx_msg void OnSysColorChange( );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT uFlags, CPoint pt );
   afx_msg void OnRButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   //}}AFX_MSG

// Implementation
protected:

   void  SetValidInterval( const COleDateTime& fromDate,
                           const COleDateTime& toDate );

   void  AllocateCells( );
   void  SetCellPosition( int nMonthRow, int nMonthCol, RECT rect,
                          int nYear, int nMonth, int nPart );
   void  SetDaySpot( int nMonthRow, int nMonthCol, int nDay,
                     COleDateTime& date, RECT rect );
   int   DrawTodayButton( CDC& dc, int nY );
   void  CreateFontObjects( );
   int   DrawHeader( CDC& dc, int nY, int nLeftX, int nRow, int nCol,
                     int nYear, int nMonth );
   int   DrawDaysOfWeek( CDC& dc, int nY, int nLeftX, int nRow, int nCol,
                   int nYear, int nMonth );
   int   DrawDays( CDC& dc, int nY, int nLeftX, int nRow, int nCol,
                   int nYear, int nMonth );

   void  OnSelectionChanged( );
   ZMiniCalendarDaySpot * FindDaySpotByDate( COleDateTime& date );
   void  ClearAllDaySpots( );
   void  ClearAllSelectedDays( );
   void  AddSelectedDay( COleDateTime& date, BOOL bRedraw = TRUE );
   void  RemoveSelectedDay( COleDateTime& date );
   BOOL  IsDaySelected( COleDateTime& date ) const;
   int   GetSelectedDayCount( );

   BOOL  IsToday( COleDateTime& date ) const;
   BOOL  IsDateValid( const COleDateTime& date ) const;
   BOOL  IsSpecialDay( COleDateTime& date );
   BOOL  MarkSpecialDay( COleDateTime& date, BOOL bMarked );
   void  UnMarkAllSpecialDays( );
   void  OnTimer( UINT uIDEvent );

// Attributes
protected:
   ZDayTimer                 *m_pDayTimer;

   COleDateTime              m_dateToday;
   int                       m_nInPaint;
   int                       m_nCurrentMonth;
   int                       m_nCurrentYear;
   int                       m_nDayCnt;
   ZMiniCalendarCtrlFontInfo m_HeaderFontInfo;
   ZMiniCalendarCtrlFontInfo m_DaysOfWeekFontInfo;
   ZMiniCalendarCtrlFontInfo m_DaysFontInfo;
   ZMiniCalendarCtrlFontInfo m_SpecialDaysFontInfo;

   COLORREF                  m_clrBack;
   COLORREF                  m_clrLight;
   COLORREF                  m_clrShadow;
   COLORREF                  m_clrText;
   COLORREF                  m_clrSelBack;
   COLORREF                  m_clrSelText;
   COLORREF                  m_clrNonMonthDay;

   BOOL                      m_bHighlightToday;
   BOOL                      m_bShowTodayButton;
   BOOL                      m_bShowNoneButton;
   CString                   m_csMonthNamesArray[ zMONTHS_IN_YEAR ];
   CString                   m_csDaysOfWeekNamesArray[ zDAYS_IN_WEEK ];
   int                       m_nFirstDayOfWeek;
   int                       m_nRows;
   int                       m_nCols;

   BOOL                      m_bMultipleSel;
   BOOL                      m_bPermitNull;
   BOOL                      m_bToday;
   BOOL                      m_bAbsoluteSel;
   BOOL                      m_bShow3dBorder;
   BOOL                      m_bUseAutoSettings;

   CSize                     m_sizeMax;
   CString                   m_csDefaultFontName;
   int                       m_nDefaultMinFontSize;
   int                       m_nMaxSelDays;
   BOOL                      m_bShowNonMonthDays;
   fnSPECIALDATE             m_pfnSpecialDaysCallback;

   COleDateTime              m_dateMin;
   COleDateTime              m_dateMax;

   COleDateTime              m_dateTrackingStart;
   BOOL                      m_bScrollTracking;
   BOOL                      m_bHeaderTracking;
   BOOL                      m_bDayTracking;
   BOOL                      m_bTodayTracking;
   BOOL                      m_bTodayDown;
   BOOL                      m_bNoneTracking;
   BOOL                      m_bNoneDown;
   ZMiniCalendarList         *m_pHeaderList;
   ZMiniCalendarMonthCell    *m_pHeaderCell;

   UINT                      m_nHeaderTimerID;
   BOOL                      m_bSlowTimerMode;
   int                       m_nSlowTimerCount;

   // Month button, scroll images/positions/sizes.
   enum eBUTTON_ID { ePrevYear, ePrevMonth, eToday,
                     eNextMonth, eNextYear, eNone };
   CToolTipCtrl              m_ToolTip;

   CImageList                m_Images;
   CSize                     m_ImageSize;

   CRect                     m_rectScrollButtons[ zMINICAL_BUTTON_IMAGE_CNT + 1 ];
   int                       m_nTrackedScrollButton;
   int                       m_nPressedScrollButton;
   int                       m_nTrackedMonthMonthYear;
   int                       m_nPressedMonthMonthYear;
   int                       m_nTrackedMonthWeekday;
   int                       m_nPressedMonthWeekday;
   ZMiniCalendarDaySpot      *m_pTrackedDaySpot;
   ZMiniCalendarDaySpot      *m_pPressedDaySpot;

   // Computed values of importance.
   BOOL                      m_bFontsCreated;
   BOOL                      m_bSizeComputed;
   int                       m_nHeaderHeight;
   int                       m_nIndividualDayWidth;
   int                       m_nDaysOfWeekHeight;
   int                       m_nDaysHeight;
   CSize                     m_sizeMonth;

   // Lists.
   int                       m_nMonthCells;
   ZMiniCalendarMonthCell    *m_pMonthCells;

   CList< ZMiniCalendarSelSpot *, ZMiniCalendarSelSpot * > m_MarkedDays;
   CList< ZMiniCalendarSelSpot *, ZMiniCalendarSelSpot * > m_SelectedDays;
   CList< ZMiniCalendarSelSpot *, ZMiniCalendarSelSpot * > m_SelectedDayOrder;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarList
// Author:  Matt Gullett
//       gullettm@yahoo.com
// Copyright © 2001
//
// This is a companion control for use by the ZMiniCalendar.
// It is probably useless for any other purpose.
//
// You may freely use this source code in personal, freeware, shareware or
// commercial applications provided that 1) my name is recognized in the code
// and if this code represents a substantial portion of the application that
// my name be included in the credits for the application (about box, etc.)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Keith Rule for the CMemDC class
// http://www.codeproject.com/gdi/flickerfree.asp
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarList window

class ZMiniCalendarList : public CWnd
{
// Construction
public:
   ZMiniCalendarList( );

// Attributes
public:

// Operations
public:
   int GetSelMonth( ) { return m_nSelMonth; }
   int GetSelYear( ) { return m_nSelYear; }

   void SetCalendar( ZMiniCalendar *pWnd );
   void SetMiddleMonthYear( int nMonth, int nYear );
   void SetItemsPerPage( int nItems );
   void SetBackColor( COLORREF clrBack ) { m_clrBack = clrBack; }
   void SetTextColor( COLORREF clrText ) { m_clrText = clrText; }

   void SetFont( LPCTSTR lpszFont, int nSize, BOOL bBold, BOOL bItalic,
                 BOOL bUnderline, COLORREF clr );
   ZMiniCalendarCtrlFontInfo& GetFont( ) { return m_FontInfo; }

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZMiniCalendarList )
   //}}AFX_VIRTUAL

// Implementation
public:
   void ScrollDown( int nCnt = 1 );
   void ScrollUp( int nCnt = 1 );
   void AutoConfigure( );
   void ForwardMessage( MSG *pMSG );
   virtual ~ZMiniCalendarList( );

   // Generated message map functions
protected:
   BOOL IsSelected( int nX, CRect rectItem );

   //{{AFX_MSG( ZMiniCalendarList )
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnPaint( );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )

   int      m_nMiddleMonth;
   int      m_nMiddleYear;
   int      m_nItemsPerPage;

   int      m_nSelMonth;
   int      m_nSelYear;

   COLORREF m_clrBack;
   COLORREF m_clrText;
   ZMiniCalendarCtrlFontInfo m_FontInfo;
   ZMiniCalendar         *m_pWndCalendar;

   int      m_nUpFactor;
   int      m_nDownFactor;
};

#if 0
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarDlg

class ZMiniCalendarDlg : public CDialog
{
// Construction
public:
   ZMiniCalendarDlg( CWnd *pParent = 0 );   // standard constructor

// Dialog Data
   //{{AFX_DATA( ZMiniCalendarDlg )
   enum { IDD = IDD_MINICALENDAR_DIALOG };
   BOOL  m_b3DBorder;
   int   m_nCols;
   BOOL  m_bHighlightToday;
   int   m_nMaxSel;
   BOOL  m_bMultiSelect;
   BOOL  m_bNoneButton;
   int   m_nRows;
   BOOL  m_bTodayButton;
   int   m_nFirstDayOfWeek;
   BOOL  m_bShowNonMonthDays;
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZMiniCalendarDlg )
   protected:
   virtual void DoDataExchange( CDataExchange *pDX );   // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   HICON m_hIcon;

   // Generated message map functions
   //{{AFX_MSG( ZMiniCalendarDlg )
   virtual BOOL OnInitDialog( );
   afx_msg void OnSysCommand( UINT uID, LPARAM lParam );
   afx_msg void OnPaint( );
   afx_msg HCURSOR OnQueryDragIcon( );
   afx_msg void OnUpdate( );
   //}}AFX_MSG
   afx_msg void OnCalendarClick( NMHDR  *pNotifyStruct, LRESULT  *result );
   DECLARE_MESSAGE_MAP( )

   ZMiniCalendar m_wndCalendar;
};
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 0
#define WMID_CALENDAR 32999

// MiniCalendarDlg.cpp : implementation file
//

BEGIN_MESSAGE_MAP( ZMiniCalendarDlg, CDialog )
   //{{AFX_MSG_MAP( ZMiniCalendarDlg )
   ON_WM_SYSCOMMAND( )
   ON_WM_PAINT( )
   ON_WM_QUERYDRAGICON( )
   ON_BN_CLICKED( IDC_UPDATE, OnUpdate )
   //}}AFX_MSG_MAP
   ON_NOTIFY( NM_CLICK, WMID_CALENDAR, OnCalendarClick )
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarDlg dialog

ZMiniCalendarDlg::ZMiniCalendarDlg( CWnd *pParent /*=0*/ ) :
                  CDialog( ZMiniCalendarDlg::IDD, pParent )
{
   //{{AFX_DATA_INIT( ZMiniCalendarDlg )
   m_b3DBorder = TRUE;
   m_nCols = 1;
   m_bHighlightToday = TRUE;
   m_nMaxSel = 0;
   m_bMultnSelect = TRUE;
   m_bNoneButton = TRUE;
   m_nRows = 1;
   m_bTodayButton = TRUE;
   m_nFirstDayOfWeek = 1;
   m_bShowNonMonthDays = TRUE;
   //}}AFX_DATA_INIT
   // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
   m_hIcon = AfxGetApp( )->LoadIcon( IDR_MAINFRAME );
}

void
ZMiniCalendarDlg::DoDataExchange( CDataExchange *pDX )
{
   CDialog::DoDataExchange( pDX );
   //{{AFX_DATA_MAP( ZMiniCalendarDlg )
   DDX_Check( pDX, IDC_3D_BORDER, m_b3DBorder );
   DDX_Text( pDX, IDC_COLS, m_nCols );
   DDX_Check( pDX, IDC_HIGHLIGHT_TODAY, m_bHighlightToday );
   DDX_Text( pDX, IDC_MAXSEL, m_nMaxSel );
   DDX_Check( pDX, IDC_MULTI_SELECT, m_bMultnSelect );
   DDX_Check( pDX, IDC_NONE_BUTTON, m_bNoneButton );
   DDX_Text( pDX, IDC_ROWS, m_nRows );
   DDX_Check( pDX, IDC_TODAY_BUTTON, m_bTodayButton );
   DDX_Text( pDX, IDC_FIRST_DAY_OF_WEEK, m_nFirstDayOfWeek );
   DDX_Check( pDX, IDC_SHOW_NONMONTH_DAYS, m_bShowNonMonthDays );
   //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// ZMiniCalendarDlg message handlers

BOOL
ZMiniCalendarDlg::OnInitDialog( )
{
   CDialog::OnInitDialog( );

   // Add "About..." menu item to system menu.

   // IDM_ABOUTBOX must be in the system command range.
   ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
   ASSERT( IDM_ABOUTBOX < 0xF000 );

   CMenu *pSysMenu = GetSystemMenu( FALSE );
   if ( pSysMenu )
   {
      CString csAboutMenu;
      csAboutMenu.LoadString( IDS_ABOUTBOX );
      if ( csAboutMenu.IsEmpty( ) == FALSE )
      {
         pSysMenu->AppendMenu( MF_SEPARATOR );
         pSysMenu->AppendMenu( MF_STRING, IDM_ABOUTBOX, csAboutMenu );
      }
   }

   // Set the icon for this dialog.  The framework does this automatically
   // when the application's main window is not a dialog.
   SetIcon( m_hIcon, TRUE );       // set big icon
   SetIcon( m_hIcon, FALSE );      // set small icon

   CRect CalendarRect;
   CWnd *pWnd = GetDlgItem( IDC_CALENDAR_GOES_HERE );
   ASSERT( pWnd );
   pWnd->GetWindowRect( CalendarRect );
   ScreenToClient( CalendarRect );

   m_wndCalendar.Create( 0, 0, WS_CHILD | WS_VISIBLE | FMC_MULTISELECT |
                                 FMC_AUTOSETTINGS | FMC_TODAYBUTTON |
                                 FMC_NONEBUTTON,
                         CalendarRect, this, WMID_CALENDAR );
   m_wndCalendar.SetDate( m_dateToday );
   m_wndCalendar.SetSpecialDaysCallback( IsSpecialDateCallback );
   m_wndCalendar.SetRowsAndColumns( 1, 1 );

   return( TRUE );  // return TRUE  unless you set the focus to a control
}

void
ZMiniCalendarDlg::OnSysCommand( UINT uID, LPARAM lParam )
{
   if ( (nID & 0xFFF0) == IDM_ABOUTBOX )
   {
      CAboutDlg dlgAbout;
      dlgAbout.DoModal( );
   }
   else
   {
      CDialog::OnSysCommand( nID, lParam );
   }
}

// If you add a minimize button to your dialog, you will need the code below
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.
void
ZMiniCalendarDlg::OnPaint( )
{
   if ( IsIconic( ) )
   {
      CPaintDC dc( this ); // device context for painting

      SendMessage( Wm_nCONERASEBKGND, (WPARAM) dc.GetSafeHdc( ), 0 );

      // Center icon in client rectangle
      int cxIcon = ::GetSystemMetrics( SM_CXICON );
      int cyIcon = ::GetSystemMetrics( SM_CYICON );
      CRect rect;
      GetClientRect( &rect );
      int x = (rect.Width( ) - cxIcon + 1) / 2;
      int y = (rect.Height( ) - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon( x, y, m_hIcon );
   }
   else
   {
      CDialog::OnPaint( );
   }
}

// The system calls this to obtain the cursor to display while the user
// drags the minimized window.
HCURSOR
ZMiniCalendarDlg::OnQueryDragIcon( )
{
   return( (HCURSOR) m_hIcon );
}

void
ZMiniCalendarDlg::OnCalendarClick( NMHDR *nmhdr, LRESULT *lResult )
{
   COleDateTime dateBegin;
   COleDateTime dateEnd;
   CString csMessage;

   m_wndCalendar.GetDateSel( dateBegin, dateEnd );

   if ( dateBegin.GetStatus( ) != COleDateTime::valid )
   {
      csMessage = "You selected none\n";
   }
   else
   {
      if ( m_bMultnSelect )
      {
         csMessage = "You selected: ";
         csMessage += dateBegin.Format( "%m/%d/%Y" );
         csMessage += " to ";
         csMessage += dateEnd.Format( "%m/%d/%Y\n" );
      }
      else
      {
         csMessage = "You selected: ";
         csMessage += dateBegin.Format( "%m/%d/%Y\n" );
      }
   }

   AfxTrace( csMessage );
}

void
ZMiniCalendarDlg::OnUpdate( )
{
   UpdateData( );

   COleDateTime dateTemp;
   dateTemp.SetStatus( COleDateTime::invalid );

   m_wndCalendar.SetShow3DBorder( m_b3DBorder );
   m_wndCalendar.SetHighlightToday( m_bHighlightToday );
   m_wndCalendar.SetMultnSelect( m_bMultnSelect );
   m_wndCalendar.SetShowNoneButton( m_bNoneButton );
   m_wndCalendar.SetShowTodayButton( m_bTodayButton );
   m_wndCalendar.SetDate( dateTemp );
   m_wndCalendar.SetFirstDayOfWeek( m_nFirstDayOfWeek );
   m_wndCalendar.SetShowNonMonthDays( m_bShowNonMonthDays );
   if ( m_nMaxSel <= 0 )
      m_wndCalendar.SetMaxSelDays( FMC_NO_MAX_SEL_DAYS );
   else
      m_wndCalendar.SetMaxSelDays( m_nMaxSel );

   m_wndCalendar.SetRowsAndColumns( m_nRows, m_nCols );
   m_wndCalendar.AutoConfigure( );
   m_wndCalendar.RedrawWindow( );
}
#endif

#endif   // __zdctldt_h__ sentry.

