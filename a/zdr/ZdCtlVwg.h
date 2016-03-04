//////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
/********************************************************************************************/
//
// File:            zdctlvwg.h
// *******************************************************************
// *  Copyright © 2001 - 2016 QuinSoft, Inc. and TONBELLER AG.       *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           20. April 2001
// describtion:
//
// list of classes :
// Name:                               describtion:
// ------------------------------------------------------------------------------------
// ZWebGridCell
// ZWebGridCol
// ZWebGridColEdit
// ZWebGridColCheckBox
// ZWebGridColComboBox
// ZWebGridColRadioGroup
// ZWebGridColRadioButton
// ZWebGridRow
// ZWebGrid
// ZWebGridColMapping
//
/* Change log most recent first order:

   2001.4.20   TMV   2000   ActiveX
      create


*/
#ifndef __zdctlvwg_h__  // Sentry, use file only if it's not already included
#define __zdctlvwg_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// those events that are handled with truegrid in windows as well as in web

#define DISPID_AfterColUpdate             0x1   // helpstring("Occurs after data moves from cell to the grid buffer"), helpcontext(0x0005f452)]
#define DISPID_AfterDelete                0x2   // helpstring("Occurs after record deletion from grid"), helpcontext(0x0005f453)]
#define DISPID_AfterInsert                0x3   // helpstring("Occurs after record insertion in grid"), helpcontext(0x0005f454)]
#define DISPID_AfterUpdate                0x4   // helpstring("Occurs after record changes are written to the data source"), helpcontext(0x0005f455)]
#define DISPID_BeforeColUpdate            0x5   // helpstring("Occurs before data moves from cell to the grid buffer"), helpcontext(0x0005f456)]
#define DISPID_BeforeDelete               0x6   // helpstring("Occurs before record deletion from grid"), helpcontext(0x0005f457)]
#define DISPID_BeforeInsert               0x7   // helpstring("Occurs before record insertion in grid"), helpcontext(0x0005f458)]
#define DISPID_BeforeUpdate               0x8   // helpstring("Occurs before record changes are written to the data source"), helpcontext(0x0005f459)]
#define DISPID_HeadClick                  0xa   // helpstring("Occurs when a column header is clicked"), helpcontext(0x0005f45b)]
#define DISPID_RowColChange               0xb   // helpstring("Occurs after a different cell becomes current in the grid"), helpcontext(0x0005f45c)]
#define DISPID_SelChange                  0xf   // helpstring("Occurs whenever a row or column is selected or deselected in the grid"), helpcontext(0x0005f45f)]
#define DISPID_ColEdit                    0x1b   // helpstring("Fired when an edit is initiated for a column"), helpcontext(0x0005f476)]
#define DISPID_OnAddNew                   0x25   // helpstring("Fired when a user action has invoked an AddNew operation"), helpcontext(0x0005f47f)]
#define DISPID_BeforeRowColChange         0x3f   // helpstring("Occurs before a different cell becomes current"), helpcontext(0x0005f62d)]
#define DISPID_RowColChange               0xb   // helpstring("Occurs after a different cell becomes current in the grid"), helpcontext(0x0005f45c)]




/////////////////////////////////////////////////////////////////////////////

// forward declarations for all classes implemented in this file
class ZWebGridCell;
class ZWebGridCol;
class ZWebGridColEdit;
class ZWebGridColCheckBox;
class ZWebGridColComboBox;
class ZWebGridColRadioGroup;
class ZWebGridColRadioButton;
class ZWebGridRow;
class ZDCTL_CLASS ZWebGrid;
class ZWebGridColMapping;
class ZVirtualWebColumns;
class ZValueItem;
class ZWebGridEvent;
class ZWebGridEventChain;



typedef ZWebGridCell            * LPGRIDCELL;
typedef ZWebGridCol             * LPGRIDCOL;
typedef ZWebGridColEdit         * LPGRIDCOLEDIT;
typedef ZWebGridColCheckBox     * LPGRIDCOLCHECKBOX;
typedef ZWebGridColComboBox     * LPGRIDCOLCOMBOBOX;
typedef ZWebGridColRadioGroup   * LPGRIDCOlRADIOGROUP;
typedef ZWebGridColRadioButton  * LPGRIDCOLRADIOBUTTON;
typedef ZWebGridRow             * LPGRIDROW;
typedef ZWebGrid                * LPGRID;
typedef ZValueItem              * LPVALUEITEM;
typedef ZWebGridColMapping      * LPGRIDCOLMAPPING;
typedef ZWebGridEventChain      * LPGRIDEVENTCHAIN;
typedef ZWebGridEvent           * LPGRIDEVENT;

// use of class CMap to create a set of cell and a set of rows
// CMap has implemented an hash-allgorithm that should gain performance
// when we are going to access a cells data. If so, we can use this implementation
// for listboxes too
typedef CMap < DWORD, DWORD, LPGRIDCELL,  LPGRIDCELL  & > ZWebGridCellSet;
typedef CMap < DWORD, DWORD, LPGRIDROW,   LPGRIDROW   & > ZWebGridRowSet;
typedef CMap < zLONG, zLONG, LPVALUEITEM, LPVALUEITEM & > ZValueItemSet;
typedef CMap < zLONG, zLONG, LPGRIDEVENT, LPGRIDEVENT & > ZWebGridEventSet;


//////////////////////////////////////////////////////////////////////////
// QuickSort functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// QuickSortRecursive - core of algorithm, do not call it, use QuickSort,
// see below
template <class T> void QuickSortRecursive(T *pArr, int d, int h, zBOOL bAscending)
{
   int i,j;
   T cs;

   i = h;
   j = d;

   cs = pArr[ ((int) ((d+h) / 2))];

   do
   {
      if ( bAscending )
      {
         while (pArr[ j ] < cs ) j++;
         while (pArr[ i ] > cs ) i--;
      }
      else
      {
         while (pArr[ j ] > cs) j++;
         while (pArr[ i ] < cs) i--;
      }

      if ( i >= j )
      {
         if ( i != j )
         {
            T zal;

            zal = pArr[ i ];
            pArr[ i ] = pArr[ j ];
            pArr[ j ] = zal;

         }

         i--;
         j++;
      }

   } while ( j <= i );

   if ( d < i ) QuickSortRecursive( pArr, d, i, bAscending );
   if ( j < h ) QuickSortRecursive( pArr, j, h, bAscending );
}

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// T *pArr         ... pointer of array to sort
// int iSize      ... size of array T *pArr
// zBOOL bAscending   ... if bAscending == TRUE, then sort ascending,
//                  otherwise descending
//
// return TRUE if no error, error can be bad parameter, call ::GetLastError()
// if QuickSort returned FALSE
template <class T> zBOOL QuickSort(T *pArr, int iSize, zBOOL bAscending = TRUE)
{
   zBOOL rc = TRUE;

   if (iSize > 1) {

      try {

         int   low = 0,
            high = iSize - 1;

         QuickSortRecursive(pArr,low,high,bAscending);

      } catch (...) {
         ::SetLastError(ERROR_INVALID_PARAMETER);
         rc = FALSE;
      }

   } else {
      ::SetLastError(ERROR_INVALID_PARAMETER);
      rc = FALSE;
   }

   return rc;
}

//////////////////////////////////////////////////////////////////////////
// CQArray
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CQArray declaration

template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
   void QuickSort( zBOOL bAscending = TRUE );
};

//////////////////////////////////////////////////////////////////////////
// CQArray implementation

//////////////////////////////////////////////////////////////////////////
// QuickSort - entry to algorithm
//
// zBOOL bAscending  ... if bAscending == TRUE, then sort ascending,
//                   otherwise descending
//
template <class T, class TP> void CQArray<T,TP>::QuickSort( zBOOL bAscending /* = TRUE*/ )
{
   if ( GetSize( ) > 1 )
   {
      ::QuickSort( GetData( ), GetSize( ), bAscending );
   }
}


class ZValueItem : public CObject
{
   DECLARE_DYNAMIC( ZValueItem )

public:

   ZValueItem( zLONG lIdentifier = -1, zCPCHAR pszValue = "" );
   ZValueItem( ZValueItem &v );
   ~ZValueItem( );

   void SetValue( zCPCHAR szValue );
   void SetId   ( zLONG lIdentifier );

   zBOOL operator > ( ZValueItem & comp);
   zBOOL operator < ( ZValueItem & comp );
   zBOOL operator == ( ZValueItem & comp );
   void operator = ( ZValueItem & comp );// deep copy

private:
   CString m_csValue;

   // This is a unique Identifier e.g. a value retrieved by GetEntityKey.
   zLONG   m_lIdentifier;

public:
   CString GetValue();
   zLONG GetID();

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

};

class ZVirtualWebColumns : public CObList
{
public:

   ZVirtualWebColumns();
   virtual ~ZVirtualWebColumns();
   POSITION GetFirstColumnPosition(){ return GetHeadPosition(); }
   ZWebGridCol *GetNextColumn( POSITION &pos ){ return (ZWebGridCol *)GetNext(pos); }

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const;
#endif

};

class ZWebGridCell : public CObject
{
   DECLARE_DYNAMIC( ZWebGridCell )

public:
   LPGRIDROW GetRow();

   ZWebGridCell( );
   ~ZWebGridCell( );

   friend class ZWebGridRow;
   friend class ZWebGridCol;

   zLONG GetIndex( );

   // identify if column if cell is changed
   zBOOL    IsChanged();
   // get the value of the current cell
   zCPCHAR  GetValue();

   // get the selected item if there is a list of valueitems attached
   // to the column. This is only valid for Combobox and RadioGroup
   zLONG GetSelection( void );

   // set the value of the current cell
   void     SetValue( zCPCHAR lpszValue );

   // set the value of the current cell via get view.entity.attribute
   zBOOL     SetValueFromOI( zVIEW vApp );

   // store the value of the current cell in the underlying view.entity.attribute
   zBOOL     SaveValueToOI( zVIEW vApp );

   // set the value of the current cell via RemoteObject KZXRA
   zBOOL     SetValueFromXRA( zVIEW vXRA );

   // store the value of the current cell in the RemoteObject KZXRA
   zBOOL     SaveValueToXRA( zVIEW vApp );

   // set cell locked
   void     SetEnabled( zVIEW vAPP );
   void     SetEnabled( zBOOL bEnable = TRUE );
   zBOOL    IsEnabled();

   // set the checked member if current cell appears as a checkbox
   void SetChecked ( CString csValue );
   // set the selected member if current cell appears as a combobox or radiogrp
   void SetSelected( CString csValue );

   void SetGridColumn( LPGRIDCOL pCol );
#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

protected:
   void SetIndex( zLONG lIndex);
   void SetRow( LPGRIDROW pRow );
   // set the m_lSelected member if Data is returned via KZXRA
   void SetSelected( zLONG lSelection );

private: // member variables
   zLONG     m_lIndex;

   zBOOL     m_bEnabled;     // identifies whether a cell is editable
   zBOOL     m_bChangedNode; // identifies whether a cell has been changed
   zBOOL     m_bChecked;     // identifies whether a cell is checked
   zLONG     m_lSelected;    // identifies which entry of a combobox is selected
   CString   m_csValue;      // the current value of a cell
   LPGRID    m_pParentGrid;  // the grid to which this cell belongs to
   LPGRIDROW m_pRow;         // the row to which this cell belongs to
   LPGRIDCOL m_pCol;         // column to which the cell belongs to. It is
                             // used to identify the column type e.g. Checkbox, ComboBox
};

class ZWebGridCol : public CObject
{
   DECLARE_DYNAMIC( ZWebGridCol )

public:

   virtual void SetInitiallyLocked( zBOOL bLockIt );

   virtual zSHORT MapValueToOI( zVIEW vApp, LPGRIDCELL pCell );
   virtual zVIEW MapToRemoteDataSet(  zVIEW vXRA );
   zLONG GetSelection( CString csValue );
   CString GetSelection( zLONG lSelection );

   zBOOL IsInitiallyLocked();
   zBOOL IsChanged( );
   zBOOL IsChecked( CString csValue );
   CString GetChecked( zBOOL bChecked );
   virtual void SetMapping( ZWebGridColMapping *pMapping );
   void SetParentGrid( LPGRID pParent );
   LPGRID GetGrid();
   ZWebGridCol( zKZWDLGXO_Ctrl_DEF *CtrlDef = NULL );
   ~ZWebGridCol( );

   zLONG  GetMappingDisplayLength( );
   zSHORT GetVariantFromAttribute( COleVariant &v, zVIEW vApp );
   zBOOL  CanUpdateAttribute( zVIEW vApp );

   // wrapper functions used by global grid operations
   // to manipulate column appearance at runtime
   void SetAllowSetFocus( zBOOL bSet );
   void SetColumnWidth( zLONG lWidth );
   void SetForeColor( zLONG lForeColor );
   void SetBackColor( zLONG lBackColor );
   void SetButtonAlways( zBOOL bButtonAllways );
   void SetTextColor( zLONG lTextColor );
   void SetFont( zCPCHAR pFontName,
                 zLONG   sFontSize,
                 zBOOL   bBold,
                 zBOOL   bUnderLine,
                 zBOOL   bItalic,
                 zBOOL   bStrikeThrough );
   void SetAlignment( zLONG lAlignment );
   void SetTextLength( zLONG lDisplayLength );
   void SetShowColumn( zBOOL bShow );

   // helperfunctions for wrappers
   void SetAlignment( CString csAlignment );
   void SetFontName     ( zCPCHAR pFontName );
   void SetFontSize     ( zLONG   sFontSize );
   void SetBold         ( zBOOL   bBold );
   void SetUnderline    ( zBOOL   bUnderline );
   void SetItalic       ( zBOOL   bItalic );
   void SetStrikeThrough( zBOOL   bStrikeThrough );


#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

private:

protected:
   ZWebGridColMapping *m_pMapping;
   LPGRID              m_pParentGrid;
   zKZWDLGXO_Ctrl_DEF * m_pCtrlDef;
   // column state information
   zBOOL m_bInsertNode;  // new column is inserted this can be forced by
                         // grid creation or Showing a hidden column
   zBOOL m_bChangedNode; // column has changed e.g. column width, colorsetting
   zBOOL m_bDeleteNode;  // column is deleted eg hiding a column

   zBOOL m_bShow;        // show or hide column

   zBOOL m_bAllowSetFocus;
   zBOOL m_bInitiallyLocked;

   zLONG m_lColumnWidth;
   CString m_csAlignment;

   // color members
   zLONG m_lForeColor;
   zLONG m_lBackColor;
   zLONG m_lTextColor;
   //Font members
   CString m_csFontName;
   zLONG   m_lFontSize;
   zBOOL   m_bBold;
   zBOOL   m_bItalic;
   zBOOL   m_bUnderline;
   zBOOL   m_bStrikeThrough;

   zLONG   m_lDisplayLength;
   zBOOL   m_bButtonAlways;
};

class ZWebGridColEdit : public ZWebGridCol
{
   DECLARE_DYNAMIC( ZWebGridColEdit )

public:

   ZWebGridColEdit( zKZWDLGXO_Ctrl_DEF *CtrlDef = NULL );
   ~ZWebGridColEdit( );

   virtual zVIEW MapToRemoteDataSet(  zVIEW vXRA );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

private:
   CString m_csCase;
   zBOOL m_bPassword;
};

class ZWebGridColCheckBox : public ZWebGridCol
{
   DECLARE_DYNAMIC( ZWebGridColCheckBox )

public:

   ZWebGridColCheckBox( zKZWDLGXO_Ctrl_DEF *CtrlDef = NULL );
   ~ZWebGridColCheckBox( );

   virtual zVIEW MapToRemoteDataSet(  zVIEW vXRA );

   zBOOL IsChecked( CString csValue );
   CString GetChecked( zBOOL bChecked );

   void SetCheckboxTranslationValues( zCPCHAR pszCheckedValue, zCPCHAR pszUncheckedValue );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

private:
   CString m_csCheckedValue;
   CString m_csUncheckedValue;
};


class ZWebGridColComboBox : public ZWebGridCol
{
   DECLARE_DYNAMIC( ZWebGridColComboBox )

public:
   zBOOL m_bPopulated;
   ZWebGridColComboBox( zKZWDLGXO_Ctrl_DEF *CtrlDef );
   ~ZWebGridColComboBox( );


   zLONG GetSelection( CString csValue );
   CString GetSelection( zLONG lSelection );

   virtual zSHORT MapValueToOI( zVIEW vApp, LPGRIDCELL pCell );
   virtual zVIEW MapToRemoteDataSet(  zVIEW vXRA );

   virtual void SetInitiallyLocked( zBOOL bLockIt );
   virtual void SetMapping( ZWebGridColMapping *pMapping );

   LPVALUEITEM AddValueItem( DWORD dwKey );
   LPVALUEITEM AddValueItem( DWORD dwKey, ZValueItem v );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif
private: // methods

   void Populate();
   void BuildListOfValuesFromOI( );
   void RestListOfValues();
   zLONG GetEntryIndex( CString csValue );
   POSITION GetFirstValueItemPosition();
   void GetNextValueItem(POSITION &rPos, zLONG &rdwKey, LPVALUEITEM &rItem);
   zLONG GetSubtype();

private: // member variables
   ZWebGridColMapping *m_pListMapping;
   ZWebGridColMapping *m_pBrowseMapping;

   ZValueItemSet m_mapValueItemSet; // list of valid values
};

class ZWebGridColRadioGroup : public ZWebGridCol
{
   DECLARE_DYNAMIC( ZWebGridColRadioGroup )

public:

   ZWebGridColRadioGroup( zKZWDLGXO_Ctrl_DEF *CtrlDef = NULL );
   ~ZWebGridColRadioGroup( );

   zLONG GetSelection( CString csValue );
    CString GetSelection( zLONG lSelection );

   void AddButton( LPGRIDCOLRADIOBUTTON pButton );

   virtual zVIEW MapToRemoteDataSet(  zVIEW vXRA );

   POSITION GetFirstButtonPosition( );
   LPGRIDCOLRADIOBUTTON GetNextButton( POSITION &pos );

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const;
#endif

private:
   CObList m_lRadioButtons;

private:
};

class ZWebGridColRadioButton : public CObject
{
   DECLARE_DYNAMIC( ZWebGridColRadioButton )

public:

   friend class ZWebGridColRadioGroup;

   ZWebGridColRadioButton( );
   ZWebGridColRadioButton( zKZWDLGXO_Ctrl_DEF &CtrlDefRB );

   ~ZWebGridColRadioButton( );


#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

   CString GetCheckedValue(){ return m_csValue; }
private:
protected:
   zBOOL m_bDefault;
   CString m_csDisplayValue; // the radio buttons text
   CString m_csValue;        // the internal value to be set if its checked
};

class ZWebGridRow : public CObject
{
   DECLARE_DYNAMIC( ZWebGridRow )

public:

   ZWebGridRow( LPGRID pGrid = NULL,
                zLONG lIndex = 0,
                zLONG lCells = 0,
                zVIEW vTemp = NULL );

   ~ZWebGridRow( );

   // checks whether a col value of row has been changed
   zBOOL IsChanged();
   // checks whether a row is a new one
   zBOOL IsInserted();
   // checks whether a row is a deleted one
   zBOOL IsDeleted();
   // set the deleted flag for a row
   void SetDeleted( zBOOL bDelete );

   // sets select flag for a row on / off
   void SetSelect( zBOOL bSet );
   // returns current select state
   zBOOL IsSelected();

   // add a column and return a pointer to it
   LPGRIDCELL AddCell( zLONG lKey );
   // set the number of columns in this row to init the hashtable for
   // the m_mapCellSet of this row
   void SetNumberOfCells( zLONG nCellCount );

   // delete all cells
   void ResetContent();
   /*
   // retrieve list iterator for the first cell
   POSITION GetFirstCellPosition();
   // retrieve cell information adressed by list iterator
   void GetNextCell(POSITION &rPos, DWORD &rdwKey, LPGRIDCELL &rCell);
   */
   // retrieve the cell with a given key
   zBOOL LookupCell( zLONG lKey, LPGRIDCELL &rCell);
   // retrieve the cell with a given Index
   LPGRIDCELL GetCell( zLONG lCell );

   // retrieve number of cells in the current row
   DWORD GetCellCount( );

   // set the external identifier external means the row will
   // use this to update itselv against the underlying OI
   void SetEntityKey( zULONG ulKey );
   // set the external identifier the can be used to
   // to update the current row against the underlying OI
   zULONG GetEntityKey(  );
   // Identifier using HashTable allgorithm
   zLONG GetIndex(  );
   // do an update from the underlying OI
   zSHORT MapFromOI( zVIEW vTemp );
   // do an update against the underlying OI
   zSHORT MapToOI( zVIEW vTemp );
   // do an update against the KZXRA
   zSHORT MapToRemoteDataset( zVIEW vXRA );
private:
   zBOOL       m_bSelected;    // is it selected?
   zBOOL       m_bChangedNode; // is it changed?
   zBOOL       m_bInsertNode;  // is it inserted?
   zBOOL       m_bDeleteNode;  // is it deleted?
   zLONG       m_lCells;       // number of cells
   zULONG      m_ulEntityKey;  // entity identifier
   zLONG       m_lIndex;       // index identifying the row using HashTable
   LPGRIDCELL  m_pCellSetArray;// array to hold cell information
   LPGRID      m_pParentGrid;  // the grid to which this row belongs to
public:
#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

};

class ZWebGridEventChain : public CObject
{
     DECLARE_DYNAMIC( ZWebGridEventChain )
public:

   ZWebGridEventChain();
   virtual ~ZWebGridEventChain();

   void Cancel();                   //to cancel the whole processing
   void RemoveAll();
private:
   ZWebGridEventSet m_mapEventSet;  // list Events that have to be processed
                                    // as a result of manipulations on lightdriver site

#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CObject::Dump( dc );
   }

#endif
};


class ZDCTL_CLASS ZWebGrid : public ZMapAct , public CWnd
{
   DECLARE_DYNAMIC( ZWebGrid )
public:

   ZWebGrid( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   virtual ~ZWebGrid( );

   virtual void CreateZ( void );

   // delete all Rows
   void ResetContent();
   // reset all selections
   void ResetSelectionSet( zBOOL bSet = FALSE );
   // set the value for a specific cell
   zBOOL SetValue( DWORD nRow, DWORD nColumn, zCPCHAR pszValue );
   // Get the value for a specific cell
   CString GetValue( DWORD nRow, DWORD nColumn );
   // set dimensions of the underlying dataset/array
   void SetArrayDimensions( DWORD nRows, DWORD nColumns );

   // Add a row and return a pointer to that row
   LPGRIDROW AddRow( zLONG lEntityKey, zVIEW vTemp );
   // transfer data from objectinstance to the control
   zSHORT MapFromOI( WPARAM wFlag = 0 );
   // transfer data from control to the objectinstance
   zSHORT MapToOI( zLONG lFlag = 0 );

   //returns the view to the dialog window
   zVIEW GetDialogView();
   //return the column with specified index
   LPGRIDCOL GetGridColumn( zSHORT lCol );
   //add a column to the end of the list
   void AddColumn( LPGRIDCOL pCol );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif
   zCPCHAR  GetListViewName();
   zCPCHAR  GetListEntity();
   zCPCHAR  GetScope();

public: // wrapper for TG_??? functions
   void DeleteRowFromXRA( LPGRIDROW pRow );
   LPGRIDROW CreateRowFromXRA();
   zSHORT FireVirtualEvent( zLONG lEventId );
   zSHORT LockColumn( zSHORT nIndex, zBOOL bLock );
   zSHORT SetCellData( zCPCHAR pchReturnData, zLONG lRowNbr, zLONG lColNbr);
   zSHORT GetCellData( zPCHAR pchReturnData, zLONG lMaxLth, zLONG lRowNbr, zLONG lColNbr);
   zSHORT GetActiveCell( zPLONG plRowNbr, zPLONG plColNbr);
   zSHORT SetCurrentCell( zLONG lRowNbr, zLONG lColNbr );

   zSHORT SetCheckBoxTranslation( zSHORT  nColumn,
                                  zCPCHAR cpcTRUE,
                                  zCPCHAR cpcFALSE );
   zSHORT EnableAutosort( zBOOL bEnable );
   zSHORT SetAllowAddNew    ( zBOOL bAllowAddNew    );
   zSHORT SetAllowDelete    ( zBOOL bAllowDelete    );
   zSHORT SetAllowRowSelect ( zBOOL bAllowRowSelect );
   zSHORT SetAllowColSelect ( zBOOL bAllowColSelect );
   zSHORT SetMultiSelect    ( zLONG lMultiSelect    );

   zSHORT SetCurrentRow( zLONG lRowNbr );

private:

   // transfer data from to the control to KZXRA
   zSHORT MapToRemoteDataSet( );
   // browse the kzwdlgxo to extract column and mapping information
   zSHORT LoadMapping( zVIEW vDialog );
   // transfer data from to the KZXRA to control, update row and cell
   // states and cell data
   zSHORT MapFromRemoteDataset( );
private:
   void GetNextRow( POSITION& rPos, DWORD& dwKey, LPGRIDROW& rRow );
   POSITION GetFirstRowPosition( );
   ZVirtualWebColumns m_listColumns;   // a list of Grid Columns
   ZWebGridRowSet m_mapRowSet;         // rows attached to the grid
   ZWebGridEventChain m_mapEventChain; // List of events that have to be fired due to XRA update
   DWORD m_nRows;                      // number of rows used to init the hashtable of m_mapRowsSet
   DWORD m_nColumns;                   // number of columns in each row of m_mapRowSet

   //Zeidon Internals
   CString  m_csListView;
   CString  m_csListEntityName;
   CString  m_csScopeEntityName;
   zBOOL    m_bProcessSubControls;
   zULONG   m_ulFlag;
   zLONG    m_lCurrentRow;
   zLONG    m_lCurrentCell;
   zLONG    m_lInactiveForeColor;
   zLONG    m_lInactiveBackColor;
   zBOOL    m_bAllowAddNew;
   zBOOL    m_bAllowDelete;
   zBOOL    m_bAllowRowSelect;
   zBOOL    m_bAllowColSelect;
   zLONG    m_lMultiSelect;
   zBOOL    m_bChangedNode;
   zBOOL    m_bInsertMode;
};

class ZWebGridColMapping : public CObject
{
     DECLARE_DYNAMIC( ZWebGridColMapping )
public:
   void InitMappingAttributeLength( zVIEW vApp );

   ZWebGridColMapping();
   // copy constructor
   ZWebGridColMapping(const ZWebGridColMapping& ColumnMapping);
   ZWebGridColMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext);
   ZWebGridColMapping( zKZWDLGXO_CtrlMap_DEF CtrlMapDEF );

   virtual ~ZWebGridColMapping();

   inline CString GetMapViewName() const      { return m_csVName; }
   inline CString GetMapEntityName() const    { return m_csEName; }
   inline CString GetMapAttributeName() const { return m_csAName; }
   inline CString GetMapContextName() const   { return m_csContext; }
   inline zULONG GetAttributeDisplayLength();

   inline void SetMapViewName( zCPCHAR pszValue )      { m_csVName = pszValue ; }
   inline void SetMapEntityName( zCPCHAR pszValue )    { m_csEName = pszValue ; }
   inline void SetMapAttributeName( zCPCHAR pszValue ) { m_csAName = pszValue ; }
   inline void SetMapContextName( zCPCHAR pszValue )   { m_csContext = pszValue ; }

   void SetMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext);

#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CObject::Dump( dc );
      dc << m_csVName   << "\n";
      dc << m_csEName   << "\n";
      dc << m_csAName   << "\n";
      dc << m_csContext << "\n";
   }

#endif

private:
   CString m_csVName;        // View name to application map object
   CString m_csEName;        // Entity name within map object
   CString m_csAName;        // Attribute name within map object
   CString m_csContext;      // Domain context for this attribute
   zULONG  m_ulDisplayLth;    // displaylength of attribute used at runtime not at designtime
                              // so it will not be serialized
};

class ZWebGridEvent : public CObject
{
     DECLARE_DYNAMIC( ZWebGridEvent )
public:

   ZWebGridEvent( zLONG lEventId = 0 );
   virtual ~ZWebGridEvent();

private:
   zLONG   m_lEventId;   //comparable with the DispID of an ActiveX-Event
   CObList m_lstParam;  //Some parameters passed with the event

#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CObject::Dump( dc );
   }

#endif
};



#endif   // __zdctlvwg_h__ sentry.

