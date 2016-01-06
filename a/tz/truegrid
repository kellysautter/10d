`/********************************************************************************************/
//
// File:            tdbg6.cpp
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           6. September 2000
// describtion:
//
//
/* Change log most recent first order:

   2001.10.8   TMV   2000   ActiveX
      changed ZGridColumn::CheckLocking:
         keep track about the AllowAddNew Property
         do a CheckExistenceOfEntity prior to call MiGetUpdateForViewEntityAttr

   2001.08.29   TMV   2000   ActiveX
      Changed UpdateAttributeEx
         call SetupComboBoxSelection if current column is a combobox
         check if a column is changed prior to update the underlying view

   2001.7.3   TMV   2000   ActiveX
      implement handler for loose focus

   2001.4.26   TMV   2000   R55120/F51749
      move inline method ZGridColumn::GetMappingEx to this file
      and check the pointer to the attached Column against null

   2001.4.6   TMV   2000   ActiveX
      temporal fix for EditMasks

   2001.3.20   TMV   2000   ActiveX
      fix bug for checkbox translation

   2001.3.13   TMV   2000   ActiveX
      - adding ITrueDBGridCtrl::EnableAutosort to enable/disable autosort
      - retrieve Inactive BackColor as color for locked cells

   2001.3.12   TMV/HH
      fix bug: crash if no data available.

   2001.3.6   TMV   2000   ActiveX
      add new method ZGridColumn::CheckLocking( zVIEW vApp )

   2001.2.9   TMV   2000   ActiveX
      Implement complex combobox mapping

   2000.9.21   TMV   2000   ActiveX
      Initialize ZGridColumn correctly when calling ZGridColumn::SetColumnIndex

   15.09.2000   TMV   2000   ActiveX
      do inline documentation



   2000.09.06   TMV
      Created, generated with Devstudio class wizzard



*/
/*********************************************************************************************/


#include "zstdafx.h"

#include "tdbg6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ITrueDBGridCtrl properties

BOOL ITrueDBGridCtrl::GetAllowDelete()
{
   BOOL result;
   GetProperty(0x1, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowDelete(BOOL propVal)
{
   SetProperty(0x1, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAllowAddNew()
{
   BOOL result;
   GetProperty(0x2, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowAddNew(BOOL propVal)
{
   SetProperty(0x2, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAllowRowSizing()
{
   BOOL result;
   GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowRowSizing(BOOL propVal)
{
   SetProperty(0x3, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAllowUpdate()
{
   BOOL result;
   GetProperty(0x4, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowUpdate(BOOL propVal)
{
   SetProperty(0x4, VT_BOOL, propVal);
}

short ITrueDBGridCtrl::GetAppearance()
{
   short result;
   GetProperty(DISPID_APPEARANCE, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAppearance(short propVal)
{
   SetProperty(DISPID_APPEARANCE, VT_I4, propVal);
}

short ITrueDBGridCtrl::GetBorderStyle()
{
   short result;
   GetProperty(DISPID_BORDERSTYLE, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetBorderStyle(short propVal)
{
   SetProperty(DISPID_BORDERSTYLE, VT_I4, propVal);
}

VARIANT ITrueDBGridCtrl::GetBookmark()
{
   VARIANT result;
   GetProperty(0x5, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetBookmark(const VARIANT& propVal)
{
   SetProperty(0x5, VT_VARIANT, &propVal);
}

short ITrueDBGridCtrl::GetBookmarkType()
{
   short result;
   GetProperty(0xa6, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetBookmarkType(short propVal)
{
   SetProperty(0xa6, VT_I2, propVal);
}

short ITrueDBGridCtrl::GetCol()
{
   short result;
   GetProperty(0x6, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCol(short propVal)
{
   SetProperty(0x6, VT_I2, propVal);
}

BOOL ITrueDBGridCtrl::GetColumnHeaders()
{
   BOOL result;
   GetProperty(0x7, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetColumnHeaders(BOOL propVal)
{
   SetProperty(0x7, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetColumnFooters()
{
   BOOL result;
   GetProperty(0x8f, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetColumnFooters(BOOL propVal)
{
   SetProperty(0x8f, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetCurrentCellVisible()
{
   BOOL result;
   GetProperty(0x48, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCurrentCellVisible(BOOL propVal)
{
   SetProperty(0x48, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetCurrentCellModified()
{
   BOOL result;
   GetProperty(0x40, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCurrentCellModified(BOOL propVal)
{
   SetProperty(0x40, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetDataChanged()
{
   BOOL result;
   GetProperty(0x46, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetDataChanged(BOOL propVal)
{
   SetProperty(0x46, VT_BOOL, propVal);
}

long ITrueDBGridCtrl::GetDataMode()
{
   long result;
   GetProperty(0x25, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetDataMode(long propVal)
{
   SetProperty(0x25, VT_I4, propVal);
}

LPUNKNOWN ITrueDBGridCtrl::GetDataSource()
{
   LPUNKNOWN result;
   GetProperty(0x9, VT_UNKNOWN, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetDataSource(LPUNKNOWN propVal)
{
   SetProperty(0x9, VT_UNKNOWN, propVal);
}

float ITrueDBGridCtrl::GetDefColWidth()
{
   float result;
   GetProperty(0xa, VT_R4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetDefColWidth(float propVal)
{
   SetProperty(0xa, VT_R4, propVal);
}

BOOL ITrueDBGridCtrl::GetEditActive()
{
   BOOL result;
   GetProperty(0x3f, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEditActive(BOOL propVal)
{
   SetProperty(0x3f, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetEditDropDown()
{
   BOOL result;
   GetProperty(0x50, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEditDropDown(BOOL propVal)
{
   SetProperty(0x50, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetEnabled()
{
   BOOL result;
   GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEnabled(BOOL propVal)
{
   SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetExtendRightColumn()
{
   BOOL result;
   GetProperty(0x49, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetExtendRightColumn(BOOL propVal)
{
   SetProperty(0x49, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetPartialRightColumn()
{
   BOOL result;
   GetProperty(0x9b, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPartialRightColumn(BOOL propVal)
{
   SetProperty(0x9b, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAnchorRightColumn()
{
   BOOL result;
   GetProperty(0x9e, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAnchorRightColumn(BOOL propVal)
{
   SetProperty(0x9e, VT_BOOL, propVal);
}

VARIANT ITrueDBGridCtrl::GetFirstRow()
{
   VARIANT result;
   GetProperty(0x16, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFirstRow(const VARIANT& propVal)
{
   SetProperty(0x16, VT_VARIANT, &propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetHeadFont()
{
   LPDISPATCH result;
   GetProperty(0xb, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetHeadFont(LPDISPATCH propVal)
{
   SetProperty(0xb, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetFooterFont()
{
   LPDISPATCH result;
   GetProperty(0x90, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFooterFont(LPDISPATCH propVal)
{
   SetProperty(0x90, VT_DISPATCH, propVal);
}

float ITrueDBGridCtrl::GetHeadLines()
{
   float result;
   GetProperty(0xc, VT_R4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetHeadLines(float propVal)
{
   SetProperty(0xc, VT_R4, propVal);
}

float ITrueDBGridCtrl::GetFootLines()
{
   float result;
   GetProperty(0x91, VT_R4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFootLines(float propVal)
{
   SetProperty(0x91, VT_R4, propVal);
}

OLE_HANDLE ITrueDBGridCtrl::GetHWnd()
{
   OLE_HANDLE result;
   GetProperty(DISPID_HWND, VT_I4, (void*)&result);
   return result;
}

long ITrueDBGridCtrl::GetHWndEditor()
{
   long result;
   GetProperty(0x28, VT_I4, (void*)&result);
   return result;
}

BOOL ITrueDBGridCtrl::GetMarqueeUnique()
{
   BOOL result;
   GetProperty(0x4a, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetMarqueeUnique(BOOL propVal)
{
   SetProperty(0x4a, VT_BOOL, propVal);
}

long ITrueDBGridCtrl::GetMarqueeStyle()
{
   long result;
   GetProperty(0x4b, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetMarqueeStyle(long propVal)
{
   SetProperty(0x4b, VT_I4, propVal);
}

BOOL ITrueDBGridCtrl::GetRecordSelectors()
{
   BOOL result;
   GetProperty(0xd, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetRecordSelectors(BOOL propVal)
{
   SetProperty(0xd, VT_BOOL, propVal);
}

short ITrueDBGridCtrl::GetRow()
{
   short result;
   GetProperty(0xe, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetRow(short propVal)
{
   SetProperty(0xe, VT_I2, propVal);
}

long ITrueDBGridCtrl::GetRowDividerStyle()
{
   long result;
   GetProperty(0xf, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetRowDividerStyle(long propVal)
{
   SetProperty(0xf, VT_I4, propVal);
}

float ITrueDBGridCtrl::GetRowHeight()
{
   float result;
   GetProperty(0x10, VT_R4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetRowHeight(float propVal)
{
   SetProperty(0x10, VT_R4, propVal);
}

long ITrueDBGridCtrl::GetScrollBars()
{
   long result;
   GetProperty(0x20, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetScrollBars(long propVal)
{
   SetProperty(0x20, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetSelLength()
{
   long result;
   GetProperty(0x41, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelLength(long propVal)
{
   SetProperty(0x41, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetSelStart()
{
   long result;
   GetProperty(0x42, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelStart(long propVal)
{
   SetProperty(0x42, VT_I4, propVal);
}

CString ITrueDBGridCtrl::GetSelText()
{
   CString result;
   GetProperty(0x43, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelText(LPCTSTR propVal)
{
   SetProperty(0x43, VT_BSTR, propVal);
}

short ITrueDBGridCtrl::GetSelStartCol()
{
   short result;
   GetProperty(0x11, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelStartCol(short propVal)
{
   SetProperty(0x11, VT_I2, propVal);
}

short ITrueDBGridCtrl::GetSelEndCol()
{
   short result;
   GetProperty(0x12, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelEndCol(short propVal)
{
   SetProperty(0x12, VT_I2, propVal);
}

short ITrueDBGridCtrl::GetSplit()
{
   short result;
   GetProperty(0x2b, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSplit(short propVal)
{
   SetProperty(0x2b, VT_I2, propVal);
}

short ITrueDBGridCtrl::GetVisibleCols()
{
   short result;
   GetProperty(0x21, VT_I2, (void*)&result);
   return result;
}

short ITrueDBGridCtrl::GetVisibleRows()
{
   short result;
   GetProperty(0x13, VT_I2, (void*)&result);
   return result;
}

OLE_COLOR ITrueDBGridCtrl::GetBackColor()
{
   OLE_COLOR result;
   GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetBackColor(OLE_COLOR propVal)
{
   SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

OLE_COLOR ITrueDBGridCtrl::GetForeColor()
{
   OLE_COLOR result;
   GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetForeColor(OLE_COLOR propVal)
{
   SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

CString ITrueDBGridCtrl::GetCaption()
{
   CString result;
   GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCaption(LPCTSTR propVal)
{
   SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}

LPFONTDISP ITrueDBGridCtrl::GetFont()
{
   LPFONTDISP result;
   GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFont(LPFONTDISP propVal)
{
   SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

short ITrueDBGridCtrl::GetLeftCol()
{
   short result;
   GetProperty(0x17, VT_I2, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetLeftCol(short propVal)
{
   SetProperty(0x17, VT_I2, propVal);
}

BOOL ITrueDBGridCtrl::GetAllowArrows()
{
   BOOL result;
   GetProperty(0x2f, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowArrows(BOOL propVal)
{
   SetProperty(0x2f, VT_BOOL, propVal);
}

long ITrueDBGridCtrl::GetExposeCellMode()
{
   long result;
   GetProperty(0x30, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetExposeCellMode(long propVal)
{
   SetProperty(0x30, VT_I4, propVal);
}

BOOL ITrueDBGridCtrl::GetTabAcrossSplits()
{
   BOOL result;
   GetProperty(0x31, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetTabAcrossSplits(BOOL propVal)
{
   SetProperty(0x31, VT_BOOL, propVal);
}

long ITrueDBGridCtrl::GetTabAction()
{
   long result;
   GetProperty(0x32, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetTabAction(long propVal)
{
   SetProperty(0x32, VT_I4, propVal);
}

BOOL ITrueDBGridCtrl::GetWrapCellPointer()
{
   BOOL result;
   GetProperty(0x33, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetWrapCellPointer(BOOL propVal)
{
   SetProperty(0x33, VT_BOOL, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetStyle()
{
   LPDISPATCH result;
   GetProperty(0x2c, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetStyle(LPDISPATCH propVal)
{
   SetProperty(0x2c, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetHeadingStyle()
{
   LPDISPATCH result;
   GetProperty(0x2d, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetHeadingStyle(LPDISPATCH propVal)
{
   SetProperty(0x2d, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetFooterStyle()
{
   LPDISPATCH result;
   GetProperty(0x92, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFooterStyle(LPDISPATCH propVal)
{
   SetProperty(0x92, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetEditorStyle()
{
   LPDISPATCH result;
   GetProperty(0x2e, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEditorStyle(LPDISPATCH propVal)
{
   SetProperty(0x2e, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetInactiveStyle()
{
   LPDISPATCH result;
   GetProperty(0x34, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetInactiveStyle(LPDISPATCH propVal)
{
   SetProperty(0x34, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetSelectedStyle()
{
   LPDISPATCH result;
   GetProperty(0x35, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelectedStyle(LPDISPATCH propVal)
{
   SetProperty(0x35, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetCaptionStyle()
{
   LPDISPATCH result;
   GetProperty(0x36, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCaptionStyle(LPDISPATCH propVal)
{
   SetProperty(0x36, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetHighlightRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x78, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetHighlightRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x78, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetEvenRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x7a, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEvenRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x7a, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetOddRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x7b, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetOddRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x7b, VT_DISPATCH, propVal);
}

unsigned long ITrueDBGridCtrl::GetHeadBackColor()
{
   unsigned long result;
   GetProperty(0x37, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetHeadBackColor(unsigned long propVal)
{
   SetProperty(0x37, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetHeadForeColor()
{
   unsigned long result;
   GetProperty(0x38, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetHeadForeColor(unsigned long propVal)
{
   SetProperty(0x38, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetFooterBackColor()
{
   unsigned long result;
   GetProperty(0x93, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFooterBackColor(unsigned long propVal)
{
   SetProperty(0x93, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetFooterForeColor()
{
   unsigned long result;
   GetProperty(0x94, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFooterForeColor(unsigned long propVal)
{
   SetProperty(0x94, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetEditBackColor()
{
   unsigned long result;
   GetProperty(0x39, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEditBackColor(unsigned long propVal)
{
   SetProperty(0x39, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetEditForeColor()
{
   unsigned long result;
   GetProperty(0x3a, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEditForeColor(unsigned long propVal)
{
   SetProperty(0x3a, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetInactiveBackColor()
{
   unsigned long result;
   GetProperty(0x3b, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetInactiveBackColor(unsigned long propVal)
{
   SetProperty(0x3b, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetInactiveForeColor()
{
   unsigned long result;
   GetProperty(0x3c, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetInactiveForeColor(unsigned long propVal)
{
   SetProperty(0x3c, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetSelectedBackColor()
{
   unsigned long result;
   GetProperty(0x3d, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelectedBackColor(unsigned long propVal)
{
   SetProperty(0x3d, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetSelectedForeColor()
{
   unsigned long result;
   GetProperty(0x3e, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetSelectedForeColor(unsigned long propVal)
{
   SetProperty(0x3e, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetApproxCount()
{
   long result;
   GetProperty(0x51, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetApproxCount(long propVal)
{
   SetProperty(0x51, VT_I4, propVal);
}

CString ITrueDBGridCtrl::GetErrorText()
{
   CString result;
   GetProperty(0x52, VT_BSTR, (void*)&result);
   return result;
}

long ITrueDBGridCtrl::GetAddNewMode()
{
   long result;
   GetProperty(0x54, VT_I4, (void*)&result);
   return result;
}

BOOL ITrueDBGridCtrl::GetAllowColSelect()
{
   BOOL result;
   GetProperty(0x56, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowColSelect(BOOL propVal)
{
   SetProperty(0x56, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAllowColMove()
{
   BOOL result;
   GetProperty(0x57, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowColMove(BOOL propVal)
{
   SetProperty(0x57, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAllowRowSelect()
{
   BOOL result;
   GetProperty(0x58, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAllowRowSelect(BOOL propVal)
{
   SetProperty(0x58, VT_BOOL, propVal);
}

VARIANT ITrueDBGridCtrl::GetPictureCurrentRow()
{
   VARIANT result;
   GetProperty(0x95, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPictureCurrentRow(const VARIANT& propVal)
{
   SetProperty(0x95, VT_VARIANT, &propVal);
}

VARIANT ITrueDBGridCtrl::GetPictureModifiedRow()
{
   VARIANT result;
   GetProperty(0x96, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPictureModifiedRow(const VARIANT& propVal)
{
   SetProperty(0x96, VT_VARIANT, &propVal);
}

VARIANT ITrueDBGridCtrl::GetPictureAddnewRow()
{
   VARIANT result;
   GetProperty(0x97, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPictureAddnewRow(const VARIANT& propVal)
{
   SetProperty(0x97, VT_VARIANT, &propVal);
}

VARIANT ITrueDBGridCtrl::GetPictureStandardRow()
{
   VARIANT result;
   GetProperty(0xb0, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPictureStandardRow(const VARIANT& propVal)
{
   SetProperty(0xb0, VT_VARIANT, &propVal);
}

VARIANT ITrueDBGridCtrl::GetPictureHeaderRow()
{
   VARIANT result;
   GetProperty(0xb2, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPictureHeaderRow(const VARIANT& propVal)
{
   SetProperty(0xb2, VT_VARIANT, &propVal);
}

VARIANT ITrueDBGridCtrl::GetPictureFooterRow()
{
   VARIANT result;
   GetProperty(0xb3, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetPictureFooterRow(const VARIANT& propVal)
{
   SetProperty(0xb3, VT_VARIANT, &propVal);
}

VARIANT ITrueDBGridCtrl::GetMouseIcon()
{
   VARIANT result;
   GetProperty(0xa3, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetMouseIcon(const VARIANT& propVal)
{
   SetProperty(0xa3, VT_VARIANT, &propVal);
}

long ITrueDBGridCtrl::GetMousePointer()
{
   long result;
   GetProperty(0xa4, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetMousePointer(long propVal)
{
   SetProperty(0xa4, VT_I4, propVal);
}

LPUNKNOWN ITrueDBGridCtrl::GetISVServices()
{
   LPUNKNOWN result;
   GetProperty(0x55, VT_UNKNOWN, (void*)&result);
   return result;
}

CString ITrueDBGridCtrl::GetText()
{
   CString result;
   GetProperty(0x0, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetText(LPCTSTR propVal)
{
   SetProperty(0x0, VT_BSTR, propVal);
}

CString ITrueDBGridCtrl::GetLayoutName()
{
   CString result;
   GetProperty(0x5c, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetLayoutName(LPCTSTR propVal)
{
   SetProperty(0x5c, VT_BSTR, propVal);
}

CString ITrueDBGridCtrl::GetLayoutFileName()
{
   CString result;
   GetProperty(0x5d, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetLayoutFileName(LPCTSTR propVal)
{
   SetProperty(0x5d, VT_BSTR, propVal);
}

CString ITrueDBGridCtrl::GetLayoutURL()
{
   CString result;
   GetProperty(0xb1, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetLayoutURL(LPCTSTR propVal)
{
   SetProperty(0xb1, VT_BSTR, propVal);
}

long ITrueDBGridCtrl::GetMultipleLines()
{
   long result;
   GetProperty(0x61, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetMultipleLines(long propVal)
{
   SetProperty(0x61, VT_I4, propVal);
}

LPDISPATCH ITrueDBGridCtrl::GetArray()
{
   LPDISPATCH result;
   GetProperty(0x5e, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetArray(LPDISPATCH propVal)
{
   SetProperty(0x5e, VT_DISPATCH, propVal);
}

BOOL ITrueDBGridCtrl::GetEmptyRows()
{
   BOOL result;
   GetProperty(0x5f, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetEmptyRows(BOOL propVal)
{
   SetProperty(0x5f, VT_BOOL, propVal);
}

long ITrueDBGridCtrl::GetCellTips()
{
   long result;
   GetProperty(0x60, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCellTips(long propVal)
{
   SetProperty(0x60, VT_I4, propVal);
}

float ITrueDBGridCtrl::GetVScrollWidth()
{
   float result;
   GetProperty(0x70, VT_R4, (void*)&result);
   return result;
}

float ITrueDBGridCtrl::GetHScrollHeight()
{
   float result;
   GetProperty(0x71, VT_R4, (void*)&result);
   return result;
}

BOOL ITrueDBGridCtrl::GetBof()
{
   BOOL result;
   GetProperty(0x6b, VT_BOOL, (void*)&result);
   return result;
}

BOOL ITrueDBGridCtrl::GetEof()
{
   BOOL result;
   GetProperty(0x6c, VT_BOOL, (void*)&result);
   return result;
}

BOOL ITrueDBGridCtrl::GetFetchRowStyle()
{
   BOOL result;
   GetProperty(0x79, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetFetchRowStyle(BOOL propVal)
{
   SetProperty(0x79, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetAlternatingRowStyle()
{
   BOOL result;
   GetProperty(0x7c, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAlternatingRowStyle(BOOL propVal)
{
   SetProperty(0x7c, VT_BOOL, propVal);
}

float ITrueDBGridCtrl::GetCellTipsWidth()
{
   float result;
   GetProperty(0x7d, VT_R4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCellTipsWidth(float propVal)
{
   SetProperty(0x7d, VT_R4, propVal);
}

long ITrueDBGridCtrl::GetCellTipsDelay()
{
   long result;
   GetProperty(0x7e, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetCellTipsDelay(long propVal)
{
   SetProperty(0x7e, VT_I4, propVal);
}

BOOL ITrueDBGridCtrl::GetInsertMode()
{
   BOOL result;
   GetProperty(0x98, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetInsertMode(BOOL propVal)
{
   SetProperty(0x98, VT_BOOL, propVal);
}

BOOL ITrueDBGridCtrl::GetTransparentRowPictures()
{
   BOOL result;
   GetProperty(0x99, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetTransparentRowPictures(BOOL propVal)
{
   SetProperty(0x99, VT_BOOL, propVal);
}

long ITrueDBGridCtrl::GetMultiSelect()
{
   long result;
   GetProperty(0x84, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetMultiSelect(long propVal)
{
   SetProperty(0x84, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetOLEDragMode()
{
   long result;
   GetProperty(0x9f, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetOLEDragMode(long propVal)
{
   SetProperty(0x9f, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetOLEDropMode()
{
   long result;
   GetProperty(0xa0, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetOLEDropMode(long propVal)
{
   SetProperty(0xa0, VT_I4, propVal);
}

BOOL ITrueDBGridCtrl::GetExportEOF()
{
   BOOL result;
   GetProperty(0xae, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetExportEOF(BOOL propVal)
{
   SetProperty(0xae, VT_BOOL, propVal);
}

VARIANT ITrueDBGridCtrl::GetExportNextBookmark()
{
   VARIANT result;
   GetProperty(0xaf, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetExportNextBookmark(const VARIANT& propVal)
{
   SetProperty(0xaf, VT_VARIANT, &propVal);
}

long ITrueDBGridCtrl::GetAnimateWindow()
{
   long result;
   GetProperty(0xbd, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAnimateWindow(long propVal)
{
   SetProperty(0xbd, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetAnimateWindowDirection()
{
   long result;
   GetProperty(0xbe, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAnimateWindowDirection(long propVal)
{
   SetProperty(0xbe, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetAnimateWindowTime()
{
   long result;
   GetProperty(0xbf, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAnimateWindowTime(long propVal)
{
   SetProperty(0xbf, VT_I4, propVal);
}

long ITrueDBGridCtrl::GetAnimateWindowClose()
{
   long result;
   GetProperty(0xc0, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetAnimateWindowClose(long propVal)
{
   SetProperty(0xc0, VT_I4, propVal);
}

unsigned long ITrueDBGridCtrl::GetDeadAreaBackColor()
{
   unsigned long result;
   GetProperty(0xc4, VT_I4, (void*)&result);
   return result;
}

void ITrueDBGridCtrl::SetDeadAreaBackColor(unsigned long propVal)
{
   SetProperty(0xc4, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ITrueDBGridCtrl operations

void ITrueDBGridCtrl::AboutBox()
{
   InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH ITrueDBGridCtrl::GetColumns()
{
   LPDISPATCH result;
   InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH ITrueDBGridCtrl::GetSplits()
{
   LPDISPATCH result;
   InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH ITrueDBGridCtrl::GetStyles()
{
   LPDISPATCH result;
   InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH ITrueDBGridCtrl::GetSelBookmarks()
{
   LPDISPATCH result;
   InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

short ITrueDBGridCtrl::ColContaining(float X)
{
   short result;
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper(0x18, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      X);
   return result;
}

VARIANT ITrueDBGridCtrl::GetBookmark(long RowNum)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x19, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      RowNum);
   return result;
}

void ITrueDBGridCtrl::ReBind()
{
   InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

VARIANT ITrueDBGridCtrl::RowBookmark(short RowNum)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x1b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      RowNum);
   return result;
}

short ITrueDBGridCtrl::RowContaining(float Y)
{
   short result;
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      Y);
   return result;
}

float ITrueDBGridCtrl::RowTop(short RowNum)
{
   float result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x1d, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      RowNum);
   return result;
}

void ITrueDBGridCtrl::Scroll(long Cols, long Rows)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Cols, Rows);
}

void ITrueDBGridCtrl::Refresh()
{
   InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH ITrueDBGridCtrl::CaptureImage()
{
   LPDISPATCH result;
   InvokeHelper(0x24, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void ITrueDBGridCtrl::AddCellStyle(short Condition, const VARIANT& Style)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style);
}

void ITrueDBGridCtrl::AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT VTS_BSTR;
   InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style, Regex);
}

void ITrueDBGridCtrl::ClearCellStyle(short Condition)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x4e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition);
}

void ITrueDBGridCtrl::ClearRegexCellStyle(short Condition, const VARIANT& Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x4f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Regex);
}

void ITrueDBGridCtrl::ClearSelCols()
{
   InvokeHelper(0x4c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::ClearFields()
{
   InvokeHelper(0x4d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::HoldFields()
{
   InvokeHelper(0x59, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short ITrueDBGridCtrl::SplitContaining(float X, float Y)
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4;
   InvokeHelper(0x53, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      X, Y);
   return result;
}

long ITrueDBGridCtrl::PointAt(float X, float Y)
{
   long result;
   static BYTE parms[] =
      VTS_R4 VTS_R4;
   InvokeHelper(0xa5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      X, Y);
   return result;
}

void ITrueDBGridCtrl::PostMsg(short MsgId)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x5a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       MsgId);
}

void ITrueDBGridCtrl::Update()
{
   InvokeHelper(0x5b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::MoveRelative(long offset, const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_I4 VTS_VARIANT;
   InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       offset, &Bookmark);
}

void ITrueDBGridCtrl::MoveFirst()
{
   InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::MoveLast()
{
   InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::MoveNext()
{
   InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::MovePrevious()
{
   InvokeHelper(0x6a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::Delete()
{
   InvokeHelper(0x6d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::Close(const VARIANT& Repaint)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Repaint);
}

void ITrueDBGridCtrl::ReOpen(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void ITrueDBGridCtrl::LoadLayout()
{
   InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH ITrueDBGridCtrl::GetLayouts()
{
   LPDISPATCH result;
   InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL ITrueDBGridCtrl::CellContaining(float X, float Y, long* RowAt, short* ColAt)
{
   BOOL result;
   static BYTE parms[] =
      VTS_R4 VTS_R4 VTS_PI4 VTS_PI2;
   InvokeHelper(0x89, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      X, Y, RowAt, ColAt);
   return result;
}

void ITrueDBGridCtrl::RefreshRow(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x44, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void ITrueDBGridCtrl::RefreshCol(const VARIANT& Col)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Col);
}

void ITrueDBGridCtrl::RefetchRow(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x8a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void ITrueDBGridCtrl::RefetchCol(const VARIANT& Col)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x8b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Col);
}

void ITrueDBGridCtrl::OLEDrag()
{
   InvokeHelper(0xa1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short ITrueDBGridCtrl::IsSelected(const VARIANT& Bookmark)
{
   short result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0xa2, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      &Bookmark);
   return result;
}

void ITrueDBGridCtrl::ExportToFile(LPCTSTR outPath, BOOL append, const VARIANT& selector, const VARIANT& tableWidth)
{
   static BYTE parms[] =
      VTS_BSTR VTS_BOOL VTS_VARIANT VTS_VARIANT;
   InvokeHelper(0xaa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       outPath, append, &selector, &tableWidth);
}

void ITrueDBGridCtrl::ExportBegin(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0xab, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void ITrueDBGridCtrl::ExportRows(LPCTSTR outPath, BOOL append, long Rows, const VARIANT& tableWidth)
{
   static BYTE parms[] =
      VTS_BSTR VTS_BOOL VTS_I4 VTS_VARIANT;
   InvokeHelper(0xac, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       outPath, append, Rows, &tableWidth);
}

void ITrueDBGridCtrl::ExportEnd()
{
   InvokeHelper(0xad, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBGridCtrl::ExportToDelimitedFile(LPCTSTR outPath, const VARIANT& selector, const VARIANT& delim, const VARIANT& prefix, const VARIANT& suffix)
{
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
   InvokeHelper(0xa9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       outPath, &selector, &delim, &prefix, &suffix);
}

LPDISPATCH ITrueDBGridCtrl::GetPrintInfos()
{
   LPDISPATCH result;
   InvokeHelper(0xb4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH ITrueDBGridCtrl::GetPrintInfo()
{
   LPDISPATCH result;
   InvokeHelper(0xb5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Column properties

long Column::GetAlignment()
{
   long result;
   GetProperty(0x1, VT_I4, (void*)&result);
   return result;
}

void Column::SetAlignment(long propVal)
{
   SetProperty(0x1, VT_I4, propVal);
}

BOOL Column::GetAllowSizing()
{
   BOOL result;
   GetProperty(0x2, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetAllowSizing(BOOL propVal)
{
   SetProperty(0x2, VT_BOOL, propVal);
}

BOOL Column::GetHeaderDivider()
{
   BOOL result;
   GetProperty(0x47, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetHeaderDivider(BOOL propVal)
{
   SetProperty(0x47, VT_BOOL, propVal);
}

BOOL Column::GetFooterDivider()
{
   BOOL result;
   GetProperty(0x48, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetFooterDivider(BOOL propVal)
{
   SetProperty(0x48, VT_BOOL, propVal);
}

unsigned long Column::GetBackColor()
{
   unsigned long result;
   GetProperty(0x3, VT_I4, (void*)&result);
   return result;
}

void Column::SetBackColor(unsigned long propVal)
{
   SetProperty(0x3, VT_I4, propVal);
}

long Column::GetDividerStyle()
{
   long result;
   GetProperty(0x4, VT_I4, (void*)&result);
   return result;
}

void Column::SetDividerStyle(long propVal)
{
   SetProperty(0x4, VT_I4, propVal);
}

CString Column::GetCaption()
{
   CString result;
   GetProperty(0x5, VT_BSTR, (void*)&result);
   return result;
}

void Column::SetCaption(LPCTSTR propVal)
{
   SetProperty(0x5, VT_BSTR, propVal);
}

CString Column::GetFooterText()
{
   CString result;
   GetProperty(0x3a, VT_BSTR, (void*)&result);
   return result;
}

void Column::SetFooterText(LPCTSTR propVal)
{
   SetProperty(0x3a, VT_BSTR, propVal);
}

BOOL Column::GetDataChanged()
{
   BOOL result;
   GetProperty(0x23, VT_BOOL, (void*)&result);
   return result;
}

CString Column::GetDataField()
{
   CString result;
   GetProperty(0x8, VT_BSTR, (void*)&result);
   return result;
}

void Column::SetDataField(LPCTSTR propVal)
{
   SetProperty(0x8, VT_BSTR, propVal);
}

long Column::GetDataWidth()
{
   long result;
   GetProperty(0x24, VT_I4, (void*)&result);
   return result;
}

void Column::SetDataWidth(long propVal)
{
   SetProperty(0x24, VT_I4, propVal);
}

VARIANT Column::GetDefaultValue()
{
   VARIANT result;
   GetProperty(0x9, VT_VARIANT, (void*)&result);
   return result;
}

void Column::SetDefaultValue(const VARIANT& propVal)
{
   SetProperty(0x9, VT_VARIANT, &propVal);
}

unsigned long Column::GetEditBackColor()
{
   unsigned long result;
   GetProperty(0x21, VT_I4, (void*)&result);
   return result;
}

void Column::SetEditBackColor(unsigned long propVal)
{
   SetProperty(0x21, VT_I4, propVal);
}

unsigned long Column::GetEditForeColor()
{
   unsigned long result;
   GetProperty(0x22, VT_I4, (void*)&result);
   return result;
}

void Column::SetEditForeColor(unsigned long propVal)
{
   SetProperty(0x22, VT_I4, propVal);
}

unsigned long Column::GetForeColor()
{
   unsigned long result;
   GetProperty(0xa, VT_I4, (void*)&result);
   return result;
}

void Column::SetForeColor(unsigned long propVal)
{
   SetProperty(0xa, VT_I4, propVal);
}

long Column::GetHeadAlignment()
{
   long result;
   GetProperty(0x2a, VT_I4, (void*)&result);
   return result;
}

void Column::SetHeadAlignment(long propVal)
{
   SetProperty(0x2a, VT_I4, propVal);
}

unsigned long Column::GetHeadBackColor()
{
   unsigned long result;
   GetProperty(0xb, VT_I4, (void*)&result);
   return result;
}

void Column::SetHeadBackColor(unsigned long propVal)
{
   SetProperty(0xb, VT_I4, propVal);
}

unsigned long Column::GetHeadForeColor()
{
   unsigned long result;
   GetProperty(0xc, VT_I4, (void*)&result);
   return result;
}

void Column::SetHeadForeColor(unsigned long propVal)
{
   SetProperty(0xc, VT_I4, propVal);
}

long Column::GetFooterAlignment()
{
   long result;
   GetProperty(0x3f, VT_I4, (void*)&result);
   return result;
}

void Column::SetFooterAlignment(long propVal)
{
   SetProperty(0x3f, VT_I4, propVal);
}

unsigned long Column::GetFooterBackColor()
{
   unsigned long result;
   GetProperty(0x3b, VT_I4, (void*)&result);
   return result;
}

void Column::SetFooterBackColor(unsigned long propVal)
{
   SetProperty(0x3b, VT_I4, propVal);
}

unsigned long Column::GetFooterForeColor()
{
   unsigned long result;
   GetProperty(0x3c, VT_I4, (void*)&result);
   return result;
}

void Column::SetFooterForeColor(unsigned long propVal)
{
   SetProperty(0x3c, VT_I4, propVal);
}

short Column::GetColIndex()
{
   short result;
   GetProperty(0xe, VT_I2, (void*)&result);
   return result;
}

float Column::GetLeft()
{
   float result;
   GetProperty(0xf, VT_R4, (void*)&result);
   return result;
}

BOOL Column::GetLocked()
{
   BOOL result;
   GetProperty(0x10, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetLocked(BOOL propVal)
{
   SetProperty(0x10, VT_BOOL, propVal);
}

CString Column::GetNumberFormat()
{
   CString result;
   GetProperty(0x11, VT_BSTR, (void*)&result);
   return result;
}

void Column::SetNumberFormat(LPCTSTR propVal)
{
   SetProperty(0x11, VT_BSTR, propVal);
}

CString Column::GetText()
{
   CString result;
   GetProperty(0x0, VT_BSTR, (void*)&result);
   return result;
}

void Column::SetText(LPCTSTR propVal)
{
   SetProperty(0x0, VT_BSTR, propVal);
}

float Column::GetTop()
{
   float result;
   GetProperty(0x14, VT_R4, (void*)&result);
   return result;
}

VARIANT Column::GetValue()
{
   VARIANT result;
   GetProperty(0x15, VT_VARIANT, (void*)&result);
   return result;
}

void Column::SetValue(const VARIANT& propVal)
{
   SetProperty(0x15, VT_VARIANT, &propVal);
}

BOOL Column::GetVisible()
{
   BOOL result;
   GetProperty(0x16, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetVisible(BOOL propVal)
{
   SetProperty(0x16, VT_BOOL, propVal);
}

float Column::GetWidth()
{
   float result;
   GetProperty(0x17, VT_R4, (void*)&result);
   return result;
}

void Column::SetWidth(float propVal)
{
   SetProperty(0x17, VT_R4, propVal);
}

BOOL Column::GetWrapText()
{
   BOOL result;
   GetProperty(0x25, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetWrapText(BOOL propVal)
{
   SetProperty(0x25, VT_BOOL, propVal);
}

BOOL Column::GetButton()
{
   BOOL result;
   GetProperty(0x28, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetButton(BOOL propVal)
{
   SetProperty(0x28, VT_BOOL, propVal);
}

BOOL Column::GetMerge()
{
   BOOL result;
   GetProperty(0x33, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetMerge(BOOL propVal)
{
   SetProperty(0x33, VT_BOOL, propVal);
}

BOOL Column::GetFetchStyle()
{
   BOOL result;
   GetProperty(0x29, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetFetchStyle(BOOL propVal)
{
   SetProperty(0x29, VT_BOOL, propVal);
}

LPDISPATCH Column::GetStyle()
{
   LPDISPATCH result;
   GetProperty(0x1d, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetStyle(LPDISPATCH propVal)
{
   SetProperty(0x1d, VT_DISPATCH, propVal);
}

LPDISPATCH Column::GetHeadingStyle()
{
   LPDISPATCH result;
   GetProperty(0x1e, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetHeadingStyle(LPDISPATCH propVal)
{
   SetProperty(0x1e, VT_DISPATCH, propVal);
}

LPDISPATCH Column::GetFooterStyle()
{
   LPDISPATCH result;
   GetProperty(0x3d, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetFooterStyle(LPDISPATCH propVal)
{
   SetProperty(0x3d, VT_DISPATCH, propVal);
}

LPDISPATCH Column::GetEditorStyle()
{
   LPDISPATCH result;
   GetProperty(0x1f, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetEditorStyle(LPDISPATCH propVal)
{
   SetProperty(0x1f, VT_DISPATCH, propVal);
}

LPDISPATCH Column::GetFont()
{
   LPDISPATCH result;
   GetProperty(0x1c, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetFont(LPDISPATCH propVal)
{
   SetProperty(0x1c, VT_DISPATCH, propVal);
}

LPDISPATCH Column::GetHeadFont()
{
   LPDISPATCH result;
   GetProperty(0x20, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetHeadFont(LPDISPATCH propVal)
{
   SetProperty(0x20, VT_DISPATCH, propVal);
}

LPDISPATCH Column::GetFooterFont()
{
   LPDISPATCH result;
   GetProperty(0x3e, VT_DISPATCH, (void*)&result);
   return result;
}

void Column::SetFooterFont(LPDISPATCH propVal)
{
   SetProperty(0x3e, VT_DISPATCH, propVal);
}

short Column::GetOrder()
{
   short result;
   GetProperty(0x2b, VT_I2, (void*)&result);
   return result;
}

void Column::SetOrder(short propVal)
{
   SetProperty(0x2b, VT_I2, propVal);
}

CString Column::GetEditMask()
{
   CString result;
   GetProperty(0x2c, VT_BSTR, (void*)&result);
   return result;
}

void Column::SetEditMask(LPCTSTR propVal)
{
   SetProperty(0x2c, VT_BSTR, propVal);
}

BOOL Column::GetEditMaskUpdate()
{
   BOOL result;
   GetProperty(0x2d, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetEditMaskUpdate(BOOL propVal)
{
   SetProperty(0x2d, VT_BOOL, propVal);
}

BOOL Column::GetEditMaskRight()
{
   BOOL result;
   GetProperty(0x44, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetEditMaskRight(BOOL propVal)
{
   SetProperty(0x44, VT_BOOL, propVal);
}

VARIANT Column::GetDropDown()
{
   VARIANT result;
   GetProperty(0x2f, VT_VARIANT, (void*)&result);
   return result;
}

void Column::SetDropDown(const VARIANT& propVal)
{
   SetProperty(0x2f, VT_VARIANT, &propVal);
}

VARIANT Column::GetExternalEditor()
{
   VARIANT result;
   GetProperty(0x46, VT_VARIANT, (void*)&result);
   return result;
}

void Column::SetExternalEditor(const VARIANT& propVal)
{
   SetProperty(0x46, VT_VARIANT, &propVal);
}

float Column::GetCellTop()
{
   float result;
   GetProperty(0x30, VT_R4, (void*)&result);
   return result;
}

VARIANT Column::GetButtonPicture()
{
   VARIANT result;
   GetProperty(0x31, VT_VARIANT, (void*)&result);
   return result;
}

void Column::SetButtonPicture(const VARIANT& propVal)
{
   SetProperty(0x31, VT_VARIANT, &propVal);
}

BOOL Column::GetAllowFocus()
{
   BOOL result;
   GetProperty(0x32, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetAllowFocus(BOOL propVal)
{
   SetProperty(0x32, VT_BOOL, propVal);
}

BOOL Column::GetButtonText()
{
   BOOL result;
   GetProperty(0x34, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetButtonText(BOOL propVal)
{
   SetProperty(0x34, VT_BOOL, propVal);
}

BOOL Column::GetButtonAlways()
{
   BOOL result;
   GetProperty(0x35, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetButtonAlways(BOOL propVal)
{
   SetProperty(0x35, VT_BOOL, propVal);
}

BOOL Column::GetButtonHeader()
{
   BOOL result;
   GetProperty(0x40, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetButtonHeader(BOOL propVal)
{
   SetProperty(0x40, VT_BOOL, propVal);
}

BOOL Column::GetButtonFooter()
{
   BOOL result;
   GetProperty(0x41, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetButtonFooter(BOOL propVal)
{
   SetProperty(0x41, VT_BOOL, propVal);
}

BOOL Column::GetOwnerDraw()
{
   BOOL result;
   GetProperty(0x42, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetOwnerDraw(BOOL propVal)
{
   SetProperty(0x42, VT_BOOL, propVal);
}

BOOL Column::GetAutoDropDown()
{
   BOOL result;
   GetProperty(0x43, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetAutoDropDown(BOOL propVal)
{
   SetProperty(0x43, VT_BOOL, propVal);
}

BOOL Column::GetDropDownList()
{
   BOOL result;
   GetProperty(0x45, VT_BOOL, (void*)&result);
   return result;
}

void Column::SetDropDownList(BOOL propVal)
{
   SetProperty(0x45, VT_BOOL, propVal);
}

long Column::GetDisplayAlignment()
{
   long result;
   GetProperty(0x49, VT_I4, (void*)&result);
   return result;
}

long Column::GetConvertEmptyCell()
{
   long result;
   GetProperty(0x4c, VT_I4, (void*)&result);
   return result;
}

void Column::SetConvertEmptyCell(long propVal)
{
   SetProperty(0x4c, VT_I4, propVal);
}

CString Column::GetName()
{
   CString result;
   GetProperty(0x4a, VT_BSTR, (void*)&result);
   return result;
}

CString Column::GetTableName()
{
   CString result;
   GetProperty(0x4b, VT_BSTR, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// Column operations

LPDISPATCH Column::GetValueItems()
{
   LPDISPATCH result;
   InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

ValueItems Column::GetListOfValues()
{
   return ValueItems( GetValueItems() );
}


CString Column::CellText(const VARIANT& Bookmark)
{
   CString result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      &Bookmark);
   return result;
}

VARIANT Column::CellValue(const VARIANT& Bookmark)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      &Bookmark);
   return result;
}

void Column::AddCellStyle(short Condition, const VARIANT& Style)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style);
}

void Column::AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT VTS_BSTR;
   InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style, Regex);
}

void Column::ClearCellStyle(short Condition)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition);
}

void Column::ClearRegexCellStyle(short Condition, const VARIANT& Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Regex);
}

void Column::Refresh()
{
   InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Column::RefreshCell(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void Column::Refetch()
{
   InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Column::RefetchCell(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void Column::AutoSize()
{
   InvokeHelper(0x4f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Columns properties

long Columns::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// Columns operations

LPUNKNOWN Columns::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

LPDISPATCH Columns::GetItem(const VARIANT& Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index);
   return result;
}

LPDISPATCH Columns::Add(short Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

void Columns::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}


// Helper functions not generated
Column Columns::GetColumn( short Index )
{
   COleVariant vIndex( Index, VT_I2 );

   return( Column( GetItem( vIndex ) ) );
}

/////////////////////////////////////////////////////////////////////////////
// SelBookmarks properties

long SelBookmarks::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// SelBookmarks operations

LPUNKNOWN SelBookmarks::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

VARIANT SelBookmarks::GetItem(const VARIANT& Index)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      &Index);
   return result;
}

void SelBookmarks::Add(const VARIANT& Bookmark)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Bookmark);
}

void SelBookmarks::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}

void SelBookmarks::Remove( zLONG Index )
{
   COleVariant v;
   v = Index;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &v);
}

/////////////////////////////////////////////////////////////////////////////
// RowBuffer properties

/////////////////////////////////////////////////////////////////////////////
// RowBuffer operations

long RowBuffer::GetRowCount()
{
   long result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void RowBuffer::SetRowCount(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

short RowBuffer::GetColumnCount()
{
   short result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
   return result;
}

VARIANT RowBuffer::GetBookmark(long Row)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      Row);
   return result;
}

void RowBuffer::SetBookmark(long Row, const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_VARIANT;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Row, &newValue);
}

CString RowBuffer::GetColumnName(short Col)
{
   CString result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
      Col);
   return result;
}

VARIANT RowBuffer::GetValue(long Row, short Col)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I4 VTS_I2;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      Row, Col);
   return result;
}

void RowBuffer::SetValue(long Row, short Col, const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_I2 VTS_VARIANT;
   InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Row, Col, &newValue);
}

short RowBuffer::GetColumnIndex(long Row, short Col)
{
   short result;
   static BYTE parms[] =
      VTS_I4 VTS_I2;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, parms,
      Row, Col);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// DataObjectFiles properties

/////////////////////////////////////////////////////////////////////////////
// DataObjectFiles operations

long DataObjectFiles::GetCount()
{
   long result;
   InvokeHelper(0x7d2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPUNKNOWN DataObjectFiles::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

CString DataObjectFiles::GetItem(long Index)
{
   CString result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
      Index);
   return result;
}

void DataObjectFiles::Add(LPCTSTR Filename)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xfffffdd7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Filename);
}

void DataObjectFiles::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0xfffffdd5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}

void DataObjectFiles::Clear()
{
   InvokeHelper(0xfffffdd6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// DataObject properties

/////////////////////////////////////////////////////////////////////////////
// DataObject operations

void DataObject::Clear()
{
   InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH DataObject::GetFiles()
{
   LPDISPATCH result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

VARIANT DataObject::GetData(short Format)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      Format);
   return result;
}

BOOL DataObject::GetFormat(short Format)
{
   BOOL result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      Format);
   return result;
}

void DataObject::SetData(const VARIANT& Value, const VARIANT& Format)
{
   static BYTE parms[] =
      VTS_VARIANT VTS_VARIANT;
   InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Value, &Format);
}


/////////////////////////////////////////////////////////////////////////////
// Layouts properties

long Layouts::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// Layouts operations

LPUNKNOWN Layouts::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

VARIANT Layouts::GetItem(const VARIANT& Index)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      &Index);
   return result;
}

void Layouts::Add(const VARIANT& LayoutName)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &LayoutName);
}

void Layouts::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}



/////////////////////////////////////////////////////////////////////////////
// Splits properties

long Splits::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// Splits operations

LPUNKNOWN Splits::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

LPDISPATCH Splits::GetItem(const VARIANT& Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index);
   return result;
}

LPDISPATCH Splits::GetItem( zLONG lIndex )
{
   COleVariant vIndex( lIndex );
   return GetItem( vIndex );
}

// Helper functions not generated
Split
Splits::GetSplit( short Index )
{
   COleVariant vIndex( Index, VT_I2 );

   return( Split( GetItem( vIndex ) ) );
}


LPDISPATCH Splits::Add(short Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

void Splits::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}


/////////////////////////////////////////////////////////////////////////////
// Split properties

unsigned long Split::GetBackColor()
{
   unsigned long result;
   GetProperty(0x15, VT_I4, (void*)&result);
   return result;
}

void Split::SetBackColor(unsigned long propVal)
{
   SetProperty(0x15, VT_I4, propVal);
}

unsigned long Split::GetForeColor()
{
   unsigned long result;
   GetProperty(0x16, VT_I4, (void*)&result);
   return result;
}

void Split::SetForeColor(unsigned long propVal)
{
   SetProperty(0x16, VT_I4, propVal);
}

unsigned long Split::GetHeadBackColor()
{
   unsigned long result;
   GetProperty(0x17, VT_I4, (void*)&result);
   return result;
}

void Split::SetHeadBackColor(unsigned long propVal)
{
   SetProperty(0x17, VT_I4, propVal);
}

unsigned long Split::GetHeadForeColor()
{
   unsigned long result;
   GetProperty(0x18, VT_I4, (void*)&result);
   return result;
}

void Split::SetHeadForeColor(unsigned long propVal)
{
   SetProperty(0x18, VT_I4, propVal);
}

unsigned long Split::GetFooterBackColor()
{
   unsigned long result;
   GetProperty(0x38, VT_I4, (void*)&result);
   return result;
}

void Split::SetFooterBackColor(unsigned long propVal)
{
   SetProperty(0x38, VT_I4, propVal);
}

unsigned long Split::GetFooterForeColor()
{
   unsigned long result;
   GetProperty(0x39, VT_I4, (void*)&result);
   return result;
}

void Split::SetFooterForeColor(unsigned long propVal)
{
   SetProperty(0x39, VT_I4, propVal);
}

unsigned long Split::GetEditBackColor()
{
   unsigned long result;
   GetProperty(0x19, VT_I4, (void*)&result);
   return result;
}

void Split::SetEditBackColor(unsigned long propVal)
{
   SetProperty(0x19, VT_I4, propVal);
}

unsigned long Split::GetEditForeColor()
{
   unsigned long result;
   GetProperty(0x1a, VT_I4, (void*)&result);
   return result;
}

void Split::SetEditForeColor(unsigned long propVal)
{
   SetProperty(0x1a, VT_I4, propVal);
}

BOOL Split::GetExtendRightColumn()
{
   BOOL result;
   GetProperty(0x20, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetExtendRightColumn(BOOL propVal)
{
   SetProperty(0x20, VT_BOOL, propVal);
}

BOOL Split::GetPartialRightColumn()
{
   BOOL result;
   GetProperty(0x3a, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetPartialRightColumn(BOOL propVal)
{
   SetProperty(0x3a, VT_BOOL, propVal);
}

BOOL Split::GetAnchorRightColumn()
{
   BOOL result;
   GetProperty(0x3b, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAnchorRightColumn(BOOL propVal)
{
   SetProperty(0x3b, VT_BOOL, propVal);
}

unsigned long Split::GetInactiveBackColor()
{
   unsigned long result;
   GetProperty(0x1b, VT_I4, (void*)&result);
   return result;
}

void Split::SetInactiveBackColor(unsigned long propVal)
{
   SetProperty(0x1b, VT_I4, propVal);
}

unsigned long Split::GetInactiveForeColor()
{
   unsigned long result;
   GetProperty(0x1c, VT_I4, (void*)&result);
   return result;
}

void Split::SetInactiveForeColor(unsigned long propVal)
{
   SetProperty(0x1c, VT_I4, propVal);
}

unsigned long Split::GetSelectedBackColor()
{
   unsigned long result;
   GetProperty(0x1d, VT_I4, (void*)&result);
   return result;
}

void Split::SetSelectedBackColor(unsigned long propVal)
{
   SetProperty(0x1d, VT_I4, propVal);
}

unsigned long Split::GetSelectedForeColor()
{
   unsigned long result;
   GetProperty(0x1e, VT_I4, (void*)&result);
   return result;
}

void Split::SetSelectedForeColor(unsigned long propVal)
{
   SetProperty(0x1e, VT_I4, propVal);
}

LPDISPATCH Split::GetFont()
{
   LPDISPATCH result;
   GetProperty(0xb, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetFont(LPDISPATCH propVal)
{
   SetProperty(0xb, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetHeadFont()
{
   LPDISPATCH result;
   GetProperty(0x14, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetHeadFont(LPDISPATCH propVal)
{
   SetProperty(0x14, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetFooterFont()
{
   LPDISPATCH result;
   GetProperty(0x37, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetFooterFont(LPDISPATCH propVal)
{
   SetProperty(0x37, VT_DISPATCH, propVal);
}

BOOL Split::GetLocked()
{
   BOOL result;
   GetProperty(0x1, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetLocked(BOOL propVal)
{
   SetProperty(0x1, VT_BOOL, propVal);
}

short Split::GetIndex()
{
   short result;
   GetProperty(0x2, VT_I2, (void*)&result);
   return result;
}

short Split::GetScrollGroup()
{
   short result;
   GetProperty(0x3, VT_I2, (void*)&result);
   return result;
}

void Split::SetScrollGroup(short propVal)
{
   SetProperty(0x3, VT_I2, propVal);
}

long Split::GetMarqueeStyle()
{
   long result;
   GetProperty(0x1f, VT_I4, (void*)&result);
   return result;
}

void Split::SetMarqueeStyle(long propVal)
{
   SetProperty(0x1f, VT_I4, propVal);
}

BOOL Split::GetAllowSizing()
{
   BOOL result;
   GetProperty(0x4, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAllowSizing(BOOL propVal)
{
   SetProperty(0x4, VT_BOOL, propVal);
}

BOOL Split::GetAllowFocus()
{
   BOOL result;
   GetProperty(0x5, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAllowFocus(BOOL propVal)
{
   SetProperty(0x5, VT_BOOL, propVal);
}

long Split::GetSizeMode()
{
   long result;
   GetProperty(0x7, VT_I4, (void*)&result);
   return result;
}

void Split::SetSizeMode(long propVal)
{
   SetProperty(0x7, VT_I4, propVal);
}

VARIANT Split::GetSize()
{
   VARIANT result;
   GetProperty(0x6, VT_VARIANT, (void*)&result);
   return result;
}

void Split::SetSize(const VARIANT& propVal)
{
   SetProperty(0x6, VT_VARIANT, &propVal);
}

BOOL Split::GetAllowRowSizing()
{
   BOOL result;
   GetProperty(0xf, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAllowRowSizing(BOOL propVal)
{
   SetProperty(0xf, VT_BOOL, propVal);
}

BOOL Split::GetRecordSelectors()
{
   BOOL result;
   GetProperty(0x10, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetRecordSelectors(BOOL propVal)
{
   SetProperty(0x10, VT_BOOL, propVal);
}

long Split::GetScrollBars()
{
   long result;
   GetProperty(0x11, VT_I4, (void*)&result);
   return result;
}

void Split::SetScrollBars(long propVal)
{
   SetProperty(0x11, VT_I4, propVal);
}

LPDISPATCH Split::GetStyle()
{
   LPDISPATCH result;
   GetProperty(0xc, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetStyle(LPDISPATCH propVal)
{
   SetProperty(0xc, VT_DISPATCH, propVal);
}

BOOL Split::GetCurrentCellVisible()
{
   BOOL result;
   GetProperty(0x21, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetCurrentCellVisible(BOOL propVal)
{
   SetProperty(0x21, VT_BOOL, propVal);
}

VARIANT Split::GetFirstRow()
{
   VARIANT result;
   GetProperty(0x22, VT_VARIANT, (void*)&result);
   return result;
}

void Split::SetFirstRow(const VARIANT& propVal)
{
   SetProperty(0x22, VT_VARIANT, &propVal);
}

short Split::GetLeftCol()
{
   short result;
   GetProperty(0x23, VT_I2, (void*)&result);
   return result;
}

void Split::SetLeftCol(short propVal)
{
   SetProperty(0x23, VT_I2, propVal);
}

LPDISPATCH Split::GetHeadingStyle()
{
   LPDISPATCH result;
   GetProperty(0xd, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetHeadingStyle(LPDISPATCH propVal)
{
   SetProperty(0xd, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetFooterStyle()
{
   LPDISPATCH result;
   GetProperty(0x36, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetFooterStyle(LPDISPATCH propVal)
{
   SetProperty(0x36, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetEditorStyle()
{
   LPDISPATCH result;
   GetProperty(0xe, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetEditorStyle(LPDISPATCH propVal)
{
   SetProperty(0xe, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetInactiveStyle()
{
   LPDISPATCH result;
   GetProperty(0x12, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetInactiveStyle(LPDISPATCH propVal)
{
   SetProperty(0x12, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetSelectedStyle()
{
   LPDISPATCH result;
   GetProperty(0x13, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetSelectedStyle(LPDISPATCH propVal)
{
   SetProperty(0x13, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetHighlightRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x2e, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetHighlightRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x2e, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetEvenRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x30, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetEvenRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x30, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetOddRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x31, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetOddRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x31, VT_DISPATCH, propVal);
}

LPDISPATCH Split::GetCaptionStyle()
{
   LPDISPATCH result;
   GetProperty(0x34, VT_DISPATCH, (void*)&result);
   return result;
}

void Split::SetCaptionStyle(LPDISPATCH propVal)
{
   SetProperty(0x34, VT_DISPATCH, propVal);
}

short Split::GetSelStartCol()
{
   short result;
   GetProperty(0x24, VT_I2, (void*)&result);
   return result;
}

void Split::SetSelStartCol(short propVal)
{
   SetProperty(0x24, VT_I2, propVal);
}

short Split::GetSelEndCol()
{
   short result;
   GetProperty(0x25, VT_I2, (void*)&result);
   return result;
}

void Split::SetSelEndCol(short propVal)
{
   SetProperty(0x25, VT_I2, propVal);
}

BOOL Split::GetAllowColSelect()
{
   BOOL result;
   GetProperty(0x29, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAllowColSelect(BOOL propVal)
{
   SetProperty(0x29, VT_BOOL, propVal);
}

BOOL Split::GetAllowColMove()
{
   BOOL result;
   GetProperty(0x2a, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAllowColMove(BOOL propVal)
{
   SetProperty(0x2a, VT_BOOL, propVal);
}

BOOL Split::GetAllowRowSelect()
{
   BOOL result;
   GetProperty(0x2b, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAllowRowSelect(BOOL propVal)
{
   SetProperty(0x2b, VT_BOOL, propVal);
}

float Split::GetVScrollWidth()
{
   float result;
   GetProperty(0x2c, VT_R4, (void*)&result);
   return result;
}

float Split::GetHScrollHeight()
{
   float result;
   GetProperty(0x2d, VT_R4, (void*)&result);
   return result;
}

BOOL Split::GetFetchRowStyle()
{
   BOOL result;
   GetProperty(0x2f, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetFetchRowStyle(BOOL propVal)
{
   SetProperty(0x2f, VT_BOOL, propVal);
}

BOOL Split::GetAlternatingRowStyle()
{
   BOOL result;
   GetProperty(0x32, VT_BOOL, (void*)&result);
   return result;
}

void Split::SetAlternatingRowStyle(BOOL propVal)
{
   SetProperty(0x32, VT_BOOL, propVal);
}

CString Split::GetCaption()
{
   CString result;
   GetProperty(0x33, VT_BSTR, (void*)&result);
   return result;
}

void Split::SetCaption(LPCTSTR propVal)
{
   SetProperty(0x33, VT_BSTR, propVal);
}

long Split::GetDividerStyle()
{
   long result;
   GetProperty(0x35, VT_I4, (void*)&result);
   return result;
}

void Split::SetDividerStyle(long propVal)
{
   SetProperty(0x35, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Split operations

LPDISPATCH Split::GetColumns()
{
   LPDISPATCH result;
   InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void Split::AddCellStyle(short Condition, const VARIANT& Style)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style);
}

void Split::AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT VTS_BSTR;
   InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style, Regex);
}

void Split::ClearCellStyle(short Condition)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition);
}

void Split::ClearRegexCellStyle(short Condition, const VARIANT& Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Regex);
}

void Split::ClearSelCols()
{
   InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ValueItemDisp properties

VARIANT ValueItemDisp::GetValue()
{
   VARIANT result;
   GetProperty(0x1, VT_VARIANT, (void*)&result);
   return result;
}

void ValueItemDisp::SetValue(const VARIANT& propVal)
{
   SetProperty(0x1, VT_VARIANT, &propVal);
}

VARIANT ValueItemDisp::GetDisplayValue()
{
   VARIANT result;
   GetProperty(0x2, VT_VARIANT, (void*)&result);
   return result;
}

void ValueItemDisp::SetDisplayValue(const VARIANT& propVal)
{
   SetProperty(0x2, VT_VARIANT, &propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ValueItemDisp operations


/////////////////////////////////////////////////////////////////////////////
// ValueItems properties

long ValueItems::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

long ValueItems::GetPresentation()
{
   long result;
   GetProperty(0x1, VT_I4, (void*)&result);
   return result;
}

void ValueItems::SetPresentation(long propVal)
{
   SetProperty(0x1, VT_I4, propVal);
}

BOOL ValueItems::GetTranslate()
{
   BOOL result;
   GetProperty(0x2, VT_BOOL, (void*)&result);
   return result;
}

void ValueItems::SetTranslate(BOOL propVal)
{
   SetProperty(0x2, VT_BOOL, propVal);
}

BOOL ValueItems::GetValidate()
{
   BOOL result;
   GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}

void ValueItems::SetValidate(BOOL propVal)
{
   SetProperty(0x3, VT_BOOL, propVal);
}

BOOL ValueItems::GetCycleOnClick()
{
   BOOL result;
   GetProperty(0x4, VT_BOOL, (void*)&result);
   return result;
}

void ValueItems::SetCycleOnClick(BOOL propVal)
{
   SetProperty(0x4, VT_BOOL, propVal);
}

short ValueItems::GetMaxComboItems()
{
   short result;
   GetProperty(0x6, VT_I2, (void*)&result);
   return result;
}

void ValueItems::SetMaxComboItems(short propVal)
{
   SetProperty(0x6, VT_I2, propVal);
}

short ValueItems::GetDefaultItem()
{
   short result;
   GetProperty(0x7, VT_I2, (void*)&result);
   return result;
}

void ValueItems::SetDefaultItem(short propVal)
{
   SetProperty(0x7, VT_I2, propVal);
}

BOOL ValueItems::GetAnnotatePicture()
{
   BOOL result;
   GetProperty(0x8, VT_BOOL, (void*)&result);
   return result;
}

void ValueItems::SetAnnotatePicture(BOOL propVal)
{
   SetProperty(0x8, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ValueItems operations

LPUNKNOWN ValueItems::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

LPDISPATCH ValueItems::GetItem(const VARIANT& Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index);
   return result;
}

ValueItemDisp ValueItems::GetValueItem(long nIndex)
{
   COleVariant v;
   v = nIndex;
   return ( ValueItemDisp( GetItem( v ) ) );
}


void ValueItems::Add(LPDISPATCH vitem)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       vitem);
}

void ValueItems::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}

void ValueItems::Clear()
{
   InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Styles properties

long Styles::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// Styles operations

LPUNKNOWN Styles::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

LPDISPATCH Styles::GetItem(const VARIANT& Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index);
   return result;
}

LPDISPATCH Styles::Add(LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name);
   return result;
}

void Styles::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}


/////////////////////////////////////////////////////////////////////////////
// StyleDisp properties

unsigned long StyleDisp::GetForeColor()
{
   unsigned long result;
   GetProperty(0xb, VT_I4, (void*)&result);
   return result;
}

void StyleDisp::SetForeColor(unsigned long propVal)
{
   SetProperty(0xb, VT_I4, propVal);
}

unsigned long StyleDisp::GetBackColor()
{
   unsigned long result;
   GetProperty(0xc, VT_I4, (void*)&result);
   return result;
}

void StyleDisp::SetBackColor(unsigned long propVal)
{
   SetProperty(0xc, VT_I4, propVal);
}

LPDISPATCH StyleDisp::GetFont()
{
   LPDISPATCH result;
   GetProperty(0x1, VT_DISPATCH, (void*)&result);
   return result;
}

void StyleDisp::SetFont(LPDISPATCH propVal)
{
   SetProperty(0x1, VT_DISPATCH, propVal);
}

long StyleDisp::GetAlignment()
{
   long result;
   GetProperty(0x2, VT_I4, (void*)&result);
   return result;
}

void StyleDisp::SetAlignment(long propVal)
{
   SetProperty(0x2, VT_I4, propVal);
}

long StyleDisp::GetVerticalAlignment()
{
   long result;
   GetProperty(0x9, VT_I4, (void*)&result);
   return result;
}

void StyleDisp::SetVerticalAlignment(long propVal)
{
   SetProperty(0x9, VT_I4, propVal);
}

BOOL StyleDisp::GetLocked()
{
   BOOL result;
   GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}

void StyleDisp::SetLocked(BOOL propVal)
{
   SetProperty(0x3, VT_BOOL, propVal);
}

CString StyleDisp::GetName()
{
   CString result;
   GetProperty(0x4, VT_BSTR, (void*)&result);
   return result;
}

VARIANT StyleDisp::GetParent()
{
   VARIANT result;
   GetProperty(0x8, VT_VARIANT, (void*)&result);
   return result;
}

void StyleDisp::SetParent(const VARIANT& propVal)
{
   SetProperty(0x8, VT_VARIANT, &propVal);
}

VARIANT StyleDisp::GetValue()
{
   VARIANT result;
   GetProperty(0x0, VT_VARIANT, (void*)&result);
   return result;
}

void StyleDisp::SetValue(const VARIANT& propVal)
{
   SetProperty(0x0, VT_VARIANT, &propVal);
}

BOOL StyleDisp::GetWrapText()
{
   BOOL result;
   GetProperty(0xa, VT_BOOL, (void*)&result);
   return result;
}

void StyleDisp::SetWrapText(BOOL propVal)
{
   SetProperty(0xa, VT_BOOL, propVal);
}

BOOL StyleDisp::GetTransparentForegroundPicture()
{
   BOOL result;
   GetProperty(0xe, VT_BOOL, (void*)&result);
   return result;
}

void StyleDisp::SetTransparentForegroundPicture(BOOL propVal)
{
   SetProperty(0xe, VT_BOOL, propVal);
}

VARIANT StyleDisp::GetForegroundPicture()
{
   VARIANT result;
   GetProperty(0xf, VT_VARIANT, (void*)&result);
   return result;
}

void StyleDisp::SetForegroundPicture(const VARIANT& propVal)
{
   SetProperty(0xf, VT_VARIANT, &propVal);
}

VARIANT StyleDisp::GetBackgroundPicture()
{
   VARIANT result;
   GetProperty(0x10, VT_VARIANT, (void*)&result);
   return result;
}

void StyleDisp::SetBackgroundPicture(const VARIANT& propVal)
{
   SetProperty(0x10, VT_VARIANT, &propVal);
}

long StyleDisp::GetForegroundPicturePosition()
{
   long result;
   GetProperty(0x11, VT_I4, (void*)&result);
   return result;
}

void StyleDisp::SetForegroundPicturePosition(long propVal)
{
   SetProperty(0x11, VT_I4, propVal);
}

long StyleDisp::GetBackgroundPictureDrawMode()
{
   long result;
   GetProperty(0x12, VT_I4, (void*)&result);
   return result;
}

void StyleDisp::SetBackgroundPictureDrawMode(long propVal)
{
   SetProperty(0x12, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// StyleDisp operations

void StyleDisp::Reset()
{
   InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// PrintInfo properties

CString PrintInfo::GetName()
{
   CString result;
   GetProperty(0x1, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetName(LPCTSTR propVal)
{
   SetProperty(0x1, VT_BSTR, propVal);
}

CString PrintInfo::GetPageHeader()
{
   CString result;
   GetProperty(0x2, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetPageHeader(LPCTSTR propVal)
{
   SetProperty(0x2, VT_BSTR, propVal);
}

CString PrintInfo::GetPageFooter()
{
   CString result;
   GetProperty(0x3, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetPageFooter(LPCTSTR propVal)
{
   SetProperty(0x3, VT_BSTR, propVal);
}

LPDISPATCH PrintInfo::GetPageHeaderFont()
{
   LPDISPATCH result;
   GetProperty(0x4, VT_DISPATCH, (void*)&result);
   return result;
}

void PrintInfo::SetPageHeaderFont(LPDISPATCH propVal)
{
   SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH PrintInfo::GetPageFooterFont()
{
   LPDISPATCH result;
   GetProperty(0x5, VT_DISPATCH, (void*)&result);
   return result;
}

void PrintInfo::SetPageFooterFont(LPDISPATCH propVal)
{
   SetProperty(0x5, VT_DISPATCH, propVal);
}

BOOL PrintInfo::GetDraft()
{
   BOOL result;
   GetProperty(0x6, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetDraft(BOOL propVal)
{
   SetProperty(0x6, VT_BOOL, propVal);
}

BOOL PrintInfo::GetDefault()
{
   BOOL result;
   GetProperty(0x7, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetDefault(BOOL propVal)
{
   SetProperty(0x7, VT_BOOL, propVal);
}

BOOL PrintInfo::GetRepeatGridHeader()
{
   BOOL result;
   GetProperty(0x8, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetRepeatGridHeader(BOOL propVal)
{
   SetProperty(0x8, VT_BOOL, propVal);
}

BOOL PrintInfo::GetRepeatSplitHeaders()
{
   BOOL result;
   GetProperty(0x9, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetRepeatSplitHeaders(BOOL propVal)
{
   SetProperty(0x9, VT_BOOL, propVal);
}

BOOL PrintInfo::GetRepeatColumnHeaders()
{
   BOOL result;
   GetProperty(0xa, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetRepeatColumnHeaders(BOOL propVal)
{
   SetProperty(0xa, VT_BOOL, propVal);
}

BOOL PrintInfo::GetRepeatColumnFooters()
{
   BOOL result;
   GetProperty(0xb, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetRepeatColumnFooters(BOOL propVal)
{
   SetProperty(0xb, VT_BOOL, propVal);
}

BOOL PrintInfo::GetVariableRowHeight()
{
   BOOL result;
   GetProperty(0xc, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetVariableRowHeight(BOOL propVal)
{
   SetProperty(0xc, VT_BOOL, propVal);
}

short PrintInfo::GetNumberOfCopies()
{
   short result;
   GetProperty(0xd, VT_I2, (void*)&result);
   return result;
}

void PrintInfo::SetNumberOfCopies(short propVal)
{
   SetProperty(0xd, VT_I2, propVal);
}

BOOL PrintInfo::GetCollate()
{
   BOOL result;
   GetProperty(0xe, VT_BOOL, (void*)&result);
   return result;
}

void PrintInfo::SetCollate(BOOL propVal)
{
   SetProperty(0xe, VT_BOOL, propVal);
}

CString PrintInfo::GetPreviewCaption()
{
   CString result;
   GetProperty(0xf, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetPreviewCaption(LPCTSTR propVal)
{
   SetProperty(0xf, VT_BSTR, propVal);
}

CString PrintInfo::GetPreviewPageOf()
{
   CString result;
   GetProperty(0x10, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetPreviewPageOf(LPCTSTR propVal)
{
   SetProperty(0x10, VT_BSTR, propVal);
}

CString PrintInfo::GetRangeOfPages()
{
   CString result;
   GetProperty(0x17, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetRangeOfPages(LPCTSTR propVal)
{
   SetProperty(0x17, VT_BSTR, propVal);
}

VARIANT PrintInfo::GetSettings()
{
   VARIANT result;
   GetProperty(0x19, VT_VARIANT, (void*)&result);
   return result;
}

void PrintInfo::SetSettings(const VARIANT& propVal)
{
   SetProperty(0x19, VT_VARIANT, &propVal);
}

CString PrintInfo::GetSettingsDeviceName()
{
   CString result;
   GetProperty(0x1c, VT_BSTR, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsDeviceName(LPCTSTR propVal)
{
   SetProperty(0x1c, VT_BSTR, propVal);
}

short PrintInfo::GetSettingsOrientation()
{
   short result;
   GetProperty(0x1d, VT_I2, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsOrientation(short propVal)
{
   SetProperty(0x1d, VT_I2, propVal);
}

long PrintInfo::GetSettingsMarginLeft()
{
   long result;
   GetProperty(0x1e, VT_I4, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsMarginLeft(long propVal)
{
   SetProperty(0x1e, VT_I4, propVal);
}

long PrintInfo::GetSettingsMarginTop()
{
   long result;
   GetProperty(0x1f, VT_I4, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsMarginTop(long propVal)
{
   SetProperty(0x1f, VT_I4, propVal);
}

long PrintInfo::GetSettingsMarginRight()
{
   long result;
   GetProperty(0x20, VT_I4, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsMarginRight(long propVal)
{
   SetProperty(0x20, VT_I4, propVal);
}

long PrintInfo::GetSettingsMarginBottom()
{
   long result;
   GetProperty(0x21, VT_I4, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsMarginBottom(long propVal)
{
   SetProperty(0x21, VT_I4, propVal);
}

short PrintInfo::GetSettingsPaperSize()
{
   short result;
   GetProperty(0x22, VT_I2, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsPaperSize(short propVal)
{
   SetProperty(0x22, VT_I2, propVal);
}

long PrintInfo::GetSettingsPaperWidth()
{
   long result;
   GetProperty(0x23, VT_I4, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsPaperWidth(long propVal)
{
   SetProperty(0x23, VT_I4, propVal);
}

long PrintInfo::GetSettingsPaperHeight()
{
   long result;
   GetProperty(0x24, VT_I4, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsPaperHeight(long propVal)
{
   SetProperty(0x24, VT_I4, propVal);
}

short PrintInfo::GetSettingsPaperBin()
{
   short result;
   GetProperty(0x25, VT_I2, (void*)&result);
   return result;
}

void PrintInfo::SetSettingsPaperBin(short propVal)
{
   SetProperty(0x25, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// PrintInfo operations

void PrintInfo::PrintData(const VARIANT& selector)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &selector);
}

void PrintInfo::PrintPreview(const VARIANT& selector)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &selector);
}

void PrintInfo::PageSetup()
{
   InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void PrintInfo::SetMenuText(long Index, LPCTSTR Text)
{
   static BYTE parms[] =
      VTS_I4 VTS_BSTR;
   InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Index, Text);
}

CString PrintInfo::GetMenuText(long Index)
{
   CString result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x16, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      Index);
   return result;
}

void PrintInfo::SubstituteFont(LPCTSTR UnavailableFont, const VARIANT& AvailableFont)
{
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT;
   InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       UnavailableFont, &AvailableFont);
}

CString PrintInfo::GetSubstituteFont(LPCTSTR UnavailableFont)
{
   CString result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x1b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      UnavailableFont);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// PrintInfos properties

long PrintInfos::GetCount()
{
   long result;
   GetProperty(0x7d2, VT_I4, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// PrintInfos operations

LPUNKNOWN PrintInfos::Get_NewEnum()
{
   LPUNKNOWN result;
   InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
   return result;
}

LPDISPATCH PrintInfos::GetItem(const VARIANT& Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index);
   return result;
}

LPDISPATCH PrintInfos::Add(LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x7d5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name);
   return result;
}

void PrintInfos::Remove(const VARIANT& Index)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x7d6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &Index);
}

void PrintInfos::Clear()
{
   InvokeHelper(0xfffffdd6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ITrueDBDropDownCtrl properties
#ifdef TB_DROPDOWN_IMPLEMENT
BOOL ITrueDBDropDownCtrl::GetAllowRowSizing()
{
   BOOL result;
   GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetAllowRowSizing(BOOL propVal)
{
   SetProperty(0x3, VT_BOOL, propVal);
}

short ITrueDBDropDownCtrl::GetAppearance()
{
   short result;
   GetProperty(DISPID_APPEARANCE, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetAppearance(short propVal)
{
   SetProperty(DISPID_APPEARANCE, VT_I4, propVal);
}

short ITrueDBDropDownCtrl::GetBorderStyle()
{
   short result;
   GetProperty(DISPID_BORDERSTYLE, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetBorderStyle(short propVal)
{
   SetProperty(DISPID_BORDERSTYLE, VT_I4, propVal);
}

VARIANT ITrueDBDropDownCtrl::GetBookmark()
{
   VARIANT result;
   GetProperty(0x5, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetBookmark(const VARIANT& propVal)
{
   SetProperty(0x5, VT_VARIANT, &propVal);
}

short ITrueDBDropDownCtrl::GetBookmarkType()
{
   short result;
   GetProperty(0xa6, VT_I2, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetBookmarkType(short propVal)
{
   SetProperty(0xa6, VT_I2, propVal);
}

short ITrueDBDropDownCtrl::GetCol()
{
   short result;
   GetProperty(0x6, VT_I2, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetCol(short propVal)
{
   SetProperty(0x6, VT_I2, propVal);
}

BOOL ITrueDBDropDownCtrl::GetColumnHeaders()
{
   BOOL result;
   GetProperty(0x7, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetColumnHeaders(BOOL propVal)
{
   SetProperty(0x7, VT_BOOL, propVal);
}

BOOL ITrueDBDropDownCtrl::GetCurrentCellVisible()
{
   BOOL result;
   GetProperty(0x48, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetCurrentCellVisible(BOOL propVal)
{
   SetProperty(0x48, VT_BOOL, propVal);
}

long ITrueDBDropDownCtrl::GetDataMode()
{
   long result;
   GetProperty(0x25, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetDataMode(long propVal)
{
   SetProperty(0x25, VT_I4, propVal);
}

LPUNKNOWN ITrueDBDropDownCtrl::GetDataSource()
{
   LPUNKNOWN result;
   GetProperty(0x9, VT_UNKNOWN, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetDataSource(LPUNKNOWN propVal)
{
   SetProperty(0x9, VT_UNKNOWN, propVal);
}

float ITrueDBDropDownCtrl::GetDefColWidth()
{
   float result;
   GetProperty(0xa, VT_R4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetDefColWidth(float propVal)
{
   SetProperty(0xa, VT_R4, propVal);
}

BOOL ITrueDBDropDownCtrl::GetEnabled()
{
   BOOL result;
   GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetEnabled(BOOL propVal)
{
   SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

BOOL ITrueDBDropDownCtrl::GetExtendRightColumn()
{
   BOOL result;
   GetProperty(0x49, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetExtendRightColumn(BOOL propVal)
{
   SetProperty(0x49, VT_BOOL, propVal);
}

VARIANT ITrueDBDropDownCtrl::GetFirstRow()
{
   VARIANT result;
   GetProperty(0x16, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFirstRow(const VARIANT& propVal)
{
   SetProperty(0x16, VT_VARIANT, &propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetHeadFont()
{
   LPDISPATCH result;
   GetProperty(0xb, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetHeadFont(LPDISPATCH propVal)
{
   SetProperty(0xb, VT_DISPATCH, propVal);
}

float ITrueDBDropDownCtrl::GetHeadLines()
{
   float result;
   GetProperty(0xc, VT_R4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetHeadLines(float propVal)
{
   SetProperty(0xc, VT_R4, propVal);
}

OLE_HANDLE ITrueDBDropDownCtrl::GetHWnd()
{
   OLE_HANDLE result;
   GetProperty(DISPID_HWND, VT_I4, (void*)&result);
   return result;
}

short ITrueDBDropDownCtrl::GetRow()
{
   short result;
   GetProperty(0xe, VT_I2, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetRow(short propVal)
{
   SetProperty(0xe, VT_I2, propVal);
}

long ITrueDBDropDownCtrl::GetRowDividerStyle()
{
   long result;
   GetProperty(0xf, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetRowDividerStyle(long propVal)
{
   SetProperty(0xf, VT_I4, propVal);
}

float ITrueDBDropDownCtrl::GetRowHeight()
{
   float result;
   GetProperty(0x10, VT_R4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetRowHeight(float propVal)
{
   SetProperty(0x10, VT_R4, propVal);
}

long ITrueDBDropDownCtrl::GetScrollBars()
{
   long result;
   GetProperty(0x20, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetScrollBars(long propVal)
{
   SetProperty(0x20, VT_I4, propVal);
}

short ITrueDBDropDownCtrl::GetSelStartCol()
{
   short result;
   GetProperty(0x11, VT_I2, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetSelStartCol(short propVal)
{
   SetProperty(0x11, VT_I2, propVal);
}

short ITrueDBDropDownCtrl::GetSelEndCol()
{
   short result;
   GetProperty(0x12, VT_I2, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetSelEndCol(short propVal)
{
   SetProperty(0x12, VT_I2, propVal);
}

short ITrueDBDropDownCtrl::GetVisibleCols()
{
   short result;
   GetProperty(0x21, VT_I2, (void*)&result);
   return result;
}

short ITrueDBDropDownCtrl::GetVisibleRows()
{
   short result;
   GetProperty(0x13, VT_I2, (void*)&result);
   return result;
}

OLE_COLOR ITrueDBDropDownCtrl::GetBackColor()
{
   OLE_COLOR result;
   GetProperty(DISPID_BACKCOLOR, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetBackColor(OLE_COLOR propVal)
{
   SetProperty(DISPID_BACKCOLOR, VT_I4, propVal);
}

OLE_COLOR ITrueDBDropDownCtrl::GetForeColor()
{
   OLE_COLOR result;
   GetProperty(DISPID_FORECOLOR, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetForeColor(OLE_COLOR propVal)
{
   SetProperty(DISPID_FORECOLOR, VT_I4, propVal);
}

LPFONTDISP ITrueDBDropDownCtrl::GetFont()
{
   LPFONTDISP result;
   GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFont(LPFONTDISP propVal)
{
   SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}

short ITrueDBDropDownCtrl::GetLeftCol()
{
   short result;
   GetProperty(0x17, VT_I2, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetLeftCol(short propVal)
{
   SetProperty(0x17, VT_I2, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetStyle()
{
   LPDISPATCH result;
   GetProperty(0x2c, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetStyle(LPDISPATCH propVal)
{
   SetProperty(0x2c, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetHeadingStyle()
{
   LPDISPATCH result;
   GetProperty(0x2d, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetHeadingStyle(LPDISPATCH propVal)
{
   SetProperty(0x2d, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetHighlightRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x78, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetHighlightRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x78, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetEvenRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x7a, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetEvenRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x7a, VT_DISPATCH, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetOddRowStyle()
{
   LPDISPATCH result;
   GetProperty(0x7b, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetOddRowStyle(LPDISPATCH propVal)
{
   SetProperty(0x7b, VT_DISPATCH, propVal);
}

unsigned long ITrueDBDropDownCtrl::GetHeadBackColor()
{
   unsigned long result;
   GetProperty(0x37, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetHeadBackColor(unsigned long propVal)
{
   SetProperty(0x37, VT_I4, propVal);
}

unsigned long ITrueDBDropDownCtrl::GetHeadForeColor()
{
   unsigned long result;
   GetProperty(0x38, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetHeadForeColor(unsigned long propVal)
{
   SetProperty(0x38, VT_I4, propVal);
}

long ITrueDBDropDownCtrl::GetApproxCount()
{
   long result;
   GetProperty(0x51, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetApproxCount(long propVal)
{
   SetProperty(0x51, VT_I4, propVal);
}

CString ITrueDBDropDownCtrl::GetErrorText()
{
   CString result;
   GetProperty(0x52, VT_BSTR, (void*)&result);
   return result;
}

BOOL ITrueDBDropDownCtrl::GetAllowColSelect()
{
   BOOL result;
   GetProperty(0x56, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetAllowColSelect(BOOL propVal)
{
   SetProperty(0x56, VT_BOOL, propVal);
}

BOOL ITrueDBDropDownCtrl::GetAllowColMove()
{
   BOOL result;
   GetProperty(0x57, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetAllowColMove(BOOL propVal)
{
   SetProperty(0x57, VT_BOOL, propVal);
}

LPUNKNOWN ITrueDBDropDownCtrl::GetISVServices()
{
   LPUNKNOWN result;
   GetProperty(0x55, VT_UNKNOWN, (void*)&result);
   return result;
}

CString ITrueDBDropDownCtrl::GetText()
{
   CString result;
   GetProperty(0x0, VT_BSTR, (void*)&result);
   return result;
}

CString ITrueDBDropDownCtrl::GetLayoutName()
{
   CString result;
   GetProperty(0x5c, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetLayoutName(LPCTSTR propVal)
{
   SetProperty(0x5c, VT_BSTR, propVal);
}

CString ITrueDBDropDownCtrl::GetLayoutFileName()
{
   CString result;
   GetProperty(0x5d, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetLayoutFileName(LPCTSTR propVal)
{
   SetProperty(0x5d, VT_BSTR, propVal);
}

CString ITrueDBDropDownCtrl::GetLayoutURL()
{
   CString result;
   GetProperty(0xb1, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetLayoutURL(LPCTSTR propVal)
{
   SetProperty(0xb1, VT_BSTR, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetArray()
{
   LPDISPATCH result;
   GetProperty(0x5e, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetArray(LPDISPATCH propVal)
{
   SetProperty(0x5e, VT_DISPATCH, propVal);
}

BOOL ITrueDBDropDownCtrl::GetEmptyRows()
{
   BOOL result;
   GetProperty(0x5f, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetEmptyRows(BOOL propVal)
{
   SetProperty(0x5f, VT_BOOL, propVal);
}

CString ITrueDBDropDownCtrl::GetListField()
{
   CString result;
   GetProperty(0x63, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetListField(LPCTSTR propVal)
{
   SetProperty(0x63, VT_BSTR, propVal);
}

CString ITrueDBDropDownCtrl::GetDataField()
{
   CString result;
   GetProperty(0x72, VT_BSTR, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetDataField(LPCTSTR propVal)
{
   SetProperty(0x72, VT_BSTR, propVal);
}

BOOL ITrueDBDropDownCtrl::GetIntegralHeight()
{
   BOOL result;
   GetProperty(0x65, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetIntegralHeight(BOOL propVal)
{
   SetProperty(0x65, VT_BOOL, propVal);
}

VARIANT ITrueDBDropDownCtrl::GetSelectedItem()
{
   VARIANT result;
   GetProperty(0x64, VT_VARIANT, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetSelectedItem(const VARIANT& propVal)
{
   SetProperty(0x64, VT_VARIANT, &propVal);
}

BOOL ITrueDBDropDownCtrl::GetFetchRowStyle()
{
   BOOL result;
   GetProperty(0x79, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFetchRowStyle(BOOL propVal)
{
   SetProperty(0x79, VT_BOOL, propVal);
}

BOOL ITrueDBDropDownCtrl::GetAlternatingRowStyle()
{
   BOOL result;
   GetProperty(0x7c, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetAlternatingRowStyle(BOOL propVal)
{
   SetProperty(0x7c, VT_BOOL, propVal);
}

BOOL ITrueDBDropDownCtrl::GetPartialRightColumn()
{
   BOOL result;
   GetProperty(0x9b, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetPartialRightColumn(BOOL propVal)
{
   SetProperty(0x9b, VT_BOOL, propVal);
}

BOOL ITrueDBDropDownCtrl::GetAnchorRightColumn()
{
   BOOL result;
   GetProperty(0x9e, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetAnchorRightColumn(BOOL propVal)
{
   SetProperty(0x9e, VT_BOOL, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetFooterStyle()
{
   LPDISPATCH result;
   GetProperty(0x92, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFooterStyle(LPDISPATCH propVal)
{
   SetProperty(0x92, VT_DISPATCH, propVal);
}

BOOL ITrueDBDropDownCtrl::GetColumnFooters()
{
   BOOL result;
   GetProperty(0x8f, VT_BOOL, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetColumnFooters(BOOL propVal)
{
   SetProperty(0x8f, VT_BOOL, propVal);
}

float ITrueDBDropDownCtrl::GetFootLines()
{
   float result;
   GetProperty(0x91, VT_R4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFootLines(float propVal)
{
   SetProperty(0x91, VT_R4, propVal);
}

LPDISPATCH ITrueDBDropDownCtrl::GetFooterFont()
{
   LPDISPATCH result;
   GetProperty(0x90, VT_DISPATCH, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFooterFont(LPDISPATCH propVal)
{
   SetProperty(0x90, VT_DISPATCH, propVal);
}

unsigned long ITrueDBDropDownCtrl::GetFooterBackColor()
{
   unsigned long result;
   GetProperty(0x93, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFooterBackColor(unsigned long propVal)
{
   SetProperty(0x93, VT_I4, propVal);
}

unsigned long ITrueDBDropDownCtrl::GetFooterForeColor()
{
   unsigned long result;
   GetProperty(0x94, VT_I4, (void*)&result);
   return result;
}

void ITrueDBDropDownCtrl::SetFooterForeColor(unsigned long propVal)
{
   SetProperty(0x94, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// ITrueDBDropDownCtrl operations

void ITrueDBDropDownCtrl::AboutBox()
{
   InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH ITrueDBDropDownCtrl::GetColumns()
{
   LPDISPATCH result;
   InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH ITrueDBDropDownCtrl::GetStyles()
{
   LPDISPATCH result;
   InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

short ITrueDBDropDownCtrl::ColContaining(float X)
{
   short result;
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper(0x18, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      X);
   return result;
}

VARIANT ITrueDBDropDownCtrl::GetBookmark(long RowNum)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x19, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      RowNum);
   return result;
}

void ITrueDBDropDownCtrl::ReBind()
{
   InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

VARIANT ITrueDBDropDownCtrl::RowBookmark(short RowNum)
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x1b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      RowNum);
   return result;
}

short ITrueDBDropDownCtrl::RowContaining(float Y)
{
   short result;
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      Y);
   return result;
}

float ITrueDBDropDownCtrl::RowTop(short RowNum)
{
   float result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x1d, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      RowNum);
   return result;
}

void ITrueDBDropDownCtrl::Scroll(long Cols, long Rows)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Cols, Rows);
}

void ITrueDBDropDownCtrl::Refresh()
{
   InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH ITrueDBDropDownCtrl::CaptureImage()
{
   LPDISPATCH result;
   InvokeHelper(0x24, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void ITrueDBDropDownCtrl::AddCellStyle(short Condition, const VARIANT& Style)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style);
}

void ITrueDBDropDownCtrl::AddRegexCellStyle(short Condition, const VARIANT& Style, LPCTSTR Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT VTS_BSTR;
   InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Style, Regex);
}

void ITrueDBDropDownCtrl::ClearCellStyle(short Condition)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x4e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition);
}

void ITrueDBDropDownCtrl::ClearRegexCellStyle(short Condition, const VARIANT& Regex)
{
   static BYTE parms[] =
      VTS_I2 VTS_VARIANT;
   InvokeHelper(0x4f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Condition, &Regex);
}

void ITrueDBDropDownCtrl::ClearSelCols()
{
   InvokeHelper(0x4c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBDropDownCtrl::ClearFields()
{
   InvokeHelper(0x4d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBDropDownCtrl::HoldFields()
{
   InvokeHelper(0x59, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ITrueDBDropDownCtrl::PostMsg(short MsgId)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x5a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       MsgId);
}

void ITrueDBDropDownCtrl::LoadLayout()
{
   InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH ITrueDBDropDownCtrl::GetLayouts()
{
   LPDISPATCH result;
   InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}
#endif

IMPLEMENT_DYNAMIC( ZGridColumn, CObject )

ZGridColumn::ZGridColumn( )
{
   Init();
}

ZGridColumn::ZGridColumn( ITrueDBGridCtrl * pParent )
{
   Init();
   m_pParentGrid = pParent;
}



/**************************************************************************/
// Function name   : ZGridColumn::Init
// Description     : to initilize the object used by contructors
// Return type     : void
/**************************************************************************/
void ZGridColumn::Init()
{
   m_pZeidonControlColumn = NULL;
   m_pMapping = NULL;
   m_pParentGrid = NULL;
   m_bInitiallyLocked = FALSE;
   SetFocusAllowed( TRUE );
   SetReadOnly( m_bInitiallyLocked );
   SetLockColor( RGB( 155,155,155 ) );
   m_strTrueValue.Empty();
   m_strFalseValue.Empty();
   m_nMyGridColumn = -1;
   m_bHasDateCtrl = FALSE;
}

ZGridColumn::~ZGridColumn( )
{
}


/**************************************************************************/
// Function name   : ZGridColumn::SetColumnIndex
// Description     : Assigns a Grid column to an ZGridColumn instance. This
//                   function can be calles only once, otherwise an assertion
//                   occurs. This is to prevent inconsistant mapping via the ZMapping
//                   member that is supplied by a seperate call ( see SetMapping... )
// Return type     : void
// Argument        : short nIndex Index of the column shown in the grid
/**************************************************************************/
void ZGridColumn::SetColumnIndex( short nIndex )
{
   ASSERT( m_nMyGridColumn < 0 ); // make sure this is called only once
   Columns cols;

   if ( m_pParentGrid )
      cols.AttachDispatch( m_pParentGrid->GetColumns() );
   else
   {
      TraceLineS( "ZGridColumn ", "No Parent Grid attached");
      return;
   }

   if ( m_nMyGridColumn < 0 && cols.GetCount() > nIndex )
      m_nMyGridColumn = nIndex;
   else
   {
      TraceLineI( "Unable to set internal properties for column ", nIndex );
      return;
   }

   Column  c = cols.GetColumn( m_nMyGridColumn );
   // make sure that there is a difference between lock and unlook color
   if( m_pParentGrid->GetInactiveBackColor() != c.GetBackColor() )
      SetLockColor( m_pParentGrid->GetInactiveBackColor() );
   SetUnlockColor( c.GetBackColor() );
   SetFocusAllowed( c.GetAllowFocus( ) );

   // Set the column to be locked/unlocked without checking the mapped attributes
   // because user decided to lock/unlock this column in design mode
   m_bInitiallyLocked = c.GetLocked();
   if( !m_bInitiallyLocked )
   {
      c.SetLocked( FALSE );
      c.SetAllowFocus( TRUE );
      c.SetBackColor( GetUnlockColor() );
   }
   else
   {
      c.SetLocked( TRUE );
      c.SetAllowFocus( FALSE );
      c.SetBackColor( GetLockColor() );
   }
}



/**************************************************************************/
// Function name   : ZGridColumn::GetCheckBoxValue
// Description     : returns the string assigned to an Checked/Unchecked state
//                   of a Combobox in a grids column
// Return type     : CString
// Argument        : BOOL bTrue referes to the state (Checked = TRUE) of the checkbox
/**************************************************************************/
CString ZGridColumn::GetCheckBoxValue( BOOL bTrue )
{
   // new column mapping mechanism overrules the old one
   if( m_pZeidonControlColumn )
   {
      if ( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) ) )
      {
         return ((ZColCheckBox *)m_pZeidonControlColumn)->GetCheckboxTranslationValue( bTrue );
      }
      return "";
   }
   else // this is for backward compatibility
   {
      if ( bTrue )
         return m_strTrueValue;
      else
         return m_strFalseValue;
   }
}


/**************************************************************************/
// Function name   : ZGridColumn::SetCheckBoxTranslation
// Description     : Sets the strings that have to be stored if the CheckBox
//                   column is checked or unchecked
// Return type     : BOOL - TRUE if values are different otherwise FALSE
// Argument        : CString strTrueValue  value if Checkbox column is checked
// Argument        : CString strFalseValue value if Checkbox column is unchecked
/**************************************************************************/
BOOL ZGridColumn::SetCheckBoxTranslation(CString strTrue, CString strFalse)
{
   BOOL bReturn = FALSE;

   // new column mapping mechanism overrules the old one
   if( m_pZeidonControlColumn )
   {
      if ( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) ) )
      {
         ((ZColCheckBox*)(m_pZeidonControlColumn))->SetCheckboxTranslationValues( strTrue, strFalse );
         bReturn = TRUE;
      }
   }
   else //this is for backward compatibility
   if( strTrue != strFalse )
   {
      m_strTrueValue = strTrue;
      m_strFalseValue = strFalse;
      bReturn = TRUE;
   }

   return bReturn;
}


/**************************************************************************/
// Function name   : ZGridColumn::HasValidCheckBoxTranslation
// Description     : checks whether a column has valid checkbox translation values.
//                   This is if the Checked and Uncecked string are different.
// Return type     : BOOL - TRUE if values are different otherwise FALSE
/**************************************************************************/
BOOL ZGridColumn::HasValidCheckBoxTranslation()
{
   BOOL bReturn = FALSE;

   // new column mapping mechanism overrules the old one
   if( m_pZeidonControlColumn )
   {
      bReturn = m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) );
   }
   else //this is for backward compatibility
   if( m_strTrueValue != m_strFalseValue ) // can only occur if checkbox translation
       bReturn = TRUE;                     // has not been set

   return bReturn;
}


// caches the mapping

/**************************************************************************/
// Function name   : ZGridColumn::SetMapping
// Description     : Sets basic mapping information. If used with complex
//                   mapping this function does nothing but low level validation.
//                   errors will only be traced
// Return type     : void
// Argument        : ZMapping *pMapping
/**************************************************************************/
void ZGridColumn::SetMapping( ZMapping *pMapping )
{
   CString strHelp;
   if ( m_pZeidonControlColumn )
   {
      ZColumnMapping *m = m_pZeidonControlColumn->GetColumnMapping();
      strHelp = m->GetMapViewName() + m->GetMapEntityName() + m->GetMapAttributeName();
      TraceLineS("Non unique Mapping using extended Mapping ", strHelp );
   }
   else // for backward compatibility
      m_pMapping = pMapping ;
}


/**************************************************************************/
// Function name   : ZGridColumn::UpdateAttribute
// Description     : updates or validates a column against the Zeidonn attribute
//                   depending on the bValidateOnly argument
// Return type     : short - 0 if successful
//                           zCALL_ERROR if the instance is not properly instanciated
//                           -2 if there is no mapping
//                           otherwise any return value of SetAttributeFromVariable
//
// Argument        : zVIEW vApp         - the view that is used to update or validate
//                                        a columns value
// Argument        : BOOL bValidateOnly - TRUE if the value only has to be validated
//                                        FALSE if the value only has to be updated
/**************************************************************************/
short ZGridColumn::UpdateAttribute( zVIEW vApp, BOOL bValidateOnly )
{
   short nRC = 0;

   ASSERT ( m_pParentGrid != NULL );
   ASSERT ( m_nMyGridColumn >= 0 );

   if ( !m_pParentGrid || m_nMyGridColumn < 0)
      return zCALL_ERROR;

   Columns cols( m_pParentGrid->GetColumns() );
   Column col = cols.GetColumn( m_nMyGridColumn );

   // using new style ??
   if ( m_pZeidonControlColumn && col.GetDataChanged() )
      return UpdateAttributeEx( vApp, bValidateOnly );
   else if ( col.GetDataChanged() )// this for backward compatibility
   {
      ZMapping* pMapping = GetMapping();
      if ( pMapping )
      {
         CString strText;
         strText.Empty();
         ValueItems ListOfValues = col.GetListOfValues();

         if ( HasValidCheckBoxTranslation() )
         {
            if( col.GetNumberFormat() == "Yes/No" ||
                col.GetNumberFormat() == "True/False" ||
                col.GetNumberFormat() == "On/Off" ||
                ListOfValues.GetPresentation() == dbgCheckBox )
            {
               COleVariant v = col.GetValue();
               CString strHelp = v.bstrVal;
               if( strHelp == "0" )
                  strText = GetCheckBoxValue( FALSE );
               else
                  strText = GetCheckBoxValue( TRUE );
            }
         }
         if ( strText.IsEmpty() )
         {
            strText = col.GetText();
         }

         short lCallFlags = zACCEPT_NULL_ENTITY;

         if ( !(*pMapping->m_zsContext) )
            lCallFlags |= zUSE_DEFAULT_CONTEXT;

         if ( bValidateOnly )
            lCallFlags |= zVALIDATE;

         nRC = SetAttributeFromVariable( vApp,
                                         pMapping->m_zsEName,
                                         pMapping->m_zsAName,
                                         (LPCSTR) strText,
                                         zTYPE_STRING,
                                         (zSHORT)strText.GetLength(),
                                         pMapping->m_zsContext,
                                         lCallFlags );
      }
      else
      {
         TraceLineI( "TrueGrid - Unable to determine Mapping in ZGridColumn::UpdateAttribute column # ", m_nMyGridColumn );
         nRC = -2;
      }

   }
   return nRC;
}


short ZGridColumn::UpdateAttribute( zVIEW vApp, CString strValue, BOOL bValidateOnly )
{
   return UpdateAttributeEx( vApp, strValue, bValidateOnly );
}

/**************************************************************************/
// Function name   : ZGridColumn::UpdateAttributeEx ( complex combobox mapping )
// Description     : updates or validates a column against the Zeidon attribute
//                   depending on the bValidateOnly argument
// Return type     : short - 0 if successful
//                           zCALL_ERROR if the instance is not properly instanciated
//                           -2 if there is no mapping
//                           otherwise any return value of SetAttributeFromVariable
//
// Argument        : zVIEW vApp         - the view that is used to update or validate
//                                        a columns value
// Argument        : BOOL bValidateOnly - TRUE if the value only has to be validated
//                                        FALSE if the value only has to be updated
/**************************************************************************/
short ZGridColumn::UpdateAttributeEx( zVIEW vApp, BOOL bValidateOnly )
{
   short nRC = 0;

   ASSERT ( m_pParentGrid != NULL );
   ASSERT ( m_nMyGridColumn >= 0 );

   if ( !m_pParentGrid || m_nMyGridColumn < 0 || !m_pZeidonControlColumn )
      return zCALL_ERROR;

   Columns cols( m_pParentGrid->GetColumns() );
   Column col = cols.GetColumn( m_nMyGridColumn );

   // check if current column is a combobox.
   // if so and mapping is not type of domain every thing is already done
   // by a call of SetupComboboxSelection otherwise the Domainvalue
   // has to be stored in the entity attr.
   if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
   {
      ZColComboBox *pZCBox = ( ZColComboBox *) m_pZeidonControlColumn;
      zLONG lFormatType = pZCBox->GetFormatType();
      if ( lFormatType & zCOMBOBOX_POSITION_ONLY )
      {
         if ( col.GetDataChanged() )
            nRC = (zSHORT)SetupComboBoxSelection();
         return nRC;
      }
   }

   if ( col.GetDataChanged() )
   {
      ZColumnMapping* pMapping = GetMappingEx();
      if ( pMapping )
      {
         CString strText;
         // fix for using EditMask property
         COleVariant v = col.GetValue();
         strText = v.bstrVal;
         if ( HasValidCheckBoxTranslation() )
         {
            ValueItems ListOfValues = col.GetListOfValues();
            if( col.GetNumberFormat() == "Yes/No" ||
                col.GetNumberFormat() == "True/False" ||
                col.GetNumberFormat() == "On/Off" ||
                ListOfValues.GetPresentation() == dbgCheckBox )
            {
               CString strHelp = v.bstrVal;  // this will be ether 0 or -1 representing VARIANT_TRUE / VARIANT_FALSE
               if( strHelp == "0" )
                  strText = GetCheckBoxValue( FALSE );
               else
                  strText = GetCheckBoxValue( TRUE );
            }
            else
            {
               if ( strText.IsEmpty() )
                  strText = col.GetText();
            }
         }
         else
         {
            if ( strText.IsEmpty() )
               strText = col.GetText();
         }

         long lCallFlags = zACCEPT_NULL_ENTITY;

         CString strContext = pMapping->GetMapContextName();
         if ( strContext.IsEmpty() )
            lCallFlags |= zUSE_DEFAULT_CONTEXT;

         if ( bValidateOnly )
            lCallFlags |= zVALIDATE;

         nRC = SetAttributeFromVariable( vApp,
                                         pMapping->GetMapEntityName(),
                                         pMapping->GetMapAttributeName(),
                                         (LPCSTR) strText,
                                         zTYPE_STRING,
                                         (zSHORT)strText.GetLength(),
                                         strContext,
                                         (zSHORT)lCallFlags );
      }
      else
      {
         TraceLineI( "TrueGrid - Unable to determine Mapping in ZGridColumn::UpdateAttributeEx column # ", m_nMyGridColumn );
         nRC = -2;
      }

   }
   return nRC;
}

short ZGridColumn::UpdateAttributeEx( zVIEW vApp, CString strValue, BOOL bValidateOnly )
{
   short nRC = 0;

   ASSERT ( m_pParentGrid != NULL );
   ASSERT ( m_nMyGridColumn >= 0 );

   if ( !m_pParentGrid || m_nMyGridColumn < 0 || !m_pZeidonControlColumn )
      return zCALL_ERROR;

   // check if current column is a combobox.
   // if so and mapping is not type of domain every thing is already done
   // by a call of SetupComboboxSelection otherwise the Domainvalue
   // has to be stored in the entity attr.
   if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
   {
      ZColComboBox *pZCBox = ( ZColComboBox *) m_pZeidonControlColumn;
      zLONG lFormatType = pZCBox->GetFormatType();
      if ( lFormatType & zCOMBOBOX_POSITION_ONLY )
      {
         //nRC = (zSHORT)SetupComboBoxSelection();
         return nRC;
      }
   }

   ZColumnMapping* pMapping = GetMappingEx();
   if ( pMapping )
   {
      long lCallFlags = zACCEPT_NULL_ENTITY;

      CString strContext = pMapping->GetMapContextName();
      if ( strContext.IsEmpty() )
         lCallFlags |= zUSE_DEFAULT_CONTEXT;

      if ( bValidateOnly )
         lCallFlags |= zVALIDATE;

      nRC = SetAttributeFromVariable( vApp,
                                      pMapping->GetMapEntityName(),
                                      pMapping->GetMapAttributeName(),
                                      (LPCSTR) strValue,
                                      zTYPE_STRING,
                                      (zSHORT)strValue.GetLength(),
                                      strContext,
                                      (zSHORT)lCallFlags );
   }
   else
   {
      TraceLineI( "TrueGrid - Unable to determine Mapping in ZGridColumn::UpdateAttributeEx column # ", m_nMyGridColumn );
      nRC = -2;
   }

   return nRC;
}

short ZGridColumn::UpdateAttributeEx( zVIEW vApp, ZOleDateTime &dt, BOOL bValidateOnly )
{
   short nRC = 0;

   ASSERT ( m_pParentGrid != NULL );
   ASSERT ( m_nMyGridColumn >= 0 );

   // convert datetime to zeidon date
   CString strValue = dt;

   if ( !m_pParentGrid || m_nMyGridColumn < 0 || !m_pZeidonControlColumn )
      return zCALL_ERROR;

   ZColumnMapping* pMapping = GetMappingEx();
   if ( pMapping )
   {
      long lCallFlags = zACCEPT_NULL_ENTITY;

      if ( bValidateOnly )
         lCallFlags |= zVALIDATE;

      nRC = SetAttributeFromVariable( vApp,
                                      pMapping->GetMapEntityName(),
                                      pMapping->GetMapAttributeName(),
                                      (LPCSTR) strValue,
                                      zTYPE_STRING,
                                      (zSHORT)strValue.GetLength(),
                                      "",
                                      (zSHORT)lCallFlags );
   }
   else
   {
      TraceLineI( "TrueGrid - Unable to determine Mapping in ZGridColumn::UpdateAttributeEx column # ", m_nMyGridColumn );
      nRC = -2;
   }

   return nRC;
}


/**************************************************************************/
// Function name   : ZGridColumn::ValidateAttribute
// Description     : validates a columns value against the oe
// Return type     : short      - any returns of UpdateAttribute
// Argument        : zVIEW vApp - the view that is used to update or validate
//                                        a columns value
/**************************************************************************/
short ZGridColumn::ValidateAttribute( zVIEW vApp )
{
   return UpdateAttribute( vApp, TRUE );
}

short ZGridColumn::ValidateAttribute( zVIEW vApp, CString strValue )
{
   return UpdateAttribute( vApp, strValue, TRUE );
}

void  ZGridColumn::AttachZeidonControlColumn( ZGridColCtrl *pZGridColumn )
{
   m_pZeidonControlColumn = pZGridColumn;
}



/**************************************************************************/
// Function name   : ZGridColumn::GetAttributeValue
// Description     : Returns a string containing the value of the
//                   mapped attribute
// Return type     : CString
// Argument        : zVIEW vApp  - the view to the mapped attribute
/**************************************************************************/
CString ZGridColumn::GetAttributeValue( zVIEW vApp )
{
   CString str;
   CString strEntity;
   CString strAttr;
   CString strContext;
   zULONG  ulDisplayLth;
   BOOL bCheckBox = FALSE;

   str.Empty();

   if( m_pZeidonControlColumn )
   {
      strEntity    = m_pZeidonControlColumn->GetMapEntityName();
      strAttr      = m_pZeidonControlColumn->GetMapAttributeName();
      strContext   = m_pZeidonControlColumn->GetMapContextName();
      ulDisplayLth = m_pZeidonControlColumn->GetAttributeDisplayLength();
      bCheckBox    = m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) );
   }
   else // for backward compatibility
   {
      if( m_pMapping )
      {
         strEntity    = m_pMapping->m_zsEName;
         strAttr      = m_pMapping->m_zsAName;
         strContext   = m_pMapping->m_zsContext;
         ulDisplayLth = m_pMapping->m_ulDisplayLth;
      }
   }

   if( strEntity.IsEmpty() || strAttr.IsEmpty() )
      return str;

   zSHORT nFlag = zACCEPT_NULL_ENTITY;
   if( strContext.IsEmpty() )
       nFlag = zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT;

   LPSTR szValue = str.GetBufferSetLength( ulDisplayLth + 1 );
   zSHORT nRC = GetVariableFromAttribute( szValue, 0, zTYPE_STRING,
                                   ulDisplayLth,
                                   vApp,
                                   strEntity ,
                                   strAttr,
                                   strContext,
                                   nFlag );

   if ( nRC == zVAR_NULL )
      szValue[ 0 ] = 0;

   str.ReleaseBuffer();

   if ( bCheckBox )
   {
      CString strHelp = GetCheckBoxValue( TRUE );
      if( strHelp == str )
         str = "0";
      else
         str = "-1";
   }
   return str;
}


zSHORT
ZGridColumn::GetVariantFromAttribute( COleVariant &v, zVIEW vApp )
{
   zSHORT nRC = zCURSOR_NULL;

   CString str;
   CString strEntity;
   CString strAttr;
   CString strContext;
   zULONG  ulDisplayLth;
   BOOL bCheckBox = FALSE;

   str.Empty();

   if( m_pZeidonControlColumn )
   {
      strEntity    = m_pZeidonControlColumn->GetMapEntityName();
      strAttr      = m_pZeidonControlColumn->GetMapAttributeName();
      strContext   = m_pZeidonControlColumn->GetMapContextName();
      ulDisplayLth = m_pZeidonControlColumn->GetAttributeDisplayLength();
      bCheckBox    = m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) );
   }

   if( strEntity.IsEmpty() || strAttr.IsEmpty() )
      return nRC;

   zSHORT nFlag = zACCEPT_NULL_ENTITY;
   if( strContext.IsEmpty() )
       nFlag = zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT;

   LPSTR szValue = str.GetBufferSetLength( ulDisplayLth + 1 );
   nRC = GetVariableFromAttribute( szValue, 0, zTYPE_STRING,
                                   ulDisplayLth,
                                   vApp,
                                   strEntity ,
                                   strAttr,
                                   strContext,
                                   nFlag );

   if ( nRC == zVAR_NULL )
      szValue[ 0 ] = 0;

   str.ReleaseBuffer();

   if ( bCheckBox )
   {
      CString strHelp = GetCheckBoxValue( TRUE );
      CString strFalse = GetCheckBoxValue( FALSE );
      if( strHelp == str )
      {
         v.ChangeType( VT_BOOL );
         v.boolVal = VARIANT_TRUE;
      }
      else
      {
         v.ChangeType( VT_BOOL );
         v.boolVal = VARIANT_FALSE;
      }
   }
   else
      v = str;

   return nRC;
}

zSHORT
ZGridColumn::GetStringFromAttribute( CString &strValue, zVIEW vApp )
{
   zSHORT nRC = zCURSOR_NULL;

   CString strEntity;
   CString strAttr;
   zULONG  ulDisplayLth;

   if( m_pZeidonControlColumn )
   {
      strEntity    = m_pZeidonControlColumn->GetMapEntityName();
      strAttr      = m_pZeidonControlColumn->GetMapAttributeName();
      ulDisplayLth = m_pZeidonControlColumn->GetAttributeDisplayLength();
   }

   if( strEntity.IsEmpty() || strAttr.IsEmpty() )
      return nRC;

   LPSTR szValue = strValue.GetBufferSetLength( ulDisplayLth + 1 );
   nRC = GetVariableFromAttribute( szValue, 0, zTYPE_STRING,
                                   ulDisplayLth,
                                   vApp,
                                   strEntity ,
                                   strAttr,
                                   0,
                                   0 );
   strValue.ReleaseBuffer();
   return nRC;
}


zSHORT
ZGridColumn::SetAttributeFromVariant( COleVariant &v, zVIEW vApp )
{
   zSHORT nRC = zCURSOR_NULL;

   CString str;
   CString strEntity;
   CString strAttr;
   CString strContext;
   zULONG  ulDisplayLth;
   BOOL bCheckBox = FALSE;

   str.Empty();

   if( m_pZeidonControlColumn )
   {
      strEntity    = m_pZeidonControlColumn->GetMapEntityName();
      strAttr      = m_pZeidonControlColumn->GetMapAttributeName();
      strContext   = m_pZeidonControlColumn->GetMapContextName();
      ulDisplayLth = m_pZeidonControlColumn->GetAttributeDisplayLength();
      bCheckBox    = m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) );
   }

   if( strEntity.IsEmpty() || strAttr.IsEmpty() )
      return nRC;

   zSHORT nFlag = zACCEPT_NULL_ENTITY;
   if( strContext.IsEmpty() )
       nFlag = zACCEPT_NULL_ENTITY | zUSE_DEFAULT_CONTEXT;

   if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
   {
      nRC = (zSHORT)SetupComboBoxSelection();
   }
   else
   {
      str = v.bstrVal;
      nRC = SetAttributeFromVariable( vApp,
                                      strEntity ,
                                      strAttr,
                                      (zCPVOID)(zCPCHAR)str,
                                      zTYPE_STRING,
                                      (zSHORT)ulDisplayLth,
                                      strContext,
                                      nFlag );
   }

   return nRC;
}

/**************************************************************************/
// Function name   : ZGridColumn::PopulateListOfValues
// Description     : creates a list of valid entries for the mapped Attribute
//                   if the column represents a combobox
// Return type     : zLONG  - a negative value if failed
//                            ( -1 -> mapping error )
//                            ( -2 -> if Grid column can not be adressed )
//                            ( -3 -> if column is not a combobox )
//                            otherwise the number of entries in the combobox
/**************************************************************************/
zLONG ZGridColumn::PopulateListOfValues()
{
   zBOOL bListDomainOnly = FALSE;
   zBOOL bIsComboBox = FALSE;
   CString strView;
   CString strEntity;
   CString strAttr;
   CString strContext;
   CString strHelp;
   zULONG  ulDisplayLth;
   zULONG  ulFormatType = 0;
   zVIEW vApp = NULL;
   zVIEW vApplication = NULL;

   Columns cols( m_pParentGrid->GetColumns () );
   if( cols.GetCount() <= m_nMyGridColumn )
      return -2;

   Column col = cols.GetColumn( m_nMyGridColumn );

   if( m_pZeidonControlColumn )
   {
      strView      = m_pZeidonControlColumn->GetMapViewName();
      strEntity    = m_pZeidonControlColumn->GetMapEntityName();
      strAttr      = m_pZeidonControlColumn->GetMapAttributeName();
      strContext   = m_pZeidonControlColumn->GetMapContextName();
      ulDisplayLth = m_pZeidonControlColumn->GetAttributeDisplayLength();
   }

   // using new style?
   if ( m_pZeidonControlColumn )
   {
      if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
      {
         ulFormatType = ((ZColComboBox*)m_pZeidonControlColumn)->GetFormatType();
         if( (ulFormatType & zCOMBOBOX_POSITION_ONLY ) == 0 )
         {
            bListDomainOnly = TRUE;
         }
      }
      if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColCheckBox ) ) )
      {
         // populate translationtable for checkbox
         CString strTrueValue = ((ZColCheckBox*)m_pZeidonControlColumn)->GetCheckboxTranslationValue( TRUE );
         CString strFalseValue = ((ZColCheckBox*)m_pZeidonControlColumn)->GetCheckboxTranslationValue( FALSE );
         ValueItems ListOfValues = col.GetListOfValues();
         ListOfValues.Clear();// we do not like any user defined stuff here

         COleVariant vValue = strTrueValue;
         COleVariant vInternalValue( VARIANT_TRUE, VT_BOOL );
         ValueItemDisp ValueItem;
         ValueItem.CreateDispatch( ValueItem.GetCLSID() );

         ValueItem.SetDisplayValue( vValue ); // set external attribute value
         ValueItem.SetValue( vInternalValue );
         ListOfValues.Add( ValueItem );

         vValue = strFalseValue;
         vInternalValue = VARIANT_FALSE;
         ValueItem.SetDisplayValue( vValue ); // set external attribute value
         ValueItem.SetValue( vInternalValue );
         ListOfValues.Add( ValueItem );
      }
   }
   else // for backward compatibility
   {
      bListDomainOnly = TRUE;     // old style supports only Domain lists
      ZMapping *pMapping = GetMapping();
      if( m_pMapping )
      {
         strView      = m_pMapping->m_zsVName;
         strEntity    = m_pMapping->m_zsEName;
         strAttr      = m_pMapping->m_zsAName;
         strContext   = m_pMapping->m_zsContext;
         ulDisplayLth = m_pMapping->m_ulDisplayLth;
      }
   }

   if( strEntity.IsEmpty() || strAttr.IsEmpty() )
      return -1;

   ValueItems ListOfValues = col.GetListOfValues();
   if (    ListOfValues.GetPresentation() != dbgComboBox // Combo Box
        && ListOfValues.GetPresentation() != dbgSortedComboBox // Sorted Combo Box
        && ListOfValues.GetPresentation() != dbgRadioButton )
        return -3;

   if ( ListOfValues.GetPresentation() == dbgRadioButton )
      bListDomainOnly = TRUE;

   if ( bListDomainOnly )
   {
      zVIEW vApplication = m_pParentGrid->GetViewNamed( strView );
      if ( !vApplication )
      {
         return -1;
      }
      else
      {
         CreateViewFromView( &vApp, vApplication );
      }

      if ( !vApp )
      {
         CString strHelp = "Unable to load view for TrueGrid - ";
         strHelp += m_pParentGrid->GetControlTag();
         strHelp += " ";
         TraceLineS( strHelp, strView );
         return -1;
      }

      zPVOID pvCursor = NULL;
      PCHAR pchBuffer = strHelp.GetBufferSetLength( ulDisplayLth );

      zSHORT sDomainType = tgGetDomainType( vApp,
                                            strEntity,
                                            strAttr);
      if( sDomainType == 'T' )
         ListOfValues.Clear();// we do not like any user defined stuff here
      else
      {
         DropView( vApp );
         return -1;
      }
      zSHORT nRC = GetFirstTableEntryForAttribute( pchBuffer,
                                            vApp,
                                            strEntity,
                                            strAttr,
                                            strContext,
                                            &pvCursor );
      while ( nRC == 0 )
      {
         strHelp.ReleaseBuffer();
         COleVariant vValue = strHelp;

         ValueItemDisp ValueItem;
         ValueItem.CreateDispatch( ValueItem.GetCLSID() );
         ValueItem.SetDisplayValue( vValue ); // set external attribute value
         ValueItem.SetValue( vValue );        // set internal attribute value
                                              // translation will be made by zeidon oe so both must be the same
         ListOfValues.Add( ValueItem );

         pchBuffer = strHelp.GetBufferSetLength( ulDisplayLth );
         nRC = GetNextTableEntryForAttribute( pchBuffer,
                                              vApp,
                                              strEntity,
                                              strAttr,
                                              strContext,
                                              &pvCursor );
      }
      if ( m_pZeidonControlColumn && m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
      {
         ((ZColComboBox*)m_pZeidonControlColumn)->SetListLoaded();
      }
   }
   else
   {
      if( ulFormatType & zCOMBOBOX_POSITION_ONLY ||
          ulFormatType & zCOMBOBOX_SELECT        ||
          ulFormatType & zCOMBOBOX_INCLUDE       ||
          ulFormatType & zCOMBOBOX_FOREIGN_KEY )
      {
         ListOfValues.Clear();// we do not like any user defined stuff here
         if ( m_pZeidonControlColumn && m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
         {
            BuildListOfValuesFromOI( );
         }
      }
   }

   if( vApp )
      DropView( vApp );


   return ( ListOfValues.GetCount() );
}



/**************************************************************************/
// Function name   : ZGridColumn::BuildListOfValuesFromOI
// Description     : Setup a list of values for the list portion of a combobox
// Return type     : zSHORT - negative if failed
/**************************************************************************/
zSHORT ZGridColumn::BuildListOfValuesFromOI( )
{
   zSHORT   nRC;
   ZColComboBox *pZCBox = NULL;
   ZColumnMapping *pZCListMapping = NULL;
   zBOOL bIsComboBox = FALSE;
   CString strListView;
   CString strListEntity;
   CString strListAttr;
   CString strListContext;
   CString strHelp;
   zULONG  ulDisplayLth;
   zULONG  ulFormatType = 0;
   zVIEW vApp = NULL;

   ValidateControlState();

   Columns cols( m_pParentGrid->GetColumns () );
   if( cols.GetCount() <= m_nMyGridColumn )
      return -1;

   Column col = cols.GetColumn( m_nMyGridColumn );

   ValueItems ListOfValues = col.GetListOfValues();
   if (    ListOfValues.GetPresentation() != dbgComboBox // Combo Box
        && ListOfValues.GetPresentation() != dbgSortedComboBox )// Sorted Combo Box
        return -1;

   if( m_pZeidonControlColumn &&
      m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
   {
      pZCBox = ((ZColComboBox*)m_pZeidonControlColumn);
      pZCListMapping = pZCBox->GetListMapping();
      if( pZCListMapping ) // this should only be the case when
      {                    // when this guy is a domain
         strListView      = pZCListMapping->GetMapViewName();
         strListEntity    = pZCListMapping->GetMapEntityName();
         strListAttr      = pZCListMapping->GetMapAttributeName();
         strListContext   = pZCListMapping->GetMapContextName();
      }
      ulDisplayLth = pZCBox ->GetAttributeDisplayLength();
      ulFormatType = pZCBox ->GetFormatType();
      pZCBox->SetListLoaded( FALSE );
   }
   else
   {
      return -1;
   }

   if ( strListView.IsEmpty() )
   {
      if( strListView.IsEmpty() )
         TraceLineI( "List Mapping error for TrueGrid Column # ", m_nMyGridColumn );
   }
   else
   {
      vApp = m_pParentGrid->GetViewNamed( strListView );
      if( !vApp )
         return -1;
   }

   if ( ulFormatType & zCOMBOBOX_POSITION_ONLY )
   {
      zVIEW   vAppList;
      CString strEntity;
      CString strBrowseScope  = pZCBox->GetBrowseScopeEntity();
      CString strBrowseEntity = pZCBox->GetBrowseEntity();
      CString strMapContext;
      zSHORT  nFlag = 0;

      CreateViewFromView( &vAppList, vApp );

      // if no browse entity is specified, defaulting to list entity
      if ( !strBrowseEntity.IsEmpty() )
         strEntity = strBrowseEntity;
      else
         strEntity = strListEntity;

      // if no context is specified retrieve the "list attributes" default context
      if ( strListContext.IsEmpty( ) )
      {
         LPVIEWENTITY lpViewEntity;
         LPVIEWATTRIB lpViewAttrib;
         zLPCONTEXT   lpContext;

         lpViewEntity = MiGetViewEntityForView( vApp, strEntity );
         if ( lpViewEntity )
         {
            for ( lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstViewAttrib );
                  lpViewAttrib;
                  lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextViewAttrib ) )
            {
               if ( zstrcmp( lpViewAttrib->szName, strListAttr ) == 0 )
               {
                  LPDOMAIN lpDomain = (LPDOMAIN) zGETPTR( lpViewAttrib->hDomain );
                  if ( GetDefaultContext( &lpContext, lpDomain ) &&
                       *(lpContext->szName) )
                  {
                     strListContext = lpContext->szName;
                  }
                  break;
               }
            }
         }
      }

      if ( !strListContext.IsEmpty( ) )
         strMapContext = strListContext;
      else
      {
         // if there is still no context trace an error
         CString strError;
         strError.Format( "ZGridColumn::BuildListOfValuesFromOI unable to locate attribute %s for entity ", strListAttr );
         TraceLineS( strError, strEntity );
      }

      nRC = SetEntityCursor( vAppList, strEntity, 0,
                             zPOS_FIRST , 0, 0, 0, 0,
                             strBrowseScope, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CString strValue;
         zPCHAR szMap = strValue.GetBufferSetLength( ulDisplayLth );
         nRC = GetVariableFromAttribute( szMap, 0, zTYPE_STRING,
                                         ulDisplayLth,
                                         vAppList, strEntity,
                                         strListAttr,
                                         strListContext, nFlag );
         if ( nRC == zVAR_NULL )
            szMap[ 0 ] = 0;

         strValue.ReleaseBuffer();
         COleVariant vValue;
         vValue = strValue;

         ValueItemDisp ValueItem;
         ValueItem.CreateDispatch( ValueItem.GetCLSID() );
         ValueItem.SetDisplayValue( vValue ); // set external attribute value
         ValueItem.SetValue( vValue );        // set internal attribute value
                                              // translatiion will be made by zeidon oe so both must be the same
         ListOfValues.Add( ValueItem );

         nRC = SetEntityCursor( vAppList, strEntity, 0,
                                zPOS_NEXT , 0, 0, 0, 0,
                                strBrowseScope, 0 );

      }  // end of:  while ( nRC > zCURSOR_UNCHANGED ) ... looping thru entities

      // get the view name of the edit part of the combobox and check cardinality
      // to see whether an empty entry can occur
      DropView( vAppList );
      if ( GetViewByName( &vApp, pZCBox->GetMapViewName() ,
                          m_pParentGrid->GetDialogView(), zLEVEL_ANY ) > 0 )
      {
         if ( (ulFormatType & zCOMBOBOX_NO_NULL) == 0 )
         {
            zSHORT  nMinCardinality;
            zSHORT  nMaxCardinality;

            // Insert a blank line at the beginning if the cardinality
            // permits it.
            MiGetCardinalityForEntity( (zPUSHORT) &nMinCardinality,
                                       (zPUSHORT) &nMaxCardinality,
                                       vApp, pZCBox->GetMapEntityName() );

            // If this combo box is permitted to have a null entry, then
            // put it in.
            if ( nMinCardinality == 0 )
            {
               CString strValue;
               strValue.Empty();
               COleVariant vValue;
               vValue = strValue;

               ValueItemDisp ValueItem;
               ValueItem.CreateDispatch( ValueItem.GetCLSID() );
               ValueItem.SetDisplayValue( vValue ); // set external attribute value
               ValueItem.SetValue( vValue );        // set internal attribute value
                                                    // translatiion will be made by zeidon oe so both must be the same
               ListOfValues.Add( ValueItem );
            }
         }
      }
   }
   pZCBox->SetListLoaded( TRUE );

   return( 0 );
}




/**************************************************************************/
// Function name   : ZGridColumn::ValidateControlState
// Description     : Automatic graying the combobox column. There are three
//                   cases ...
//                        1.  Domains ... simple since there is one view and one entity
//                            involved.  If the entity does not exist or is not updateable,
//                            the control is grayed.
//
//                        2.  Foreign Key ... still somewhat simple.  The source and target
//                            entities must exist and the target entity must be updateable,
//                            otherwise, the control is grayed.
//
//                        3.  Includable ... the tough case.  The source entity must exist
//                            and the target entity must be updateable, otherwise the control
//                            is grayed.
// Return type     : void
/**************************************************************************/
void ZGridColumn::ValidateControlState()
{
   zBOOL bListEntityExists = FALSE;
   zBOOL bEditIsUpdateable = FALSE;
   ZColComboBox *pZCBox = NULL;
   ZColumnMapping *pZCListMapping = NULL;
   CString strListView;
   CString strListEntity;
   CString strListAttr;
   CString strListContext;
   CString strBrowseScope;
   CString strHelp;
   zULONG  ulFormatType = 0;
   zVIEW vApp = NULL;
   zVIEW vAppList = NULL;

   Columns cols( m_pParentGrid->GetColumns () );
   if( cols.GetCount() <= m_nMyGridColumn )
      return;

   Column col = cols.GetColumn( m_nMyGridColumn );

   if( m_pZeidonControlColumn &&
      m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
   {
      pZCBox = ((ZColComboBox*)m_pZeidonControlColumn);
      pZCListMapping = pZCBox->GetListMapping();
      pZCListMapping = pZCBox->GetListMapping();
      if( pZCListMapping ) // this should only be the case when
      {                    // when this guy is a domain
         strListView      = pZCListMapping->GetMapViewName();
         strListEntity    = pZCListMapping->GetMapEntityName();
         strListAttr      = pZCListMapping->GetMapAttributeName();
         strListContext   = pZCListMapping->GetMapContextName();
      }
      ulFormatType = pZCBox ->GetFormatType();
   }
   else
   {
      return;
   }

   // get the views that are mapped to the list and the edit part of the combobox
   GetViewByName( &vAppList, strListView, m_pParentGrid->GetDialogView(), zLEVEL_ANY );
   GetViewByName( &vApp, pZCBox->GetMapViewName(), m_pParentGrid->GetDialogView(), zLEVEL_ANY );
   if( !vApp || !vAppList )
   {
      SetReadOnly( TRUE );
      col.SetLocked( IsReadOnly() );
      col.SetAllowFocus( FALSE );
      col.SetBackColor( GetLockColor() );
      CString strError;
      strError.Format("Invalid view for Truegrid - Combobox Column # %d " ,m_nMyGridColumn );
      TraceLineS( strError, m_pParentGrid->GetControlTag() );
      strError.Format("    List view name = %s " ,strListView );
      TraceLineS( strError, m_pParentGrid->GetControlTag() );
      strError.Format("    Edit view name = %s " ,pZCBox->GetMapViewName() );
      TraceLineS( strError, m_pParentGrid->GetControlTag() );
      return;
   }

   if ( ulFormatType & zCOMBOBOX_FOREIGN_KEY )
   {
      bListEntityExists = (CheckExistenceOfEntity( vAppList, strListEntity ) == zCURSOR_SET );
      bEditIsUpdateable = (zBOOL)(MiGetUpdateForViewEntityAttr( vApp,
                                                         pZCBox->GetMapEntityName(),
                                                         pZCBox->GetMapAttributeName(), 0 ) );
   }
   else
   {
      // if there are no entities to list ... quit
      if ( SetEntityCursor( vAppList, strListEntity, 0, zPOS_FIRST | zTEST_CSR_RESULT,
                            0, 0, 0, 0, strBrowseScope, 0 ) >= 0 )
      {
         bListEntityExists = TRUE;
      }

      // The target view must be updateable ... otherwise disable the combobox.
      if ( !(ulFormatType & zCOMBOBOX_INCLUDE) )
      {
         bEditIsUpdateable = TRUE;
      }
      else
      if ( MiGetUpdateForViewEntityAttr( vApp,
                                         pZCBox->GetMapEntityName(),
                                         0, zUPD_INCLUDE ) )
      {
         bEditIsUpdateable = TRUE; // ==> updateable
      }

   }
   // disable column if edit attribute is not updateable
   // or the list entity instance does not exist,
   if ( bEditIsUpdateable && bListEntityExists )
   {
      SetReadOnly( FALSE );
      col.SetLocked( IsReadOnly() );
      col.SetAllowFocus( TRUE );
      col.SetBackColor( GetUnlockColor() );
   }
   else
   {
      SetReadOnly( TRUE );
      col.SetLocked( IsReadOnly() );
      col.SetAllowFocus( FALSE );
      col.SetBackColor( GetLockColor() );
   }
}


/**************************************************************************/
// Function name   : ZGridColumn::SetupComboBoxSelection
// Description     : used by event that occurs if an entry in the combobox
//                   is selected depending on the formattype
//                   -  an automatic include occurs
//                   -  a entity get select state
//                   -  or an attribute is set
// Return type     : zLONG  -1 if there is a mapping failure
//                           0  if succesful
//                           or any value returned by one of the OE calls
//                            - IncludeSubobjectFromSubobject
//                            - SetAttributeFromVariable
//                            - SetSelectStateOfEntity
/**************************************************************************/
zLONG ZGridColumn::SetupComboBoxSelection( )
{
   ZColComboBox *pZCBox = NULL;
   ZColumnMapping *pZCListMapping = NULL;
   CString strListView;
   CString strListEntity;
   CString strListAttr;
   CString strListContext;
   CString strBrowseScope;
   zULONG  ulFormatType = 0;
   zULONG  ulDisplayLth = 0;
   zVIEW vApp = NULL;
   zVIEW vAppList = NULL;
   zLONG lReturn = -1;
   zSHORT nRC = 0;

   Columns cols( m_pParentGrid->GetColumns () );
   if( cols.GetCount() <= m_nMyGridColumn )
      return lReturn;

   Column col = cols.GetColumn( m_nMyGridColumn );
   if( !col.GetDataChanged() )
      return nRC;

   if( m_pZeidonControlColumn &&
      m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
   {
      pZCBox = ((ZColComboBox*)m_pZeidonControlColumn);
      pZCListMapping = pZCBox->GetListMapping();
      if( pZCListMapping ) // this should only be the case when
      {                    // when this guy is a domain
         strListView      = pZCListMapping->GetMapViewName();
         strListEntity    = pZCListMapping->GetMapEntityName();
         strListAttr      = pZCListMapping->GetMapAttributeName();
         strListContext   = pZCListMapping->GetMapContextName();
      }
      ulDisplayLth = pZCBox ->GetAttributeDisplayLength();
      ulFormatType = pZCBox ->GetFormatType();
   }
   else
   {
      return lReturn;
   }

   // Note that if ViewName is set ... EntityName and AttributeName
   // must be set as required for a particular control???
   GetViewByName( &vApp, pZCBox->GetMapViewName(),
                     m_pParentGrid->GetDialogView(), zLEVEL_ANY );
   if( !vApp )
      return lReturn;

   // simple combobox domain mapping
   CString strText;
   if( ( ulFormatType & zCOMBOBOX_POSITION_ONLY ) == 0 )
   {
      ValueItems ListOfValues = col.GetListOfValues();
      if( ListOfValues.GetAnnotatePicture() )
      {
         zSHORT  nFlag = 0;
         COleVariant v = col.GetValue();
         strText = v.bstrVal;
         col.SetText( strText ); // anything else will be done by UpdateAttribute()
         nRC = SetAttributeFromVariable( vApp, pZCBox->GetMapEntityName(),
                                   pZCBox->GetMapAttributeName(), strText,
                                   zTYPE_STRING, 0,
                                   pZCBox->GetMapContextName(),
                                   pZCBox->GetMapContextName().IsEmpty() ?
                                   nFlag | zUSE_DEFAULT_CONTEXT :
                                   (zSHORT) nFlag );
      }
   }
   else   // anything but domain
   if ( ulFormatType & zCOMBOBOX_POSITION_ONLY )
   {
      COleVariant v = col.GetValue();
      strText = v.bstrVal; // get internal value, that should be the one we need
      if ( GetViewByName( &vAppList, strListView,
                          m_pParentGrid->GetDialogView(), zLEVEL_ANY ) > 0 )
      {
         // Use the Browse entity name if specified, otherwise use the
         // Mapping entity name as the default to traverse.
         CString strEntity;
         CString strBrowseScope  = pZCBox->GetBrowseScopeEntity();
         CString strBrowseEntity = pZCBox->GetBrowseEntity();
         zSHORT  nFlag = 0;

         // if no browse entity is specified, defaulting to list entity
         if ( !strBrowseEntity.IsEmpty() )
            strEntity = strBrowseEntity;
         else
            strEntity = strListEntity;

         // is there really an entity or is an empty entry selected?
         if ( strText.IsEmpty() ||
              SetEntityCursor( vAppList, strEntity, strListAttr,
                               zPOS_FIRST | zQUAL_STRING | zSPARENT_KEEPCHILDREN | nFlag,
                               strText, 0, 0, 0,
                               strBrowseScope, strListContext ) >= 0  )
         {
            if ( ulFormatType & zCOMBOBOX_SELECT )
            {
               SetAllSelectStatesForEntity( vAppList, strListEntity,
                                            FALSE, strBrowseScope );
               if ( !strText.IsEmpty() )
                  nRC = SetSelectStateOfEntity( vAppList, strListEntity, TRUE );
            }
            else
            if ( ulFormatType & zCOMBOBOX_INCLUDE )
            {
               if ( CheckExistenceOfEntity( vApp, pZCBox->GetMapEntityName() ) == zCURSOR_SET )
                  ExcludeEntity( vApp, pZCBox->GetMapEntityName(), zREPOS_BEFORE );

               if ( !strText.IsEmpty() )
               {
                  nRC = IncludeSubobjectFromSubobject( vApp, pZCBox->GetMapEntityName(),
                                                       vAppList, strListEntity, zPOS_AFTER );
               }
            }
            else
            if ( ulFormatType & zCOMBOBOX_FOREIGN_KEY )
            {
               nRC = SetAttributeFromVariable( vApp, pZCBox->GetMapEntityName(),
                                         pZCBox->GetMapAttributeName(), strText,
                                         zTYPE_STRING, 0,
                                         pZCBox->GetMapContextName(),
                                         pZCBox->GetMapContextName().IsEmpty() ?
                                          nFlag | zUSE_DEFAULT_CONTEXT :
                                          (zSHORT) nFlag );
            }
         }
      }
   }

   return nRC;
}


//./ ADD NAME=ZGridColumn::CheckLocking
// Source Module=tdbg6.cpp
//**************************************************************************/
//
//  ENTRY:       ZGridColumn::CheckLocking
//                                            3/6/2001Modified
//  PURPOSE:    check whether the underlying attribute is updateable / includeable
//              if not the column is disabled
//
//  PARAMETERS: zVIEW vApp - the view
/**************************************************************************/
//./END + 1
BOOL ZGridColumn::CheckLocking( zVIEW vApp )
{
   if( !m_pParentGrid ) // no grid?
      return FALSE;

   if( m_bInitiallyLocked ) // if locked due to property dialog setting return immediately
      return m_bInitiallyLocked;

   Columns cols( m_pParentGrid->GetColumns() );
   // by default any column is editable
   BOOL bColumnEditable = TRUE;

   // for backward compatibility
   ZMapping *pMapping = GetMapping( );

   Column c = cols.GetColumn( m_nMyGridColumn );

   if( pMapping )// use olde mode
   {
      if( CheckExistenceOfEntity( vApp, pMapping->m_zsEName ) == zCURSOR_SET )
         bColumnEditable = MiGetUpdateForViewEntityAttr( vApp,
                                       pMapping->m_zsEName,
                                       pMapping->m_zsAName,
                                       0 );
   }
   else
   {
      // check to see whether it is the new mapping implementation
      ZColumnMapping *pNewMapping = GetMappingEx();
      if( pNewMapping )// use new mode
      {
         zSHORT sOption = 0;
         if( m_pZeidonControlColumn )
         {
            // check whether it is a combobox and if its type of autoinclude
            // to set the option for MiGetUpdateForViewEntityAttr properly
            if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
            {
               zLONG nFormatType = (( ZColComboBox *)m_pZeidonControlColumn)->GetFormatType();
               if( nFormatType & zCOMBOBOX_INCLUDE )
                  sOption = zUPD_INCLUDE;
            }
         }
         if( CheckExistenceOfEntity( vApp, pNewMapping->GetMapEntityName() ) == zCURSOR_SET )
            bColumnEditable = MiGetUpdateForViewEntityAttr( vApp,
                                          pNewMapping->GetMapEntityName(),
                                          pNewMapping->GetMapAttributeName(),
                                          sOption );
      }
   }
   // check for update permission
   // if there is no update permission we still have to check the AllowAddNew property
   // to give the user a chance to create entities on demand
   if( bColumnEditable || m_pParentGrid->GetAllowAddNew() )
   {
      c.SetLocked( FALSE );
      c.SetAllowFocus( TRUE );
      c.SetBackColor( GetUnlockColor() );
   }
   else
   {
      c.SetLocked( TRUE );
      c.SetAllowFocus( FALSE );
      c.SetBackColor( GetLockColor() );
   }
   return bColumnEditable;
}


//./ ADD NAME=ZGridColumn::GetMappingEx
// Source Module=tdbg6.cpp
//**************************************************************************/
//
//  ENTRY:       ZGridColumn::GetMappingEx
//                                            4/26/2001 Modified
//  PURPOSE:     to get a mapping object of the current column
//
//  PARAMETERS:
/**************************************************************************/
//./END + 1
ZColumnMapping * ZGridColumn::GetMappingEx( )
{
   ZColumnMapping *p = NULL;
   if( m_pZeidonControlColumn )
      p = m_pZeidonControlColumn->GetColumnMapping();

   return p;
}


void ZGridColumn::SetInitiallyLocked( BOOL bLockIt )
{

   Columns cols( m_pParentGrid->GetColumns() );
   // by default any column is not editable
   BOOL bColumnEditable = FALSE;

   if( bLockIt == IsInitiallyLocked() ) // there is nothing to do
      return;

   // for backward compatibility
   ZMapping *pMapping = GetMapping( );

   Column c = cols.GetColumn( m_nMyGridColumn );

   if ( !bLockIt ) // if it's going to get unlocked check if Zeidon allows update
   {
      if( pMapping )// use olde mode
      {
         zVIEW vApp = 0;
         GetViewByName( &vApp, pMapping->m_zsVName,
                          m_pParentGrid->GetDialogView(), zLEVEL_ANY );
         if( vApp )
         {
            bColumnEditable = MiGetUpdateForViewEntityAttr( vApp,
                                       pMapping->m_zsEName,
                                       pMapping->m_zsAName,
                                       0 );
         }
      }
      else
      {
         // check to see whether it is the new mapping implementation
         ZColumnMapping *pNewMapping = GetMappingEx();
         if( pNewMapping )// use new mode
         {
            zSHORT sOption = 0;
            if( m_pZeidonControlColumn )
            {
               // check whether it is a combobox and if its type of autoinclude
               // to set the option for MiGetUpdateForViewEntityAttr properly
               if( m_pZeidonControlColumn->IsKindOf( RUNTIME_CLASS( ZColComboBox ) ) )
               {
                  zLONG nFormatType = (( ZColComboBox *)m_pZeidonControlColumn)->GetFormatType();
                  if( nFormatType & zCOMBOBOX_INCLUDE )
                     sOption = zUPD_INCLUDE;
               }
            }
            zVIEW vApp = 0;
            GetViewByName( &vApp, pNewMapping->GetMapViewName(),
                             m_pParentGrid->GetDialogView(), zLEVEL_ANY );
            if( vApp )
            {
               bColumnEditable = MiGetUpdateForViewEntityAttr( vApp,
                                             pNewMapping->GetMapEntityName(),
                                             pNewMapping->GetMapAttributeName(),
                                             sOption );
            }
         }
      }
   }

   if( bColumnEditable )
   {
      c.SetLocked( FALSE );
      c.SetAllowFocus( TRUE );
      c.SetBackColor( GetUnlockColor() );
      SetReadOnly( FALSE );
   }
   else
   {
      c.SetLocked( TRUE );
      c.SetAllowFocus( FALSE );
      c.SetBackColor( GetLockColor() );
      SetReadOnly( TRUE );
   }

   m_bInitiallyLocked = bLockIt;

}


void ZGridColumn::SetReadOnly( BOOL bSet )
{
   if( IsInitiallyLocked() )
   {
      m_bReadOnly = TRUE;
   }
   else
   {
      m_bReadOnly = bSet;
   }
}

BOOL ITrueDBGridCtrl::EnableAutosort(BOOL bEnable)
{
   BOOL bOldValue = m_bAutosortEnabled;
   m_bAutosortEnabled = bEnable;
   return bOldValue;
}

void ITrueDBGridCtrl::OnLooseFocus()
{
   UpdateAllColumnValues();
}

zBOOL ITrueDBGridCtrl::CreateDateTimeCtrl(int nCol)
{
   zBOOL bRC = FALSE;
   HFONT hFontTab = NULL;
   int nColumn = nCol;

   if( nColumn < 0 )
      nColumn = GetCol();

   TRY
   {
      LPDISPATCH lpDispatchColumns = NULL;
      lpDispatchColumns = GetColumns();
      if ( lpDispatchColumns )
      {
         Columns cols( lpDispatchColumns );
         Column col = cols.GetColumn( nColumn );

         if ( m_pWndArray == 0 )
         {
            m_pWndDateCtrl = 0;
            zLONG lColumns = cols.GetCount( );
            m_pWndArray = new ZGridDateCtrl*[ lColumns ];

            for ( long k = 0; k < lColumns; k++ )
               m_pWndArray[ k ] = NULL;

            hFontTab = (HFONT) ::GetStockObject( DEFAULT_GUI_FONT );
            if ( !hFontTab )
               hFontTab = (HFONT) ::GetStockObject( ANSI_VAR_FONT );
         }

         /*
         if ( m_pWndDateCtrl &&
              ::IsWindowVisible( m_pWndDateCtrl->m_hWnd ) &&
              m_pWndDateCtrl == m_pWndArray[ nColumn ] )
         {
            m_pWndDateCtrl->ShowWindow( SW_HIDE );
            m_pWndDateCtrl = 0;
            return( FALSE );
         }
         */

         m_pWndDateCtrl = m_pWndArray[ nColumn ];

         if ( !m_pWndDateCtrl )
         {
            m_pWndDateCtrl = new ZGridDateCtrl();
            m_pWndArray[ nColumn ] = m_pWndDateCtrl;
            m_pWndDateCtrl->Create( WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                                    CRect( 0, 0, 0, 0 ),
                                    m_pZActiveX->m_pZSubtask->m_pZView, -1 );
            // remove border & 3d style implemented by grateful help of GAAZ
            m_pWndDateCtrl->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_WINDOWEDGE |
                                          WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);

            // TMV
            m_pWndDateCtrl->m_pParentGrid = this;
            m_pWndDateCtrl->SendMessage( WM_SETFONT, (WPARAM) hFontTab );
         }

         ZOleDateTime dtDateTime;
         zPCHAR pValue = NULL;
         zVIEW vApp = NULL;

         ZGridColumn *pGC = GetGridColumn( nColumn );
         vApp = GetViewNamed( );

         if( !pGC || !vApp )
            return ( FALSE );

         CString strValue;
         strValue.Empty();
         //get the value pure zeidon format assuming it is date
         pGC->CheckLocking( vApp );
         pGC->GetStringFromAttribute( strValue, vApp );

         if( strValue.IsEmpty() )
         {
            dtDateTime = COleDateTime::GetCurrentTime();
         }
         else
         {
            dtDateTime.SetValue( strValue );
         }
         m_pWndDateCtrl->SetTime( dtDateTime );
                  CRect rect;
         HWND hWnd = (HWND)GetHWndEditor();
         ::GetWindowRect( hWnd, rect );

//         rect.top = rect.top + rect.Height();

         m_pZActiveX->m_pZSubtask->m_pZView->ScreenToClient( rect );

         //rect.bottom = rect.top + GetRowHeight();

         m_pWndDateCtrl->SetWindowPos( &CWnd::wndTop,
                                      (INT) rect.left, (INT) rect.top,
                                      (INT) rect.Width(),
                                      (INT) rect.Height(),
                                      SWP_SHOWWINDOW );
         m_pWndDateCtrl->SetFocus( );
      }
   }

   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ITrueDBGridCtrl::CreateDateTimeCtrl" );
      bRC = FALSE;
   }
   END_CATCH_ALL

   return bRC;

}

zSHORT ITrueDBGridCtrl::SetDateCtrlForColumn(zLONG nColumn, zBOOL bSet )
{
   zSHORT nRC = zCALL_ERROR;
   ZGridColumn *pGC = GetGridColumn( (zSHORT)nColumn );

   if( pGC )
   {
      pGC->SetDateCtrl( bSet );
      nRC = 0;
   }

   return nRC;
}


/////////////////////////////////////////////////////////////////////////////
// COleFont properties

CString COleFont::GetName()
{
   CString result;
   GetProperty( DISPID_FONT_NAME, VT_BSTR, (void*)&result);
   return result;
}

void COleFont::SetName(LPCTSTR propVal)
{
   SetProperty( DISPID_FONT_NAME, VT_BSTR, propVal);
}

CY COleFont::GetSize()
{
   CY result;
   GetProperty( DISPID_FONT_SIZE , VT_CY, (void*)&result);
   return result;
}

void COleFont::SetSize(const CY& propVal)
{
   SetProperty( DISPID_FONT_SIZE , VT_CY, &propVal);
}


BOOL COleFont::GetBold()
{
   BOOL result;
   GetProperty( DISPID_FONT_BOLD, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetBold(BOOL propVal)
{
   SetProperty( DISPID_FONT_BOLD, VT_BOOL, propVal);
}

BOOL COleFont::GetItalic()
{
   BOOL result;
   GetProperty( DISPID_FONT_ITALIC, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetItalic(BOOL propVal)
{
   SetProperty( DISPID_FONT_ITALIC, VT_BOOL, propVal);
}

BOOL COleFont::GetUnderline()
{
   BOOL result;
   GetProperty( DISPID_FONT_UNDER, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetUnderline(BOOL propVal)
{
   SetProperty( DISPID_FONT_UNDER, VT_BOOL, propVal);
}


BOOL COleFont::GetStrikethrough()
{
   BOOL result;
   GetProperty( DISPID_FONT_STRIKE, VT_BOOL, (void*)&result);
   return result;
}

void COleFont::SetStrikethrough(BOOL propVal)
{
   SetProperty( DISPID_FONT_STRIKE, VT_BOOL, propVal);
}

short COleFont::GetWeight()
{
   short result;
   GetProperty( DISPID_FONT_WEIGHT , VT_I2, (void*)&result);
   return result;
}

void COleFont::SetWeight(short propVal)
{
   SetProperty( DISPID_FONT_WEIGHT , VT_I2, propVal);
}

short COleFont::GetCharset()
{
   short result;
   GetProperty( DISPID_FONT_CHARSET , VT_I2, (void*)&result);
   return result;
}

void COleFont::SetCharset(short propVal)
{
   SetProperty( DISPID_FONT_CHARSET , VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// COleFont operations



/////////////////////////////////////////////////////////////////////////////
// CPicture properties

long CPicture::GetHandle()
{
   long result;
   GetProperty(0x0, VT_I4, (void*)&result);
   return result;
}

long CPicture::GetHPal()
{
   long result;
   GetProperty(0x2, VT_I4, (void*)&result);
   return result;
}

void CPicture::SetHPal(long propVal)
{
   SetProperty(0x2, VT_I4, propVal);
}

short CPicture::GetType()
{
   short result;
   GetProperty(0x3, VT_I2, (void*)&result);
   return result;
}

long CPicture::GetWidth()
{
   long result;
   GetProperty(0x4, VT_I4, (void*)&result);
   return result;
}

long CPicture::GetHeight()
{
   long result;
   GetProperty(0x5, VT_I4, (void*)&result);
   return result;
}


IMPLEMENT_DYNAMIC( ZGridSplit, CObject )

ZGridSplit::ZGridSplit( )
{
   Init();
}

ZGridSplit::ZGridSplit( ITrueDBGridCtrl * pParent )
{
   Init();
   m_pParentGrid = pParent;
}

void ZGridSplit::Init()
{
   m_pParentGrid = NULL;
   SetLockColor( RGB( 155,155,155 ) );
   m_nMySplit = -1;
}

ZGridSplit::~ZGridSplit( )
{
}


/**************************************************************************/
// Function name   : ZGridSplit::SetColumnIndex
// Description     : Assigns a Grid column to an ZGridSplit instance. This
//                   function can be calles only once, otherwise an assertion
//                   occurs. This is to prevent inconsistant mapping via the ZMapping
//                   member that is supplied by a seperate call ( see SetMapping... )
// Return type     : void
// Argument        : short nIndex Index of the column shown in the grid
/**************************************************************************/
void ZGridSplit::SetSplitIndex( short nIndex )
{
   ASSERT( m_nMySplit < 0 ); // make sure this is called only once

   if ( !m_pParentGrid )
   {
      TraceLineS( "ZGridSplit ", "No Parent Grid attached");
      return;
   }

   Splits splits( m_pParentGrid->GetSplits() );

   if ( m_nMySplit < 0 && splits.GetCount() > nIndex )
      m_nMySplit = nIndex;
   else
   {
      TraceLineI( "Unable to set internal properties for Split ", nIndex );
      return;
   }

   Split split = splits.GetSplit( m_nMySplit );
   // make sure that there is a difference between lock and unlook color
   if( m_pParentGrid->GetInactiveBackColor() != split.GetBackColor() )
      SetLockColor( m_pParentGrid->GetInactiveBackColor() );

   SetUnlockColor( split.GetBackColor() );
}


/////////////////////////////////////////////////////////////////////////////
// ZGridDateCtrl

BEGIN_MESSAGE_MAP(ZGridDateCtrl, CDateTimeCtrl )
   //{{AFX_MSG_MAP(ZGridDateCtrl)
      // NOTE - the ClassWizard will add and remove mapping macros here.
   ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
   ON_NOTIFY_REFLECT(DTN_DATETIMECHANGE, OnDatetimeChange)
   ON_NOTIFY_REFLECT(DTN_DROPDOWN, OnDropdown)
   ON_NOTIFY_REFLECT(DTN_FORMAT, OnFormatDatetime)
   ON_NOTIFY_REFLECT(DTN_FORMATQUERY, OnFormatquery)
   ON_NOTIFY_REFLECT(DTN_USERSTRING, OnUserString)
   ON_NOTIFY_REFLECT(DTN_WMKEYDOWN, OnWMKeydown)
   ON_WM_SETFOCUS( )
   ON_WM_KILLFOCUS( )
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

ZGridDateCtrl::ZGridDateCtrl()
{
}

ZGridDateCtrl::~ZGridDateCtrl()
{
}

void ZGridDateCtrl::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnCloseup" );
#endif // _DEBUG

   *pResult = 0;
}

void ZGridDateCtrl::OnDatetimeChange(NMHDR* pNMHDR, LRESULT* pResult)
{
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnDatetimeChange" );
#endif // _DEBUG

   LPNMDATETIMECHANGE lpChange = (LPNMDATETIMECHANGE) pNMHDR;

   ZOleDateTime dt = lpChange->st;

   ZGridColumn *pGC = m_pParentGrid->GetGridColumn( m_pParentGrid->GetCol() );
   if( pGC )
   {
      zVIEW vApp = m_pParentGrid->GetViewNamed();
      if( pGC->UpdateAttributeEx( vApp, dt, TRUE ) == 0 )//validation
      {
         pGC->UpdateAttributeEx( vApp, dt ); // real update to OI
         CString strText = pGC->GetAttributeValue( vApp );
         m_pParentGrid->SetCellData( strText, -1, -1 );
      }
   }
   *pResult = 0;
}

void ZGridDateCtrl::OnDropdown(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnDropdown" );
#endif // _DEBUG

   *pResult = 0;
}

void ZGridDateCtrl::OnFormatDatetime(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnFormatDatetime" );
#endif // _DEBUG

   *pResult = 0;
}

void ZGridDateCtrl::OnFormatquery(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnFormatquery" );
#endif // _DEBUG

   *pResult = 0;
}

void ZGridDateCtrl::OnUserString(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnUserString" );
#endif // _DEBUG

   *pResult = 0;
}

void ZGridDateCtrl::OnWMKeydown(NMHDR* pNMHDR, LRESULT* pResult)
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnWMKeydown" );
#endif // _DEBUG

   *pResult = 0;
}



void ZGridDateCtrl::OnKillFocus( CWnd *pWndGetFocus )
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnKillFocus" );
#endif // _DEBUG

   SendMessage( DTN_CLOSEUP, 0L, 0L );
   CDateTimeCtrl::OnKillFocus( pWndGetFocus );
   ShowWindow( SW_HIDE );
}

void ZGridDateCtrl::OnSetFocus( CWnd *pWndLostFocus )
{
   // TODO: Add your control notification handler code here
#ifdef _DEBUG
   TraceLineS( "ZGridDateCtrl ","OnSetFocus" );
#endif // _DEBUG
   CDateTimeCtrl::OnSetFocus( pWndLostFocus );

}


ZOleDateTime::ZOleDateTime( const COleDateTime& dateSrc ) :   COleDateTime( dateSrc )
{
}
ZOleDateTime::ZOleDateTime( const VARIANT& varSrc ) : COleDateTime( varSrc )
{
}

ZOleDateTime::ZOleDateTime(DATE dtSrc) : COleDateTime( dtSrc )
{
}

ZOleDateTime::ZOleDateTime(time_t timeSrc) : COleDateTime( timeSrc )
{
}

ZOleDateTime::ZOleDateTime(const SYSTEMTIME& systimeSrc) : COleDateTime( systimeSrc )
{
}

ZOleDateTime::ZOleDateTime( const FILETIME& filetimeSrc ) : COleDateTime(filetimeSrc)
{
}

ZOleDateTime::ZOleDateTime(int nYear, int nMonth, int nDay,
                           int nHour, int nMin, int nSec)
             : COleDateTime( nYear, nMonth, nDay,
                             nHour, nMin, nSec )
{
}

ZOleDateTime::ZOleDateTime(WORD wDosDate, WORD wDosTime)
             : COleDateTime( wDosDate, wDosTime)
{
}

ZOleDateTime::ZOleDateTime( zCPCHAR pValue )
{
   if( pValue )
      SetValue( pValue );
}

void
ZOleDateTime::operator = ( zCPCHAR pValue )
{
   SetValue( pValue );
}

const ZOleDateTime& ZOleDateTime::operator=(const COleDateTime& dateSrc)
{
   m_dt = dateSrc.m_dt; m_status = dateSrc.m_status; return *this;
}

void
ZOleDateTime::SetValue( zCPCHAR pValue )
{
   int iYearPart = 4;
   int iMonthPart = 2;
   int iDayPart = 2;
   int iHourPart = 2;
   int iMinPart = 2;
   int iSecPart = 2;
   int iFractionPart = 3;

   zCHAR szYearPart[5];
   zCHAR szMonthPart[3];
   zCHAR szDayPart[3];
   zCHAR szHourPart[5];
   zCHAR szMinPart[3];
   zCHAR szSecPart[3];
   zCHAR szFractionPart[4];

   if( zstrlen( pValue ) != ZEIDON_DATETIME_LTH )
      return;

   for( zPCHAR p = (zPCHAR)pValue; *p != '\0'; p++ )
   {
      if ( !::isdigit( *p ) )
         return;
   }

   zstrncpy( szYearPart, pValue, iYearPart );
   szYearPart[4] = '\0';
   int nOffset = iYearPart;

   zstrncpy( szMonthPart, pValue + nOffset , iMonthPart );
   szMonthPart[2] = '\0';
   nOffset += iMonthPart;

   zstrncpy( szDayPart, pValue + nOffset , iDayPart );
   szDayPart[2] = '\0';
   nOffset += iDayPart;

   zstrncpy( szHourPart, pValue + nOffset , iHourPart );
   szHourPart[2] = '\0';
   nOffset += iHourPart;

   zstrncpy( szMinPart, pValue + nOffset , iMinPart );
   szMinPart[2] = '\0';
   nOffset += iMinPart;

   zstrncpy( szSecPart, pValue + nOffset , iSecPart );
   szSecPart[2] = '\0';
   nOffset += iSecPart;

   zstrncpy( szFractionPart, pValue + nOffset , iFractionPart );
   szFractionPart[2] = '\0';
   nOffset += iFractionPart;

   iYearPart = atoi( szYearPart );
   iMonthPart = atoi( szMonthPart );
   iDayPart = atoi( szDayPart );
   iHourPart = atoi( szHourPart );
   iMinPart = atoi( szMinPart );
   iSecPart = atoi( szSecPart );
   iFractionPart = atoi( szFractionPart );

   SetDateTime( iYearPart, iMonthPart, iDayPart,
                iHourPart, iMinPart, iSecPart );
}

CString ZOleDateTime::GetValue()
{
   CString strHelp;
   strHelp.Format( "%04d%02d%02d%02d%02d%02d000",GetYear(),
                                                 GetMonth(),
                                                 GetDay(),
                                                 GetHour(),
                                                 GetMinute(),
                                                 GetSecond());
   return strHelp;
}

ZOleDateTime::operator CString() const
{
   CString strHelp;
   strHelp.Format( "%04d%02d%02d%02d%02d%02d000",GetYear(),
                                                 GetMonth(),
                                                 GetDay(),
                                                 GetHour(),
                                                 GetMinute(),
                                                 GetSecond());
   return strHelp;
}
