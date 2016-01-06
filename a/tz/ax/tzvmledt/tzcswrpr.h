/********************************************************************************************/
//
// File:            tzcswrpr.h
// *******************************************************************
// * Copyright © 2002 QuinSoft Corporation and TONBELLER AG.         *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           14. März 2002
// describtion:
//
// list of classes :
// Name:             describtion:
// ------------------------------------------------------------------------------------
//
//
//
/* Change log most recent first order:

   2002.3.14   TMV
      Machine generated IDispatch wrapper class(es) created with ClassWizard
      ICodeSense wrapper class

      maually added one time include and changed
         class ICodeSense : public COleDispatchDriver
      to class ZDCTL_CLASS ICodeSense : public COleDispatchDriver
         class ZDCTL_CLASS IRange : public COleDispatchDriver
      to class ZDCTL_CLASS IRange : public COleDispatchDriver
      so that those classes are completely exported and usable within dialogs


*/
/*********************************************************************************************/
#ifndef __TZCSWRPR_H__
#define __TZCSWRPR_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ZDCTL_CLASS
#define ZDCTL_CLASS AFX_EXT_CLASS
#endif

class ZDCTL_CLASS ICodeSense : public COleDispatchDriver
{
public:
   ICodeSense() {}      // Calls COleDispatchDriver default constructor
   ICodeSense(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeSense(const ICodeSense& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetRefFont(LPDISPATCH newValue);
   void SetFont(LPDISPATCH newValue);
   LPDISPATCH GetFont();
   void SetEnabled(BOOL bNewValue);
   BOOL GetEnabled();
   long GetWindow();
   void SetText(LPCTSTR lpszNewValue);
   CString GetText();
   void SetColorSyntax(BOOL bNewValue);
   BOOL GetColorSyntax();
   void SetDisplayWhitespace(BOOL bNewValue);
   BOOL GetDisplayWhitespace();
   void SetExpandTabs(BOOL bNewValue);
   BOOL GetExpandTabs();
   void SetSmoothScrolling(BOOL bNewValue);
   BOOL GetSmoothScrolling();
   void SetReadOnly(BOOL bNewValue);
   BOOL GetReadOnly();
   void SetLineToolTips(BOOL bNewValue);
   BOOL GetLineToolTips();
   void SetDisplayLeftMargin(BOOL bNewValue);
   BOOL GetDisplayLeftMargin();
   void SetEnableColumnSel(BOOL bNewValue);
   BOOL GetEnableColumnSel();
   void SetEnableDragDrop(BOOL bNewValue);
   BOOL GetEnableDragDrop();
   void SetOvertype(BOOL bNewValue);
   BOOL GetOvertype();
   void SetSearchCaseSensitive(BOOL bNewValue);
   BOOL GetSearchCaseSensitive();
   void SetReplacePreserveCase(BOOL bNewValue);
   BOOL GetReplacePreserveCase();
   void SetSearchWholeWord(BOOL bNewValue);
   BOOL GetSearchWholeWord();
   void SetModified(BOOL bNewValue);
   BOOL GetModified();
   void SetEnableCRLF(BOOL bNewValue);
   BOOL GetEnableCRLF();
   void SetGlobalProps(BOOL bNewValue);
   BOOL GetGlobalProps();
   void SetVScrollVisible(BOOL bNewValue);
   BOOL GetVScrollVisible();
   void SetHScrollVisible(BOOL bNewValue);
   BOOL GetHScrollVisible();
   void SetEnableVSplitter(BOOL bNewValue);
   BOOL GetEnableVSplitter();
   void SetEnableHSplitter(BOOL bNewValue);
   BOOL GetEnableHSplitter();
   void SetLanguage(LPCTSTR lpszNewValue);
   CString GetLanguage();
   void SetTabSize(long nNewValue);
   long GetTabSize();
   void SetTopIndex(long nNewValue);
   long GetTopIndex();
   void SetUndoLimit(long nNewValue);
   long GetUndoLimit();
   long GetCurrentView();
   long GetTextLength();
   long GetTextLengthLogical();
   long GetLineCount();
   CString GetCurrentWord();
   long GetCurrentWordLength();
   long GetCurrentWordLengthLogical();
   void SetAutoIndentMode(long nNewValue);
   long GetAutoIndentMode();
   long GetViewCount();
   void SetVSplitterPos(long nNewValue);
   long GetVSplitterPos();
   void SetHSplitterPos(long nNewValue);
   long GetHSplitterPos();
   void SetFindText(LPCTSTR lpszNewValue);
   CString GetFindText();
   void SetReplText(LPCTSTR lpszNewValue);
   CString GetReplText();
   void SetLineNumbering(BOOL bNewValue);
   BOOL GetLineNumbering();
   void SetLineNumberStart(long nNewValue);
   long GetLineNumberStart();
   void SetLineNumberStyle(long nNewValue);
   long GetLineNumberStyle();
   CString GetWord(LPDISPATCH Pos);
   long GetWordLength(LPDISPATCH Pos, BOOL bLogical);
   long GetTopIndex(long lView);
   void SetTopIndex(long lView, long lLine);
   LPDISPATCH GetSel(BOOL bNormalized);
   void SetSel(LPDISPATCH Sel, BOOL bMakeVisible);
   LPDISPATCH GetSelFromPoint(long lxClientPos, long lyClientPos);
   long GetVisibleLineCount(long lView, BOOL bFullyVisible);
   CString GetLine(long lLine);
   long GetLineLength(long lLine);
   void SetColor(long lColorItem, unsigned long crColor);
   unsigned long GetColor(long lColorItem);
   void OpenFile(LPCTSTR strFileName);
   void SaveFile(LPCTSTR strFileName, BOOL bClearUndo);
   void InsertFile(LPCTSTR strFileName, LPDISPATCH InsertPos);
   void InsertText(LPCTSTR strText, LPDISPATCH InsertPos);
   void ReplaceText(LPCTSTR strText, LPDISPATCH Range);
   long HitTest(long lxClientPos, long lyClientPos, long* plView);
   void AddText(LPCTSTR strText);
   void DeleteLine(long lLine);
   void InsertLine(long lLine, LPCTSTR strLine);
   void SelectLine(long lLine, BOOL bMakeVisible);
   void DeleteSel();
   void ReplaceSel(LPCTSTR strText);
   void ExecuteCmd(long lCommand, const VARIANT& CmdData);
   BOOL CanUndo();
   BOOL CanRedo();
   BOOL CanCut();
   BOOL CanCopy();
   BOOL CanPaste();
   void Undo();
   void Redo();
   void Cut();
   void Copy();
   void Paste();
   void ClearUndoBuffer();
   BOOL IsRecordingMacro();
   BOOL IsPlayingMacro();
   void SetDlgParent(long lhWnd);
   CString GetSelText();
   void SetSelText(LPCTSTR lpszNewValue);
   void SetSelBounds(BOOL bNewValue);
   BOOL GetSelBounds();
   void SetFontStyle(long lFontStyleItem, long byFontStyle);
   long GetFontStyle(long lFontStyleItem);
   void SetSearchRegExp(BOOL bNewValue);
   BOOL GetSearchRegExp();
   void SetItemData(long lLine, long lData);
   long GetItemData(long lLine);
   void SetLineStyle(long lLine, long lStyle);
   long GetLineStyle(long lLine);
   void SetBookmark(long lLine, BOOL bVal);
   BOOL GetBookmark(long lLine);
   // method 'SetAllBookmarks' not emitted because of invalid return type or parameter type
   // method 'GetAllBookmarks' not emitted because of invalid return type or parameter type
   LPDISPATCH PosFromChar(LPDISPATCH pPos);
   void SetHideSel(BOOL bNewValue);
   BOOL GetHideSel();
   void SetHighlightedLine(long nNewValue);
   long GetHighlightedLine();
   void SetNormalizeCase(BOOL bNewValue);
   BOOL GetNormalizeCase();
   void SetDivider(long lLine, BOOL bVal);
   BOOL GetDivider(long lLine);
   void SetOvertypeCaret(BOOL bNewValue);
   BOOL GetOvertypeCaret();
   // method 'SetMarginImages' not emitted because of invalid return type or parameter type
   // method 'GetMarginImages' not emitted because of invalid return type or parameter type
   // method 'SetHImageList' not emitted because of invalid return type or parameter type
   // method 'GetHImageList' not emitted because of invalid return type or parameter type
   LPDISPATCH GetImageList();
   void SetImageList(LPDISPATCH newValue);
   void SetRefImageList(LPDISPATCH newValue);
   void PrintContents(long hDC, long lFlags);
   void SetCaretPos(long lLine, long lCol);
   long ViewColToBufferCol(long lLine, long lViewCol);
   long BufferColToViewCol(long lLine, long lBuffCol);
   long GetSelLength();
   long GetSelLengthLogical();
   void SetBorderStyle(long nNewValue);
   long GetBorderStyle();
   long GetHWnd();
   void SetCurrentView(long nNewValue);
   void SetStealKeys(BOOL bNewValue);
   BOOL GetStealKeys();
   long GetCurrentToken();
   void UpdateControlPositions();
};
/////////////////////////////////////////////////////////////////////////////
// IPosition wrapper class

class ZDCTL_CLASS IPosition : public COleDispatchDriver
{
public:
   IPosition() {}      // Calls COleDispatchDriver default constructor
   IPosition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   IPosition(const IPosition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetLineNo(long nNewValue);
   long GetLineNo();
   void SetColNo(long nNewValue);
   long GetColNo();
};
/////////////////////////////////////////////////////////////////////////////
// IRange wrapper class

class ZDCTL_CLASS IRange : public COleDispatchDriver
{
public:
   IRange() {}      // Calls COleDispatchDriver default constructor
   IRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   IRange(const IRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetStartLineNo(long nNewValue);
   long GetStartLineNo();
   void SetStartColNo(long nNewValue);
   long GetStartColNo();
   void SetEndLineNo(long nNewValue);
   long GetEndLineNo();
   void SetEndColNo(long nNewValue);
   long GetEndColNo();
   void SetColumnSel(BOOL bNewValue);
   BOOL GetColumnSel();
   BOOL IsEmpty();
   void Empty();
   void Normalize();
};
/////////////////////////////////////////////////////////////////////////////
// IRect wrapper class

class IRect : public COleDispatchDriver
{
public:
   IRect() {}      // Calls COleDispatchDriver default constructor
   IRect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   IRect(const IRect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetLeft(long nNewValue);
   long GetLeft();
   void SetTop(long nNewValue);
   long GetTop();
   void SetRight(long nNewValue);
   long GetRight();
   void SetBottom(long nNewValue);
   long GetBottom();
};
/////////////////////////////////////////////////////////////////////////////
// ICodeSenseEvents wrapper class

class ICodeSenseEvents : public COleDispatchDriver
{
public:
   ICodeSenseEvents() {}      // Calls COleDispatchDriver default constructor
   ICodeSenseEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeSenseEvents(const ICodeSenseEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void Change(LPDISPATCH Control);
   void HScroll(LPDISPATCH Control);
   void VScroll(LPDISPATCH Control);
   void SelChange(LPDISPATCH Control);
   void ViewChange(LPDISPATCH Control);
   void ModifiedChange(LPDISPATCH Control);
   void ShowProps(LPDISPATCH Control, BOOL* pbStop);
   void PropsChange(LPDISPATCH Control);
   void Create(LPDISPATCH Control);
   void Destroy(LPDISPATCH Control);
   void SetFocus(LPDISPATCH Control);
   void KillFocus(LPDISPATCH Control);
   void Click(LPDISPATCH Control, BOOL* pbStop);
   void DblClick(LPDISPATCH Control, BOOL* pbStop);
   void RDblClick(LPDISPATCH Control);
   void Return(LPDISPATCH Control, BOOL* pbStop);
   void RClick(LPDISPATCH Control, BOOL* pbStop);
   void DeleteLine(LPDISPATCH Control, long lLine, long lItemData);
   void CmdFailure(LPDISPATCH Control, long lCmd, long lErrCode, BOOL* pbStop);
   void RegisteredCmd(LPDISPATCH Control, long lCmd);
   void KeyDown(LPDISPATCH Control, long KeyCode, long Shift, BOOL* pbStop);
   void KeyUp(LPDISPATCH Control, long KeyCode, long Shift, BOOL* pbStop);
   void KeyPress(LPDISPATCH Control, long KeyAscii, long Shift, BOOL* pbStop);
   void MouseDown(LPDISPATCH Control, long Button, long Shift, long X, long Y, BOOL* pbStop);
   void MouseMove(LPDISPATCH Control, long Button, long Shift, long X, long Y, BOOL* pbStop);
   void MouseUp(LPDISPATCH Control, long Button, long Shift, long X, long Y, BOOL* pbStop);
   void OvertypeChange(LPDISPATCH Control);
   void FindWrapped(LPDISPATCH Control, long lCmd, BOOL bForward, BOOL* pbStop);
   void CodeList(LPDISPATCH Control, LPDISPATCH ListCtrl, BOOL* bContinue);
   void CodeListSelMade(LPDISPATCH Control, LPDISPATCH ListCtrl, BOOL* bStop);
   void CodeListCancel(LPDISPATCH Control, LPDISPATCH ListCtrl, BOOL* bKeepVisible);
   void CodeListChar(LPDISPATCH Control, LPDISPATCH ListCtrl, long wChar, long lKeyData, BOOL* bHandled);
   void CodeTip(LPDISPATCH Control, long* ToolTipType);
   void CodeTipCancel(LPDISPATCH Control, LPDISPATCH ToolTipCtrl, BOOL* bKeepVisible);
   void CodeTipUpdate(LPDISPATCH Control, LPDISPATCH ToolTipCtrl);
   void CodeListSelWord(LPDISPATCH Control, LPDISPATCH ListCtrl, long lItem, BOOL* bAllowSelection);
   void CodeListSelChange(LPDISPATCH Control, LPDISPATCH ListCtrl, long lItem, BSTR* strTipText);
   void CodeListHotTrack(LPDISPATCH Control, LPDISPATCH ListCtrl, long lItem);
   void CodeTipInitialize(LPDISPATCH Control, LPDISPATCH ToolTipCtrl);
};
/////////////////////////////////////////////////////////////////////////////
// ICodeList wrapper class

class ICodeList : public COleDispatchDriver
{
public:
   ICodeList() {}      // Calls COleDispatchDriver default constructor
   ICodeList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeList(const ICodeList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   long GetHWnd();
   void SetHImageList(long nNewValue);
   long GetHImageList();
   long GetSelectedItem();
   void SetSelectedItem(long nNewValue);
   long AddItem(LPCTSTR strAdd, const VARIANT& ImageIndex, const VARIANT& ItemData);
   CString GetItemText(long ItemIndex);
   long GetItemData(long ItemIndex);
   long FindString(LPCTSTR strFind, const VARIANT& bAcceptPartial);
   void Destroy();
   void DeleteItem(long ItemIndex);
   long GetItemCount();
   void EnableHotTracking(const VARIANT& bEnable);
   long GetSortStyle();
   void SetSortStyle(long nNewValue);
   long InsertItem(long lIndex, LPCTSTR strAdd, const VARIANT& ImageIndex, const VARIANT& ItemData);
   unsigned long GetTextColor();
   void SetTextColor(unsigned long newValue);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long newValue);
   void SetRefFont(LPDISPATCH newValue);
   void SetFont(LPDISPATCH newValue);
   LPDISPATCH GetFont();
};
/////////////////////////////////////////////////////////////////////////////
// ICodeTip wrapper class

class ICodeTip : public COleDispatchDriver
{
public:
   ICodeTip() {}      // Calls COleDispatchDriver default constructor
   ICodeTip(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeTip(const ICodeTip& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   long GetHWnd();
   CString GetTipText();
   void SetTipText(LPCTSTR lpszNewValue);
   long GetTipTextLength();
   void Destroy();
   unsigned long GetTextColor();
   void SetTextColor(unsigned long newValue);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long newValue);
   void SetRefFont(LPDISPATCH newValue);
   void SetFont(LPDISPATCH newValue);
   LPDISPATCH GetFont();
};
/////////////////////////////////////////////////////////////////////////////
// ILanguage wrapper class

class ILanguage : public COleDispatchDriver
{
public:
   ILanguage() {}      // Calls COleDispatchDriver default constructor
   ILanguage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ILanguage(const ILanguage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetCaseSensitive(BOOL bNewValue);
   BOOL GetCaseSensitive();
   void SetKeywords(LPCTSTR lpszNewValue);
   CString GetKeywords();
   void SetOperators(LPCTSTR lpszNewValue);
   CString GetOperators();
   void SetSingleLineComments(LPCTSTR lpszNewValue);
   CString GetSingleLineComments();
   void SetMultiLineComments1(LPCTSTR lpszNewValue);
   CString GetMultiLineComments1();
   void SetMultiLineComments2(LPCTSTR lpszNewValue);
   CString GetMultiLineComments2();
   void SetScopeKeywords1(LPCTSTR lpszNewValue);
   CString GetScopeKeywords1();
   void SetScopeKeywords2(LPCTSTR lpszNewValue);
   CString GetScopeKeywords2();
   void SetStringDelims(LPCTSTR lpszNewValue);
   CString GetStringDelims();
   void SetEscapeChar(LPCTSTR lpszNewValue);
   CString GetEscapeChar();
   void SetTerminatorChar(LPCTSTR lpszNewValue);
   CString GetTerminatorChar();
   void SetTagElementNames(LPCTSTR lpszNewValue);
   CString GetTagElementNames();
   void SetTagAttributeNames(LPCTSTR lpszNewValue);
   CString GetTagAttributeNames();
   void SetTagEntities(LPCTSTR lpszNewValue);
   CString GetTagEntities();
   void SetStyle(long nNewValue);
   long GetStyle();
};
/////////////////////////////////////////////////////////////////////////////
// IHotKey wrapper class

class IHotKey : public COleDispatchDriver
{
public:
   IHotKey() {}      // Calls COleDispatchDriver default constructor
   IHotKey(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   IHotKey(const IHotKey& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetModifiers1(long nNewValue);
   long GetModifiers1();
   void SetVirtKey1(LPCTSTR lpszNewValue);
   CString GetVirtKey1();
   void SetModifiers2(long nNewValue);
   long GetModifiers2();
   void SetVirtKey2(LPCTSTR lpszNewValue);
   CString GetVirtKey2();
};
/////////////////////////////////////////////////////////////////////////////
// IGlobals wrapper class

class IGlobals : public COleDispatchDriver
{
public:
   IGlobals() {}      // Calls COleDispatchDriver default constructor
   IGlobals(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   IGlobals(const IGlobals& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   void SetFindMRUList(LPCTSTR lpszNewValue);
   CString GetFindMRUList();
   void SetReplaceMRUList(LPCTSTR lpszNewValue);
   CString GetReplaceMRUList();
   void RegisterLanguage(LPCTSTR strName, LPDISPATCH LanguageDef);
   void UnregisterLanguage(LPCTSTR strName);
   // method 'GetHotKeys' not emitted because of invalid return type or parameter type
   // method 'SetHotKeys' not emitted because of invalid return type or parameter type
   void ResetDefaultHotKeys();
   LPDISPATCH GetHotKeyForCmd(long lCmd, long lNum);
   long GetNumHotKeysForCmd(long lCmd);
   void RegisterHotKey(LPDISPATCH pHotKey, long lCmd);
   void UnregisterHotKey(LPDISPATCH pHotKey);
   CString GetCommandString(long lCmd, BOOL bDescription);
   // method 'GetMacro' not emitted because of invalid return type or parameter type
   // method 'SetMacro' not emitted because of invalid return type or parameter type
   void RegisterCommand(long lCmdId, LPCTSTR strName, LPCTSTR strDescription);
   void UnregisterCommand(long lCmdId);
   LPDISPATCH GetLanguageDef(LPCTSTR strName);
   void UnregisterAllLanguages();
};
/////////////////////////////////////////////////////////////////////////////
// ICodeTipHighlight wrapper class

class ICodeTipHighlight : public COleDispatchDriver
{
public:
   ICodeTipHighlight() {}      // Calls COleDispatchDriver default constructor
   ICodeTipHighlight(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeTipHighlight(const ICodeTipHighlight& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   long GetHWnd();
   CString GetTipText();
   void SetTipText(LPCTSTR lpszNewValue);
   long GetTipTextLength();
   void Destroy();
   unsigned long GetTextColor();
   void SetTextColor(unsigned long newValue);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long newValue);
   void SetRefFont(LPDISPATCH newValue);
   void SetFont(LPDISPATCH newValue);
   LPDISPATCH GetFont();
   long GetHighlightStartPos();
   void SetHighlightStartPos(long nNewValue);
   long GetHighlightEndPos();
   void SetHighlightEndPos(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ICodeTipFuncHighlight wrapper class

class ICodeTipFuncHighlight : public COleDispatchDriver
{
public:
   ICodeTipFuncHighlight() {}      // Calls COleDispatchDriver default constructor
   ICodeTipFuncHighlight(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeTipFuncHighlight(const ICodeTipFuncHighlight& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   long GetHWnd();
   CString GetTipText();
   void SetTipText(LPCTSTR lpszNewValue);
   long GetTipTextLength();
   void Destroy();
   unsigned long GetTextColor();
   void SetTextColor(unsigned long newValue);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long newValue);
   void SetRefFont(LPDISPATCH newValue);
   void SetFont(LPDISPATCH newValue);
   LPDISPATCH GetFont();
   long GetArgument();
   void SetArgument(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ICodeTipMultiFunc wrapper class

class ICodeTipMultiFunc : public COleDispatchDriver
{
public:
   ICodeTipMultiFunc() {}      // Calls COleDispatchDriver default constructor
   ICodeTipMultiFunc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICodeTipMultiFunc(const ICodeTipMultiFunc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   long GetHWnd();
   CString GetTipText();
   void SetTipText(LPCTSTR lpszNewValue);
   long GetTipTextLength();
   void Destroy();
   unsigned long GetTextColor();
   void SetTextColor(unsigned long newValue);
   unsigned long GetBackColor();
   void SetBackColor(unsigned long newValue);
   void SetRefFont(LPDISPATCH newValue);
   void SetFont(LPDISPATCH newValue);
   LPDISPATCH GetFont();
   long GetArgument();
   void SetArgument(long nNewValue);
   long GetFunctionCount();
   void SetFunctionCount(long nNewValue);
   long GetCurrentFunction();
   void SetCurrentFunction(long nNewValue);
};

#endif //__TZCSWRPR_H__
