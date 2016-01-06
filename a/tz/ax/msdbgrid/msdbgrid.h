/**************************************************************
   Generated wrapper for ActiveX library named: MSDBGrid
   Generated at:   Wednesday, March 29, 2000
**************************************************************/
/*
CHANGE LOG - most recent first order

   200y.mm.dd    xxx
      Note ...

   2000.08.16    TMV
      Implement routines for multiple selection
      Fix Bookmark bug when reading a single row

   2000.06.15    TMV
      Implement routines for manual navigation by using row and col indexes

   2000.05.29    TMV
      Add mapping initialization routine InitMapping ( )
*/

#ifndef _MSDBGRID_H_
#define _MSDBGRID_H_

#define VC_EXTRALEAN
#include "zstdafx.h"
#include <kzoegdef.h>

zEXTERN HINSTANCE  g_hInstanceDLL;

static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };
#include "Zdr.h"
#include "Zdctl.h"

#include "TZAXHLPR.H"

#define IFontDisp        COleFont
#define IPictureDisp     CPicture

#ifdef zDBGRID_VT_BOOKMARK
#undef zDBGRID_VT_BOOKMARK
#endif
// having long values for bookmarks which means
// VARIANT.lVal gets the value
#define zDBGRID_VT_BOOKMARK VT_I4


#define zDBGRID_AVOID_EVENT_ROUTING LONG_MIN

#define zDBGRID_PROCESSING_REFRESH zMAPACT_CTRL_SPECIFIC1


// Event IDs of current control extracted using ole/com object viewer
#define DISPID_Click                0xfffffda8 // Fired when a mouse click occurs
#define DISPID_DblClick             0xfffffda7 // Fired when a mouse double click occurs
#define DISPID_MouseMove            0xfffffda2 // Occurs when the mouse moves
#define DISPID_MouseDown            0xfffffda3 // Occurs when a mouse button is pressed
#define DISPID_MouseUp              0xfffffda1 // Occurs when a mouse button is released
#define DISPID_KeyUp                0xfffffda4 // Occurs when a key is released
#define DISPID_KeyDown              0xfffffda6 // Occurs when a key is pressed
#define DISPID_KeyPress             0xfffffda5 // Occurs when an ANSI key is pressed and released
#define DISPID_AfterColUpdate       0x00000001 // Occurs after data moves from cell to the grid buffer
#define DISPID_AfterDelete          0x00000002 // Occurs after record deletion from grid
#define DISPID_AfterInsert          0x00000003 // Occurs after record insertion in grid
#define DISPID_AfterUpdate          0x00000004 // Occurs after record changes are written to the database
#define DISPID_BeforeColUpdate      0x00000005 // Occurs before data moves from cell to the grid buffer
#define DISPID_BeforeDelete         0x00000006 // Occurs before record deletion from grid
#define DISPID_BeforeInsert         0x00000007 // Occurs before record insertion in grid
#define DISPID_BeforeUpdate         0x00000008 // Occurs before record changes are written to the database
#define DISPID_ColResize            0x00000009 // Occurs before repainting the grid when a column has been resized
#define DISPID_HeadClick            0x0000000a // Occurs when a column header has been clicked
#define DISPID_RowColChange         0x0000000b // Occurs when a different cell becomes current
#define DISPID_RowResize            0x0000000d // Occurs when rows are resized
#define DISPID_Scroll               0x0000000e // Occurs when the grid is scrolled using the scrollbars
#define DISPID_SelChange            0x0000000f // Occurs when the current range changes to a different range of cells
#define DISPID_Change               0x00000010 // Occurs when the grid contents have changed
#define DISPID_UnboundReadData      0x00000012 // fired if the control needs data
#define DISPID_UnboundWriteData     0x00000013 // fired if the control wants to store data
#define DISPID_UnboundAddData       0x00000014 // fired if the control wants to add data
#define DISPID_UnboundDeleteData    0x00000015 // fired if the control wants to delete data
#define DISPID_Error                0x00000016 // Occurs when an associated action fails
#define DISPID_SplitChange          0x00000018 // Occurs when a different split becomes current
#define DISPID_AfterColEdit         0x00000019 // Fired after column data is edited
#define DISPID_BeforeColEdit        0x0000001a // Fired upon an attempt to edit column data
#define DISPID_ColEdit              0x0000001b // Fired when column data is edited
#define DISPID_ButtonClick          0x00000021 // Occurs when a column button has been clicked
#define DISPID_OnAddNew             0x00000025 // Fired when a user action has invoked an AddNew operation
#define DISPID_UnboundGetBookmark   0x0000002a // fired if the control wants to get a bookmark for positioning

BOOL IsVariantNULL( const VARIANT v );

// Definitions for Interfaces other than IDispatch
#define IDummy IUnknown
#define ICursor IUnknown
// Forward Declarations
class IMsDgridCtrl;
// End Forward Declarations

/*
(null)
*/
typedef enum tagenumScrollBarsConstants {
   enumScrollBarsConstants__None = 0,
   enumScrollBarsConstants__Horizontal = 1,
   enumScrollBarsConstants__Vertical = 2,
   enumScrollBarsConstants__Both = 3,
   enumScrollBarsConstants__Automatic = 4
} enumScrollBarsConstants;

/*
(null)
*/
typedef enum tagScrollBarsConstants {
   ScrollBarsConstants_dbgNone = 0,
   ScrollBarsConstants_dbgHorizontal = 1,
   ScrollBarsConstants_dbgVertical = 2,
   ScrollBarsConstants_dbgBoth = 3,
   ScrollBarsConstants_dbgAutomatic = 4
} ScrollBarsConstants;

/*
(null)
*/
typedef enum tagenumBorderStyleConstants {
   enumBorderStyleConstants__None = 0,
   enumBorderStyleConstants__FixedSingle = 1
} enumBorderStyleConstants;

/*
(null)
*/
typedef enum tagBorderStyleConstants {
   BorderStyleConstants_dbgNoBorder = 0,
   BorderStyleConstants_dbgFixedSingle = 1
} BorderStyleConstants;

/*
(null)
*/
typedef enum tagenumDividerStyleConstants {
   enumDividerStyleConstants__NoDividers = 0,
   enumDividerStyleConstants__BlackLine = 1,
   enumDividerStyleConstants__DarkGrayLine = 2,
   enumDividerStyleConstants__Raised = 3,
   enumDividerStyleConstants__Inset = 4,
   enumDividerStyleConstants__UseForeColor = 5,
   enumDividerStyleConstants__LightGrayLine = 6
} enumDividerStyleConstants;

/*
(null)
*/
typedef enum tagDividerStyleConstants {
   DividerStyleConstants_dbgNoDividers = 0,
   DividerStyleConstants_dbgBlackLine = 1,
   DividerStyleConstants_dbgDarkGrayLine = 2,
   DividerStyleConstants_dbgRaised = 3,
   DividerStyleConstants_dbgInset = 4,
   DividerStyleConstants_dbgUseForeColor = 5,
   DividerStyleConstants_dbgLightGrayLine = 6
} DividerStyleConstants;

/*
(null)
*/
typedef enum tagenumAlignmentConstants {
   enumAlignmentConstants__Left = 0,
   enumAlignmentConstants__Right = 1,
   enumAlignmentConstants__Center = 2,
   enumAlignmentConstants__General = 3
} enumAlignmentConstants;

/*
(null)
*/
typedef enum tagAlignmentConstants {
   AlignmentConstants_dbgLeft = 0,
   AlignmentConstants_dbgRight = 1,
   AlignmentConstants_dbgCenter = 2,
   AlignmentConstants_dbgGeneral = 3
} AlignmentConstants;

/*
(null)
*/
typedef enum tagenumDataModeConstants {
   enumDataModeConstants__Bound = 0,
   enumDataModeConstants__Unbound = 1
} enumDataModeConstants;

/*
(null)
*/
typedef enum tagDataModeConstants {
   DataModeConstants_dbgBound = 0,
   DataModeConstants_dbgUnbound = 1
} DataModeConstants;

/*
(null)
*/
typedef enum tagenumSplitSizeModeConstants {
   enumSplitSizeModeConstants__Scalable = 0,
   enumSplitSizeModeConstants__Exact = 1,
   enumSplitSizeModeConstants__NumberOfColumns = 2
} enumSplitSizeModeConstants;

/*
(null)
*/
typedef enum tagSplitSizeModeConstants {
   SplitSizeModeConstants_dbgScalable = 0,
   SplitSizeModeConstants_dbgExact = 1,
   SplitSizeModeConstants_dbgNumberOfColumns = 2
} SplitSizeModeConstants;

/*
(null)
*/
typedef enum tagenumTabActionConstants {
   enumTabActionConstants__ControlNavigation = 0,
   enumTabActionConstants__ColumnNavigation = 1,
   enumTabActionConstants__GridNavigation = 2
} enumTabActionConstants;

/*
(null)
*/
typedef enum tagTabActionConstants {
   TabActionConstants_dbgControlNavigation = 0,
   TabActionConstants_dbgColumnNavigation = 1,
   TabActionConstants_dbgGridNavigation = 2
} TabActionConstants;

/*
(null)
*/
typedef enum tagenumAppearanceConstants {
   enumAppearanceConstants__Flat = 0,
   enumAppearanceConstants__3D = 1
} enumAppearanceConstants;

/*
(null)
*/
typedef enum tagAppearanceConstants {
   AppearanceConstants_dbgFlat = 0,
   AppearanceConstants_dbg3D = 1
} AppearanceConstants;

/*
(null)
*/
typedef enum tagenumMarqueeStyleConstants {
   enumMarqueeStyleConstants__DottedCellBorder = 0,
   enumMarqueeStyleConstants__SolidCellBorder = 1,
   enumMarqueeStyleConstants__HighlightCell = 2,
   enumMarqueeStyleConstants__HighlightRow = 3,
   enumMarqueeStyleConstants__HighlightRowRaiseCell = 4,
   enumMarqueeStyleConstants__NoMarquee = 5,
   enumMarqueeStyleConstants__FloatingEditor = 6
} enumMarqueeStyleConstants;

/*
(null)
*/
typedef enum tagMarqueeStyleConstants {
   MarqueeStyleConstants_dbgDottedCellBorder = 0,
   MarqueeStyleConstants_dbgSolidCellBorder = 1,
   MarqueeStyleConstants_dbgHighlightCell = 2,
   MarqueeStyleConstants_dbgHighlightRow = 3,
   MarqueeStyleConstants_dbgHighlightRowRaiseCell = 4,
   MarqueeStyleConstants_dbgNoMarquee = 5,
   MarqueeStyleConstants_dbgFloatingEditor = 6
} MarqueeStyleConstants;

/*
(null)
*/
typedef enum tagenumAddNewModeConstants {
   enumAddNewModeConstants__NoAddNew = 0,
   enumAddNewModeConstants__AddNewCurrent = 1,
   enumAddNewModeConstants__AddNewPending = 2
} enumAddNewModeConstants;

/*
(null)
*/
typedef enum tagAddNewModeConstants {
   AddNewModeConstants_dbgNoAddNew = 0,
   AddNewModeConstants_dbgAddNewCurrent = 1,
   AddNewModeConstants_dbgAddNewPending = 2
} AddNewModeConstants;

/*
(null)
*/
typedef enum tagenumErrorConstants {
   enumErrorConstants__BINDERROR = 4097,
   enumErrorConstants__INVPROPVAL = 4098,
   enumErrorConstants__COLINDEX = 6145,
   enumErrorConstants__NOTINIT = 6146,
   enumErrorConstants__CNOTFOUND = 6147,
   enumErrorConstants__INVROWNUM = 6148,
   enumErrorConstants__INVBOOKMARK = 6149,
   enumErrorConstants__BADSELRIDX = 6150,
   enumErrorConstants__SCROLLRANGE = 6151,
   enumErrorConstants__INVSBSTYLE = 6152,
   enumErrorConstants__UPDERROR = 6153,
   enumErrorConstants__ADDERROR = 6154,
   enumErrorConstants__DELERROR = 6155,
   enumErrorConstants__COLDATA = 6156,
   enumErrorConstants__INCOMPAT = 6157,
   enumErrorConstants__FIELDERR = 6158,
   enumErrorConstants__DELMULTROWS = 6159,
   enumErrorConstants__DATAACCESS = 6160,
   enumErrorConstants__BADEVENT = 6161,
   enumErrorConstants__NOPROPNOW = 6162,
   enumErrorConstants__NOCURREC = 6163,
   enumErrorConstants__CAPTOOLONG = 6164,
   enumErrorConstants__SPLITINDEX = 6244,
   enumErrorConstants__VLINDEX = 6245,
   enumErrorConstants__VITEMERR = 6246,
   enumErrorConstants__STYLEINDEX = 6247,
   enumErrorConstants__DUPSTYLE = 6248,
   enumErrorConstants__STYLEERR = 6249,
   enumErrorConstants__UPDSTYLE = 6250,
   enumErrorConstants__REMSTYLE = 6251,
   enumErrorConstants__ADDCELLCOND = 6252,
   enumErrorConstants__STYLENAME = 6253,
   enumErrorConstants__APPLYSTYLE = 6254,
   enumErrorConstants__BMPTOOLARGE = 6255
} enumErrorConstants;

/*
(null)
*/
typedef enum tagErrorConstants {
   ErrorConstants_dbgBINDERROR = 4097,
   ErrorConstants_dbgINVPROPVAL = 4098,
   ErrorConstants_dbgCOLINDEX = 6145,
   ErrorConstants_dbgNOTINIT = 6146,
   ErrorConstants_dbgCNOTFOUND = 6147,
   ErrorConstants_dbgINVROWNUM = 6148,
   ErrorConstants_dbgINVBOOKMARK = 6149,
   ErrorConstants_dbgBADSELRIDX = 6150,
   ErrorConstants_dbgSCROLLRANGE = 6151,
   ErrorConstants_dbgINVSBSTYLE = 6152,
   ErrorConstants_dbgUPDERROR = 6153,
   ErrorConstants_dbgADDERROR = 6154,
   ErrorConstants_dbgDELERROR = 6155,
   ErrorConstants_dbgCOLDATA = 6156,
   ErrorConstants_dbgINCOMPAT = 6157,
   ErrorConstants_dbgFIELDERR = 6158,
   ErrorConstants_dbgDELMULTROWS = 6159,
   ErrorConstants_dbgDATAACCESS = 6160,
   ErrorConstants_dbgBADEVENT = 6161,
   ErrorConstants_dbgNOPROPNOW = 6162,
   ErrorConstants_dbgNOCURREC = 6163,
   ErrorConstants_dbgCAPTOOLONG = 6164,
   ErrorConstants_dbgSPLITINDEX = 6244,
   ErrorConstants_dbgVLINDEX = 6245,
   ErrorConstants_dbgVITEMERR = 6246,
   ErrorConstants_dbgSTYLEINDEX = 6247,
   ErrorConstants_dbgDUPSTYLE = 6248,
   ErrorConstants_dbgSTYLEERR = 6249,
   ErrorConstants_dbgUPDSTYLE = 6250,
   ErrorConstants_dbgREMSTYLE = 6251,
   ErrorConstants_dbgADDCELLCOND = 6252,
   ErrorConstants_dbgSTYLENAME = 6253,
   ErrorConstants_dbgAPPLYSTYLE = 6254,
   ErrorConstants_dbgBMPTOOLARGE = 6255
} ErrorConstants;

class CActiveXEvent : public CObject
{
public:
   CActiveXEvent(){};
   CActiveXEvent(CString strName, long lID){
      m_strName = strName;
      m_lID = lID;
   };

   ~CActiveXEvent(){};
public:
   CString m_strName;
   long m_lID;
};

class IMsDbGridList : public CListBox
{
   friend class IMsDgridCtrl;
// Construction
public:
   IMsDbGridList();

// Attributes
protected:
   int m_nCurrentColumn;
   IMsDgridCtrl *m_pParentGrid;
   ZActiveX *m_pParentMapAct;
   CWnd *m_pWndLastFocus;
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(IMsDbGridList)
   afx_msg void OnLButtonDblClk( UINT uModKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );

   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~IMsDbGridList();
   void UpdateEntityAttributeImmediate( CString strText );

   // Generated message map functions
protected:
   //{{AFX_MSG(IMsDbGridList)
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};


// The main grid object.

class AFX_CLASS_EXPORT IMsDgridCtrl : public  COleDispatchDriver
{
public:
   // Contruction and destruction all inline
   IMsDgridCtrl() {
      Init();
   }
   IMsDgridCtrl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {
      Init();
   }
   IMsDgridCtrl(const IMsDgridCtrl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {
      Init();
   }

   virtual ~IMsDgridCtrl();

   /////////////////////////////////////////////////////////////////////////////////////

   zSHORT InitMapping( );

private:

   // initialize member variables
   void Init();


public: // methods for setting and getting poperties

   BOOL   GetAllowDelete();
   void   SetAllowDelete( BOOL propVal );

   BOOL   GetAllowAddNew();
   void   SetAllowAddNew( BOOL propVal );

   BOOL   GetAllowRowSizing();
   void   SetAllowRowSizing( BOOL propVal );

   BOOL   GetAllowUpdate();
   void   SetAllowUpdate( BOOL propVal );

   enumAppearanceConstants   GetAppearance();
   void   SetAppearance( enumAppearanceConstants propVal );

   enumBorderStyleConstants   GetBorderStyle();
   void   SetBorderStyle( enumBorderStyleConstants propVal );

   VARIANT GetBookmark();
   void SetBookmark(const VARIANT &propVal);

   short  GetCol();
   void   SetCol( short propVal );

   BOOL   GetColumnHeaders();
   void   SetColumnHeaders( BOOL propVal );

   BOOL   GetCurrentCellVisible();
   void   SetCurrentCellVisible( BOOL propVal );

   BOOL   GetCurrentCellModified();
   void   SetCurrentCellModified( BOOL propVal );

   BOOL   GetDataChanged();
   void   SetDataChanged( BOOL propVal );

   enumDataModeConstants GetDataMode();
   void   SetDataMode( enumDataModeConstants propVal );

   ICursor * GetDataSource();
   void   SetDataSource( ICursor* propVal );

   float  GetDefColWidth();
   void   SetDefColWidth( float propVal );

   BOOL   GetEditActive();
   void   SetEditActive( BOOL propVal );

   BOOL   GetEnabled();
   void   SetEnabled( BOOL propVal );

   VARIANT GetFirstRow();
   void SetFirstRow(const VARIANT& propVal);

   IFontDisp * GetHeadFont();
   void   SetHeadFont( IFontDisp* propVal );

   float  GetHeadLines();
   void   SetHeadLines( float propVal );

   OLE_HANDLE GethWnd();
   void   SethWnd( OLE_HANDLE propVal );

   OLE_HANDLE GethWndEditor();
   void   SethWndEditor( OLE_HANDLE propVal );

   BOOL   GetMarqueeUnique();
   void   SetMarqueeUnique( BOOL propVal );

   enumMarqueeStyleConstants   GetMarqueeStyle();
   void   SetMarqueeStyle( enumMarqueeStyleConstants propVal );

   BOOL   GetRecordSelectors();
   void   SetRecordSelectors( BOOL propVal );

   short  GetRow();
   void   SetRow( short propVal );

   enumDividerStyleConstants GetRowDividerStyle();
   void   SetRowDividerStyle( enumDividerStyleConstants propVal );

   float  GetRowHeight();
   void   SetRowHeight( float propVal );

   enumScrollBarsConstants GetScrollBars();
   void   SetScrollBars( enumScrollBarsConstants propVal );

   long   GetSelLength();
   void   SetSelLength( long propVal );

   long   GetSelStart();
   void   SetSelStart( long propVal );

   BSTR   GetSelText();
   void   SetSelText( BSTR propVal );

   short  GetSelStartCol();
   void   SetSelStartCol( short propVal );

   short  GetSelEndCol();
   void   SetSelEndCol( short propVal );

   short  GetSplit();
   void   SetSplit( short propVal );

   short  GetVisibleCols();
   void   SetVisibleCols( short propVal );

   short  GetVisibleRows();
   void   SetVisibleRows( short propVal );

   OLE_COLOR GetBackColor();
   void   SetBackColor( OLE_COLOR propVal );

   OLE_COLOR   GetForeColor();
   void   SetForeColor( OLE_COLOR propVal );

   BSTR   GetCaption();
   void   SetCaption( BSTR propVal );

   IFontDisp * GetFont();
   void   SetFont( IFontDisp* propVal );

   short  GetLeftCol();
   void   SetLeftCol( short propVal );

   BOOL   GetAllowArrows();
   void   SetAllowArrows( BOOL propVal );

   BOOL   GetTabAcrossSplits();
   void   SetTabAcrossSplits( BOOL propVal );

   enumTabActionConstants GetTabAction();
   void   SetTabAction( enumTabActionConstants propVal );

   BOOL   GetWrapCellPointer();
   void   SetWrapCellPointer( BOOL propVal );

   long   GetApproxCount();
   void   SetApproxCount( long propVal );

   BSTR   GetErrorText();
   void   SetErrorText( BSTR propVal );

   enumAddNewModeConstants   GetAddNewMode();
   void   SetAddNewMode( enumAddNewModeConstants propVal );

   CString GetText();
   void    SetText( LPCTSTR propVal );

public: // methods of the current Dispinterface

   void           AboutBox(  );
   LPDISPATCH     GetColumns( const VARIANT & Index );
   LPDISPATCH     GetColumns( );
   LPDISPATCH     GetSplits( const VARIANT & Index );
   VARIANT        GetSelBookmarks( const VARIANT & Index );
   VARIANT        GetSelBookmarks( );
   short          ColContaining( float X );
   VARIANT        GetBookmark( long RowNum );
   void           ReBind(  );
   VARIANT        RowBookmark( short RowNum );
   short          RowContaining( float Y );
   float          RowTop( short RowNum );
   void           Scroll( long Cols, long Rows );
   void           Refresh(  );
   IPictureDisp   CaptureImage(  );
   void           ClearSelCols(  );
   void           ClearFields(  );
   void           HoldFields(  );
   short          SplitContaining( float X, float Y );


   // helper methods for mapping and event - handling
   zSHORT   MapFromOI(  );
   zSHORT   MapToOI(  );

   zSHORT   HandleEvent(  DISPID dispEvent );
   // tell Zeidon control which events are handled by this DLL
   void     WantHandleEvents( );
   // synchronize view against Grid
   zLONG    SetViewCursorFromGridCursor( COleVariant vCurrentRow );
   // synchronize Grid against View
   void     SetGridCursorFromViewCursor( );
   void     GridSetSelection( );

   // Event Handlers
   zLONG    OnRowColChange( );
   zLONG    OnButtonClick( );
   zLONG    OnSelChange( );
   zLONG    OnHeadClick( );
   zLONG    OnAfterColUpdate( );
   zLONG    OnBeforeColUpdate( );
   zLONG    OnSplitChange( );
   zLONG    OnKeyDown();
   zLONG    OnMouseDown();
   void     OnUnboundGetRelativeBookmark( );
   void     OnUnboundReadData( );
   void     OnUnboundWriteData( );

   double   TwipsToPixel( double dTwipCoord, zBOOL bX );
   double   PixelToTwips( double dY, zBOOL bX );

   zSHORT   GetCellData( zPCHAR pchReturnData, zLONG lRowNbr, zLONG lColNbr);
   zSHORT   SetCellData( zCPCHAR pchReturnData, zLONG lRowNbr, zLONG lColNbr);
   zSHORT   GetActiveCell( zPLONG plRowNbr, zPLONG plColNbr);
   zSHORT   SetCurrentCell( zLONG lRowNbr, zLONG lColNbr, zBOOL bFromExternal = FALSE );

   void     PopulateListBox( zSHORT nColumn );


   CString GetEventName( long l);
   void InitEventList();

   // for caching view/entity/scope entity name
   CString m_strScopeEntityName;
   CString m_strEntityName;
   CString m_strViewName;

   ZActiveX* m_pZActiveX; // Pointer to Zeidon Control Instance

private:
   BOOL CreateComboBox( int nColumn );

   zBOOL m_bTraceEvents;
   //zBOOL m_bRowChangedByApplication;
   zBOOL m_bExternalRowChange;
   // members to handle ordering
   zSHORT m_nCurrentOrderCol;  // index of the last order column
   zSHORT m_nCurrentOrderMode; // 0 = ascending other descending
   // helper functions for bookmark handling
   void MakeBookmark( long lAbsolutePosition, LPVARIANT vBookmark );
   long GetPositionFromBookmark( COleVariant v);
   void UpdateAllColumnValues();
   DISPID m_DispIdPrevEvent; // previous Event;
   //
   zVIEW             m_vGridApp;
   IMsDbGridList     **m_pWndListArray;
   IMsDbGridList     *m_pWndListBox;
   zLONG             m_lColumns;
   HFONT             m_hFontTab;
   zSHORT            m_nLogX;
   zSHORT            m_nLogY;
   zLONG             m_lPixelsPerInchX;
   zLONG             m_lPixelsPerInchY;

   // for debugging
   CObList m_EventList;
};

/////////////////////////////////////////////////////////////////////////////
// Column wrapper class

class Column : public COleDispatchDriver
{
public:
   Column() {}      // Calls COleDispatchDriver default constructor
   Column(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Column(const Column& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetAlignment();
   void SetAlignment(long);
   BOOL GetAllowSizing();
   void SetAllowSizing(BOOL);
   long GetDividerStyle();
   void SetDividerStyle(long);
   CString GetCaption();
   void SetCaption(LPCTSTR);
   BOOL GetDataChanged();
   CString GetDataField();
   void SetDataField(LPCTSTR);
   VARIANT GetDefaultValue();
   void SetDefaultValue(const VARIANT&);
   short GetColIndex();
   float GetLeft();
   BOOL GetLocked();
   void SetLocked(BOOL);
   CString GetNumberFormat();
   void SetNumberFormat(LPCTSTR);
   CString GetText();
   void SetText(LPCTSTR);
   float GetTop();
   VARIANT GetValue();
   void SetValue(const VARIANT&);
   BOOL GetVisible();
   void SetVisible(BOOL);
   float GetWidth();
   void SetWidth(float);
   BOOL GetWrapText();
   void SetWrapText(BOOL);
   BOOL GetButton();
   void SetButton(BOOL);

// Operations
public:
   CString CellText(const VARIANT& Bookmark);
   VARIANT CellValue(const VARIANT& Bookmark);
};
/////////////////////////////////////////////////////////////////////////////
// Columns wrapper class

class Columns : public COleDispatchDriver
{
public:
   Columns() {}      // Calls COleDispatchDriver default constructor
   Columns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Columns(const Columns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();

// Operations
public:
   LPUNKNOWN Get_NewEnum();
   LPDISPATCH GetItem(const VARIANT& Index);
   void SetAdd(short Index);
   LPDISPATCH GetAdd(short Index);
   void Remove(const VARIANT& Index);
   Column GetColumn( short Index );
};
/////////////////////////////////////////////////////////////////////////////
// SelBookmarks wrapper class

class SelBookmarks : public COleDispatchDriver
{
public:
   SelBookmarks() {}      // Calls COleDispatchDriver default constructor
   SelBookmarks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   SelBookmarks(const SelBookmarks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();

// Operations
public:
   LPUNKNOWN Get_NewEnum();
   VARIANT GetItem(const VARIANT& Index);
   void Add(const VARIANT& Bookmark);
   void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// RowBuffer wrapper class

class RowBuffer : public COleDispatchDriver
{
public:
   RowBuffer() {}      // Calls COleDispatchDriver default constructor
   RowBuffer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   RowBuffer(const RowBuffer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetRowCount();
   void SetRowCount(long);
   short GetColumnCount();

// Operations
public:
   void SetBookmark(long Row, const VARIANT& newValue);
   VARIANT GetBookmark(long Row);
   CString GetColumnName(short Col);
   void SetValue(long Row, short Col, const VARIANT& newValue);
   VARIANT GetValue(long Row, short Col);
};
/////////////////////////////////////////////////////////////////////////////
// Splits wrapper class

class Splits : public COleDispatchDriver
{
public:
   Splits() {}      // Calls COleDispatchDriver default constructor
   Splits(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Splits(const Splits& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();

// Operations
public:
   LPUNKNOWN Get_NewEnum();
   LPDISPATCH GetItem(const VARIANT& Index);
   void SetAdd(short Index);
   LPDISPATCH GetAdd(short Index);
   void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Split wrapper class

class Split : public COleDispatchDriver
{
public:
   Split() {}      // Calls COleDispatchDriver default constructor
   Split(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Split(const Split& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   BOOL GetLocked();
   void SetLocked(BOOL);
   short GetIndex();
   short GetScrollGroup();
   void SetScrollGroup(short);
   long GetMarqueeStyle();
   void SetMarqueeStyle(long);
   BOOL GetAllowSizing();
   void SetAllowSizing(BOOL);
   BOOL GetAllowFocus();
   void SetAllowFocus(BOOL);
   long GetSizeMode();
   void SetSizeMode(long);
   VARIANT GetSize();
   void SetSize(const VARIANT&);
   BOOL GetAllowRowSizing();
   void SetAllowRowSizing(BOOL);
   BOOL GetRecordSelectors();
   void SetRecordSelectors(BOOL);
   long GetScrollBars();
   void SetScrollBars(long);
   BOOL GetCurrentCellVisible();
   void SetCurrentCellVisible(BOOL);
   VARIANT GetFirstRow();
   void SetFirstRow(const VARIANT&);
   short GetLeftCol();
   void SetLeftCol(short);
   short GetSelStartCol();
   void SetSelStartCol(short);
   short GetSelEndCol();
   void SetSelEndCol(short);

// Operations
public:
   LPDISPATCH GetColumns(const VARIANT& Index);
   void ClearSelCols();
};


#endif //_MSDBGRID_H_
#ifdef __cplusplus
extern "C"
{
#endif

zOPER_EXPORT LPCTSTR OPERATION    IMsDgridCtrl_GetText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, LPCTSTR propVal );

zOPER_EXPORT enumAddNewModeConstants OPERATION    IMsDgridCtrl_GetAddNewMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAddNewMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumAddNewModeConstants propVal );

zOPER_EXPORT BSTR OPERATION    IMsDgridCtrl_GetErrorText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetErrorText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal );

zOPER_EXPORT long OPERATION    IMsDgridCtrl_GetApproxCount( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetApproxCount( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetWrapCellPointer( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetWrapCellPointer( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT enumTabActionConstants OPERATION    IMsDgridCtrl_GetTabAction( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetTabAction( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumTabActionConstants propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetTabAcrossSplits( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetTabAcrossSplits( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowArrows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowArrows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetLeftCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetLeftCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT IFontDisp* OPERATION    IMsDgridCtrl_GetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, IFontDisp* propVal );

zOPER_EXPORT BSTR OPERATION    IMsDgridCtrl_GetCaption( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCaption( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal );

zOPER_EXPORT OLE_COLOR OPERATION    IMsDgridCtrl_GetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal );

zOPER_EXPORT OLE_COLOR OPERATION    IMsDgridCtrl_GetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetVisibleRows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetVisibleRows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetVisibleCols( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetVisibleCols( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetSplit( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSplit( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetSelEndCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelEndCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetSelStartCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelStartCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT BSTR OPERATION    IMsDgridCtrl_GetSelText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal );

zOPER_EXPORT long OPERATION    IMsDgridCtrl_GetSelStart( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelStart( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long propVal );

zOPER_EXPORT long OPERATION    IMsDgridCtrl_GetSelLength( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelLength( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long propVal );

zOPER_EXPORT enumScrollBarsConstants OPERATION    IMsDgridCtrl_GetScrollBars( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetScrollBars( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumScrollBarsConstants propVal );

zOPER_EXPORT float OPERATION    IMsDgridCtrl_GetRowHeight( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRowHeight( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float propVal );

zOPER_EXPORT enumDividerStyleConstants OPERATION    IMsDgridCtrl_GetRowDividerStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRowDividerStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumDividerStyleConstants propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetRecordSelectors( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRecordSelectors( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT enumMarqueeStyleConstants OPERATION    IMsDgridCtrl_GetMarqueeStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetMarqueeStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumMarqueeStyleConstants propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetMarqueeUnique( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetMarqueeUnique( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT OLE_HANDLE OPERATION    IMsDgridCtrl_GethWndEditor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SethWndEditor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_HANDLE propVal );

zOPER_EXPORT OLE_HANDLE OPERATION    IMsDgridCtrl_GethWnd( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SethWnd( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_HANDLE propVal );

zOPER_EXPORT float OPERATION    IMsDgridCtrl_GetHeadLines( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetHeadLines( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float propVal );

zOPER_EXPORT IFontDisp* OPERATION    IMsDgridCtrl_GetHeadFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetHeadFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, IFontDisp* propVal );

zOPER_EXPORT VARIANT OPERATION    IMsDgridCtrl_GetFirstRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetFirstRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, VARIANT propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetEditActive( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetEditActive( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT float OPERATION    IMsDgridCtrl_GetDefColWidth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDefColWidth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float propVal );

zOPER_EXPORT ICursor* OPERATION    IMsDgridCtrl_GetDataSource( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDataSource( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, ICursor* propVal );

zOPER_EXPORT enumDataModeConstants OPERATION    IMsDgridCtrl_GetDataMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDataMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumDataModeConstants propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetDataChanged( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDataChanged( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetCurrentCellModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCurrentCellModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetCurrentCellVisible( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCurrentCellVisible( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetColumnHeaders( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetColumnHeaders( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT VARIANT OPERATION    IMsDgridCtrl_GetBookmark( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetBookmark( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, VARIANT propVal );

zOPER_EXPORT enumBorderStyleConstants OPERATION    IMsDgridCtrl_GetBorderStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetBorderStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumBorderStyleConstants propVal );

zOPER_EXPORT enumAppearanceConstants OPERATION    IMsDgridCtrl_GetAppearance( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAppearance( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumAppearanceConstants propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowUpdate( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowUpdate( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowRowSizing( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowRowSizing( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowAddNew( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowAddNew( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowDelete( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowDelete( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT short OPERATION IMsDgridCtrl_SplitContaining( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float X, float Y );

zOPER_EXPORT void OPERATION IMsDgridCtrl_HoldFields( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

zOPER_EXPORT void OPERATION IMsDgridCtrl_ClearFields( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

zOPER_EXPORT void OPERATION IMsDgridCtrl_ClearSelCols( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

zOPER_EXPORT void OPERATION IMsDgridCtrl_Refresh( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

zOPER_EXPORT void OPERATION IMsDgridCtrl_Scroll( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long Cols, long Rows );

zOPER_EXPORT float OPERATION IMsDgridCtrl_RowTop( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short RowNum );

zOPER_EXPORT short OPERATION IMsDgridCtrl_RowContaining( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float Y );

zOPER_EXPORT VARIANT OPERATION IMsDgridCtrl_RowBookmark( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short RowNum );

zOPER_EXPORT void OPERATION IMsDgridCtrl_ReBind( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

zOPER_EXPORT VARIANT OPERATION IMsDgridCtrl_GetBookmark0( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long RowNum );

zOPER_EXPORT short OPERATION IMsDgridCtrl_ColContaining( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float X );

zOPER_EXPORT VARIANT OPERATION IMsDgridCtrl_GetSelBookmarks( zVIEW vSubtask, zCPCHAR  cpcCtrlTag,  const VARIANT & Index );

zOPER_EXPORT IDispatch* OPERATION IMsDgridCtrl_GetSplits( zVIEW vSubtask, zCPCHAR  cpcCtrlTag,  const VARIANT & Index );

zOPER_EXPORT IDispatch* OPERATION IMsDgridCtrl_GetColumns( zVIEW vSubtask, zCPCHAR  cpcCtrlTag,  const VARIANT & Index );

zOPER_EXPORT void OPERATION IMsDgridCtrl_AboutBox( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_LockColumn(zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zSHORT nIndex, zBOOL bLock );

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_SetCellData( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zCPCHAR  pchData, zLONG lRowNbr, zLONG lColNbr );

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_GetCellData( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zPCHAR  pchData, zLONG lRowNbr, zLONG lColNbr );

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_GetActiveCell( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zPLONG lRowNbr, zPLONG lColNbr );

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_SetActiveCell( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zLONG lRowNbr, zLONG lColNbr );

#ifdef __cplusplus
}
#endif

