/********************************************************************************************/
//
// File:            tdbg6.h
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           6. September 2000
// describtion:
//
//
// list of classes :
// Name:                 describtion:
// ------------------------------------------------------------------------------------
// ITrueDBGridCtrl       wrapper for True DBGrid
// ITrueDBDropDownCtrl   wrapper for True DBGrid DropDown
// Columns               wrapper for ITrueDBGridCtrl column collection
// Column                wrapper for ITrueDBGridCtrl representing a single column
// Layouts               wrapper for a specific layout of an ITrueDBGridCtrl instance
// PrintInfos            wrapper for True DBGrid
// PrintInfo             wrapper for True DBGrid DropDown
// RowBuffer             wrapper for ITrueDBGridCtrl Buffer for transfering data in unbound mode
// SelBookmarks          wrapper for ITrueDBGridCtrl representing a selected entry
// Split                 wrapper for a specific Split of an ITrueDBGridCtrl instance
// Splits                wrapper for ITrueDBGridCtrl Split collection
// StyleDisp             wrapper for a specific Style of an ITrueDBGridCtrl instance
// Styles                wrapper for ITrueDBGridCtrl StyleDisp collection
// ValueItemDisp         wrapper for single entry in an ComboBox in an ITrueDBGridCtrl instance
// ValueItems            wrapper for ITrueDBGridCtrl ValueItemDisp collection
// ZGridColumn           helper class for caching zeidon column properties
//
/* Change log most recent first order:

   2002.08.09  BL
      add GetCheckBoxValueForSetData and GetCheckBoxValueForGetData: if column a
      CheckBox, than return/set the CheckBox Value (not Text Value)

   2001.11.1   TMV   2000   ActiveX
      add handlers for events BeforeUpdate, AfterUpdate

   2001.8.16   TMV   2000   ActiveX
      - Add TraceState for debugging
      - renamed SetRadioGroupsDefault to SetDefaultValueItem
      - changed IsRowInsertMode

   2001.8.16   TMV   2000   ActiveX
      - introduced SetRadioGroupsDefault for initial RadioGroup setup
        if a new row is inserted via OnBeforeInsert

   2001.7.3   TMV   2000   ActiveX
      implement handler for loose focus

   2001.6.18   BL    2000
      Change Parameter Type in Operation lTrueDBGridCtrl_LockColumn

   2001.6.17   BL    2000
      added ITrueDBGridCtrl_GetMouseDownParam

   2001.4.26   TMV   2000   R55120/F51749
      move inline method ZGridColumn::GetMappingEx to tdbg6.cpp
      and check the pointer to the attached Column against null

   2001.3.20   TMV   2000   ActiveX
      Introduced new method GetVariantFromAttribute to clean code

   2001.3.15   TMV   2000   ActiveX
      introduce new membervariable m_nCurrentCell to ITrueDBGridCtrl

   2001.3.13   TMV   2000   ActiveX
      add ITrueDBGridCtrl::EnableAutosort to enable/disable autosort

   2001.3.6   TMV   2000   ActiveX
      add new method ZGridColumn::CheckLocking( zVIEW vApp )

   2001.2.8   TMV   2000   ActiveX
      Implement complex combobox mapping to allow mapping of a combobox - List
      Entity.

   2001.01.03  BL    2000   ActiveX
      Added m_strTempViewName for save temporaly Application View
   2001.01.01  BL    2000   ActiveX
      Added Member Variable to save View Name

   2000.9.22   TMV   2000   ActiveX
      Implement method GetGridColumn to avoid direct access to m_aGridCols

   2000.9.19   TMV   2000   ActiveX
      fix for handling selectstates of an entity during Mapping

   2000.9.18   TMV   2000   ActiveX
      remove obsolete eventhandler

   15.09.2000   TMV   2000   ActiveX
      do inline documentation



   2000.09.06   TMV
      Created


*/
/*********************************************************************************************/
#ifndef _TRUEDBGRID_H_
#define _TRUEDBGRID_H_

#define ZDCTL_CLASS AFX_EXT_CLASS


#define VC_EXTRALEAN
#include "zstdafx.h"
#include <kzoegdef.h>

zEXTERN HINSTANCE  g_hInstanceDLL;

static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };
#include "trhlpr.h"
#include "tgcolser.h"
#include "xarraydb.h"
#include "..\..\..\INCR\zdrvropr.h"   // Added by ClassView

#define DISPID_Click                      0xfffffda8   // helpstring("Fired when a mouse click occurs in the grid"), helpcontext(0x0005f450)]
#define DISPID_DblClick                   0xfffffda7   // helpstring("Fired when a mouse double click occurs in the grid"), helpcontext(0x0005f451)]
#define DISPID_MouseMove                  0xfffffda2   // helpstring("Occurs when the mouse cursor is moved over the grid"), helpcontext(0x0005f461)]
#define DISPID_MouseDown                  0xfffffda3   // helpstring("Occurs when a mouse button is pressed while cursor is over the grid"), helpcontext(0x0005f462)]
#define DISPID_MouseUp                    0xfffffda1   // helpstring("Occurs when a mouse button is released while cursor is over the grid"), helpcontext(0x0005f463)]
#define DISPID_KeyUp                      0xfffffda4   // helpstring("Occurs when a key is released"), helpcontext(0x0005f464)]
#define DISPID_KeyDown                    0xfffffda6   // helpstring("Occurs when a key is pressed"), helpcontext(0x0005f465)]
#define DISPID_KeyPress                   0xfffffda5   // helpstring("Occurs when an ANSI key is pressed"), helpcontext(0x0005f466)]
#define DISPID_AfterColUpdate             0x1   // helpstring("Occurs after data moves from cell to the grid buffer"), helpcontext(0x0005f452)]
#define DISPID_AfterDelete                0x2   // helpstring("Occurs after record deletion from grid"), helpcontext(0x0005f453)]
#define DISPID_AfterInsert                0x3   // helpstring("Occurs after record insertion in grid"), helpcontext(0x0005f454)]
#define DISPID_AfterUpdate                0x4   // helpstring("Occurs after record changes are written to the data source"), helpcontext(0x0005f455)]
#define DISPID_BeforeColUpdate            0x5   // helpstring("Occurs before data moves from cell to the grid buffer"), helpcontext(0x0005f456)]
#define DISPID_BeforeDelete               0x6   // helpstring("Occurs before record deletion from grid"), helpcontext(0x0005f457)]
#define DISPID_BeforeInsert               0x7   // helpstring("Occurs before record insertion in grid"), helpcontext(0x0005f458)]
#define DISPID_BeforeUpdate               0x8   // helpstring("Occurs before record changes are written to the data source"), helpcontext(0x0005f459)]
#define DISPID_ColResize                  0x9   // helpstring("Occurs before repainting the grid when a column has been resized"), helpcontext(0x0005f45a)]
#define DISPID_HeadClick                  0xa   // helpstring("Occurs when a column header is clicked"), helpcontext(0x0005f45b)]
#define DISPID_RowColChange               0xb   // helpstring("Occurs after a different cell becomes current in the grid"), helpcontext(0x0005f45c)]
#define DISPID_RowResize                  0xd   // helpstring("Occurs before repainting the grid when rows have been resized"), helpcontext(0x0005f45d)]
#define DISPID_Scroll                     0xe   // helpstring("Occurs when the grid is scrolled using the scrollbars"), helpcontext(0x0005f45e)]
#define DISPID_SelChange                  0xf   // helpstring("Occurs whenever a row or column is selected or deselected in the grid"), helpcontext(0x0005f45f)]
#define DISPID_SplitChange                0x18   // helpstring("Occurs when a different split becomes current"), helpcontext(0x0005f460)]
#define DISPID_Change                     0x10   // helpstring("Occurs when the grid contents have changed"), helpcontext(0x0005f467)]
#define DISPID_UnboundReadData            0x12   // helpstring("Fired when the grid requires unbound data for display"), helpcontext(0x0005f469)]
#define DISPID_UnboundWriteData           0x13   // helpstring("Fired when an unbound row needs to be modified"), helpcontext(0x0005f46f)]
#define DISPID_UnboundAddData             0x14   // helpstring("Fired when a new row is added to the unbound data source"), helpcontext(0x0005f470)]
#define DISPID_UnboundDeleteRow           0x15   // helpstring("Fired when an unbound row needs to be deleted"), helpcontext(0x0005f471)]
#define DISPID_Error                      0x16   // helpstring("Occurs when an associated action fails"), helpcontext(0x0005f472)]
#define DISPID_UnboundColumnFetch         0x17   // helpstring("Fired to fetch data for an unbound column when the grid is in bound mode"), helpcontext(0x0005f473)]
#define DISPID_AfterColEdit               0x19   // helpstring("Fired after column data is edited"), helpcontext(0x0005f474)]
#define DISPID_BeforeColEdit              0x1a   // helpstring("Fired when any attempt to edit column data is made"), helpcontext(0x0005f475)]
#define DISPID_ColEdit                    0x1b   // helpstring("Fired when an edit is initiated for a column"), helpcontext(0x0005f476)]
#define DISPID_FirstRowChange             0x1c   // helpstring("Fired when the FirstRow property changes"), helpcontext(0x0005f477)]
#define DISPID_LeftColChange              0x1d   // helpstring("Fired when the LeftCol property changes"), helpcontext(0x0005f478)]
#define DISPID_FormatText                 0x1e   // helpstring("Fired when the NumberFormat property is set to FormatText Event"), helpcontext(0x0005f479)]
#define DISPID_DragCell                   0x20   // helpstring("Fired whenever a drag operation is initiated"), helpcontext(0x0005f47a)]
#define DISPID_ButtonClick                0x21   // helpstring("Occurs when a column button is clicked"), helpcontext(0x0005f47b)]
#define DISPID_ComboSelect                0x22   // helpstring("Fired when the user selects an entry from a ValueItems combo box"), helpcontext(0x0005f47c)]
#define DISPID_ValueItemError             0x23   // helpstring("Fired when the user enters invalid data into a ValueItems column"), helpcontext(0x0005f47d)]
#define DISPID_FetchCellStyle             0x24   // helpstring("Fired when the FetchStyle property is set to True for a column"), helpcontext(0x0005f47e)]
#define DISPID_OnAddNew                   0x25   // helpstring("Fired when a user action has invoked an AddNew operation"), helpcontext(0x0005f47f)]
#define DISPID_Paint                      0x26   // helpstring("Fired when the grid repaints (receives a WM_PAINT message)"), helpcontext(0x0005f480)]
#define DISPID_ColMove                    0x27   // helpstring("Occurs before repainting the grid when the selected columns have been moved"), helpcontext(0x0005f481)]
#define DISPID_PostEvent                  0x28   // helpstring("Fired in response to the PostMsg method"), helpcontext(0x0005f482)]
#define DISPID_UnboundReadDataEx          0x29   // helpstring("Fired when the grid requires unbound data and/or bookmarks for display"), helpcontext(0x0005f46a)]
#define DISPID_UnboundGetRelativeBookmark 0x2a   // helpstring("Fired when the grid requires an unbound bookmark for display"), helpcontext(0x0005f468)]
#define DISPID_ClassicRead                0x2b   // helpstring("Fired in application mode when data is needed for display"), helpcontext(0x0005f46b)]
#define DISPID_ClassicWrite               0x2c   // helpstring("Fired in application mode when modified column data must be written to the data source"), helpcontext(0x0005f46c)]
#define DISPID_ClassicDelete              0x2d   // helpstring("Fired in application mode when a row needs to be deleted from the data source"), helpcontext(0x0005f46d)]
#define DISPID_ClassicAdd                 0x2e   // helpstring("Fired in application mode when a row needs to be added to the data source"), helpcontext(0x0005f46e)]
#define DISPID_DropDownOpen               0x2f
#define DISPID_DropDownClose              0x30
#define DISPID_FetchCellTips              0x31   // helpstring("Fired before a cell tip is shown when the CellTips property is set to True"), helpcontext(0x0005f55d)]
#define DISPID_UnboundFindData            0x32
#define DISPID_FetchRowStyle              0x33   // helpstring("Fired when the FetchRowStyle property is set to True"), helpcontext(0x0005f573)]
#define DISPID_RowChange                  0x34
#define DISPID_FootClick                  0x36   // helpstring("Occurs when a column footer has been clicked"), helpcontext(0x0005f5b1)]
#define DISPID_OwnerDrawCell              0x37   // helpstring("Fired when the column OwnerDraw property is True and the specified cell must be drawn by the user"), helpcontext(0x0005f5ba)]
#define DISPID_OLECompleteDrag            0x38   // helpstring("Occurs at the OLE drag/drop Source control when drag/drop has been completed or canceled"), helpcontext(0x0005f5f6)]
#define DISPID_OLEDragDrop                0x39   // helpstring("Occurs when data is dropped onto the control via an OLE drag/drop operation, and OLEDropMode is set to manual"), helpcontext(0x0005f5f7)]
#define DISPID_OLEDragOver                0x3a   // helpstring("Occurs when the mouse is moved over the control during an OLE drag/drop operation when OLEDropMode is set to manual"), helpcontext(0x0005f5f8)]
#define DISPID_OLEGiveFeedback            0x3b   // helpstring("Occurs at the source control of an OLE drag/drop operation when the mouse cursor needs to be changed"), helpcontext(0x0005f5f9)]
#define DISPID_OLESetData                 0x3c   // helpstring("Occurs at the OLE drag/drop source control when the drop target requests data that was not provided to the DataObject during the OLEDragStart event"), helpcontext(0x0005f5fa)]
#define DISPID_OLEStartDrag               0x3d   // helpstring("Occurs when an OLE drag/drop operation is initiated either manually or automatically"), helpcontext(0x0005f5fb)]
#define DISPID_OnInit                     0x3e   // helpstring("Occurs after the grid is initially loaded"), helpcontext(0x0005f609)]
#define DISPID_BeforeRowColChange         0x3f   // helpstring("Occurs before a different cell becomes current"), helpcontext(0x0005f62d)]
#define DISPID_DataSourceChanged          0x40   // helpstring("Fired when the DataSource of the grid changes"), helpcontext(0x0005f633)]
#define DISPID_LayoutReady                0x41   // helpstring("Fired after layout information specified with LayoutURL is downloaded"), helpcontext(0x0005f64a)]

// constants use by PostEvent &PostMesg
#define zDBGRID_PostEventButtonClick          0x01
#define zDBGRID_PostEventCreateDateCtrl       0x02

#define zDBGRID_VT_BOOKMARK VT_I4
#define zDBGRID_AVOID_EVENT_ROUTING LONG_MIN

#define zGRIDROWS 1
#define zGRIDCOLS 2

#define ZEIDON_DATETIME_LTH 17

BOOL IsVariantNULL( const VARIANT v );

//////////////////////////////////////////////////////////////////
//
// conversion routines Twips to pixel and vice versa
//
void GetDeviceConstants(  );
zLONG XTwipsToPixels( zLONG twips );
zLONG YTwipsToPixels( zLONG twips );
zLONG XPixelsToTwips( zLONG pixels );
zLONG YPixelsToTwips( zLONG pixels);


// GridConstants
// ValueItems Presentation constants
const long dbgNormal         = 0;
const long dbgRadioButton    = 1;
const long dbgComboBox       = 2;
const long dbgSortedComboBox = 3;
const long dbgCheckBox       = 4;


// DataMode constants
const long dbgBound      = 0;
const long dbgUnbound    = 1;
const long dbgUnboundEx  = 2;
const long dbgUnboundAp  = 3;
const long dbgUnboundSt  = 4;

const long dbgNoAddNew        = 0;
const long dbgAddNewCurrent   = 1;
const long dbgAddNewPending   = 2;

// zeidon specific combobox control type identification constants
const static zLONG CMB_TYPE_DOMAIN                 = 0;
const static zLONG CMB_TYPE_AUTO_INC               = 20480;
const static zLONG CMB_TYPE_SET_FK                 = 36846;
const static zLONG CMB_TYPE_SEL_SOURCE             = 12288;
const static zLONG CMB_TYPE_SET_FK_AND_SEL_SOURCE  = 45056;

class ITrueDBGridCtrl;
class Split;
class ZGridDateCtrl;

/////////////////////////////////////////////////////////////////////////////
// ValueItemDisp wrapper class

class ZDCTL_CLASS ValueItemDisp : public COleDispatchDriver
{
public:
   ValueItemDisp() {}      // Calls COleDispatchDriver default constructor
   ValueItemDisp(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ValueItemDisp(const ValueItemDisp& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   VARIANT GetValue();
   void SetValue(const VARIANT&);
   VARIANT GetDisplayValue();
   void SetDisplayValue(const VARIANT&);

// Operations
public:

   CLSID GetCLSID()
   {
      CString strCLSID = "{00028CE7-0000-0000-0000-000000000046}";
      LPOLESTR lpoleCLSID;
      CLSID clsid;

      lpoleCLSID = strCLSID.AllocSysString();

      CLSIDFromString( lpoleCLSID, &clsid );
      return clsid;
   }

};

/////////////////////////////////////////////////////////////////////////////
// ValueItems wrapper class
class ZDCTL_CLASS ValueItems : public COleDispatchDriver
{
public:
   ValueItems() {}      // Calls COleDispatchDriver default constructor
   ValueItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ValueItems(const ValueItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();
   long GetPresentation();
   void SetPresentation(long);
   BOOL GetTranslate();
   void SetTranslate(BOOL);
   BOOL GetValidate();
   void SetValidate(BOOL);
   BOOL GetCycleOnClick();
   void SetCycleOnClick(BOOL);
   short GetMaxComboItems();
   void SetMaxComboItems(short);
   short GetDefaultItem();
   void SetDefaultItem(short);
   BOOL GetAnnotatePicture();
   void SetAnnotatePicture(BOOL);

// Operations
public:
   ValueItemDisp GetValueItem( long nIndex );
   LPUNKNOWN Get_NewEnum();
   LPDISPATCH GetItem(const VARIANT& Index);
   void Add(LPDISPATCH vitem);
   void Remove(const VARIANT& Index);
   void Clear();
};

class ZOleDateTime : public COleDateTime
{
public:
   ZOleDateTime( zCPCHAR pValue = NULL );
   ZOleDateTime(const COleDateTime& dateSrc);
   ZOleDateTime(const VARIANT& varSrc);
   ZOleDateTime(DATE dtSrc);

   ZOleDateTime(time_t timeSrc);
   ZOleDateTime(const SYSTEMTIME& systimeSrc);
   ZOleDateTime(const FILETIME& filetimeSrc);

   ZOleDateTime(int nYear, int nMonth, int nDay,
      int nHour, int nMin, int nSec);
   ZOleDateTime(WORD wDosDate, WORD wDosTime);

   ~ZOleDateTime(){};

   void operator = ( zCPCHAR pValue );
   void SetValue( zCPCHAR pValue );
   CString GetValue( );
   operator CString() const;
   const ZOleDateTime& operator=(const COleDateTime& dateSrc);
    const ZOleDateTime& operator=(const SYSTEMTIME& systimeSrc);

};


/*
The class ZGridColumn is a representative for a column in a grid.
Basically it caches properties of a grids column such as colors
and locks and the ability to get the focus. In addition this class
matches up the mappings provided by the ZActiveX, so that a mapping
is assigned to a column is a mapping that is used to display data.
So each ZGridColumn that has a mapping can store its value in a zeidon
attribute directly. Using this Object we do not have to keep track about
whether a mapping is used to specify "List" or "Scope" capabilities
*/
class ZGridColumn : public CObject
{
   DECLARE_DYNAMIC( ZGridColumn )

public:
   void SetInitiallyLocked( BOOL bLockIt = FALSE );
   BOOL CheckLocking( zVIEW vApp );
   zLONG PopulateListOfValues();

   ZGridColumn( );
   ZGridColumn( ITrueDBGridCtrl * pParent );
   virtual ~ZGridColumn();
   // initialize
   void Init();

   // getter Methods

   // returns the color for a locked column
   unsigned long GetLockColor(){ return m_ulLockColor; }
   // returns the color for an unlocked column
   unsigned long GetUnlockColor(){ return m_ulUnLockColor; }
   // returns the mapping for current column
   ZMapping *GetMapping( ){ return m_pMapping;}
   // returns the mapping for current column new style
   ZColumnMapping *GetMappingEx( );
   // return the appropriate Check/Uncheck string if column represents a Checkbox
   CString GetCheckBoxValue( BOOL bTrue = TRUE );

   // setter methods

   // caches the color for a column in read only modus
   void SetLockColor( unsigned long ulColor ){ m_ulLockColor = ulColor; }
   // caches the color for a column in normal modus
   void SetUnlockColor( unsigned long ulColor ) { m_ulUnLockColor = ulColor; }
   // caches Readonly flag
   void SetReadOnly( BOOL bSet );
   // caches the mapping
   void SetMapping( ZMapping *pMapping );
   // caches if focus is allowed
   void SetFocusAllowed( BOOL bFocusAllowed ) { m_bFocusAllowed = bFocusAllowed;}
   // caches the index of the column in the grid
   void SetColumnIndex ( short nIndex );
   // sets translation strings for a checkbox
   BOOL SetCheckBoxTranslation( CString strTrueValue, CString strFalseValue );
   // sets flag to create a date time picker for editing a cell
   void SetDateCtrl( BOOL bSet ) { m_bHasDateCtrl = bSet; }


   // runtime validation routines
   BOOL IsReadOnly() { return m_bReadOnly; }
   BOOL IsFocusAllowed() { return m_bFocusAllowed; }
   BOOL IsInitiallyLocked() { return m_bInitiallyLocked; }
   BOOL IsDateCtrl() { return m_bHasDateCtrl; }
   BOOL HasValidCheckBoxTranslation();

   // data manipulation methods
   short UpdateAttribute  ( zVIEW vApp, BOOL bValidateOnly = FALSE );
   short UpdateAttribute  ( zVIEW vApp, CString strValue, BOOL bValidateOnly = FALSE );

   short UpdateAttributeEx  ( zVIEW vApp, BOOL bValidateOnly = FALSE );
   short UpdateAttributeEx  ( zVIEW vApp, CString strValue, BOOL bValidateOnly = FALSE );
   short UpdateAttributeEx  ( zVIEW vApp, ZOleDateTime &dt, BOOL bValidateOnly = FALSE );

   short ValidateAttribute( zVIEW vApp );
   short ValidateAttribute( zVIEW vApp, CString strValue );

   void  AttachZeidonControlColumn( ZGridColCtrl *pZGridColumn );
   CString GetAttributeValue( zVIEW vApp );
   zSHORT GetVariantFromAttribute( COleVariant &v, zVIEW vApp );
   zSHORT GetStringFromAttribute( CString &strValue, zVIEW vApp );
   zSHORT SetAttributeFromVariant( COleVariant &v, zVIEW vApp );
   zLONG SetupComboBoxSelection( );

   // retrieves all values of a list view mapped to a grids combobox
   zSHORT BuildListOfValuesFromOI( );
private:
   BOOL m_bHasDateCtrl;
   void ValidateControlState();
   short m_nMyGridColumn;
   unsigned long m_ulLockColor;
   unsigned long m_ulUnLockColor;

   BOOL m_bReadOnly;
   BOOL m_bFocusAllowed;
   BOOL m_bInitiallyLocked;          // locked due to property dialog

   CString m_strFalseValue;
   CString m_strTrueValue;

   ZMapping *m_pMapping;            // mapping for current column
   ZGridColCtrl *m_pZeidonControlColumn;

   ITrueDBGridCtrl * m_pParentGrid; // Pointer to the grid where this column belongs to

#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

};


class ZGridSplit : public CObject
{
   DECLARE_DYNAMIC( ZGridSplit )

public:
   void SetInitiallyLocked( BOOL bLockIt = FALSE );

   ZGridSplit( );
   ZGridSplit( ITrueDBGridCtrl * pParent );
   virtual ~ZGridSplit();
   // initialize
   void Init();

   // getter Methods
   // returns the color for a locked split
   unsigned long GetLockColor(){ return m_ulLockColor; }
   // returns the color for an unlocked column
   unsigned long GetUnlockColor(){ return m_ulUnLockColor; }
   // returns the mapping for current column
   // setter methods

   // caches the color for a split in read only mode
   void SetLockColor( unsigned long ulColor ){ m_ulLockColor = ulColor; }
   // caches the color for a split in normal mode
   void SetUnlockColor( unsigned long ulColor ) { m_ulUnLockColor = ulColor; }
   // caches Readonly flag
   void SetReadOnly( BOOL bSet );
   // caches the index of the column in the grid
   void SetSplitIndex ( short nIndex );

private:
   short m_nMySplit;
   unsigned long m_ulLockColor;
   unsigned long m_ulUnLockColor;

   ITrueDBGridCtrl * m_pParentGrid; // Pointer to the grid where this column belongs to

#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

};


/////////////////////////////////////////////////////////////////////////////
// Column wrapper class

class ZDCTL_CLASS Column : public COleDispatchDriver
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
   BOOL GetHeaderDivider();
   void SetHeaderDivider(BOOL);
   BOOL GetFooterDivider();
   void SetFooterDivider(BOOL);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long);
   long GetDividerStyle();
   void SetDividerStyle(long);
   CString GetCaption();
   void SetCaption(LPCTSTR);
   CString GetFooterText();
   void SetFooterText(LPCTSTR);
   BOOL GetDataChanged();
   CString GetDataField();
   void SetDataField(LPCTSTR);
   long GetDataWidth();
   void SetDataWidth(long);
   VARIANT GetDefaultValue();
   void SetDefaultValue(const VARIANT&);
   unsigned long GetEditBackColor();
   void SetEditBackColor(unsigned long);
   unsigned long GetEditForeColor();
   void SetEditForeColor(unsigned long);
   unsigned long GetForeColor();
   void SetForeColor(unsigned long);
   long GetHeadAlignment();
   void SetHeadAlignment(long);
   unsigned long GetHeadBackColor();
   void SetHeadBackColor(unsigned long);
   unsigned long GetHeadForeColor();
   void SetHeadForeColor(unsigned long);
   long GetFooterAlignment();
   void SetFooterAlignment(long);
   unsigned long GetFooterBackColor();
   void SetFooterBackColor(unsigned long);
   unsigned long GetFooterForeColor();
   void SetFooterForeColor(unsigned long);
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
   BOOL GetMerge();
   void SetMerge(BOOL);
   BOOL GetFetchStyle();
   void SetFetchStyle(BOOL);
   LPDISPATCH GetStyle();
   void SetStyle(LPDISPATCH);
   LPDISPATCH GetHeadingStyle();
   void SetHeadingStyle(LPDISPATCH);
   LPDISPATCH GetFooterStyle();
   void SetFooterStyle(LPDISPATCH);
   LPDISPATCH GetEditorStyle();
   void SetEditorStyle(LPDISPATCH);
   LPDISPATCH GetFont();
   void SetFont(LPDISPATCH);
   LPDISPATCH GetHeadFont();
   void SetHeadFont(LPDISPATCH);
   LPDISPATCH GetFooterFont();
   void SetFooterFont(LPDISPATCH);
   short GetOrder();
   void SetOrder(short);
   CString GetEditMask();
   void SetEditMask(LPCTSTR);
   BOOL GetEditMaskUpdate();
   void SetEditMaskUpdate(BOOL);
   BOOL GetEditMaskRight();
   void SetEditMaskRight(BOOL);
   VARIANT GetDropDown();
   void SetDropDown(const VARIANT&);
   VARIANT GetExternalEditor();
   void SetExternalEditor(const VARIANT&);
   float GetCellTop();
   VARIANT GetButtonPicture();
   void SetButtonPicture(const VARIANT&);
   BOOL GetAllowFocus();
   void SetAllowFocus(BOOL);
   BOOL GetButtonText();
   void SetButtonText(BOOL);
   BOOL GetButtonAlways();
   void SetButtonAlways(BOOL);
   BOOL GetButtonHeader();
   void SetButtonHeader(BOOL);
   BOOL GetButtonFooter();
   void SetButtonFooter(BOOL);
   BOOL GetOwnerDraw();
   void SetOwnerDraw(BOOL);
   BOOL GetAutoDropDown();
   void SetAutoDropDown(BOOL);
   BOOL GetDropDownList();
   void SetDropDownList(BOOL);
   long GetDisplayAlignment();
   long GetConvertEmptyCell();
   void SetConvertEmptyCell(long);
   CString GetName();
   CString GetTableName();

// Operations
public:
   LPDISPATCH GetValueItems();
   CString CellText(const VARIANT& Bookmark);
   VARIANT CellValue(const VARIANT& Bookmark);
   void AddCellStyle(short Condition, const VARIANT& Style);
   void AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex);
   void ClearCellStyle(short Condition);
   void ClearRegexCellStyle(short Condition, const VARIANT& Regex);
   void Refresh();
   void RefreshCell(const VARIANT& Bookmark);
   void Refetch();
   void RefetchCell(const VARIANT& Bookmark);
   void AutoSize();

public:// additional methods
   // additional method to simplify ValueItems handling
   ValueItems GetListOfValues();
};


/*
  The class ITrueDBGridCtrl is a dispatch wrapper for a grid.

  Basically it is generated form class wizzard

  there are changes made to the ctor & dtor
  and there are additional membervariables and methods that are declared
  at the very to of this class definition

  Any generated code follows the comment block
  "Wrapper generated by class wizzard"
*/
class ZDCTL_CLASS ITrueDBGridCtrl : public COleDispatchDriver
{
public:
   ITrueDBGridCtrl()
   {
      m_strScopeEntityName = "";
      m_strEntityName      = "";
      m_strViewName        = "";
      m_strTempViewName    = "";
      m_bTraceEvents       = FALSE;
      m_bLayoutChanges     = FALSE;
//      m_vGridApp = 0;

      // Grid cursor set by SetCurrentCell
      m_bExternalRowChange = FALSE;
      m_pZActiveX = NULL;
   }      // Calls COleDispatchDriver default constructor

   ITrueDBGridCtrl( ZActiveX* pzma, LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch)
   {
      m_pZActiveX = pzma;
      Init();
      InitEventList();
   }
   ITrueDBGridCtrl(const ITrueDBGridCtrl& dispatchSrc) : COleDispatchDriver(dispatchSrc)
   {
      m_pZActiveX = dispatchSrc.m_pZActiveX;
      Init();
      InitEventList();
   }

   ~ITrueDBGridCtrl();

   //
   // additional functions
   //
   // initialize member variables
   void Init();
   zSHORT InitMapping( );
   void InitEventList();
   void InitMappingAttributeLength( zVIEW vApp );

   // retrieving data
   zSHORT StorageModeMapping();
   // retrieving data new style
   zSHORT StorageModeMappingEx();

   // helper methods for mapping and event - handling
   zSHORT   MapFromOI(  );
   zSHORT   MapToOI(  );

   // used to call an event handler assigned to an event
   zSHORT   HandleEvent(  DISPID dispEvent );
   // tell Zeidon control which events are handled by this DLL
   void     WantHandleEvents( );

   // synchronize view against Grid
   zLONG    SetViewCursorFromGridCursor( COleVariant vCurrentRow );
   // synchronize Grid against View
   void     SetGridCursorFromViewCursor( );

   // Event Handlers
   zLONG    OnAddNew();
   zLONG    OnAfterColUpdate( );
   zLONG    OnAfterInsert();
   zLONG    OnAfterUpdate( );
   zLONG    OnBeforeColUpdate( );
   zLONG    OnBeforeDelete( );
   zLONG    OnBeforeRowColChange( );
   zLONG    OnBeforeInsert();
   zLONG    OnBeforeUpdate();
   zLONG    OnButtonClick( );
   zLONG    OnComboSelect( );
   zLONG    OnColEdit( );
   zLONG    OnError( );
   zLONG    OnHeadClick( );
   zLONG    OnKeyDown( );
   zLONG    OnMouseUp( );
   zLONG    OnScroll( );
   zLONG    OnSelChange( );
   zLONG    OnRowColChange( );
   zLONG    OnPostEvent();

   zSHORT   GetCellData( zPCHAR pchReturnData, zLONG lRowNbr, zLONG lColNbr);
   zSHORT   SetCellData( zCPCHAR pchReturnData, zLONG lRowNbr, zLONG lColNbr);
   zSHORT   GetActiveCell( zPLONG plRowNbr, zPLONG plColNbr);
   zSHORT   SetCurrentCell( zLONG lRowNbr, zLONG lColNbr, zBOOL bFromExternal = FALSE );
   zSHORT   SetCheckBoxTranslation( zSHORT nColumn, zCPCHAR zcpcTRUE, zCPCHAR zcpcFALSE );

   zLONG    SetZCtrlState( zLONG  lStatusType, zULONG ulValue );

   // translates a given event identifier into a human readable expression
   CString GetEventName( long l);

   // returns the Zeidon view to the dialog
   zVIEW GetDialogView();
   // locks or unlocks a column specified by nIndex
   zSHORT LockColumn( zSHORT nIndex, zBOOL bLock );
   // returns the internal column object
   ZGridColumn * GetGridColumn( short lIndex );
   // returns the internal split object
   ZGridSplit  * GetGridSplit( short lIndex );
   zVIEW GetViewNamed( CString strViewName = "" );

   // returns the controls tag
   LPCSTR   GetControlTag();

   // for caching view/entity/scope entity name
   CString m_strScopeEntityName;
   CString m_strEntityName;
   CString m_strViewName;
   CString m_strTempViewName;

   ZActiveX* m_pZActiveX; // Pointer to Zeidon Control Instance

private: // member functions

   void PopulateList( zSHORT nColumn )   ;
   void PopulateListOfValues( zVIEW vApp );
   // helper functions for bookmark handling
   void MakeBookmark( long lAbsolutePosition, LPVARIANT vBookmark );
   long GetPositionFromBookmark( COleVariant v);

   void UpdateAllColumnValues();

   void StoreLayoutChanges();
   void LoadLayoutChanges();
   zSHORT LoadZeidonControlProperties();

   zBOOL IsRowInsertMode( );
   zBOOL IsGridLocked( ){ return m_bLock; }
   void  SetRowInsertMode( zBOOL bSet ){ m_bInsertMode = bSet; }

   void TraceExceptionError( CException *e, CString strExtra );

private: // member variables
   CString GetCheckBoxValueForSetData( CString strText, Column col, zLONG lCol );
   CString GetCheckBoxValueForGetData( CString strText, short bText, Column col, zLONG lCol );

   zBOOL CreateDateTimeCtrl( int nCol = -1);
   void SetDefaultValueItem( zVIEW vApp );
   BOOL m_bAutosortEnabled;

   CObList  m_EventList;            // for debugging
   zBOOL    m_bTraceEvents;         // for debugging

   zBOOL    m_bExternalRowChange;   // Row changed by VML call
   zBOOL    m_bInsertMode;          // We are about to insert a Row

   // members to handle ordering
   zSHORT   m_nCurrentOrderCol;     // index of the last order column
   zSHORT   m_nCurrentOrderMode;    // 0 = ascending other descending
   zBOOL    m_bLayoutChanges;       // to know when Rowheight and colwidth have
                                    // to be made persistent via StoreLayoutChanges
   zSHORT   m_nCurrentCol;          // index of column set by SetCurrentCell

   ZGridDateCtrl     **m_pWndArray;
   ZGridDateCtrl     *m_pWndDateCtrl;

   CArray<ZGridColumn *, ZGridColumn *> m_aGridCols;
   CArray<ZGridSplit  *, ZGridSplit  *> m_aGridSplits;
   ZColumns m_ZeidonColumns;        // to serialize Zeidon Column properties created
                                    // by the painter such as Mapping
   IXArrayDB m_xArrayDB;

   BOOL m_bLock;

   void TraceState( CString strExtra );

   /**************************************************************************************/
   /*                                                                                    */
   /*               Wrapper generated by class wizzard                                   */
   /*                                                                                    */
   /**************************************************************************************/
// Attributes
public:
   BOOL GetAllowDelete();
   void SetAllowDelete(BOOL);
   BOOL GetAllowAddNew();
   void SetAllowAddNew(BOOL);
   BOOL GetAllowRowSizing();
   void SetAllowRowSizing(BOOL);
   BOOL GetAllowUpdate();
   void SetAllowUpdate(BOOL);
   short GetAppearance();
   void SetAppearance(short);
   short GetBorderStyle();
   void SetBorderStyle(short);
   VARIANT GetBookmark();
   void SetBookmark(const VARIANT&);
   short GetBookmarkType();
   void SetBookmarkType(short);
   short GetCol();
   void SetCol(short);
   BOOL GetColumnHeaders();
   void SetColumnHeaders(BOOL);
   BOOL GetColumnFooters();
   void SetColumnFooters(BOOL);
   BOOL GetCurrentCellVisible();
   void SetCurrentCellVisible(BOOL);
   BOOL GetCurrentCellModified();
   void SetCurrentCellModified(BOOL);
   BOOL GetDataChanged();
   void SetDataChanged(BOOL);
   long GetDataMode();
   void SetDataMode(long);
   LPUNKNOWN GetDataSource();
   void SetDataSource(LPUNKNOWN);
   float GetDefColWidth();
   void SetDefColWidth(float);
   BOOL GetEditActive();
   void SetEditActive(BOOL);
   BOOL GetEditDropDown();
   void SetEditDropDown(BOOL);
   BOOL GetEnabled();
   void SetEnabled(BOOL);
   BOOL GetExtendRightColumn();
   void SetExtendRightColumn(BOOL);
   BOOL GetPartialRightColumn();
   void SetPartialRightColumn(BOOL);
   BOOL GetAnchorRightColumn();
   void SetAnchorRightColumn(BOOL);
   VARIANT GetFirstRow();
   void SetFirstRow(const VARIANT&);
   LPDISPATCH GetHeadFont();
   void SetHeadFont(LPDISPATCH);
   LPDISPATCH GetFooterFont();
   void SetFooterFont(LPDISPATCH);
   float GetHeadLines();
   void SetHeadLines(float);
   float GetFootLines();
   void SetFootLines(float);
   OLE_HANDLE GetHWnd();
   long GetHWndEditor();
   BOOL GetMarqueeUnique();
   void SetMarqueeUnique(BOOL);
   long GetMarqueeStyle();
   void SetMarqueeStyle(long);
   BOOL GetRecordSelectors();
   void SetRecordSelectors(BOOL);
   short GetRow();
   void SetRow(short);
   long GetRowDividerStyle();
   void SetRowDividerStyle(long);
   float GetRowHeight();
   void SetRowHeight(float);
   long GetScrollBars();
   void SetScrollBars(long);
   long GetSelLength();
   void SetSelLength(long);
   long GetSelStart();
   void SetSelStart(long);
   CString GetSelText();
   void SetSelText(LPCTSTR);
   short GetSelStartCol();
   void SetSelStartCol(short);
   short GetSelEndCol();
   void SetSelEndCol(short);
   short GetSplit();
   void SetSplit(short);
   short GetVisibleCols();
   short GetVisibleRows();
   OLE_COLOR GetBackColor();
   void SetBackColor(OLE_COLOR);
   OLE_COLOR GetForeColor();
   void SetForeColor(OLE_COLOR);
   CString GetCaption();
   void SetCaption(LPCTSTR);
   LPFONTDISP GetFont();
   void SetFont(LPFONTDISP);
   short GetLeftCol();
   void SetLeftCol(short);
   BOOL GetAllowArrows();
   void SetAllowArrows(BOOL);
   long GetExposeCellMode();
   void SetExposeCellMode(long);
   BOOL GetTabAcrossSplits();
   void SetTabAcrossSplits(BOOL);
   long GetTabAction();
   void SetTabAction(long);
   BOOL GetWrapCellPointer();
   void SetWrapCellPointer(BOOL);
   LPDISPATCH GetStyle();
   void SetStyle(LPDISPATCH);
   LPDISPATCH GetHeadingStyle();
   void SetHeadingStyle(LPDISPATCH);
   LPDISPATCH GetFooterStyle();
   void SetFooterStyle(LPDISPATCH);
   LPDISPATCH GetEditorStyle();
   void SetEditorStyle(LPDISPATCH);
   LPDISPATCH GetInactiveStyle();
   void SetInactiveStyle(LPDISPATCH);
   LPDISPATCH GetSelectedStyle();
   void SetSelectedStyle(LPDISPATCH);
   LPDISPATCH GetCaptionStyle();
   void SetCaptionStyle(LPDISPATCH);
   LPDISPATCH GetHighlightRowStyle();
   void SetHighlightRowStyle(LPDISPATCH);
   LPDISPATCH GetEvenRowStyle();
   void SetEvenRowStyle(LPDISPATCH);
   LPDISPATCH GetOddRowStyle();
   void SetOddRowStyle(LPDISPATCH);
   unsigned long GetHeadBackColor();
   void SetHeadBackColor(unsigned long);
   unsigned long GetHeadForeColor();
   void SetHeadForeColor(unsigned long);
   unsigned long GetFooterBackColor();
   void SetFooterBackColor(unsigned long);
   unsigned long GetFooterForeColor();
   void SetFooterForeColor(unsigned long);
   unsigned long GetEditBackColor();
   void SetEditBackColor(unsigned long);
   unsigned long GetEditForeColor();
   void SetEditForeColor(unsigned long);
   unsigned long GetInactiveBackColor();
   void SetInactiveBackColor(unsigned long);
   unsigned long GetInactiveForeColor();
   void SetInactiveForeColor(unsigned long);
   unsigned long GetSelectedBackColor();
   void SetSelectedBackColor(unsigned long);
   unsigned long GetSelectedForeColor();
   void SetSelectedForeColor(unsigned long);
   long GetApproxCount();
   void SetApproxCount(long);
   CString GetErrorText();
   long GetAddNewMode();
   BOOL GetAllowColSelect();
   void SetAllowColSelect(BOOL);
   BOOL GetAllowColMove();
   void SetAllowColMove(BOOL);
   BOOL GetAllowRowSelect();
   void SetAllowRowSelect(BOOL);
   VARIANT GetPictureCurrentRow();
   void SetPictureCurrentRow(const VARIANT&);
   VARIANT GetPictureModifiedRow();
   void SetPictureModifiedRow(const VARIANT&);
   VARIANT GetPictureAddnewRow();
   void SetPictureAddnewRow(const VARIANT&);
   VARIANT GetPictureStandardRow();
   void SetPictureStandardRow(const VARIANT&);
   VARIANT GetPictureHeaderRow();
   void SetPictureHeaderRow(const VARIANT&);
   VARIANT GetPictureFooterRow();
   void SetPictureFooterRow(const VARIANT&);
   VARIANT GetMouseIcon();
   void SetMouseIcon(const VARIANT&);
   long GetMousePointer();
   void SetMousePointer(long);
   LPUNKNOWN GetISVServices();
   CString GetText();
   void SetText(LPCTSTR);
   CString GetLayoutName();
   void SetLayoutName(LPCTSTR);
   CString GetLayoutFileName();
   void SetLayoutFileName(LPCTSTR);
   CString GetLayoutURL();
   void SetLayoutURL(LPCTSTR);
   long GetMultipleLines();
   void SetMultipleLines(long);
   LPDISPATCH GetArray();
   void SetArray(LPDISPATCH);
   BOOL GetEmptyRows();
   void SetEmptyRows(BOOL);
   long GetCellTips();
   void SetCellTips(long);
   float GetVScrollWidth();
   float GetHScrollHeight();
   BOOL GetBof();
   BOOL GetEof();
   BOOL GetFetchRowStyle();
   void SetFetchRowStyle(BOOL);
   BOOL GetAlternatingRowStyle();
   void SetAlternatingRowStyle(BOOL);
   float GetCellTipsWidth();
   void SetCellTipsWidth(float);
   long GetCellTipsDelay();
   void SetCellTipsDelay(long);
   BOOL GetInsertMode();
   void SetInsertMode(BOOL);
   BOOL GetTransparentRowPictures();
   void SetTransparentRowPictures(BOOL);
   long GetMultiSelect();
   void SetMultiSelect(long);
   long GetOLEDragMode();
   void SetOLEDragMode(long);
   long GetOLEDropMode();
   void SetOLEDropMode(long);
   BOOL GetExportEOF();
   void SetExportEOF(BOOL);
   VARIANT GetExportNextBookmark();
   void SetExportNextBookmark(const VARIANT&);
   long GetAnimateWindow();
   void SetAnimateWindow(long);
   long GetAnimateWindowDirection();
   void SetAnimateWindowDirection(long);
   long GetAnimateWindowTime();
   void SetAnimateWindowTime(long);
   long GetAnimateWindowClose();
   void SetAnimateWindowClose(long);
   unsigned long GetDeadAreaBackColor();
   void SetDeadAreaBackColor(unsigned long);

// Operations
public:
   zSHORT SetDateCtrlForColumn( zLONG nColumn, zBOOL bSet );
   zSHORT LockGrid( zLONG nSplit, BOOL bLock );
   void OnLooseFocus();
   BOOL EnableAutosort( BOOL bEnable );

   void AboutBox();
   LPDISPATCH GetColumns();
   LPDISPATCH GetSplits();
   LPDISPATCH GetStyles();
   LPDISPATCH GetSelBookmarks();
   short ColContaining(float X);
   VARIANT GetBookmark(long RowNum);
   void ReBind();
   VARIANT RowBookmark(short RowNum);
   short RowContaining(float Y);
   float RowTop(short RowNum);
   void Scroll(long Cols, long Rows);
   void Refresh();
   LPDISPATCH CaptureImage();
   void AddCellStyle(short Condition, const VARIANT& Style);
   void AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex);
   void ClearCellStyle(short Condition);
   void ClearRegexCellStyle(short Condition, const VARIANT& Regex);
   void ClearSelCols();
   void ClearFields();
   void HoldFields();
   short SplitContaining(float X, float Y);
   long PointAt(float X, float Y);
   void PostMsg(short MsgId);
   void Update();
   void MoveRelative(long offset, const VARIANT& Bookmark);
   void MoveFirst();
   void MoveLast();
   void MoveNext();
   void MovePrevious();
   void Delete();
   void Close(const VARIANT& Repaint);
   void ReOpen(const VARIANT& Bookmark);
   void LoadLayout();
   LPDISPATCH GetLayouts();
   BOOL CellContaining(float X, float Y, long* RowAt, short* ColAt);
   void RefreshRow(const VARIANT& Bookmark);
   void RefreshCol(const VARIANT& Col);
   void RefetchRow(const VARIANT& Bookmark);
   void RefetchCol(const VARIANT& Col);
   void OLEDrag();
   short IsSelected(const VARIANT& Bookmark);
   void ExportToFile(LPCTSTR outPath, BOOL append, const VARIANT& selector, const VARIANT& tableWidth);
   void ExportBegin(const VARIANT& Bookmark);
   void ExportRows(LPCTSTR outPath, BOOL append, long Rows, const VARIANT& tableWidth);
   void ExportEnd();
   void ExportToDelimitedFile(LPCTSTR outPath, const VARIANT& selector, const VARIANT& delim, const VARIANT& prefix, const VARIANT& suffix);
   LPDISPATCH GetPrintInfos();
   LPDISPATCH GetPrintInfo();

};
/////////////////////////////////////////////////////////////////////////////
// Columns wrapper class

class ZDCTL_CLASS Columns : public COleDispatchDriver
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
   LPDISPATCH Add(short Index);
   void Remove(const VARIANT& Index);

public:// additional methods
   // additional method to simplify retrieving a single column
   Column GetColumn( short nIndex );

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
   void Remove( zLONG Index );
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

// Operations
public:
   long GetRowCount();
   void SetRowCount(long nNewValue);
   short GetColumnCount();
   VARIANT GetBookmark(long Row);
   void SetBookmark(long Row, const VARIANT& newValue);
   CString GetColumnName(short Col);
   VARIANT GetValue(long Row, short Col);
   void SetValue(long Row, short Col, const VARIANT& newValue);
   short GetColumnIndex(long Row, short Col);
};
/////////////////////////////////////////////////////////////////////////////
// DataObjectFiles wrapper class

class DataObjectFiles : public COleDispatchDriver
{
public:
   DataObjectFiles() {}      // Calls COleDispatchDriver default constructor
   DataObjectFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataObjectFiles(const DataObjectFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   long GetCount();
   LPUNKNOWN Get_NewEnum();
   CString GetItem(long Index);
   void Add(LPCTSTR Filename);
   void Remove(const VARIANT& Index);
   void Clear();
};
/////////////////////////////////////////////////////////////////////////////
// DataObject wrapper class

class DataObject : public COleDispatchDriver
{
public:
   DataObject() {}      // Calls COleDispatchDriver default constructor
   DataObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataObject(const DataObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void Clear();
   LPDISPATCH GetFiles();
   VARIANT GetData(short Format);
   BOOL GetFormat(short Format);
   void SetData(const VARIANT& Value, const VARIANT& Format);
};
/////////////////////////////////////////////////////////////////////////////
// Layouts wrapper class

class Layouts : public COleDispatchDriver
{
public:
   Layouts() {}      // Calls COleDispatchDriver default constructor
   Layouts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Layouts(const Layouts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();

// Operations
public:
   LPUNKNOWN Get_NewEnum();
   VARIANT GetItem(const VARIANT& Index);
   void Add(const VARIANT& LayoutName);
   void Remove(const VARIANT& Index);
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
   LPDISPATCH GetItem( zLONG lIndex );
   Split GetSplit( short Index );
   LPDISPATCH Add(short Index);
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
   unsigned long GetBackColor();
   void SetBackColor(unsigned long);
   unsigned long GetForeColor();
   void SetForeColor(unsigned long);
   unsigned long GetHeadBackColor();
   void SetHeadBackColor(unsigned long);
   unsigned long GetHeadForeColor();
   void SetHeadForeColor(unsigned long);
   unsigned long GetFooterBackColor();
   void SetFooterBackColor(unsigned long);
   unsigned long GetFooterForeColor();
   void SetFooterForeColor(unsigned long);
   unsigned long GetEditBackColor();
   void SetEditBackColor(unsigned long);
   unsigned long GetEditForeColor();
   void SetEditForeColor(unsigned long);
   BOOL GetExtendRightColumn();
   void SetExtendRightColumn(BOOL);
   BOOL GetPartialRightColumn();
   void SetPartialRightColumn(BOOL);
   BOOL GetAnchorRightColumn();
   void SetAnchorRightColumn(BOOL);
   unsigned long GetInactiveBackColor();
   void SetInactiveBackColor(unsigned long);
   unsigned long GetInactiveForeColor();
   void SetInactiveForeColor(unsigned long);
   unsigned long GetSelectedBackColor();
   void SetSelectedBackColor(unsigned long);
   unsigned long GetSelectedForeColor();
   void SetSelectedForeColor(unsigned long);
   LPDISPATCH GetFont();
   void SetFont(LPDISPATCH);
   LPDISPATCH GetHeadFont();
   void SetHeadFont(LPDISPATCH);
   LPDISPATCH GetFooterFont();
   void SetFooterFont(LPDISPATCH);
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
   LPDISPATCH GetStyle();
   void SetStyle(LPDISPATCH);
   BOOL GetCurrentCellVisible();
   void SetCurrentCellVisible(BOOL);
   VARIANT GetFirstRow();
   void SetFirstRow(const VARIANT&);
   short GetLeftCol();
   void SetLeftCol(short);
   LPDISPATCH GetHeadingStyle();
   void SetHeadingStyle(LPDISPATCH);
   LPDISPATCH GetFooterStyle();
   void SetFooterStyle(LPDISPATCH);
   LPDISPATCH GetEditorStyle();
   void SetEditorStyle(LPDISPATCH);
   LPDISPATCH GetInactiveStyle();
   void SetInactiveStyle(LPDISPATCH);
   LPDISPATCH GetSelectedStyle();
   void SetSelectedStyle(LPDISPATCH);
   LPDISPATCH GetHighlightRowStyle();
   void SetHighlightRowStyle(LPDISPATCH);
   LPDISPATCH GetEvenRowStyle();
   void SetEvenRowStyle(LPDISPATCH);
   LPDISPATCH GetOddRowStyle();
   void SetOddRowStyle(LPDISPATCH);
   LPDISPATCH GetCaptionStyle();
   void SetCaptionStyle(LPDISPATCH);
   short GetSelStartCol();
   void SetSelStartCol(short);
   short GetSelEndCol();
   void SetSelEndCol(short);
   BOOL GetAllowColSelect();
   void SetAllowColSelect(BOOL);
   BOOL GetAllowColMove();
   void SetAllowColMove(BOOL);
   BOOL GetAllowRowSelect();
   void SetAllowRowSelect(BOOL);
   float GetVScrollWidth();
   float GetHScrollHeight();
   BOOL GetFetchRowStyle();
   void SetFetchRowStyle(BOOL);
   BOOL GetAlternatingRowStyle();
   void SetAlternatingRowStyle(BOOL);
   CString GetCaption();
   void SetCaption(LPCTSTR);
   long GetDividerStyle();
   void SetDividerStyle(long);

// Operations
public:
   LPDISPATCH GetColumns();
   void AddCellStyle(short Condition, const VARIANT& Style);
   void AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex);
   void ClearCellStyle(short Condition);
   void ClearRegexCellStyle(short Condition, const VARIANT& Regex);
   void ClearSelCols();
};

/////////////////////////////////////////////////////////////////////////////
// Styles wrapper class

class Styles : public COleDispatchDriver
{
public:
   Styles() {}      // Calls COleDispatchDriver default constructor
   Styles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Styles(const Styles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();

// Operations
public:
   LPUNKNOWN Get_NewEnum();
   LPDISPATCH GetItem(const VARIANT& Index);
   LPDISPATCH Add(LPCTSTR Name);
   void Remove(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// StyleDisp wrapper class

class StyleDisp : public COleDispatchDriver
{
public:
   StyleDisp() {}      // Calls COleDispatchDriver default constructor
   StyleDisp(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   StyleDisp(const StyleDisp& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   unsigned long GetForeColor();
   void SetForeColor(unsigned long);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long);
   LPDISPATCH GetFont();
   void SetFont(LPDISPATCH);
   long GetAlignment();
   void SetAlignment(long);
   long GetVerticalAlignment();
   void SetVerticalAlignment(long);
   BOOL GetLocked();
   void SetLocked(BOOL);
   CString GetName();
   VARIANT GetParent();
   void SetParent(const VARIANT&);
   VARIANT GetValue();
   void SetValue(const VARIANT&);
   BOOL GetWrapText();
   void SetWrapText(BOOL);
   BOOL GetTransparentForegroundPicture();
   void SetTransparentForegroundPicture(BOOL);
   VARIANT GetForegroundPicture();
   void SetForegroundPicture(const VARIANT&);
   VARIANT GetBackgroundPicture();
   void SetBackgroundPicture(const VARIANT&);
   long GetForegroundPicturePosition();
   void SetForegroundPicturePosition(long);
   long GetBackgroundPictureDrawMode();
   void SetBackgroundPictureDrawMode(long);

// Operations
public:
   void Reset();
};
/////////////////////////////////////////////////////////////////////////////
// PrintInfo wrapper class

class PrintInfo : public COleDispatchDriver
{
public:
   PrintInfo() {}      // Calls COleDispatchDriver default constructor
   PrintInfo(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   PrintInfo(const PrintInfo& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   CString GetName();
   void SetName(LPCTSTR);
   CString GetPageHeader();
   void SetPageHeader(LPCTSTR);
   CString GetPageFooter();
   void SetPageFooter(LPCTSTR);
   LPDISPATCH GetPageHeaderFont();
   void SetPageHeaderFont(LPDISPATCH);
   LPDISPATCH GetPageFooterFont();
   void SetPageFooterFont(LPDISPATCH);
   BOOL GetDraft();
   void SetDraft(BOOL);
   BOOL GetDefault();
   void SetDefault(BOOL);
   BOOL GetRepeatGridHeader();
   void SetRepeatGridHeader(BOOL);
   BOOL GetRepeatSplitHeaders();
   void SetRepeatSplitHeaders(BOOL);
   BOOL GetRepeatColumnHeaders();
   void SetRepeatColumnHeaders(BOOL);
   BOOL GetRepeatColumnFooters();
   void SetRepeatColumnFooters(BOOL);
   BOOL GetVariableRowHeight();
   void SetVariableRowHeight(BOOL);
   short GetNumberOfCopies();
   void SetNumberOfCopies(short);
   BOOL GetCollate();
   void SetCollate(BOOL);
   CString GetPreviewCaption();
   void SetPreviewCaption(LPCTSTR);
   CString GetPreviewPageOf();
   void SetPreviewPageOf(LPCTSTR);
   CString GetRangeOfPages();
   void SetRangeOfPages(LPCTSTR);
   VARIANT GetSettings();
   void SetSettings(const VARIANT&);
   CString GetSettingsDeviceName();
   void SetSettingsDeviceName(LPCTSTR);
   short GetSettingsOrientation();
   void SetSettingsOrientation(short);
   long GetSettingsMarginLeft();
   void SetSettingsMarginLeft(long);
   long GetSettingsMarginTop();
   void SetSettingsMarginTop(long);
   long GetSettingsMarginRight();
   void SetSettingsMarginRight(long);
   long GetSettingsMarginBottom();
   void SetSettingsMarginBottom(long);
   short GetSettingsPaperSize();
   void SetSettingsPaperSize(short);
   long GetSettingsPaperWidth();
   void SetSettingsPaperWidth(long);
   long GetSettingsPaperHeight();
   void SetSettingsPaperHeight(long);
   short GetSettingsPaperBin();
   void SetSettingsPaperBin(short);

// Operations
public:
   void PrintData(const VARIANT& selector);
   void PrintPreview(const VARIANT& selector);
   void PageSetup();
   void SetMenuText(long Index, LPCTSTR Text);
   CString GetMenuText(long Index);
   void SubstituteFont(LPCTSTR UnavailableFont, const VARIANT& AvailableFont);
   CString GetSubstituteFont(LPCTSTR UnavailableFont);
};
/////////////////////////////////////////////////////////////////////////////
// PrintInfos wrapper class

class PrintInfos : public COleDispatchDriver
{
public:
   PrintInfos() {}      // Calls COleDispatchDriver default constructor
   PrintInfos(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   PrintInfos(const PrintInfos& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetCount();

// Operations
public:
   LPUNKNOWN Get_NewEnum();
   LPDISPATCH GetItem(const VARIANT& Index);
   LPDISPATCH Add(LPCTSTR Name);
   void Remove(const VARIANT& Index);
   void Clear();
};


/////////////////////////////////////////////////////////////////////////////
// ITrueDBDropDownCtrl wrapper class
/*
  The class ITrueDBDropDownCtrl is a dispatch wrapper for a DropDown Control.

  Basically it is generated form class wizzard

  there are changes made to the ctor & dtor
  and there are additional membervariables and methods that are declared
  at the very to of this class definition

  Any generated code follows the comment block
  "Wrapper generated by class wizzard"

  many parts of the implemetation is similar to the ITrueDBGridCtrl implementation
*/

#ifdef TB_DROPDOWN_IMPLEMENT
class ITrueDBDropDownCtrl : public COleDispatchDriver
{
public:
   ITrueDBDropDownCtrl() {
      m_strScopeEntityName = "";
      m_strEntityName      = "";
      m_strViewName        = "";
      m_strTempViewName    = "";
      m_bTraceEvents       = FALSE;
      m_bLayoutChanges     = FALSE;
      m_vGridApp = 0;

      // Grid cursor set by SetCurrentCell
      m_bExternalRowChange = FALSE;
      m_pZActiveX = NULL;
   }      // Calls COleDispatchDriver default constructor
   ITrueDBDropDownCtrl(ZActiveX* pzma, LPDISPATCH pDispatch)
      : COleDispatchDriver(pDispatch)
   {
      m_pZActiveX = pzma;
      Init();
      InitEventList();
   }

   ITrueDBDropDownCtrl(const ITrueDBDropDownCtrl& dispatchSrc) : COleDispatchDriver(dispatchSrc)
   {
      m_pZActiveX = dispatchSrc.m_pZActiveX;
      Init();
      InitEventList();
   }
   ~ITrueDBDropDownCtrl();

   // my functions
   // initialize member variables
   void Init();
   zSHORT InitMapping( );
   void InitEventList();
   void StoreLayoutChanges();
   void LoadLayoutChanges();

   zSHORT StorageModeMapping();

   // helper methods for mapping and event - handling
   zSHORT   MapFromOI(  );
   zSHORT   MapToOI(  );
   zSHORT   HandleEvent(  DISPID dispEvent );
   // tell Zeidon control which events are handled by this DLL
   void     WantHandleEvents( );

   zLONG    OnRowChange( );
   zLONG    OnSelChange( );

   // synchronize view against Grid
   zLONG    SetViewCursorFromGridCursor( COleVariant vCurrentRow );
   // synchronize Grid against View
   void     SetGridCursorFromViewCursor( );

   void MakeBookmark( long lAbsolutePosition, LPVARIANT vBookmark );
   long GetPositionFromBookmark( COleVariant v);

   CString GetEventName( long l);
   zVIEW GetDialogView();

   // for caching view/entity/scope entity name
   CString m_strScopeEntityName;
   CString m_strEntityName;
   CString m_strViewName;
   CString m_strTempViewName;

   ZActiveX* m_pZActiveX; // Pointer to Zeidon Control Instance
   zVIEW    m_vGridApp;
   zBOOL    m_bTraceEvents;         // for debugging
   CObList  m_EventList;            // for debugging
   zBOOL    m_bExternalRowChange;
   zBOOL    m_bLayoutChanges;

   /**************************************************************************************/
   /*                                                                                    */
   /*               Wrapper generated by class wizzard                                   */
   /*                                                                                    */
   /**************************************************************************************/
// Attributes
public:
   BOOL GetAllowRowSizing();
   void SetAllowRowSizing(BOOL);
   short GetAppearance();
   void SetAppearance(short);
   short GetBorderStyle();
   void SetBorderStyle(short);
   VARIANT GetBookmark();
   void SetBookmark(const VARIANT&);
   short GetBookmarkType();
   void SetBookmarkType(short);
   short GetCol();
   void SetCol(short);
   BOOL GetColumnHeaders();
   void SetColumnHeaders(BOOL);
   BOOL GetCurrentCellVisible();
   void SetCurrentCellVisible(BOOL);
   long GetDataMode();
   void SetDataMode(long);
   LPUNKNOWN GetDataSource();
   void SetDataSource(LPUNKNOWN);
   float GetDefColWidth();
   void SetDefColWidth(float);
   BOOL GetEnabled();
   void SetEnabled(BOOL);
   BOOL GetExtendRightColumn();
   void SetExtendRightColumn(BOOL);
   VARIANT GetFirstRow();
   void SetFirstRow(const VARIANT&);
   LPDISPATCH GetHeadFont();
   void SetHeadFont(LPDISPATCH);
   float GetHeadLines();
   void SetHeadLines(float);
   OLE_HANDLE GetHWnd();
   short GetRow();
   void SetRow(short);
   long GetRowDividerStyle();
   void SetRowDividerStyle(long);
   float GetRowHeight();
   void SetRowHeight(float);
   long GetScrollBars();
   void SetScrollBars(long);
   short GetSelStartCol();
   void SetSelStartCol(short);
   short GetSelEndCol();
   void SetSelEndCol(short);
   short GetVisibleCols();
   short GetVisibleRows();
   OLE_COLOR GetBackColor();
   void SetBackColor(OLE_COLOR);
   OLE_COLOR GetForeColor();
   void SetForeColor(OLE_COLOR);
   LPFONTDISP GetFont();
   void SetFont(LPFONTDISP);
   short GetLeftCol();
   void SetLeftCol(short);
   LPDISPATCH GetStyle();
   void SetStyle(LPDISPATCH);
   LPDISPATCH GetHeadingStyle();
   void SetHeadingStyle(LPDISPATCH);
   LPDISPATCH GetHighlightRowStyle();
   void SetHighlightRowStyle(LPDISPATCH);
   LPDISPATCH GetEvenRowStyle();
   void SetEvenRowStyle(LPDISPATCH);
   LPDISPATCH GetOddRowStyle();
   void SetOddRowStyle(LPDISPATCH);
   unsigned long GetHeadBackColor();
   void SetHeadBackColor(unsigned long);
   unsigned long GetHeadForeColor();
   void SetHeadForeColor(unsigned long);
   long GetApproxCount();
   void SetApproxCount(long);
   CString GetErrorText();
   BOOL GetAllowColSelect();
   void SetAllowColSelect(BOOL);
   BOOL GetAllowColMove();
   void SetAllowColMove(BOOL);
   LPUNKNOWN GetISVServices();
   CString GetText();
   CString GetLayoutName();
   void SetLayoutName(LPCTSTR);
   CString GetLayoutFileName();
   void SetLayoutFileName(LPCTSTR);
   CString GetLayoutURL();
   void SetLayoutURL(LPCTSTR);
   LPDISPATCH GetArray();
   void SetArray(LPDISPATCH);
   BOOL GetEmptyRows();
   void SetEmptyRows(BOOL);
   CString GetListField();
   void SetListField(LPCTSTR);
   CString GetDataField();
   void SetDataField(LPCTSTR);
   BOOL GetIntegralHeight();
   void SetIntegralHeight(BOOL);
   VARIANT GetSelectedItem();
   void SetSelectedItem(const VARIANT&);
   BOOL GetFetchRowStyle();
   void SetFetchRowStyle(BOOL);
   BOOL GetAlternatingRowStyle();
   void SetAlternatingRowStyle(BOOL);
   BOOL GetPartialRightColumn();
   void SetPartialRightColumn(BOOL);
   BOOL GetAnchorRightColumn();
   void SetAnchorRightColumn(BOOL);
   LPDISPATCH GetFooterStyle();
   void SetFooterStyle(LPDISPATCH);
   BOOL GetColumnFooters();
   void SetColumnFooters(BOOL);
   float GetFootLines();
   void SetFootLines(float);
   LPDISPATCH GetFooterFont();
   void SetFooterFont(LPDISPATCH);
   unsigned long GetFooterBackColor();
   void SetFooterBackColor(unsigned long);
   unsigned long GetFooterForeColor();
   void SetFooterForeColor(unsigned long);

// Operations
public:
   void AboutBox();
   LPDISPATCH GetColumns();
   LPDISPATCH GetStyles();
   short ColContaining(float X);
   VARIANT GetBookmark(long RowNum);
   void ReBind();
   VARIANT RowBookmark(short RowNum);
   short RowContaining(float Y);
   float RowTop(short RowNum);
   void Scroll(long Cols, long Rows);
   void Refresh();
   LPDISPATCH CaptureImage();
   void AddCellStyle(short Condition, const VARIANT& Style);
   void AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex);
   void ClearCellStyle(short Condition);
   void ClearRegexCellStyle(short Condition, const VARIANT& Regex);
   void ClearSelCols();
   void ClearFields();
   void HoldFields();
   void PostMsg(short MsgId);
   void LoadLayout();
   LPDISPATCH GetLayouts();


};

#endif

/////////////////////////////////////////////////////////////////////////////
// COleFont wrapper class

class COleFont : public COleDispatchDriver
{
public:
   COleFont() {}     // Calls COleDispatchDriver default constructor
   COleFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   COleFont(const COleFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   CString GetName();
   void SetName(LPCTSTR);
   CY GetSize();
   void SetSize(const CY&);
   BOOL GetBold();
   void SetBold(BOOL);
   BOOL GetItalic();
   void SetItalic(BOOL);
   BOOL GetUnderline();
   void SetUnderline(BOOL);
   BOOL GetStrikethrough();
   void SetStrikethrough(BOOL);
   short GetWeight();
   void SetWeight(short);
   short GetCharset();
   void SetCharset(short);

// Operations
public:
};


class CPicture : public COleDispatchDriver
{
public:
   CPicture() {}     // Calls COleDispatchDriver default constructor
   CPicture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   CPicture(const CPicture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
   long GetHandle();
   long GetHPal();
   void SetHPal(long);
   short GetType();
   long GetWidth();
   long GetHeight();

// Operations
public:
   // method 'Render' not emitted because of invalid return type or parameter type
};


class ZGridDateCtrl : public CDateTimeCtrl
{
   friend class ITrueDBGridCtrl;
// Construction
public:
   ZGridDateCtrl();

// Attributes
protected:
   ITrueDBGridCtrl *m_pParentGrid;
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZGridDateCtrl)
   afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDatetimeChange(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDropdown(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnFormatDatetime(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnFormatquery(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnUserString(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnWMKeydown(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZGridDateCtrl();

   // Generated message map functions
protected:
   //{{AFX_MSG(ZGridDateCtrl)
      // NOTE - the ClassWizard will add and remove member functions here.
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};


/******************************************************************************************
**
**  C-Style exported functions to access the underlying C++ wrapper via global operations
**
*******************************************************************************************/
zSHORT OPERATION tgGetDomainType( zVIEW v, zCPCHAR szEntity, zCPCHAR szAttribute );

#ifdef __cplusplus
extern "C"
{
#endif
zOPER_EXPORT zSHORT OPERATION
ITrueDBGridCtrl_SetDataMode( zVIEW vSubtask, zCPCHAR cpcCtrlTag, zLONG nDataMode );

#ifdef __cplusplus
}
#endif


#endif //_TRUEDBGRID_H_

