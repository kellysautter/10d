// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "zstdafx.h"
#include "tzcswrpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ICodeSense properties

/////////////////////////////////////////////////////////////////////////////
// ICodeSense operations

void ICodeSense::SetRefFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeSense::SetFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH ICodeSense::GetFont()
{
   LPDISPATCH result;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetEnabled(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetEnabled()
{
   BOOL result;
   InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetWindow()
{
   long result;
   InvokeHelper(DISPID_HWND, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ICodeSense::GetText()
{
   CString result;
   InvokeHelper(DISPID_TEXT, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetColorSyntax(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetColorSyntax()
{
   BOOL result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetDisplayWhitespace(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetDisplayWhitespace()
{
   BOOL result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetExpandTabs(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetExpandTabs()
{
   BOOL result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetSmoothScrolling(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetSmoothScrolling()
{
   BOOL result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetReadOnly(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetReadOnly()
{
   BOOL result;
   InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetLineToolTips(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetLineToolTips()
{
   BOOL result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetDisplayLeftMargin(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetDisplayLeftMargin()
{
   BOOL result;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetEnableColumnSel(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetEnableColumnSel()
{
   BOOL result;
   InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetEnableDragDrop(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetEnableDragDrop()
{
   BOOL result;
   InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetOvertype(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetOvertype()
{
   BOOL result;
   InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetSearchCaseSensitive(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetSearchCaseSensitive()
{
   BOOL result;
   InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetReplacePreserveCase(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetReplacePreserveCase()
{
   BOOL result;
   InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetSearchWholeWord(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetSearchWholeWord()
{
   BOOL result;
   InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetModified(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetModified()
{
   BOOL result;
   InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetEnableCRLF(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetEnableCRLF()
{
   BOOL result;
   InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetGlobalProps(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetGlobalProps()
{
   BOOL result;
   InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetVScrollVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetVScrollVisible()
{
   BOOL result;
   InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetHScrollVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetHScrollVisible()
{
   BOOL result;
   InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetEnableVSplitter(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetEnableVSplitter()
{
   BOOL result;
   InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetEnableHSplitter(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetEnableHSplitter()
{
   BOOL result;
   InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetLanguage(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ICodeSense::GetLanguage()
{
   CString result;
   InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetTabSize(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetTabSize()
{
   long result;
   InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetTopIndex(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetTopIndex()
{
   long result;
   InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetUndoLimit(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetUndoLimit()
{
   long result;
   InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetCurrentView()
{
   long result;
   InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetTextLength()
{
   long result;
   InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetTextLengthLogical()
{
   long result;
   InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetLineCount()
{
   long result;
   InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString ICodeSense::GetCurrentWord()
{
   CString result;
   InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetCurrentWordLength()
{
   long result;
   InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetCurrentWordLengthLogical()
{
   long result;
   InvokeHelper(0x69, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetAutoIndentMode(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetAutoIndentMode()
{
   long result;
   InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetViewCount()
{
   long result;
   InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetVSplitterPos(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetVSplitterPos()
{
   long result;
   InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetHSplitterPos(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetHSplitterPos()
{
   long result;
   InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetFindText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ICodeSense::GetFindText()
{
   CString result;
   InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetReplText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ICodeSense::GetReplText()
{
   CString result;
   InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetLineNumbering(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x57, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetLineNumbering()
{
   BOOL result;
   InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetLineNumberStart(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x58, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetLineNumberStart()
{
   long result;
   InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetLineNumberStyle(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetLineNumberStyle()
{
   long result;
   InvokeHelper(0x59, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString ICodeSense::GetWord(LPDISPATCH Pos)
{
   CString result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x23, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      Pos);
   return result;
}

long ICodeSense::GetWordLength(LPDISPATCH Pos, BOOL bLogical)
{
   long result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_BOOL;
   InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      Pos, bLogical);
   return result;
}

long ICodeSense::GetTopIndex(long lView)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lView);
   return result;
}

void ICodeSense::SetTopIndex(long lView, long lLine)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lView, lLine);
}

LPDISPATCH ICodeSense::GetSel(BOOL bNormalized)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x27, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      bNormalized);
   return result;
}

void ICodeSense::SetSel(LPDISPATCH Sel, BOOL bMakeVisible)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_BOOL;
   InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Sel, bMakeVisible);
}

LPDISPATCH ICodeSense::GetSelFromPoint(long lxClientPos, long lyClientPos)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x29, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      lxClientPos, lyClientPos);
   return result;
}

long ICodeSense::GetVisibleLineCount(long lView, BOOL bFullyVisible)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_BOOL;
   InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lView, bFullyVisible);
   return result;
}

CString ICodeSense::GetLine(long lLine)
{
   CString result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      lLine);
   return result;
}

long ICodeSense::GetLineLength(long lLine)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lLine);
   return result;
}

void ICodeSense::SetColor(long lColorItem, unsigned long crColor)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lColorItem, crColor);
}

unsigned long ICodeSense::GetColor(long lColorItem)
{
   unsigned long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lColorItem);
   return result;
}

void ICodeSense::OpenFile(LPCTSTR strFileName)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strFileName);
}

void ICodeSense::SaveFile(LPCTSTR strFileName, BOOL bClearUndo)
{
   static BYTE parms[] =
      VTS_BSTR VTS_BOOL;
   InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strFileName, bClearUndo);
}

void ICodeSense::InsertFile(LPCTSTR strFileName, LPDISPATCH InsertPos)
{
   static BYTE parms[] =
      VTS_BSTR VTS_DISPATCH;
   InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strFileName, InsertPos);
}

void ICodeSense::InsertText(LPCTSTR strText, LPDISPATCH InsertPos)
{
   static BYTE parms[] =
      VTS_BSTR VTS_DISPATCH;
   InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strText, InsertPos);
}

void ICodeSense::ReplaceText(LPCTSTR strText, LPDISPATCH Range)
{
   static BYTE parms[] =
      VTS_BSTR VTS_DISPATCH;
   InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strText, Range);
}

long ICodeSense::HitTest(long lxClientPos, long lyClientPos, long* plView)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_PI4;
   InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lxClientPos, lyClientPos, plView);
   return result;
}

void ICodeSense::AddText(LPCTSTR strText)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strText);
}

void ICodeSense::DeleteLine(long lLine)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine);
}

void ICodeSense::InsertLine(long lLine, LPCTSTR strLine)
{
   static BYTE parms[] =
      VTS_I4 VTS_BSTR;
   InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, strLine);
}

void ICodeSense::SelectLine(long lLine, BOOL bMakeVisible)
{
   static BYTE parms[] =
      VTS_I4 VTS_BOOL;
   InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, bMakeVisible);
}

void ICodeSense::DeleteSel()
{
   InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeSense::ReplaceSel(LPCTSTR strText)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strText);
}

void ICodeSense::ExecuteCmd(long lCommand, const VARIANT& CmdData)
{
   static BYTE parms[] =
      VTS_I4 VTS_VARIANT;
   InvokeHelper(0x3b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lCommand, &CmdData);
}

BOOL ICodeSense::CanUndo()
{
   BOOL result;
   InvokeHelper(0x3c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL ICodeSense::CanRedo()
{
   BOOL result;
   InvokeHelper(0x3d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL ICodeSense::CanCut()
{
   BOOL result;
   InvokeHelper(0x3e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL ICodeSense::CanCopy()
{
   BOOL result;
   InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL ICodeSense::CanPaste()
{
   BOOL result;
   InvokeHelper(0x40, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::Undo()
{
   InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeSense::Redo()
{
   InvokeHelper(0x42, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeSense::Cut()
{
   InvokeHelper(0x43, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeSense::Copy()
{
   InvokeHelper(0x44, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeSense::Paste()
{
   InvokeHelper(0x45, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeSense::ClearUndoBuffer()
{
   InvokeHelper(0x46, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL ICodeSense::IsRecordingMacro()
{
   BOOL result;
   InvokeHelper(0x47, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL ICodeSense::IsPlayingMacro()
{
   BOOL result;
   InvokeHelper(0x48, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetDlgParent(long lhWnd)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x49, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lhWnd);
}

CString ICodeSense::GetSelText()
{
   CString result;
   InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetSelText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

void ICodeSense::SetSelBounds(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetSelBounds()
{
   BOOL result;
   InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetFontStyle(long lFontStyleItem, long byFontStyle)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x4c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lFontStyleItem, byFontStyle);
}

long ICodeSense::GetFontStyle(long lFontStyleItem)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x4d, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lFontStyleItem);
   return result;
}

void ICodeSense::SetSearchRegExp(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetSearchRegExp()
{
   BOOL result;
   InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetItemData(long lLine, long lData)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x4f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, lData);
}

long ICodeSense::GetItemData(long lLine)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x50, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lLine);
   return result;
}

void ICodeSense::SetLineStyle(long lLine, long lStyle)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x51, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, lStyle);
}

long ICodeSense::GetLineStyle(long lLine)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x52, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lLine);
   return result;
}

void ICodeSense::SetBookmark(long lLine, BOOL bVal)
{
   static BYTE parms[] =
      VTS_I4 VTS_BOOL;
   InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, bVal);
}

BOOL ICodeSense::GetBookmark(long lLine)
{
   BOOL result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x54, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      lLine);
   return result;
}

LPDISPATCH ICodeSense::PosFromChar(LPDISPATCH pPos)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x5a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      pPos);
   return result;
}

void ICodeSense::SetHideSel(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetHideSel()
{
   BOOL result;
   InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetHighlightedLine(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetHighlightedLine()
{
   long result;
   InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetNormalizeCase(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetNormalizeCase()
{
   BOOL result;
   InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetDivider(long lLine, BOOL bVal)
{
   static BYTE parms[] =
      VTS_I4 VTS_BOOL;
   InvokeHelper(0x5e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, bVal);
}

BOOL ICodeSense::GetDivider(long lLine)
{
   BOOL result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x5f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      lLine);
   return result;
}

void ICodeSense::SetOvertypeCaret(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetOvertypeCaret()
{
   BOOL result;
   InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

LPDISPATCH ICodeSense::GetImageList()
{
   LPDISPATCH result;
   InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetImageList(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeSense::SetRefImageList(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x64, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeSense::PrintContents(long hDC, long lFlags)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       hDC, lFlags);
}

void ICodeSense::SetCaretPos(long lLine, long lCol)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lLine, lCol);
}

long ICodeSense::ViewColToBufferCol(long lLine, long lViewCol)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x6a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lLine, lViewCol);
   return result;
}

long ICodeSense::BufferColToViewCol(long lLine, long lBuffCol)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x6b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lLine, lBuffCol);
   return result;
}

long ICodeSense::GetSelLength()
{
   long result;
   InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetSelLengthLogical()
{
   long result;
   InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetBorderStyle(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeSense::GetBorderStyle()
{
   long result;
   InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetHWnd()
{
   long result;
   InvokeHelper(0x6f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::SetCurrentView(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

void ICodeSense::SetStealKeys(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x70, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ICodeSense::GetStealKeys()
{
   BOOL result;
   InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

long ICodeSense::GetCurrentToken()
{
   long result;
   InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeSense::UpdateControlPositions()
{
   InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IPosition properties

/////////////////////////////////////////////////////////////////////////////
// IPosition operations

void IPosition::SetLineNo(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IPosition::GetLineNo()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IPosition::SetColNo(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IPosition::GetColNo()
{
   long result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// IRange properties

/////////////////////////////////////////////////////////////////////////////
// IRange operations

void IRange::SetStartLineNo(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRange::GetStartLineNo()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRange::SetStartColNo(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRange::GetStartColNo()
{
   long result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRange::SetEndLineNo(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRange::GetEndLineNo()
{
   long result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRange::SetEndColNo(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRange::GetEndColNo()
{
   long result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRange::SetColumnSel(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL IRange::GetColumnSel()
{
   BOOL result;
   InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL IRange::IsEmpty()
{
   BOOL result;
   InvokeHelper(0x4, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}

void IRange::Empty()
{
   InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IRange::Normalize()
{
   InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IRect properties

/////////////////////////////////////////////////////////////////////////////
// IRect operations

void IRect::SetLeft(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRect::GetLeft()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRect::SetTop(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRect::GetTop()
{
   long result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRect::SetRight(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRect::GetRight()
{
   long result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IRect::SetBottom(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IRect::GetBottom()
{
   long result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// ICodeSenseEvents properties

/////////////////////////////////////////////////////////////////////////////
// ICodeSenseEvents operations

void ICodeSenseEvents::Change(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::HScroll(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::VScroll(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::SelChange(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::ViewChange(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::ModifiedChange(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::ShowProps(LPDISPATCH Control, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, pbStop);
}

void ICodeSenseEvents::PropsChange(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::Create(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::Destroy(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::SetFocus(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::KillFocus(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::Click(LPDISPATCH Control, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, pbStop);
}

void ICodeSenseEvents::DblClick(LPDISPATCH Control, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, pbStop);
}

void ICodeSenseEvents::RDblClick(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::Return(LPDISPATCH Control, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, pbStop);
}

void ICodeSenseEvents::RClick(LPDISPATCH Control, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, pbStop);
}

void ICodeSenseEvents::DeleteLine(LPDISPATCH Control, long lLine, long lItemData)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4;
   InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, lLine, lItemData);
}

void ICodeSenseEvents::CmdFailure(LPDISPATCH Control, long lCmd, long lErrCode, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, lCmd, lErrCode, pbStop);
}

void ICodeSenseEvents::RegisteredCmd(LPDISPATCH Control, long lCmd)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4;
   InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, lCmd);
}

void ICodeSenseEvents::KeyDown(LPDISPATCH Control, long KeyCode, long Shift, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, KeyCode, Shift, pbStop);
}

void ICodeSenseEvents::KeyUp(LPDISPATCH Control, long KeyCode, long Shift, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, KeyCode, Shift, pbStop);
}

void ICodeSenseEvents::KeyPress(LPDISPATCH Control, long KeyAscii, long Shift, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, KeyAscii, Shift, pbStop);
}

void ICodeSenseEvents::MouseDown(LPDISPATCH Control, long Button, long Shift, long X, long Y, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, Button, Shift, X, Y, pbStop);
}

void ICodeSenseEvents::MouseMove(LPDISPATCH Control, long Button, long Shift, long X, long Y, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, Button, Shift, X, Y, pbStop);
}

void ICodeSenseEvents::MouseUp(LPDISPATCH Control, long Button, long Shift, long X, long Y, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, Button, Shift, X, Y, pbStop);
}

void ICodeSenseEvents::OvertypeChange(LPDISPATCH Control)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control);
}

void ICodeSenseEvents::FindWrapped(LPDISPATCH Control, long lCmd, BOOL bForward, BOOL* pbStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4 VTS_BOOL VTS_PBOOL;
   InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, lCmd, bForward, pbStop);
}

void ICodeSenseEvents::CodeList(LPDISPATCH Control, LPDISPATCH ListCtrl, BOOL* bContinue)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, bContinue);
}

void ICodeSenseEvents::CodeListSelMade(LPDISPATCH Control, LPDISPATCH ListCtrl, BOOL* bStop)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, bStop);
}

void ICodeSenseEvents::CodeListCancel(LPDISPATCH Control, LPDISPATCH ListCtrl, BOOL* bKeepVisible)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, bKeepVisible);
}

void ICodeSenseEvents::CodeListChar(LPDISPATCH Control, LPDISPATCH ListCtrl, long wChar, long lKeyData, BOOL* bHandled)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_I4 VTS_I4 VTS_PBOOL;
   InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, wChar, lKeyData, bHandled);
}

void ICodeSenseEvents::CodeTip(LPDISPATCH Control, long* ToolTipType)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_PI4;
   InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ToolTipType);
}

void ICodeSenseEvents::CodeTipCancel(LPDISPATCH Control, LPDISPATCH ToolTipCtrl, BOOL* bKeepVisible)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_PBOOL;
   InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ToolTipCtrl, bKeepVisible);
}

void ICodeSenseEvents::CodeTipUpdate(LPDISPATCH Control, LPDISPATCH ToolTipCtrl)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH;
   InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ToolTipCtrl);
}

void ICodeSenseEvents::CodeListSelWord(LPDISPATCH Control, LPDISPATCH ListCtrl, long lItem, BOOL* bAllowSelection)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_I4 VTS_PBOOL;
   InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, lItem, bAllowSelection);
}

void ICodeSenseEvents::CodeListSelChange(LPDISPATCH Control, LPDISPATCH ListCtrl, long lItem, BSTR* strTipText)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_I4 VTS_PBSTR;
   InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, lItem, strTipText);
}

void ICodeSenseEvents::CodeListHotTrack(LPDISPATCH Control, LPDISPATCH ListCtrl, long lItem)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH VTS_I4;
   InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ListCtrl, lItem);
}

void ICodeSenseEvents::CodeTipInitialize(LPDISPATCH Control, LPDISPATCH ToolTipCtrl)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH;
   InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Control, ToolTipCtrl);
}


/////////////////////////////////////////////////////////////////////////////
// ICodeList properties

/////////////////////////////////////////////////////////////////////////////
// ICodeList operations

long ICodeList::GetHWnd()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeList::SetHImageList(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeList::GetHImageList()
{
   long result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long ICodeList::GetSelectedItem()
{
   long result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeList::SetSelectedItem(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeList::AddItem(LPCTSTR strAdd, const VARIANT& ImageIndex, const VARIANT& ItemData)
{
   long result;
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT VTS_VARIANT;
   InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      strAdd, &ImageIndex, &ItemData);
   return result;
}

CString ICodeList::GetItemText(long ItemIndex)
{
   CString result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      ItemIndex);
   return result;
}

long ICodeList::GetItemData(long ItemIndex)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      ItemIndex);
   return result;
}

long ICodeList::FindString(LPCTSTR strFind, const VARIANT& bAcceptPartial)
{
   long result;
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT;
   InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      strFind, &bAcceptPartial);
   return result;
}

void ICodeList::Destroy()
{
   InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void ICodeList::DeleteItem(long ItemIndex)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       ItemIndex);
}

long ICodeList::GetItemCount()
{
   long result;
   InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeList::EnableHotTracking(const VARIANT& bEnable)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &bEnable);
}

long ICodeList::GetSortStyle()
{
   long result;
   InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeList::SetSortStyle(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeList::InsertItem(long lIndex, LPCTSTR strAdd, const VARIANT& ImageIndex, const VARIANT& ItemData)
{
   long result;
   static BYTE parms[] =
      VTS_I4 VTS_BSTR VTS_VARIANT VTS_VARIANT;
   InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lIndex, strAdd, &ImageIndex, &ItemData);
   return result;
}

unsigned long ICodeList::GetTextColor()
{
   unsigned long result;
   InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeList::SetTextColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

unsigned long ICodeList::GetBackColor()
{
   unsigned long result;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeList::SetBackColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeList::SetRefFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeList::SetFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH ICodeList::GetFont()
{
   LPDISPATCH result;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// ICodeTip properties

/////////////////////////////////////////////////////////////////////////////
// ICodeTip operations

long ICodeTip::GetHWnd()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString ICodeTip::GetTipText()
{
   CString result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeTip::SetTipText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long ICodeTip::GetTipTextLength()
{
   long result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTip::Destroy()
{
   InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

unsigned long ICodeTip::GetTextColor()
{
   unsigned long result;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTip::SetTextColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

unsigned long ICodeTip::GetBackColor()
{
   unsigned long result;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTip::SetBackColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTip::SetRefFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTip::SetFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH ICodeTip::GetFont()
{
   LPDISPATCH result;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// ILanguage properties

/////////////////////////////////////////////////////////////////////////////
// ILanguage operations

void ILanguage::SetCaseSensitive(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL ILanguage::GetCaseSensitive()
{
   BOOL result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void ILanguage::SetKeywords(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetKeywords()
{
   CString result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetOperators(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetOperators()
{
   CString result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetSingleLineComments(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetSingleLineComments()
{
   CString result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetMultiLineComments1(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetMultiLineComments1()
{
   CString result;
   InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetMultiLineComments2(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetMultiLineComments2()
{
   CString result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetScopeKeywords1(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetScopeKeywords1()
{
   CString result;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetScopeKeywords2(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetScopeKeywords2()
{
   CString result;
   InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetStringDelims(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetStringDelims()
{
   CString result;
   InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetEscapeChar(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetEscapeChar()
{
   CString result;
   InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetTerminatorChar(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetTerminatorChar()
{
   CString result;
   InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetTagElementNames(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetTagElementNames()
{
   CString result;
   InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetTagAttributeNames(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetTagAttributeNames()
{
   CString result;
   InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetTagEntities(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString ILanguage::GetTagEntities()
{
   CString result;
   InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ILanguage::SetStyle(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ILanguage::GetStyle()
{
   long result;
   InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// IHotKey properties

/////////////////////////////////////////////////////////////////////////////
// IHotKey operations

void IHotKey::SetModifiers1(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IHotKey::GetModifiers1()
{
   long result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IHotKey::SetVirtKey1(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString IHotKey::GetVirtKey1()
{
   CString result;
   InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void IHotKey::SetModifiers2(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long IHotKey::GetModifiers2()
{
   long result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void IHotKey::SetVirtKey2(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString IHotKey::GetVirtKey2()
{
   CString result;
   InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// IGlobals properties

/////////////////////////////////////////////////////////////////////////////
// IGlobals operations

void IGlobals::SetFindMRUList(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString IGlobals::GetFindMRUList()
{
   CString result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void IGlobals::SetReplaceMRUList(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString IGlobals::GetReplaceMRUList()
{
   CString result;
   InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void IGlobals::RegisterLanguage(LPCTSTR strName, LPDISPATCH LanguageDef)
{
   static BYTE parms[] =
      VTS_BSTR VTS_DISPATCH;
   InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strName, LanguageDef);
}

void IGlobals::UnregisterLanguage(LPCTSTR strName)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       strName);
}

void IGlobals::ResetDefaultHotKeys()
{
   InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH IGlobals::GetHotKeyForCmd(long lCmd, long lNum)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      lCmd, lNum);
   return result;
}

long IGlobals::GetNumHotKeysForCmd(long lCmd)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      lCmd);
   return result;
}

void IGlobals::RegisterHotKey(LPDISPATCH pHotKey, long lCmd)
{
   static BYTE parms[] =
      VTS_DISPATCH VTS_I4;
   InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       pHotKey, lCmd);
}

void IGlobals::UnregisterHotKey(LPDISPATCH pHotKey)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       pHotKey);
}

CString IGlobals::GetCommandString(long lCmd, BOOL bDescription)
{
   CString result;
   static BYTE parms[] =
      VTS_I4 VTS_BOOL;
   InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      lCmd, bDescription);
   return result;
}

void IGlobals::RegisterCommand(long lCmdId, LPCTSTR strName, LPCTSTR strDescription)
{
   static BYTE parms[] =
      VTS_I4 VTS_BSTR VTS_BSTR;
   InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lCmdId, strName, strDescription);
}

void IGlobals::UnregisterCommand(long lCmdId)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       lCmdId);
}

LPDISPATCH IGlobals::GetLanguageDef(LPCTSTR strName)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      strName);
   return result;
}

void IGlobals::UnregisterAllLanguages()
{
   InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ICodeTipHighlight properties

/////////////////////////////////////////////////////////////////////////////
// ICodeTipHighlight operations

long ICodeTipHighlight::GetHWnd()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString ICodeTipHighlight::GetTipText()
{
   CString result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeTipHighlight::SetTipText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long ICodeTipHighlight::GetTipTextLength()
{
   long result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipHighlight::Destroy()
{
   InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

unsigned long ICodeTipHighlight::GetTextColor()
{
   unsigned long result;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipHighlight::SetTextColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

unsigned long ICodeTipHighlight::GetBackColor()
{
   unsigned long result;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipHighlight::SetBackColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTipHighlight::SetRefFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTipHighlight::SetFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH ICodeTipHighlight::GetFont()
{
   LPDISPATCH result;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long ICodeTipHighlight::GetHighlightStartPos()
{
   long result;
   InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipHighlight::SetHighlightStartPos(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeTipHighlight::GetHighlightEndPos()
{
   long result;
   InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipHighlight::SetHighlightEndPos(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x33, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// ICodeTipFuncHighlight properties

/////////////////////////////////////////////////////////////////////////////
// ICodeTipFuncHighlight operations

long ICodeTipFuncHighlight::GetHWnd()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString ICodeTipFuncHighlight::GetTipText()
{
   CString result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeTipFuncHighlight::SetTipText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long ICodeTipFuncHighlight::GetTipTextLength()
{
   long result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipFuncHighlight::Destroy()
{
   InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

unsigned long ICodeTipFuncHighlight::GetTextColor()
{
   unsigned long result;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipFuncHighlight::SetTextColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

unsigned long ICodeTipFuncHighlight::GetBackColor()
{
   unsigned long result;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipFuncHighlight::SetBackColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTipFuncHighlight::SetRefFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTipFuncHighlight::SetFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH ICodeTipFuncHighlight::GetFont()
{
   LPDISPATCH result;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long ICodeTipFuncHighlight::GetArgument()
{
   long result;
   InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipFuncHighlight::SetArgument(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// ICodeTipMultiFunc properties

/////////////////////////////////////////////////////////////////////////////
// ICodeTipMultiFunc operations

long ICodeTipMultiFunc::GetHWnd()
{
   long result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString ICodeTipMultiFunc::GetTipText()
{
   CString result;
   InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::SetTipText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long ICodeTipMultiFunc::GetTipTextLength()
{
   long result;
   InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::Destroy()
{
   InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

unsigned long ICodeTipMultiFunc::GetTextColor()
{
   unsigned long result;
   InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::SetTextColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

unsigned long ICodeTipMultiFunc::GetBackColor()
{
   unsigned long result;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::SetBackColor(unsigned long newValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTipMultiFunc::SetRefFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

void ICodeTipMultiFunc::SetFont(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH ICodeTipMultiFunc::GetFont()
{
   LPDISPATCH result;
   InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long ICodeTipMultiFunc::GetArgument()
{
   long result;
   InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::SetArgument(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeTipMultiFunc::GetFunctionCount()
{
   long result;
   InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::SetFunctionCount(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x64, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long ICodeTipMultiFunc::GetCurrentFunction()
{
   long result;
   InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void ICodeTipMultiFunc::SetCurrentFunction(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x65, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}
