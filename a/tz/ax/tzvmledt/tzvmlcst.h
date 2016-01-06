/********************************************************************************************/
//
// File:            tzvmlcst.h
// *******************************************************************
// * Copyright © 2002 QuinSoft Corporation and TONBELLER AG.         *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           13. März 2002
// describtion:
//
// describtion:
// ------------------------------------------------------------------------------------
// constans to use with current codesense control version ( 2.19 )
//
//
/* Change log most recent first order:




*/
/*********************************************************************************************/
#ifndef __TZVMLCST_H__
#define __TZVMLCST_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stock languages
#define CMLANG_CPP         _T("C/C++")
#define CMLANG_PASCAL      _T("Pascal")
#define CMLANG_BASIC       _T("Basic")
#define CMLANG_SQL         _T("SQL")
#define CMLANG_JAVA        _T("Java")
#define CMLANG_HTML        _T("HTML")
#define CMLANG_XML         _T("XML")
// this is ours
#define CMLANG_VML         _T("VML")



#define DISPID_Change            0x00000000   // ("The buffer changed")
#define DISPID_HScroll           0x00000001   // ("A view was scrolled horizontally")
#define DISPID_VScroll           0x00000002   // ("A view was scrolled vertically")
#define DISPID_SelChange         0x00000003   // ("The caret position changed")
#define DISPID_ViewChange        0x00000004   // ("The active view changed")
#define DISPID_ModifiedChange    0x00000005   // ("The modified flag changed state")
#define DISPID_ShowProps         0x00000006   // ("The user requested to show the properties dialog box")
#define DISPID_PropsChange       0x00000007   // ("The user pressed the Apply or OK button in the Window Properties dialog box")
#define DISPID_Create            0x00000008   // ("A CodeMax window was created")
#define DISPID_Destroy           0x00000009   // ("A CodeMax window was destroyed")
#define DISPID_SetFocus          0x0000000a   // ("A CodeMax window received the input focus")
#define DISPID_KillFocus         0x0000000b   // ("A CodeMax window lost the input focus")
#define DISPID_Click             0x0000000c   // ("The left mouse button was pressed down")
#define DISPID_DblClick          0x0000000d   // ("The left mouse button was double clicked")
#define DISPID_RDblClick         0x0000000e   // ("The right mouse button was double clicked")
#define DISPID_Return            0x0000000f   // ("The ENTER key was pressed")
#define DISPID_RClick            0x00000010   // ("The user clicked the right mouse button to show the context menu")
#define DISPID_DeleteLine        0x00000011   // ("The user deleted a line")
#define DISPID_CmdFailure        0x00000012   // ("A keystroke command failed")
#define DISPID_RegisteredCmd     0x00000013   // ("A registered command has been invoked")
#define DISPID_KeyDown           0x00000014   // ("A key was pressed")
#define DISPID_KeyUp             0x00000015   // ("A key was released")
#define DISPID_KeyPress          0x00000016   // ("A key was pressed and released")
#define DISPID_MouseDown         0x00000017   // ("A mouse button was pressed")
#define DISPID_MouseMove         0x00000019   // ("The mouse pointer moved")
#define DISPID_MouseUp           0x00000018   // ("A mouse button was released")
#define DISPID_OvertypeChange    0x0000001a   // ("The overtype mode has changed")
#define DISPID_FindWrapped       0x0000001b   // ("A search operation wrapped to the start or end of the buffer")
#define DISPID_CodeList          0x0000001c   // ("The user activated the CodeList control")
#define DISPID_CodeListSelMade   0x0000001d   // ("The user selected an item in CodeList control")
#define DISPID_CodeListCancel    0x0000001e   // ("The user cancelled the CodeList control")
#define DISPID_CodeListChar      0x0000001f   // ("The user pressed a key in the CodeList control")
#define DISPID_CodeTip           0x00000020   // ("The user activated the CodeTip control")
#define DISPID_CodeTipCancel     0x00000021   // ("The user cancelled the CodeTip control.")
#define DISPID_CodeTipUpdate     0x00000022   // ("The CodeTip control is ready to be updated.")
#define DISPID_CodeListSelWord   0x00000023   // ("The CodeList control is about to select a new item.")
#define DISPID_CodeListSelChange 0x00000024   // ("The selection in the CodeList control has changed.")
#define DISPID_CodeListHotTrack  0x00000025   // ("The mouse hot-tracking selection has changed.")
#define DISPID_CodeTipInitialize 0x00000026   // ("The CodeTip control is ready to be initialized.")

enum tag_cmLimit{
  cmMaxFindReplText = 100,
  cmFindReplaceMRUMax = 10,
  cmFindReplaceMRUBuffSize = 1010,
  cmMaxMacros = 10,
  cmMaxCmdString = 50,
  cmMaxCmdDescription = 100,
  cmMaxLanguageName = 30,
  cmMaxTabSize = 100,
  cmMinTabSize = 2
} cmLimit;

enum tag_cmCommand{
  cmCmdWordUppercase = 100,
  cmCmdWordTranspose = 101,
  cmCmdWordRightExtend = 102,
  cmCmdWordRight = 103,
  cmCmdWordEndRight = 104,
  cmCmdWordEndRightExtend = 105,
  cmCmdWordLowerCase = 106,
  cmCmdWordLeftExtend = 107,
  cmCmdWordLeft = 108,
  cmCmdWordEndLeft = 109,
  cmCmdWordEndLeftExtend = 110,
  cmCmdWordDeleteToStart = 111,
  cmCmdWordDeleteToEnd = 112,
  cmCmdWordCapitalize = 113,
  cmCmdWindowStart = 114,
  cmCmdWindowScrollUp = 115,
  cmCmdWindowScrollToTop = 116,
  cmCmdWindowScrollToCenter = 117,
  cmCmdWindowScrollToBottom = 118,
  cmCmdWindowScrollRight = 119,
  cmCmdWindowScrollLeft = 120,
  cmCmdWindowScrollDown = 121,
  cmCmdWindowRightEdge = 122,
  cmCmdWindowLeftEdge = 123,
  cmCmdWindowEnd = 124,
  cmCmdUppercaseSelection = 125,
  cmCmdUntabifySelection = 126,
  cmCmdUnindentSelection = 127,
  cmCmdUndoChanges = 128,
  cmCmdUndo = 129,
  cmCmdTabifySelection = 130,
  cmCmdSentenceRight = 131,
  cmCmdSentenceLeft = 132,
  cmCmdSentenceCut = 133,
  cmCmdSelectSwapAnchor = 134,
  cmCmdSelectPara = 135,
  cmCmdSelectLine = 136,
  cmCmdSelectAll = 137,
  cmCmdRedoChanges = 138,
  cmCmdRedo = 139,
  cmCmdPaste = 140,
  cmCmdParaUp = 141,
  cmCmdParaDown = 142,
  cmCmdPageUpExtend = 143,
  cmCmdPageUp = 144,
  cmCmdPageDownExtend = 145,
  cmCmdPageDown = 146,
  cmCmdLowercaseSelection = 147,
  cmCmdLineUpExtend = 148,
  cmCmdLineUp = 149,
  cmCmdLineTranspose = 150,
  cmCmdLineStart = 151,
  cmCmdLineOpenBelow = 152,
  cmCmdLineOpenAbove = 153,
  cmCmdLineEndExtend = 154,
  cmCmdLineEnd = 155,
  cmCmdLineDownExtend = 156,
  cmCmdLineDown = 157,
  cmCmdLineDeleteToStart = 158,
  cmCmdLineDeleteToEnd = 159,
  cmCmdLineDelete = 160,
  cmCmdLineCut = 161,
  cmCmdIndentToPrev = 162,
  cmCmdIndentSelection = 163,
  cmCmdHomeExtend = 164,
  cmCmdHome = 165,
  cmCmdGotoMatchBrace = 166,
  cmCmdGotoIndentation = 167,
  cmCmdGotoLine = 168,
  cmCmdFindReplace = 169,
  cmCmdReplace = 170,
  cmCmdReplaceAllInBuffer = 171,
  cmCmdReplaceAllInSelection = 172,
  cmCmdFindPrevWord = 173,
  cmCmdFindPrev = 174,
  cmCmdFindNextWord = 175,
  cmCmdFindNext = 176,
  cmCmdFindMarkAll = 177,
  cmCmdFind = 178,
  cmCmdSetFindText = 179,
  cmCmdSetReplaceText = 180,
  cmCmdTogglePreserveCase = 181,
  cmCmdToggleWholeWord = 182,
  cmCmdToggleCaseSensitive = 183,
  cmCmdEnd = 184,
  cmCmdToggleWhitespaceDisplay = 185,
  cmCmdToggleOvertype = 186,
  cmCmdSetRepeatCount = 187,
  cmCmdDocumentStartExtend = 188,
  cmCmdDocumentStart = 189,
  cmCmdDocumentEndExtend = 190,
  cmCmdDocumentEnd = 191,
  cmCmdDeleteHorizontalSpace = 192,
  cmCmdDeleteBlankLines = 193,
  cmCmdDeleteBack = 194,
  cmCmdDelete = 195,
  cmCmdCutSelection = 196,
  cmCmdCut = 197,
  cmCmdCopy = 198,
  cmCmdCharTranspose = 199,
  cmCmdCharRightExtend = 200,
  cmCmdCharRight = 201,
  cmCmdCharLeftExtend = 202,
  cmCmdCharLeft = 203,
  cmCmdBookmarkToggle = 204,
  cmCmdBookmarkPrev = 205,
  cmCmdBookmarkNext = 206,
  cmCmdBookmarkClearAll = 207,
  cmCmdBookmarkJumpToFirst = 208,
  cmCmdBookmarkJumpToLast = 209,
  cmCmdAppendNextCut = 210,
  cmCmdInsertChar = 211,
  cmCmdNewLine = 212,
  cmCmdRecordMacro = 213,
  cmCmdPlayMacro1 = 214,
  cmCmdPlayMacro2 = 215,
  cmCmdPlayMacro3 = 216,
  cmCmdPlayMacro4 = 217,
  cmCmdPlayMacro5 = 218,
  cmCmdPlayMacro6 = 219,
  cmCmdPlayMacro7 = 220,
  cmCmdPlayMacro8 = 221,
  cmCmdPlayMacro9 = 222,
  cmCmdPlayMacro10 = 223,
  cmCmdProperties = 224,
  cmCmdBeginUndo = 225,
  cmCmdEndUndo = 226,
  cmCmdToggleRegExp = 228,
  cmCmdClearSelection = 229,
  cmCmdRegExpOn = 230,
  cmCmdRegExpOff = 231,
  cmCmdWholeWordOn = 232,
  cmCmdWholeWordOff = 233,
  cmCmdPreserveCaseOn = 234,
  cmCmdPreserveCaseOff = 235,
  cmCmdCaseSensitiveOn = 236,
  cmCmdCaseSensitiveOff = 237,
  cmCmdWhitespaceDisplayOn = 238,
  cmCmdWhitespaceDisplayOff = 239,
  cmCmdOvertypeOn = 240,
  cmCmdOvertypeOff = 241,
  cmCmdCodeList = 242,
  cmCmdCodeTip = 243
} cmCommand;

enum tag_cmCommandErr{
  cmErrFailure = 1,
  cmErrInput = 2,
  cmErrSelection = 3,
  cmErrNotFound = 4,
  cmErrEmptyBuf = 5,
  cmErrReadOnly = 6
} cmCommandErr;

enum tag_cmColorItem{
  cmClrWindow = 0,
  cmClrLeftMargin = 1,
  cmClrBookmark = 2,
  cmClrBookmarkBk = 3,
  cmClrText = 4,
  cmClrTextBk = 5,
  cmClrNumber = 6,
  cmClrNumberBk = 7,
  cmClrKeyword = 8,
  cmClrKeywordBk = 9,
  cmClrOperator = 10,
  cmClrOperatorBk = 11,
  cmClrScopeKeyword = 12,
  cmClrScopeKeywordBk = 13,
  cmClrComment = 14,
  cmClrCommentBk = 15,
  cmClrString = 16,
  cmClrStringBk = 17,
  cmClrTagText = 18,
  cmClrTagTextBk = 19,
  cmClrTagEntity = 20,
  cmClrTagEntityBk = 21,
  cmClrTagElementName = 22,
  cmClrTagElementNameBk = 23,
  cmClrTagAttributeName = 24,
  cmClrTagAttributeNameBk = 25,
  cmClrLineNumber = 26,
  cmClrLineNumberBk = 27,
  cmClrHDividerLines = 28,
  cmClrVDividerLines = 29,
  cmClrHighlightedLine = 30
} cmColorItem;

enum tag_cmFontStyleItem{
  cmStyText = 0,
  cmStyNumber = 1,
  cmStyKeyword = 2,
  cmStyOperator = 3,
  cmStyScopeKeyword = 4,
  cmStyComment = 5,
  cmStyString = 6,
  cmStyTagText = 7,
  cmStyTagEntity = 8,
  cmStyTagElementName = 9,
  cmStyTagAttributeName = 10,
  cmStyLineNumber = 11
} cmFontStyleItem;

enum tag_cmFontStyle{
  cmFontNormal = 0,
  cmFontBold = 1,
  cmFontItalic = 2,
  cmFontBoldItalic = 3,
  cmFontUnderline = 4
} cmFontStyle;

enum tag_cmAutoIndentMode{
  cmIndentOff = 0,
  cmIndentScope = 1,
  cmIndentPrevLine = 2
} cmAutoIndentMode;

enum tag_cmHitTestCode{
  cmNowhere = 0,
  cmHSplitter = 1,
  cmVSplitter = 2,
  cmHVSplitter = 3,
  cmEditSpace = 4,
  cmHScrollBar = 5,
  cmVScrollBar = 6,
  cmSizeBox = 7,
  cmLeftMargin = 8
} cmHitTestCode;

enum tag_cmLineNumStyle{
  cmDecimal = 10,
  cmHexadecimal = 16,
  cmBinary = 2,
  cmOctal = 8
} cmLineNumStyle;

enum tag_cmPrintFlags{
  cmPrnPromptDlg = 0,
  cmPrnDefaultPrn = 1,
  cmPrnHDC = 2,
  cmPrnRichFonts = 4,
  cmPrnColor = 8,
  cmPrnPageNums = 16,
  cmPrnDateTime = 32,
  cmPrnBorderThin = 64,
  cmPrnBorderThick = 128,
  cmPrnBorderDouble = 256,
  cmPrnSelection = 512
} cmPrintFlags;

enum tag_cmLangStyle{
  cmLangStyleProcedural = 0,
  cmLangStyleSGML = 1
} cmLangStyle;

enum tag_cmBorderStyle{
  cmBorderNone = 0,
  cmBorderThin = 1,
  cmBorderClient = 2,
  cmBorderStatic = 4,
  cmBorderModal = 8,
  cmBorderCorral = 10
} cmBorderStyle;

enum tag_cmTokenType{
  cmTokenTypeKeyword = 1,
  cmTokenTypeOperator = 2,
  cmTokenTypeString = 3,
  cmTokenTypeSingleLineComment = 4,
  cmTokenTypeMultiLineComment = 5,
  cmTokenTypeNumber = 6,
  cmTokenTypeScopeBegin = 7,
  cmTokenTypeScopeEnd = 8,
  cmTokenTypeText = 255,
  cmTokenTypeUnknown = -1
} cmTokenType;

enum tag_cmToolTipType{
  cmToolTipTypeNone = 0,
  cmToolTipTypeNormal = 1,
  cmToolTipTypeHighlight = 2,
  cmToolTipTypeFuncHighlight = 3,
  cmToolTipTypeMultiFunc = 4
} cmToolTipType;

enum tag_cmCodeListSortStyle {
  cmCodeListSortNone = 0,
  cmCodeListSortAscending = 1,
  cmCodeListSortDescending = 2
} cmCodeListSortStyle;


#endif //__TZVMLCST_H__
