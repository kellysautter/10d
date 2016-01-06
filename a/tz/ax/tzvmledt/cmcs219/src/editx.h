/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Mar 12 17:20:37 2002
 */
/* Compiler settings for editx.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __editx_h__
#define __editx_h__

#ifdef __cplusplus
extern "C"{
#endif

/* Forward Declarations */

#ifndef __CodeSense_FWD_DEFINED__
#define __CodeSense_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeSense CodeSense;
#else
typedef struct CodeSense CodeSense;
#endif /* __cplusplus */

#endif    /* __CodeSense_FWD_DEFINED__ */


#ifndef __Range_FWD_DEFINED__
#define __Range_FWD_DEFINED__

#ifdef __cplusplus
typedef class Range Range;
#else
typedef struct Range Range;
#endif /* __cplusplus */

#endif    /* __Range_FWD_DEFINED__ */


#ifndef __Position_FWD_DEFINED__
#define __Position_FWD_DEFINED__

#ifdef __cplusplus
typedef class Position Position;
#else
typedef struct Position Position;
#endif /* __cplusplus */

#endif    /* __Position_FWD_DEFINED__ */


#ifndef __Language_FWD_DEFINED__
#define __Language_FWD_DEFINED__

#ifdef __cplusplus
typedef class Language Language;
#else
typedef struct Language Language;
#endif /* __cplusplus */

#endif    /* __Language_FWD_DEFINED__ */


#ifndef __HotKey_FWD_DEFINED__
#define __HotKey_FWD_DEFINED__

#ifdef __cplusplus
typedef class HotKey HotKey;
#else
typedef struct HotKey HotKey;
#endif /* __cplusplus */

#endif    /* __HotKey_FWD_DEFINED__ */


#ifndef __Globals_FWD_DEFINED__
#define __Globals_FWD_DEFINED__

#ifdef __cplusplus
typedef class Globals Globals;
#else
typedef struct Globals Globals;
#endif /* __cplusplus */

#endif    /* __Globals_FWD_DEFINED__ */


#ifndef __Rect_FWD_DEFINED__
#define __Rect_FWD_DEFINED__

#ifdef __cplusplus
typedef class Rect Rect;
#else
typedef struct Rect Rect;
#endif /* __cplusplus */

#endif    /* __Rect_FWD_DEFINED__ */


#ifndef __CodeList_FWD_DEFINED__
#define __CodeList_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeList CodeList;
#else
typedef struct CodeList CodeList;
#endif /* __cplusplus */

#endif    /* __CodeList_FWD_DEFINED__ */


#ifndef __CodeTip_FWD_DEFINED__
#define __CodeTip_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeTip CodeTip;
#else
typedef struct CodeTip CodeTip;
#endif /* __cplusplus */

#endif    /* __CodeTip_FWD_DEFINED__ */


#ifndef __CodeTipHighlight_FWD_DEFINED__
#define __CodeTipHighlight_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeTipHighlight CodeTipHighlight;
#else
typedef struct CodeTipHighlight CodeTipHighlight;
#endif /* __cplusplus */

#endif    /* __CodeTipHighlight_FWD_DEFINED__ */


#ifndef __CodeTipFuncHighlight_FWD_DEFINED__
#define __CodeTipFuncHighlight_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeTipFuncHighlight CodeTipFuncHighlight;
#else
typedef struct CodeTipFuncHighlight CodeTipFuncHighlight;
#endif /* __cplusplus */

#endif    /* __CodeTipFuncHighlight_FWD_DEFINED__ */


#ifndef __CodeTipMultiFunc_FWD_DEFINED__
#define __CodeTipMultiFunc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CodeTipMultiFunc CodeTipMultiFunc;
#else
typedef struct CodeTipMultiFunc CodeTipMultiFunc;
#endif /* __cplusplus */

#endif    /* __CodeTipMultiFunc_FWD_DEFINED__ */


#ifndef __IRange_FWD_DEFINED__
#define __IRange_FWD_DEFINED__
typedef interface IRange IRange;
#endif    /* __IRange_FWD_DEFINED__ */


#ifndef __IPosition_FWD_DEFINED__
#define __IPosition_FWD_DEFINED__
typedef interface IPosition IPosition;
#endif    /* __IPosition_FWD_DEFINED__ */


#ifndef __ILanguage_FWD_DEFINED__
#define __ILanguage_FWD_DEFINED__
typedef interface ILanguage ILanguage;
#endif    /* __ILanguage_FWD_DEFINED__ */


#ifndef __IHotKey_FWD_DEFINED__
#define __IHotKey_FWD_DEFINED__
typedef interface IHotKey IHotKey;
#endif    /* __IHotKey_FWD_DEFINED__ */


#ifndef __IGlobals_FWD_DEFINED__
#define __IGlobals_FWD_DEFINED__
typedef interface IGlobals IGlobals;
#endif    /* __IGlobals_FWD_DEFINED__ */


#ifndef __IRect_FWD_DEFINED__
#define __IRect_FWD_DEFINED__
typedef interface IRect IRect;
#endif    /* __IRect_FWD_DEFINED__ */


#ifndef __ICodeList_FWD_DEFINED__
#define __ICodeList_FWD_DEFINED__
typedef interface ICodeList ICodeList;
#endif    /* __ICodeList_FWD_DEFINED__ */


#ifndef __ICodeTip_FWD_DEFINED__
#define __ICodeTip_FWD_DEFINED__
typedef interface ICodeTip ICodeTip;
#endif    /* __ICodeTip_FWD_DEFINED__ */


#ifndef __ICodeTipHighlight_FWD_DEFINED__
#define __ICodeTipHighlight_FWD_DEFINED__
typedef interface ICodeTipHighlight ICodeTipHighlight;
#endif    /* __ICodeTipHighlight_FWD_DEFINED__ */


#ifndef __ICodeTipFuncHighlight_FWD_DEFINED__
#define __ICodeTipFuncHighlight_FWD_DEFINED__
typedef interface ICodeTipFuncHighlight ICodeTipFuncHighlight;
#endif    /* __ICodeTipFuncHighlight_FWD_DEFINED__ */


#ifndef __ICodeTipMultiFunc_FWD_DEFINED__
#define __ICodeTipMultiFunc_FWD_DEFINED__
typedef interface ICodeTipMultiFunc ICodeTipMultiFunc;
#endif    /* __ICodeTipMultiFunc_FWD_DEFINED__ */


#ifndef __ICodeSenseEvents_FWD_DEFINED__
#define __ICodeSenseEvents_FWD_DEFINED__
typedef interface ICodeSenseEvents ICodeSenseEvents;
#endif    /* __ICodeSenseEvents_FWD_DEFINED__ */


#ifndef __ICodeSense_FWD_DEFINED__
#define __ICodeSense_FWD_DEFINED__
typedef interface ICodeSense ICodeSense;
#endif    /* __ICodeSense_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * );

/* interface __MIDL_itf_editx_0000 */
/* [local] */

#pragma once









extern RPC_IF_HANDLE __MIDL_itf_editx_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_editx_0000_v0_0_s_ifspec;


#ifndef __CodeSense_LIBRARY_DEFINED__
#define __CodeSense_LIBRARY_DEFINED__

/* library CodeSense */
/* [helpfile][helpstring][version][uuid] */

typedef /* [uuid] */
enum cmLimit
    {   cmMaxFindReplText   = 100,
   cmFindReplaceMRUMax   = 10,
   cmFindReplaceMRUBuffSize   = 1010,
   cmMaxMacros   = 10,
   cmMaxCmdString   = 50,
   cmMaxCmdDescription   = 100,
   cmMaxLanguageName   = 30,
   cmMaxTabSize   = 100,
   cmMinTabSize   = 2
    }   cmLimit;

typedef /* [uuid] */
enum cmCommand
    {   cmCmdWordUppercase   = 100,
   cmCmdWordTranspose   = 101,
   cmCmdWordRightExtend   = 102,
   cmCmdWordRight   = 103,
   cmCmdWordEndRight   = 104,
   cmCmdWordEndRightExtend   = 105,
   cmCmdWordLowerCase   = 106,
   cmCmdWordLeftExtend   = 107,
   cmCmdWordLeft   = 108,
   cmCmdWordEndLeft   = 109,
   cmCmdWordEndLeftExtend   = 110,
   cmCmdWordDeleteToStart   = 111,
   cmCmdWordDeleteToEnd   = 112,
   cmCmdWordCapitalize   = 113,
   cmCmdWindowStart   = 114,
   cmCmdWindowScrollUp   = 115,
   cmCmdWindowScrollToTop   = 116,
   cmCmdWindowScrollToCenter   = 117,
   cmCmdWindowScrollToBottom   = 118,
   cmCmdWindowScrollRight   = 119,
   cmCmdWindowScrollLeft   = 120,
   cmCmdWindowScrollDown   = 121,
   cmCmdWindowRightEdge   = 122,
   cmCmdWindowLeftEdge   = 123,
   cmCmdWindowEnd   = 124,
   cmCmdUppercaseSelection   = 125,
   cmCmdUntabifySelection   = 126,
   cmCmdUnindentSelection   = 127,
   cmCmdUndoChanges   = 128,
   cmCmdUndo   = 129,
   cmCmdTabifySelection   = 130,
   cmCmdSentenceRight   = 131,
   cmCmdSentenceLeft   = 132,
   cmCmdSentenceCut   = 133,
   cmCmdSelectSwapAnchor   = 134,
   cmCmdSelectPara   = 135,
   cmCmdSelectLine   = 136,
   cmCmdSelectAll   = 137,
   cmCmdRedoChanges   = 138,
   cmCmdRedo   = 139,
   cmCmdPaste   = 140,
   cmCmdParaUp   = 141,
   cmCmdParaDown   = 142,
   cmCmdPageUpExtend   = 143,
   cmCmdPageUp   = 144,
   cmCmdPageDownExtend   = 145,
   cmCmdPageDown   = 146,
   cmCmdLowercaseSelection   = 147,
   cmCmdLineUpExtend   = 148,
   cmCmdLineUp   = 149,
   cmCmdLineTranspose   = 150,
   cmCmdLineStart   = 151,
   cmCmdLineOpenBelow   = 152,
   cmCmdLineOpenAbove   = 153,
   cmCmdLineEndExtend   = 154,
   cmCmdLineEnd   = 155,
   cmCmdLineDownExtend   = 156,
   cmCmdLineDown   = 157,
   cmCmdLineDeleteToStart   = 158,
   cmCmdLineDeleteToEnd   = 159,
   cmCmdLineDelete   = 160,
   cmCmdLineCut   = 161,
   cmCmdIndentToPrev   = 162,
   cmCmdIndentSelection   = 163,
   cmCmdHomeExtend   = 164,
   cmCmdHome   = 165,
   cmCmdGotoMatchBrace   = 166,
   cmCmdGotoIndentation   = 167,
   cmCmdGotoLine   = 168,
   cmCmdFindReplace   = 169,
   cmCmdReplace   = 170,
   cmCmdReplaceAllInBuffer   = 171,
   cmCmdReplaceAllInSelection   = 172,
   cmCmdFindPrevWord   = 173,
   cmCmdFindPrev   = 174,
   cmCmdFindNextWord   = 175,
   cmCmdFindNext   = 176,
   cmCmdFindMarkAll   = 177,
   cmCmdFind   = 178,
   cmCmdSetFindText   = 179,
   cmCmdSetReplaceText   = 180,
   cmCmdTogglePreserveCase   = 181,
   cmCmdToggleWholeWord   = 182,
   cmCmdToggleCaseSensitive   = 183,
   cmCmdEnd   = 184,
   cmCmdToggleWhitespaceDisplay   = 185,
   cmCmdToggleOvertype   = 186,
   cmCmdSetRepeatCount   = 187,
   cmCmdDocumentStartExtend   = 188,
   cmCmdDocumentStart   = 189,
   cmCmdDocumentEndExtend   = 190,
   cmCmdDocumentEnd   = 191,
   cmCmdDeleteHorizontalSpace   = 192,
   cmCmdDeleteBlankLines   = 193,
   cmCmdDeleteBack   = 194,
   cmCmdDelete   = 195,
   cmCmdCutSelection   = 196,
   cmCmdCut   = 197,
   cmCmdCopy   = 198,
   cmCmdCharTranspose   = 199,
   cmCmdCharRightExtend   = 200,
   cmCmdCharRight   = 201,
   cmCmdCharLeftExtend   = 202,
   cmCmdCharLeft   = 203,
   cmCmdBookmarkToggle   = 204,
   cmCmdBookmarkPrev   = 205,
   cmCmdBookmarkNext   = 206,
   cmCmdBookmarkClearAll   = 207,
   cmCmdBookmarkJumpToFirst   = 208,
   cmCmdBookmarkJumpToLast   = 209,
   cmCmdAppendNextCut   = 210,
   cmCmdInsertChar   = 211,
   cmCmdNewLine   = 212,
   cmCmdRecordMacro   = 213,
   cmCmdPlayMacro1   = 214,
   cmCmdPlayMacro2   = 215,
   cmCmdPlayMacro3   = 216,
   cmCmdPlayMacro4   = 217,
   cmCmdPlayMacro5   = 218,
   cmCmdPlayMacro6   = 219,
   cmCmdPlayMacro7   = 220,
   cmCmdPlayMacro8   = 221,
   cmCmdPlayMacro9   = 222,
   cmCmdPlayMacro10   = 223,
   cmCmdProperties   = 224,
   cmCmdBeginUndo   = 225,
   cmCmdEndUndo   = 226,
   cmCmdToggleRegExp   = 228,
   cmCmdClearSelection   = 229,
   cmCmdRegExpOn   = 230,
   cmCmdRegExpOff   = 231,
   cmCmdWholeWordOn   = 232,
   cmCmdWholeWordOff   = 233,
   cmCmdPreserveCaseOn   = 234,
   cmCmdPreserveCaseOff   = 235,
   cmCmdCaseSensitiveOn   = 236,
   cmCmdCaseSensitiveOff   = 237,
   cmCmdWhitespaceDisplayOn   = 238,
   cmCmdWhitespaceDisplayOff   = 239,
   cmCmdOvertypeOn   = 240,
   cmCmdOvertypeOff   = 241,
   cmCmdCodeList   = 242,
   cmCmdCodeTip   = 243
    }   cmCommand;

typedef /* [uuid] */
enum cmCommandErr
    {   cmErrFailure   = 1,
   cmErrInput   = 2,
   cmErrSelection   = 3,
   cmErrNotFound   = 4,
   cmErrEmptyBuf   = 5,
   cmErrReadOnly   = 6
    }   cmCommandErr;

typedef /* [uuid] */
enum cmColorItem
    {   cmClrWindow   = 0,
   cmClrLeftMargin   = 1,
   cmClrBookmark   = 2,
   cmClrBookmarkBk   = 3,
   cmClrText   = 4,
   cmClrTextBk   = 5,
   cmClrNumber   = 6,
   cmClrNumberBk   = 7,
   cmClrKeyword   = 8,
   cmClrKeywordBk   = 9,
   cmClrOperator   = 10,
   cmClrOperatorBk   = 11,
   cmClrScopeKeyword   = 12,
   cmClrScopeKeywordBk   = 13,
   cmClrComment   = 14,
   cmClrCommentBk   = 15,
   cmClrString   = 16,
   cmClrStringBk   = 17,
   cmClrTagText   = 18,
   cmClrTagTextBk   = 19,
   cmClrTagEntity   = 20,
   cmClrTagEntityBk   = 21,
   cmClrTagElementName   = 22,
   cmClrTagElementNameBk   = 23,
   cmClrTagAttributeName   = 24,
   cmClrTagAttributeNameBk   = 25,
   cmClrLineNumber   = 26,
   cmClrLineNumberBk   = 27,
   cmClrHDividerLines   = 28,
   cmClrVDividerLines   = 29,
   cmClrHighlightedLine   = 30
    }   cmColorItem;

typedef /* [uuid] */
enum cmFontStyleItem
    {   cmStyText   = 0,
   cmStyNumber   = 1,
   cmStyKeyword   = 2,
   cmStyOperator   = 3,
   cmStyScopeKeyword   = 4,
   cmStyComment   = 5,
   cmStyString   = 6,
   cmStyTagText   = 7,
   cmStyTagEntity   = 8,
   cmStyTagElementName   = 9,
   cmStyTagAttributeName   = 10,
   cmStyLineNumber   = 11
    }   cmFontStyleItem;

typedef /* [uuid] */
enum cmFontStyle
    {   cmFontNormal   = 0,
   cmFontBold   = 1,
   cmFontItalic   = 2,
   cmFontBoldItalic   = 3,
   cmFontUnderline   = 4
    }   cmFontStyle;

typedef /* [uuid] */
enum cmAutoIndentMode
    {   cmIndentOff   = 0,
   cmIndentScope   = 1,
   cmIndentPrevLine   = 2
    }   cmAutoIndentMode;

typedef /* [uuid] */
enum cmHitTestCode
    {   cmNowhere   = 0,
   cmHSplitter   = 1,
   cmVSplitter   = 2,
   cmHVSplitter   = 3,
   cmEditSpace   = 4,
   cmHScrollBar   = 5,
   cmVScrollBar   = 6,
   cmSizeBox   = 7,
   cmLeftMargin   = 8
    }   cmHitTestCode;

typedef /* [uuid] */
enum cmLineNumStyle
    {   cmDecimal   = 10,
   cmHexadecimal   = 16,
   cmBinary   = 2,
   cmOctal   = 8
    }   cmLineNumStyle;

typedef /* [uuid] */
enum cmPrintFlags
    {   cmPrnPromptDlg   = 0,
   cmPrnDefaultPrn   = 0x1,
   cmPrnHDC   = 0x2,
   cmPrnRichFonts   = 0x4,
   cmPrnColor   = 0x8,
   cmPrnPageNums   = 0x10,
   cmPrnDateTime   = 0x20,
   cmPrnBorderThin   = 0x40,
   cmPrnBorderThick   = 0x80,
   cmPrnBorderDouble   = 0x100,
   cmPrnSelection   = 0x200
    }   cmPrintFlags;

typedef /* [uuid] */
enum cmLangStyle
    {   cmLangStyleProcedural   = 0,
   cmLangStyleSGML   = 1
    }   cmLangStyle;

typedef /* [uuid] */
enum cmBorderStyle
    {   cmBorderNone   = 0,
   cmBorderThin   = 0x1,
   cmBorderClient   = 0x2,
   cmBorderStatic   = 0x4,
   cmBorderModal   = 0x8,
   cmBorderCorral   = 0xa
    }   cmBorderStyle;

typedef /* [uuid] */
enum cmTokenType
    {   cmTokenTypeKeyword   = 0x1,
   cmTokenTypeOperator   = 0x2,
   cmTokenTypeString   = 0x3,
   cmTokenTypeSingleLineComment   = 0x4,
   cmTokenTypeMultiLineComment   = 0x5,
   cmTokenTypeNumber   = 0x6,
   cmTokenTypeScopeBegin   = 0x7,
   cmTokenTypeScopeEnd   = 0x8,
   cmTokenTypeText   = 0xff,
   cmTokenTypeUnknown   = -1
    }   cmTokenType;

typedef /* [uuid] */
enum cmToolTipType
    {   cmToolTipTypeNone   = 0,
   cmToolTipTypeNormal   = 0x1,
   cmToolTipTypeHighlight   = 0x2,
   cmToolTipTypeFuncHighlight   = 0x3,
   cmToolTipTypeMultiFunc   = 0x4
    }   cmToolTipType;

typedef /* [uuid] */
enum cmCodeListSortStyle
    {   cmCodeListSortNone   = 0,
   cmCodeListSortAscending   = 0x1,
   cmCodeListSortDescending   = 0x2
    }   cmCodeListSortStyle;


EXTERN_C const IID LIBID_CodeSense;

EXTERN_C const CLSID CLSID_CodeSense;

#ifdef __cplusplus

class DECLSPEC_UUID("A0F69707-2D87-4B20-9471-FBC003427134")
CodeSense;
#endif

EXTERN_C const CLSID CLSID_Range;

#ifdef __cplusplus

class DECLSPEC_UUID("94D8D56F-73D3-4F3A-B1C0-F77EF1B2CB63")
Range;
#endif

EXTERN_C const CLSID CLSID_Position;

#ifdef __cplusplus

class DECLSPEC_UUID("96621C91-00F2-424A-B62F-C426B7FD9BD0")
Position;
#endif

EXTERN_C const CLSID CLSID_Language;

#ifdef __cplusplus

class DECLSPEC_UUID("5C5486F3-CE41-4EC6-B658-0155CE8836A1")
Language;
#endif

EXTERN_C const CLSID CLSID_HotKey;

#ifdef __cplusplus

class DECLSPEC_UUID("BE2A73A9-BE2C-420E-86A4-818E48E3F29C")
HotKey;
#endif

EXTERN_C const CLSID CLSID_Globals;

#ifdef __cplusplus

class DECLSPEC_UUID("5DFD0E0D-A13D-4B57-9A3C-FBB7B4072BE1")
Globals;
#endif

EXTERN_C const CLSID CLSID_Rect;

#ifdef __cplusplus

class DECLSPEC_UUID("7F4BD1FA-4142-4C14-BCEC-5563E7F66E9B")
Rect;
#endif

EXTERN_C const CLSID CLSID_CodeList;

#ifdef __cplusplus

class DECLSPEC_UUID("8DE7E400-050C-4B69-9EE7-2E9B0CCFD943")
CodeList;
#endif

EXTERN_C const CLSID CLSID_CodeTip;

#ifdef __cplusplus

class DECLSPEC_UUID("9FCF6BF7-2185-4A42-AD34-FF65FB4910B2")
CodeTip;
#endif

EXTERN_C const CLSID CLSID_CodeTipHighlight;

#ifdef __cplusplus

class DECLSPEC_UUID("739908FB-2BE8-40BE-9DDD-B486289BF04B")
CodeTipHighlight;
#endif

EXTERN_C const CLSID CLSID_CodeTipFuncHighlight;

#ifdef __cplusplus

class DECLSPEC_UUID("B1F5AB74-65B5-49FB-8A7C-221987BF09A4")
CodeTipFuncHighlight;
#endif

EXTERN_C const CLSID CLSID_CodeTipMultiFunc;

#ifdef __cplusplus

class DECLSPEC_UUID("0458139D-32E9-4C43-876B-73B0A21A4E9C")
CodeTipMultiFunc;
#endif
#endif /* __CodeSense_LIBRARY_DEFINED__ */

#ifndef __IRange_INTERFACE_DEFINED__
#define __IRange_INTERFACE_DEFINED__

/* interface IRange */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_IRange;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6CB7A54F-E406-4074-BB5E-6DEB17AC0010")
    IRange : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_StartLineNo(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_StartLineNo(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_StartColNo(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_StartColNo(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EndLineNo(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EndLineNo(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EndColNo(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EndColNo(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ColumnSel(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColumnSel(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IsEmpty(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Empty( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Normalize( void) = 0;

    };

#else    /* C style interface */

    typedef struct IRangeVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            IRange __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            IRange __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            IRange __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            IRange __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            IRange __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            IRange __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            IRange __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StartLineNo )(
            IRange __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StartLineNo )(
            IRange __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StartColNo )(
            IRange __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StartColNo )(
            IRange __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EndLineNo )(
            IRange __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EndLineNo )(
            IRange __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EndColNo )(
            IRange __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EndColNo )(
            IRange __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ColumnSel )(
            IRange __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ColumnSel )(
            IRange __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsEmpty )(
            IRange __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Empty )(
            IRange __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Normalize )(
            IRange __RPC_FAR * This);

        END_INTERFACE
    } IRangeVtbl;

    interface IRange
    {
        CONST_VTBL struct IRangeVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define IRange_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRange_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IRange_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define IRange_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRange_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRange_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRange_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRange_put_StartLineNo(This,lVal)   \
    (This)->lpVtbl -> put_StartLineNo(This,lVal)

#define IRange_get_StartLineNo(This,plVal)   \
    (This)->lpVtbl -> get_StartLineNo(This,plVal)

#define IRange_put_StartColNo(This,lVal)   \
    (This)->lpVtbl -> put_StartColNo(This,lVal)

#define IRange_get_StartColNo(This,plVal)   \
    (This)->lpVtbl -> get_StartColNo(This,plVal)

#define IRange_put_EndLineNo(This,lVal)   \
    (This)->lpVtbl -> put_EndLineNo(This,lVal)

#define IRange_get_EndLineNo(This,plVal)   \
    (This)->lpVtbl -> get_EndLineNo(This,plVal)

#define IRange_put_EndColNo(This,lVal)   \
    (This)->lpVtbl -> put_EndColNo(This,lVal)

#define IRange_get_EndColNo(This,plVal)   \
    (This)->lpVtbl -> get_EndColNo(This,plVal)

#define IRange_put_ColumnSel(This,bVal)   \
    (This)->lpVtbl -> put_ColumnSel(This,bVal)

#define IRange_get_ColumnSel(This,pbVal)   \
    (This)->lpVtbl -> get_ColumnSel(This,pbVal)

#define IRange_IsEmpty(This,pbAnswer)   \
    (This)->lpVtbl -> IsEmpty(This,pbAnswer)

#define IRange_Empty(This)   \
    (This)->lpVtbl -> Empty(This)

#define IRange_Normalize(This)   \
    (This)->lpVtbl -> Normalize(This)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRange_put_StartLineNo_Proxy(
    IRange __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRange_put_StartLineNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRange_get_StartLineNo_Proxy(
    IRange __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRange_get_StartLineNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRange_put_StartColNo_Proxy(
    IRange __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRange_put_StartColNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRange_get_StartColNo_Proxy(
    IRange __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRange_get_StartColNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRange_put_EndLineNo_Proxy(
    IRange __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRange_put_EndLineNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRange_get_EndLineNo_Proxy(
    IRange __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRange_get_EndLineNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRange_put_EndColNo_Proxy(
    IRange __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRange_put_EndColNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRange_get_EndColNo_Proxy(
    IRange __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRange_get_EndColNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRange_put_ColumnSel_Proxy(
    IRange __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB IRange_put_ColumnSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRange_get_ColumnSel_Proxy(
    IRange __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB IRange_get_ColumnSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IRange_IsEmpty_Proxy(
    IRange __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB IRange_IsEmpty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IRange_Empty_Proxy(
    IRange __RPC_FAR * This);


void __RPC_STUB IRange_Empty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IRange_Normalize_Proxy(
    IRange __RPC_FAR * This);


void __RPC_STUB IRange_Normalize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __IRange_INTERFACE_DEFINED__ */


#ifndef __IPosition_INTERFACE_DEFINED__
#define __IPosition_INTERFACE_DEFINED__

/* interface IPosition */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_IPosition;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9BF3BD89-D8AD-4D43-9BFE-5FDDF07CD558")
    IPosition : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineNo(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineNo(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ColNo(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColNo(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

    };

#else    /* C style interface */

    typedef struct IPositionVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            IPosition __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            IPosition __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            IPosition __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            IPosition __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            IPosition __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            IPosition __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            IPosition __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LineNo )(
            IPosition __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineNo )(
            IPosition __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ColNo )(
            IPosition __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ColNo )(
            IPosition __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        END_INTERFACE
    } IPositionVtbl;

    interface IPosition
    {
        CONST_VTBL struct IPositionVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define IPosition_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPosition_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IPosition_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define IPosition_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPosition_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPosition_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPosition_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPosition_put_LineNo(This,lVal)   \
    (This)->lpVtbl -> put_LineNo(This,lVal)

#define IPosition_get_LineNo(This,plVal)   \
    (This)->lpVtbl -> get_LineNo(This,plVal)

#define IPosition_put_ColNo(This,lVal)   \
    (This)->lpVtbl -> put_ColNo(This,lVal)

#define IPosition_get_ColNo(This,plVal)   \
    (This)->lpVtbl -> get_ColNo(This,plVal)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IPosition_put_LineNo_Proxy(
    IPosition __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IPosition_put_LineNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IPosition_get_LineNo_Proxy(
    IPosition __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IPosition_get_LineNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IPosition_put_ColNo_Proxy(
    IPosition __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IPosition_put_ColNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IPosition_get_ColNo_Proxy(
    IPosition __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IPosition_get_ColNo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __IPosition_INTERFACE_DEFINED__ */


#ifndef __ILanguage_INTERFACE_DEFINED__
#define __ILanguage_INTERFACE_DEFINED__

/* interface ILanguage */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_ILanguage;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DAFFE8A2-925B-45D4-B034-F8E9D093135A")
    ILanguage : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_CaseSensitive(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CaseSensitive(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Keywords(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Keywords(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Operators(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Operators(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SingleLineComments(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SingleLineComments(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_MultiLineComments1(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_MultiLineComments1(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_MultiLineComments2(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_MultiLineComments2(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ScopeKeywords1(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ScopeKeywords1(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ScopeKeywords2(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ScopeKeywords2(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_StringDelims(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_StringDelims(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EscapeChar(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EscapeChar(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TerminatorChar(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TerminatorChar(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TagElementNames(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TagElementNames(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TagAttributeNames(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TagAttributeNames(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TagEntities(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TagEntities(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Style(
            /* [in] */ cmLangStyle Style) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Style(
            /* [retval][out] */ cmLangStyle __RPC_FAR *pStyle) = 0;

    };

#else    /* C style interface */

    typedef struct ILanguageVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ILanguage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ILanguage __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ILanguage __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ILanguage __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ILanguage __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ILanguage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ILanguage __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CaseSensitive )(
            ILanguage __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CaseSensitive )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Keywords )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Keywords )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Operators )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Operators )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SingleLineComments )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SingleLineComments )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MultiLineComments1 )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MultiLineComments1 )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MultiLineComments2 )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MultiLineComments2 )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScopeKeywords1 )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScopeKeywords1 )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScopeKeywords2 )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScopeKeywords2 )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StringDelims )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StringDelims )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EscapeChar )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EscapeChar )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TerminatorChar )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TerminatorChar )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TagElementNames )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TagElementNames )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TagAttributeNames )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TagAttributeNames )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TagEntities )(
            ILanguage __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TagEntities )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Style )(
            ILanguage __RPC_FAR * This,
            /* [in] */ cmLangStyle Style);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Style )(
            ILanguage __RPC_FAR * This,
            /* [retval][out] */ cmLangStyle __RPC_FAR *pStyle);

        END_INTERFACE
    } ILanguageVtbl;

    interface ILanguage
    {
        CONST_VTBL struct ILanguageVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ILanguage_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILanguage_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ILanguage_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ILanguage_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILanguage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILanguage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILanguage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILanguage_put_CaseSensitive(This,bVal)   \
    (This)->lpVtbl -> put_CaseSensitive(This,bVal)

#define ILanguage_get_CaseSensitive(This,pbVal)   \
    (This)->lpVtbl -> get_CaseSensitive(This,pbVal)

#define ILanguage_put_Keywords(This,strVal)   \
    (This)->lpVtbl -> put_Keywords(This,strVal)

#define ILanguage_get_Keywords(This,pstrVal)   \
    (This)->lpVtbl -> get_Keywords(This,pstrVal)

#define ILanguage_put_Operators(This,strVal)   \
    (This)->lpVtbl -> put_Operators(This,strVal)

#define ILanguage_get_Operators(This,pstrVal)   \
    (This)->lpVtbl -> get_Operators(This,pstrVal)

#define ILanguage_put_SingleLineComments(This,strVal)   \
    (This)->lpVtbl -> put_SingleLineComments(This,strVal)

#define ILanguage_get_SingleLineComments(This,pstrVal)   \
    (This)->lpVtbl -> get_SingleLineComments(This,pstrVal)

#define ILanguage_put_MultiLineComments1(This,strVal)   \
    (This)->lpVtbl -> put_MultiLineComments1(This,strVal)

#define ILanguage_get_MultiLineComments1(This,pstrVal)   \
    (This)->lpVtbl -> get_MultiLineComments1(This,pstrVal)

#define ILanguage_put_MultiLineComments2(This,strVal)   \
    (This)->lpVtbl -> put_MultiLineComments2(This,strVal)

#define ILanguage_get_MultiLineComments2(This,pstrVal)   \
    (This)->lpVtbl -> get_MultiLineComments2(This,pstrVal)

#define ILanguage_put_ScopeKeywords1(This,strVal)   \
    (This)->lpVtbl -> put_ScopeKeywords1(This,strVal)

#define ILanguage_get_ScopeKeywords1(This,pstrVal)   \
    (This)->lpVtbl -> get_ScopeKeywords1(This,pstrVal)

#define ILanguage_put_ScopeKeywords2(This,strVal)   \
    (This)->lpVtbl -> put_ScopeKeywords2(This,strVal)

#define ILanguage_get_ScopeKeywords2(This,pstrVal)   \
    (This)->lpVtbl -> get_ScopeKeywords2(This,pstrVal)

#define ILanguage_put_StringDelims(This,strVal)   \
    (This)->lpVtbl -> put_StringDelims(This,strVal)

#define ILanguage_get_StringDelims(This,pstrVal)   \
    (This)->lpVtbl -> get_StringDelims(This,pstrVal)

#define ILanguage_put_EscapeChar(This,strVal)   \
    (This)->lpVtbl -> put_EscapeChar(This,strVal)

#define ILanguage_get_EscapeChar(This,pstrVal)   \
    (This)->lpVtbl -> get_EscapeChar(This,pstrVal)

#define ILanguage_put_TerminatorChar(This,strVal)   \
    (This)->lpVtbl -> put_TerminatorChar(This,strVal)

#define ILanguage_get_TerminatorChar(This,pstrVal)   \
    (This)->lpVtbl -> get_TerminatorChar(This,pstrVal)

#define ILanguage_put_TagElementNames(This,strVal)   \
    (This)->lpVtbl -> put_TagElementNames(This,strVal)

#define ILanguage_get_TagElementNames(This,pstrVal)   \
    (This)->lpVtbl -> get_TagElementNames(This,pstrVal)

#define ILanguage_put_TagAttributeNames(This,strVal)   \
    (This)->lpVtbl -> put_TagAttributeNames(This,strVal)

#define ILanguage_get_TagAttributeNames(This,pstrVal)   \
    (This)->lpVtbl -> get_TagAttributeNames(This,pstrVal)

#define ILanguage_put_TagEntities(This,strVal)   \
    (This)->lpVtbl -> put_TagEntities(This,strVal)

#define ILanguage_get_TagEntities(This,pstrVal)   \
    (This)->lpVtbl -> get_TagEntities(This,pstrVal)

#define ILanguage_put_Style(This,Style)   \
    (This)->lpVtbl -> put_Style(This,Style)

#define ILanguage_get_Style(This,pStyle)   \
    (This)->lpVtbl -> get_Style(This,pStyle)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_CaseSensitive_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ILanguage_put_CaseSensitive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_CaseSensitive_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ILanguage_get_CaseSensitive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_Keywords_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_Keywords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_Keywords_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_Keywords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_Operators_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_Operators_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_Operators_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_Operators_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_SingleLineComments_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_SingleLineComments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_SingleLineComments_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_SingleLineComments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_MultiLineComments1_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_MultiLineComments1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_MultiLineComments1_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_MultiLineComments1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_MultiLineComments2_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_MultiLineComments2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_MultiLineComments2_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_MultiLineComments2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_ScopeKeywords1_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_ScopeKeywords1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_ScopeKeywords1_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_ScopeKeywords1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_ScopeKeywords2_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_ScopeKeywords2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_ScopeKeywords2_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_ScopeKeywords2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_StringDelims_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_StringDelims_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_StringDelims_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_StringDelims_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_EscapeChar_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_EscapeChar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_EscapeChar_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_EscapeChar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_TerminatorChar_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_TerminatorChar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_TerminatorChar_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_TerminatorChar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_TagElementNames_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_TagElementNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_TagElementNames_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_TagElementNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_TagAttributeNames_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_TagAttributeNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_TagAttributeNames_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_TagAttributeNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_TagEntities_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ILanguage_put_TagEntities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_TagEntities_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ILanguage_get_TagEntities_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ILanguage_put_Style_Proxy(
    ILanguage __RPC_FAR * This,
    /* [in] */ cmLangStyle Style);


void __RPC_STUB ILanguage_put_Style_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ILanguage_get_Style_Proxy(
    ILanguage __RPC_FAR * This,
    /* [retval][out] */ cmLangStyle __RPC_FAR *pStyle);


void __RPC_STUB ILanguage_get_Style_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ILanguage_INTERFACE_DEFINED__ */


#ifndef __IHotKey_INTERFACE_DEFINED__
#define __IHotKey_INTERFACE_DEFINED__

/* interface IHotKey */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_IHotKey;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BCBDC4F8-EDAB-490D-A110-02D3D71A54DE")
    IHotKey : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Modifiers1(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Modifiers1(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_VirtKey1(
            /* [in] */ BSTR strKey) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VirtKey1(
            /* [retval][out] */ BSTR __RPC_FAR *pstrKey) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Modifiers2(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Modifiers2(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_VirtKey2(
            /* [in] */ BSTR strKey) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VirtKey2(
            /* [retval][out] */ BSTR __RPC_FAR *pstrKey) = 0;

    };

#else    /* C style interface */

    typedef struct IHotKeyVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            IHotKey __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            IHotKey __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            IHotKey __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            IHotKey __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            IHotKey __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            IHotKey __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            IHotKey __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Modifiers1 )(
            IHotKey __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Modifiers1 )(
            IHotKey __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VirtKey1 )(
            IHotKey __RPC_FAR * This,
            /* [in] */ BSTR strKey);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VirtKey1 )(
            IHotKey __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrKey);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Modifiers2 )(
            IHotKey __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Modifiers2 )(
            IHotKey __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VirtKey2 )(
            IHotKey __RPC_FAR * This,
            /* [in] */ BSTR strKey);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VirtKey2 )(
            IHotKey __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrKey);

        END_INTERFACE
    } IHotKeyVtbl;

    interface IHotKey
    {
        CONST_VTBL struct IHotKeyVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define IHotKey_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHotKey_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IHotKey_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define IHotKey_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHotKey_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHotKey_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHotKey_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHotKey_put_Modifiers1(This,lVal)   \
    (This)->lpVtbl -> put_Modifiers1(This,lVal)

#define IHotKey_get_Modifiers1(This,plVal)   \
    (This)->lpVtbl -> get_Modifiers1(This,plVal)

#define IHotKey_put_VirtKey1(This,strKey)   \
    (This)->lpVtbl -> put_VirtKey1(This,strKey)

#define IHotKey_get_VirtKey1(This,pstrKey)   \
    (This)->lpVtbl -> get_VirtKey1(This,pstrKey)

#define IHotKey_put_Modifiers2(This,lVal)   \
    (This)->lpVtbl -> put_Modifiers2(This,lVal)

#define IHotKey_get_Modifiers2(This,plVal)   \
    (This)->lpVtbl -> get_Modifiers2(This,plVal)

#define IHotKey_put_VirtKey2(This,strKey)   \
    (This)->lpVtbl -> put_VirtKey2(This,strKey)

#define IHotKey_get_VirtKey2(This,pstrKey)   \
    (This)->lpVtbl -> get_VirtKey2(This,pstrKey)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IHotKey_put_Modifiers1_Proxy(
    IHotKey __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IHotKey_put_Modifiers1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IHotKey_get_Modifiers1_Proxy(
    IHotKey __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IHotKey_get_Modifiers1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IHotKey_put_VirtKey1_Proxy(
    IHotKey __RPC_FAR * This,
    /* [in] */ BSTR strKey);


void __RPC_STUB IHotKey_put_VirtKey1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IHotKey_get_VirtKey1_Proxy(
    IHotKey __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrKey);


void __RPC_STUB IHotKey_get_VirtKey1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IHotKey_put_Modifiers2_Proxy(
    IHotKey __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IHotKey_put_Modifiers2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IHotKey_get_Modifiers2_Proxy(
    IHotKey __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IHotKey_get_Modifiers2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IHotKey_put_VirtKey2_Proxy(
    IHotKey __RPC_FAR * This,
    /* [in] */ BSTR strKey);


void __RPC_STUB IHotKey_put_VirtKey2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IHotKey_get_VirtKey2_Proxy(
    IHotKey __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrKey);


void __RPC_STUB IHotKey_get_VirtKey2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __IHotKey_INTERFACE_DEFINED__ */


#ifndef __IGlobals_INTERFACE_DEFINED__
#define __IGlobals_INTERFACE_DEFINED__

/* interface IGlobals */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_IGlobals;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("11065D3D-3A32-4170-962A-8A0F168DB117")
    IGlobals : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_FindMRUList(
            /* [in] */ BSTR strMRUList) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_FindMRUList(
            /* [retval][out] */ BSTR __RPC_FAR *pstrMRUList) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ReplaceMRUList(
            /* [in] */ BSTR strMRUList) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReplaceMRUList(
            /* [retval][out] */ BSTR __RPC_FAR *pstrMRUList) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE RegisterLanguage(
            /* [in] */ BSTR strName,
            /* [in] */ ILanguage __RPC_FAR *LanguageDef) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE UnregisterLanguage(
            /* [in] */ BSTR strName) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetHotKeys(
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *HotKeysBuff) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetHotKeys(
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *HotKeysBuff) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ResetDefaultHotKeys( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetHotKeyForCmd(
            /* [in] */ cmCommand lCmd,
            /* [in] */ long lNum,
            /* [retval][out] */ IHotKey __RPC_FAR *__RPC_FAR *ppHotKey) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetNumHotKeysForCmd(
            /* [in] */ cmCommand lCmd,
            /* [retval][out] */ long __RPC_FAR *plCount) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE RegisterHotKey(
            /* [in] */ IHotKey __RPC_FAR *pHotKey,
            /* [in] */ cmCommand lCmd) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE UnregisterHotKey(
            /* [in] */ IHotKey __RPC_FAR *pHotKey) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetCommandString(
            /* [in] */ cmCommand lCmd,
            /* [in] */ VARIANT_BOOL bDescription,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMacro(
            /* [in] */ long lMacro,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *MacroBuff) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetMacro(
            /* [in] */ long lMacro,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *MacroBuff) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE RegisterCommand(
            /* [in] */ long lCmdId,
            /* [in] */ BSTR strName,
            /* [in] */ BSTR strDescription) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE UnregisterCommand(
            /* [in] */ long lCmdId) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetLanguageDef(
            /* [in] */ BSTR strName,
            /* [retval][out] */ ILanguage __RPC_FAR *__RPC_FAR *Language) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE UnregisterAllLanguages( void) = 0;

    };

#else    /* C style interface */

    typedef struct IGlobalsVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            IGlobals __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            IGlobals __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            IGlobals __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            IGlobals __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            IGlobals __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            IGlobals __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            IGlobals __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FindMRUList )(
            IGlobals __RPC_FAR * This,
            /* [in] */ BSTR strMRUList);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FindMRUList )(
            IGlobals __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrMRUList);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ReplaceMRUList )(
            IGlobals __RPC_FAR * This,
            /* [in] */ BSTR strMRUList);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ReplaceMRUList )(
            IGlobals __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrMRUList);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterLanguage )(
            IGlobals __RPC_FAR * This,
            /* [in] */ BSTR strName,
            /* [in] */ ILanguage __RPC_FAR *LanguageDef);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterLanguage )(
            IGlobals __RPC_FAR * This,
            /* [in] */ BSTR strName);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHotKeys )(
            IGlobals __RPC_FAR * This,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *HotKeysBuff);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetHotKeys )(
            IGlobals __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *HotKeysBuff);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ResetDefaultHotKeys )(
            IGlobals __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHotKeyForCmd )(
            IGlobals __RPC_FAR * This,
            /* [in] */ cmCommand lCmd,
            /* [in] */ long lNum,
            /* [retval][out] */ IHotKey __RPC_FAR *__RPC_FAR *ppHotKey);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNumHotKeysForCmd )(
            IGlobals __RPC_FAR * This,
            /* [in] */ cmCommand lCmd,
            /* [retval][out] */ long __RPC_FAR *plCount);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterHotKey )(
            IGlobals __RPC_FAR * This,
            /* [in] */ IHotKey __RPC_FAR *pHotKey,
            /* [in] */ cmCommand lCmd);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterHotKey )(
            IGlobals __RPC_FAR * This,
            /* [in] */ IHotKey __RPC_FAR *pHotKey);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCommandString )(
            IGlobals __RPC_FAR * This,
            /* [in] */ cmCommand lCmd,
            /* [in] */ VARIANT_BOOL bDescription,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMacro )(
            IGlobals __RPC_FAR * This,
            /* [in] */ long lMacro,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *MacroBuff);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMacro )(
            IGlobals __RPC_FAR * This,
            /* [in] */ long lMacro,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *MacroBuff);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RegisterCommand )(
            IGlobals __RPC_FAR * This,
            /* [in] */ long lCmdId,
            /* [in] */ BSTR strName,
            /* [in] */ BSTR strDescription);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterCommand )(
            IGlobals __RPC_FAR * This,
            /* [in] */ long lCmdId);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLanguageDef )(
            IGlobals __RPC_FAR * This,
            /* [in] */ BSTR strName,
            /* [retval][out] */ ILanguage __RPC_FAR *__RPC_FAR *Language);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnregisterAllLanguages )(
            IGlobals __RPC_FAR * This);

        END_INTERFACE
    } IGlobalsVtbl;

    interface IGlobals
    {
        CONST_VTBL struct IGlobalsVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define IGlobals_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGlobals_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IGlobals_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define IGlobals_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IGlobals_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IGlobals_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IGlobals_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IGlobals_put_FindMRUList(This,strMRUList)   \
    (This)->lpVtbl -> put_FindMRUList(This,strMRUList)

#define IGlobals_get_FindMRUList(This,pstrMRUList)   \
    (This)->lpVtbl -> get_FindMRUList(This,pstrMRUList)

#define IGlobals_put_ReplaceMRUList(This,strMRUList)   \
    (This)->lpVtbl -> put_ReplaceMRUList(This,strMRUList)

#define IGlobals_get_ReplaceMRUList(This,pstrMRUList)   \
    (This)->lpVtbl -> get_ReplaceMRUList(This,pstrMRUList)

#define IGlobals_RegisterLanguage(This,strName,LanguageDef)   \
    (This)->lpVtbl -> RegisterLanguage(This,strName,LanguageDef)

#define IGlobals_UnregisterLanguage(This,strName)   \
    (This)->lpVtbl -> UnregisterLanguage(This,strName)

#define IGlobals_GetHotKeys(This,HotKeysBuff)   \
    (This)->lpVtbl -> GetHotKeys(This,HotKeysBuff)

#define IGlobals_SetHotKeys(This,HotKeysBuff)   \
    (This)->lpVtbl -> SetHotKeys(This,HotKeysBuff)

#define IGlobals_ResetDefaultHotKeys(This)   \
    (This)->lpVtbl -> ResetDefaultHotKeys(This)

#define IGlobals_GetHotKeyForCmd(This,lCmd,lNum,ppHotKey)   \
    (This)->lpVtbl -> GetHotKeyForCmd(This,lCmd,lNum,ppHotKey)

#define IGlobals_GetNumHotKeysForCmd(This,lCmd,plCount)   \
    (This)->lpVtbl -> GetNumHotKeysForCmd(This,lCmd,plCount)

#define IGlobals_RegisterHotKey(This,pHotKey,lCmd)   \
    (This)->lpVtbl -> RegisterHotKey(This,pHotKey,lCmd)

#define IGlobals_UnregisterHotKey(This,pHotKey)   \
    (This)->lpVtbl -> UnregisterHotKey(This,pHotKey)

#define IGlobals_GetCommandString(This,lCmd,bDescription,pstrVal)   \
    (This)->lpVtbl -> GetCommandString(This,lCmd,bDescription,pstrVal)

#define IGlobals_GetMacro(This,lMacro,MacroBuff)   \
    (This)->lpVtbl -> GetMacro(This,lMacro,MacroBuff)

#define IGlobals_SetMacro(This,lMacro,MacroBuff)   \
    (This)->lpVtbl -> SetMacro(This,lMacro,MacroBuff)

#define IGlobals_RegisterCommand(This,lCmdId,strName,strDescription)   \
    (This)->lpVtbl -> RegisterCommand(This,lCmdId,strName,strDescription)

#define IGlobals_UnregisterCommand(This,lCmdId)   \
    (This)->lpVtbl -> UnregisterCommand(This,lCmdId)

#define IGlobals_GetLanguageDef(This,strName,Language)   \
    (This)->lpVtbl -> GetLanguageDef(This,strName,Language)

#define IGlobals_UnregisterAllLanguages(This)   \
    (This)->lpVtbl -> UnregisterAllLanguages(This)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IGlobals_put_FindMRUList_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ BSTR strMRUList);


void __RPC_STUB IGlobals_put_FindMRUList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IGlobals_get_FindMRUList_Proxy(
    IGlobals __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrMRUList);


void __RPC_STUB IGlobals_get_FindMRUList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IGlobals_put_ReplaceMRUList_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ BSTR strMRUList);


void __RPC_STUB IGlobals_put_ReplaceMRUList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IGlobals_get_ReplaceMRUList_Proxy(
    IGlobals __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrMRUList);


void __RPC_STUB IGlobals_get_ReplaceMRUList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_RegisterLanguage_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ BSTR strName,
    /* [in] */ ILanguage __RPC_FAR *LanguageDef);


void __RPC_STUB IGlobals_RegisterLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_UnregisterLanguage_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ BSTR strName);


void __RPC_STUB IGlobals_UnregisterLanguage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_GetHotKeys_Proxy(
    IGlobals __RPC_FAR * This,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *HotKeysBuff);


void __RPC_STUB IGlobals_GetHotKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_SetHotKeys_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *HotKeysBuff);


void __RPC_STUB IGlobals_SetHotKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_ResetDefaultHotKeys_Proxy(
    IGlobals __RPC_FAR * This);


void __RPC_STUB IGlobals_ResetDefaultHotKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_GetHotKeyForCmd_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ cmCommand lCmd,
    /* [in] */ long lNum,
    /* [retval][out] */ IHotKey __RPC_FAR *__RPC_FAR *ppHotKey);


void __RPC_STUB IGlobals_GetHotKeyForCmd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_GetNumHotKeysForCmd_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ cmCommand lCmd,
    /* [retval][out] */ long __RPC_FAR *plCount);


void __RPC_STUB IGlobals_GetNumHotKeysForCmd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_RegisterHotKey_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ IHotKey __RPC_FAR *pHotKey,
    /* [in] */ cmCommand lCmd);


void __RPC_STUB IGlobals_RegisterHotKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_UnregisterHotKey_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ IHotKey __RPC_FAR *pHotKey);


void __RPC_STUB IGlobals_UnregisterHotKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_GetCommandString_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ cmCommand lCmd,
    /* [in] */ VARIANT_BOOL bDescription,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB IGlobals_GetCommandString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_GetMacro_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ long lMacro,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *MacroBuff);


void __RPC_STUB IGlobals_GetMacro_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_SetMacro_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ long lMacro,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *MacroBuff);


void __RPC_STUB IGlobals_SetMacro_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_RegisterCommand_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ long lCmdId,
    /* [in] */ BSTR strName,
    /* [in] */ BSTR strDescription);


void __RPC_STUB IGlobals_RegisterCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_UnregisterCommand_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ long lCmdId);


void __RPC_STUB IGlobals_UnregisterCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_GetLanguageDef_Proxy(
    IGlobals __RPC_FAR * This,
    /* [in] */ BSTR strName,
    /* [retval][out] */ ILanguage __RPC_FAR *__RPC_FAR *Language);


void __RPC_STUB IGlobals_GetLanguageDef_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IGlobals_UnregisterAllLanguages_Proxy(
    IGlobals __RPC_FAR * This);


void __RPC_STUB IGlobals_UnregisterAllLanguages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __IGlobals_INTERFACE_DEFINED__ */


#ifndef __IRect_INTERFACE_DEFINED__
#define __IRect_INTERFACE_DEFINED__

/* interface IRect */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_IRect;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3EFDC073-CA43-4781-9DA9-91328099A120")
    IRect : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_left(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_left(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_top(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_top(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_right(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_right(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_bottom(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_bottom(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

    };

#else    /* C style interface */

    typedef struct IRectVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            IRect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            IRect __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            IRect __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            IRect __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            IRect __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            IRect __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            IRect __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_left )(
            IRect __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_left )(
            IRect __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_top )(
            IRect __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_top )(
            IRect __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_right )(
            IRect __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_right )(
            IRect __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_bottom )(
            IRect __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_bottom )(
            IRect __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        END_INTERFACE
    } IRectVtbl;

    interface IRect
    {
        CONST_VTBL struct IRectVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define IRect_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRect_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IRect_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define IRect_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRect_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRect_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRect_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRect_put_left(This,lVal)   \
    (This)->lpVtbl -> put_left(This,lVal)

#define IRect_get_left(This,plVal)   \
    (This)->lpVtbl -> get_left(This,plVal)

#define IRect_put_top(This,lVal)   \
    (This)->lpVtbl -> put_top(This,lVal)

#define IRect_get_top(This,plVal)   \
    (This)->lpVtbl -> get_top(This,plVal)

#define IRect_put_right(This,lVal)   \
    (This)->lpVtbl -> put_right(This,lVal)

#define IRect_get_right(This,plVal)   \
    (This)->lpVtbl -> get_right(This,plVal)

#define IRect_put_bottom(This,lVal)   \
    (This)->lpVtbl -> put_bottom(This,lVal)

#define IRect_get_bottom(This,plVal)   \
    (This)->lpVtbl -> get_bottom(This,plVal)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRect_put_left_Proxy(
    IRect __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRect_put_left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRect_get_left_Proxy(
    IRect __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRect_get_left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRect_put_top_Proxy(
    IRect __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRect_put_top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRect_get_top_Proxy(
    IRect __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRect_get_top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRect_put_right_Proxy(
    IRect __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRect_put_right_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRect_get_right_Proxy(
    IRect __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRect_get_right_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE IRect_put_bottom_Proxy(
    IRect __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB IRect_put_bottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE IRect_get_bottom_Proxy(
    IRect __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB IRect_get_bottom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __IRect_INTERFACE_DEFINED__ */


#ifndef __ICodeList_INTERFACE_DEFINED__
#define __ICodeList_INTERFACE_DEFINED__

/* interface ICodeList */
/* [nonextensible][unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_ICodeList;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BA4A1EBD-AF16-407E-93A8-C440ABDAC9B5")
    ICodeList : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_hWnd(
            /* [retval][out] */ long __RPC_FAR *hwnd) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_hImageList(
            /* [in] */ long hImageList) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_hImageList(
            /* [retval][out] */ long __RPC_FAR *hImageList) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelectedItem(
            /* [retval][out] */ long __RPC_FAR *lIndex) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SelectedItem(
            /* [in] */ long ItemIndex) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE AddItem(
            /* [in] */ BSTR strAdd,
            /* [optional][in] */ VARIANT ImageIndex,
            /* [optional][in] */ VARIANT ItemData,
            /* [retval][out] */ long __RPC_FAR *lIndex) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetItemText(
            /* [in] */ long ItemIndex,
            /* [retval][out] */ BSTR __RPC_FAR *strVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetItemData(
            /* [in] */ long ItemIndex,
            /* [retval][out] */ long __RPC_FAR *lParam) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE FindString(
            /* [in] */ BSTR strFind,
            /* [optional][in] */ VARIANT bAcceptPartial,
            /* [retval][out] */ long __RPC_FAR *lItemIndex) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Destroy( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE DeleteItem(
            /* [in] */ long ItemIndex) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ItemCount(
            /* [retval][out] */ long __RPC_FAR *lItems) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE EnableHotTracking(
            /* [optional][in] */ VARIANT bEnable) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SortStyle(
            /* [retval][out] */ cmCodeListSortStyle __RPC_FAR *SortStyle) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SortStyle(
            /* [in] */ cmCodeListSortStyle SortStyle) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE InsertItem(
            /* [in] */ long lIndex,
            /* [in] */ BSTR strAdd,
            /* [optional][in] */ VARIANT ImageIndex,
            /* [optional][in] */ VARIANT ItemData,
            /* [retval][out] */ long __RPC_FAR *plIndex) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextColor(
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TextColor(
            /* [in] */ OLE_COLOR crColor) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor(
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor(
            /* [in] */ OLE_COLOR crColor) = 0;

        virtual /* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Font(
            /* [in] */ IFontDisp __RPC_FAR *pFont) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Font(
            /* [in] */ IFontDisp __RPC_FAR *pFont) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Font(
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont) = 0;

    };

#else    /* C style interface */

    typedef struct ICodeListVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ICodeList __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ICodeList __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ICodeList __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ICodeList __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ICodeList __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ICodeList __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ICodeList __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *hwnd);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hImageList )(
            ICodeList __RPC_FAR * This,
            /* [in] */ long hImageList);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hImageList )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *hImageList);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelectedItem )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lIndex);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelectedItem )(
            ICodeList __RPC_FAR * This,
            /* [in] */ long ItemIndex);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddItem )(
            ICodeList __RPC_FAR * This,
            /* [in] */ BSTR strAdd,
            /* [optional][in] */ VARIANT ImageIndex,
            /* [optional][in] */ VARIANT ItemData,
            /* [retval][out] */ long __RPC_FAR *lIndex);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemText )(
            ICodeList __RPC_FAR * This,
            /* [in] */ long ItemIndex,
            /* [retval][out] */ BSTR __RPC_FAR *strVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemData )(
            ICodeList __RPC_FAR * This,
            /* [in] */ long ItemIndex,
            /* [retval][out] */ long __RPC_FAR *lParam);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindString )(
            ICodeList __RPC_FAR * This,
            /* [in] */ BSTR strFind,
            /* [optional][in] */ VARIANT bAcceptPartial,
            /* [retval][out] */ long __RPC_FAR *lItemIndex);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Destroy )(
            ICodeList __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteItem )(
            ICodeList __RPC_FAR * This,
            /* [in] */ long ItemIndex);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ItemCount )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lItems);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnableHotTracking )(
            ICodeList __RPC_FAR * This,
            /* [optional][in] */ VARIANT bEnable);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SortStyle )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ cmCodeListSortStyle __RPC_FAR *SortStyle);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SortStyle )(
            ICodeList __RPC_FAR * This,
            /* [in] */ cmCodeListSortStyle SortStyle);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertItem )(
            ICodeList __RPC_FAR * This,
            /* [in] */ long lIndex,
            /* [in] */ BSTR strAdd,
            /* [optional][in] */ VARIANT ImageIndex,
            /* [optional][in] */ VARIANT ItemData,
            /* [retval][out] */ long __RPC_FAR *plIndex);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextColor )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TextColor )(
            ICodeList __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BackColor )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BackColor )(
            ICodeList __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Font )(
            ICodeList __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Font )(
            ICodeList __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Font )(
            ICodeList __RPC_FAR * This,
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

        END_INTERFACE
    } ICodeListVtbl;

    interface ICodeList
    {
        CONST_VTBL struct ICodeListVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ICodeList_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeList_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ICodeList_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ICodeList_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeList_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeList_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeList_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeList_get_hWnd(This,hwnd)   \
    (This)->lpVtbl -> get_hWnd(This,hwnd)

#define ICodeList_put_hImageList(This,hImageList)   \
    (This)->lpVtbl -> put_hImageList(This,hImageList)

#define ICodeList_get_hImageList(This,hImageList)   \
    (This)->lpVtbl -> get_hImageList(This,hImageList)

#define ICodeList_get_SelectedItem(This,lIndex)   \
    (This)->lpVtbl -> get_SelectedItem(This,lIndex)

#define ICodeList_put_SelectedItem(This,ItemIndex)   \
    (This)->lpVtbl -> put_SelectedItem(This,ItemIndex)

#define ICodeList_AddItem(This,strAdd,ImageIndex,ItemData,lIndex)   \
    (This)->lpVtbl -> AddItem(This,strAdd,ImageIndex,ItemData,lIndex)

#define ICodeList_GetItemText(This,ItemIndex,strVal)   \
    (This)->lpVtbl -> GetItemText(This,ItemIndex,strVal)

#define ICodeList_GetItemData(This,ItemIndex,lParam)   \
    (This)->lpVtbl -> GetItemData(This,ItemIndex,lParam)

#define ICodeList_FindString(This,strFind,bAcceptPartial,lItemIndex)   \
    (This)->lpVtbl -> FindString(This,strFind,bAcceptPartial,lItemIndex)

#define ICodeList_Destroy(This)   \
    (This)->lpVtbl -> Destroy(This)

#define ICodeList_DeleteItem(This,ItemIndex)   \
    (This)->lpVtbl -> DeleteItem(This,ItemIndex)

#define ICodeList_get_ItemCount(This,lItems)   \
    (This)->lpVtbl -> get_ItemCount(This,lItems)

#define ICodeList_EnableHotTracking(This,bEnable)   \
    (This)->lpVtbl -> EnableHotTracking(This,bEnable)

#define ICodeList_get_SortStyle(This,SortStyle)   \
    (This)->lpVtbl -> get_SortStyle(This,SortStyle)

#define ICodeList_put_SortStyle(This,SortStyle)   \
    (This)->lpVtbl -> put_SortStyle(This,SortStyle)

#define ICodeList_InsertItem(This,lIndex,strAdd,ImageIndex,ItemData,plIndex)   \
    (This)->lpVtbl -> InsertItem(This,lIndex,strAdd,ImageIndex,ItemData,plIndex)

#define ICodeList_get_TextColor(This,pcrColor)   \
    (This)->lpVtbl -> get_TextColor(This,pcrColor)

#define ICodeList_put_TextColor(This,crColor)   \
    (This)->lpVtbl -> put_TextColor(This,crColor)

#define ICodeList_get_BackColor(This,pcrColor)   \
    (This)->lpVtbl -> get_BackColor(This,pcrColor)

#define ICodeList_put_BackColor(This,crColor)   \
    (This)->lpVtbl -> put_BackColor(This,crColor)

#define ICodeList_putref_Font(This,pFont)   \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ICodeList_put_Font(This,pFont)   \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ICodeList_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_hWnd_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *hwnd);


void __RPC_STUB ICodeList_get_hWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeList_put_hImageList_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ long hImageList);


void __RPC_STUB ICodeList_put_hImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_hImageList_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *hImageList);


void __RPC_STUB ICodeList_get_hImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_SelectedItem_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lIndex);


void __RPC_STUB ICodeList_get_SelectedItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeList_put_SelectedItem_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ long ItemIndex);


void __RPC_STUB ICodeList_put_SelectedItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_AddItem_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ BSTR strAdd,
    /* [optional][in] */ VARIANT ImageIndex,
    /* [optional][in] */ VARIANT ItemData,
    /* [retval][out] */ long __RPC_FAR *lIndex);


void __RPC_STUB ICodeList_AddItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_GetItemText_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ long ItemIndex,
    /* [retval][out] */ BSTR __RPC_FAR *strVal);


void __RPC_STUB ICodeList_GetItemText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_GetItemData_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ long ItemIndex,
    /* [retval][out] */ long __RPC_FAR *lParam);


void __RPC_STUB ICodeList_GetItemData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_FindString_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ BSTR strFind,
    /* [optional][in] */ VARIANT bAcceptPartial,
    /* [retval][out] */ long __RPC_FAR *lItemIndex);


void __RPC_STUB ICodeList_FindString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_Destroy_Proxy(
    ICodeList __RPC_FAR * This);


void __RPC_STUB ICodeList_Destroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_DeleteItem_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ long ItemIndex);


void __RPC_STUB ICodeList_DeleteItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_ItemCount_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lItems);


void __RPC_STUB ICodeList_get_ItemCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_EnableHotTracking_Proxy(
    ICodeList __RPC_FAR * This,
    /* [optional][in] */ VARIANT bEnable);


void __RPC_STUB ICodeList_EnableHotTracking_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_SortStyle_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ cmCodeListSortStyle __RPC_FAR *SortStyle);


void __RPC_STUB ICodeList_get_SortStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeList_put_SortStyle_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ cmCodeListSortStyle SortStyle);


void __RPC_STUB ICodeList_put_SortStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeList_InsertItem_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ long lIndex,
    /* [in] */ BSTR strAdd,
    /* [optional][in] */ VARIANT ImageIndex,
    /* [optional][in] */ VARIANT ItemData,
    /* [retval][out] */ long __RPC_FAR *plIndex);


void __RPC_STUB ICodeList_InsertItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_TextColor_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);


void __RPC_STUB ICodeList_get_TextColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeList_put_TextColor_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ OLE_COLOR crColor);


void __RPC_STUB ICodeList_put_TextColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_BackColor_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);


void __RPC_STUB ICodeList_get_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeList_put_BackColor_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ OLE_COLOR crColor);


void __RPC_STUB ICodeList_put_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE ICodeList_putref_Font_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ICodeList_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeList_put_Font_Proxy(
    ICodeList __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ICodeList_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeList_get_Font_Proxy(
    ICodeList __RPC_FAR * This,
    /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);


void __RPC_STUB ICodeList_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ICodeList_INTERFACE_DEFINED__ */


#ifndef __ICodeTip_INTERFACE_DEFINED__
#define __ICodeTip_INTERFACE_DEFINED__

/* interface ICodeTip */
/* [nonextensible][unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_ICodeTip;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("20CCBF7B-FC8D-42E7-9F22-99100C76AA7E")
    ICodeTip : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_hWnd(
            /* [retval][out] */ long __RPC_FAR *phwnd) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TipText(
            /* [retval][out] */ BSTR __RPC_FAR *Text) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TipText(
            /* [in] */ BSTR Text) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TipTextLength(
            /* [retval][out] */ long __RPC_FAR *Length) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Destroy( void) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextColor(
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TextColor(
            /* [in] */ OLE_COLOR crColor) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor(
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor(
            /* [in] */ OLE_COLOR crColor) = 0;

        virtual /* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Font(
            /* [in] */ IFontDisp __RPC_FAR *pFont) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Font(
            /* [in] */ IFontDisp __RPC_FAR *pFont) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Font(
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont) = 0;

    };

#else    /* C style interface */

    typedef struct ICodeTipVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ICodeTip __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ICodeTip __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ICodeTip __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )(
            ICodeTip __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipText )(
            ICodeTip __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *Text);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TipText )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ BSTR Text);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipTextLength )(
            ICodeTip __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *Length);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Destroy )(
            ICodeTip __RPC_FAR * This);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextColor )(
            ICodeTip __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TextColor )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BackColor )(
            ICodeTip __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BackColor )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Font )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Font )(
            ICodeTip __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Font )(
            ICodeTip __RPC_FAR * This,
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

        END_INTERFACE
    } ICodeTipVtbl;

    interface ICodeTip
    {
        CONST_VTBL struct ICodeTipVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ICodeTip_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeTip_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ICodeTip_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ICodeTip_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeTip_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeTip_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeTip_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeTip_get_hWnd(This,phwnd)   \
    (This)->lpVtbl -> get_hWnd(This,phwnd)

#define ICodeTip_get_TipText(This,Text)   \
    (This)->lpVtbl -> get_TipText(This,Text)

#define ICodeTip_put_TipText(This,Text)   \
    (This)->lpVtbl -> put_TipText(This,Text)

#define ICodeTip_get_TipTextLength(This,Length)   \
    (This)->lpVtbl -> get_TipTextLength(This,Length)

#define ICodeTip_Destroy(This)   \
    (This)->lpVtbl -> Destroy(This)

#define ICodeTip_get_TextColor(This,pcrColor)   \
    (This)->lpVtbl -> get_TextColor(This,pcrColor)

#define ICodeTip_put_TextColor(This,crColor)   \
    (This)->lpVtbl -> put_TextColor(This,crColor)

#define ICodeTip_get_BackColor(This,pcrColor)   \
    (This)->lpVtbl -> get_BackColor(This,pcrColor)

#define ICodeTip_put_BackColor(This,crColor)   \
    (This)->lpVtbl -> put_BackColor(This,crColor)

#define ICodeTip_putref_Font(This,pFont)   \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ICodeTip_put_Font(This,pFont)   \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ICodeTip_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTip_get_hWnd_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *phwnd);


void __RPC_STUB ICodeTip_get_hWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTip_get_TipText_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *Text);


void __RPC_STUB ICodeTip_get_TipText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTip_put_TipText_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [in] */ BSTR Text);


void __RPC_STUB ICodeTip_put_TipText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTip_get_TipTextLength_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *Length);


void __RPC_STUB ICodeTip_get_TipTextLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeTip_Destroy_Proxy(
    ICodeTip __RPC_FAR * This);


void __RPC_STUB ICodeTip_Destroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTip_get_TextColor_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);


void __RPC_STUB ICodeTip_get_TextColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTip_put_TextColor_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [in] */ OLE_COLOR crColor);


void __RPC_STUB ICodeTip_put_TextColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTip_get_BackColor_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);


void __RPC_STUB ICodeTip_get_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTip_put_BackColor_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [in] */ OLE_COLOR crColor);


void __RPC_STUB ICodeTip_put_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE ICodeTip_putref_Font_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ICodeTip_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTip_put_Font_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ICodeTip_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTip_get_Font_Proxy(
    ICodeTip __RPC_FAR * This,
    /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);


void __RPC_STUB ICodeTip_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ICodeTip_INTERFACE_DEFINED__ */


#ifndef __ICodeTipHighlight_INTERFACE_DEFINED__
#define __ICodeTipHighlight_INTERFACE_DEFINED__

/* interface ICodeTipHighlight */
/* [nonextensible][dual][unique][helpstring][helpcontext][uuid][object] */


EXTERN_C const IID IID_ICodeTipHighlight;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1523D89A-5D87-479C-914E-A3070F37441B")
    ICodeTipHighlight : public ICodeTip
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HighlightStartPos(
            /* [retval][out] */ long __RPC_FAR *lPos) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_HighlightStartPos(
            /* [in] */ long lPos) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HighlightEndPos(
            /* [retval][out] */ long __RPC_FAR *lPos) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_HighlightEndPos(
            /* [in] */ long lPos) = 0;

    };

#else    /* C style interface */

    typedef struct ICodeTipHighlightVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ICodeTipHighlight __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ICodeTipHighlight __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipText )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *Text);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TipText )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ BSTR Text);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipTextLength )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *Length);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Destroy )(
            ICodeTipHighlight __RPC_FAR * This);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextColor )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TextColor )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BackColor )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BackColor )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Font )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Font )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Font )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HighlightStartPos )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lPos);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HighlightStartPos )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ long lPos);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HighlightEndPos )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lPos);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HighlightEndPos )(
            ICodeTipHighlight __RPC_FAR * This,
            /* [in] */ long lPos);

        END_INTERFACE
    } ICodeTipHighlightVtbl;

    interface ICodeTipHighlight
    {
        CONST_VTBL struct ICodeTipHighlightVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ICodeTipHighlight_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeTipHighlight_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ICodeTipHighlight_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ICodeTipHighlight_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeTipHighlight_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeTipHighlight_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeTipHighlight_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeTipHighlight_get_hWnd(This,phwnd)   \
    (This)->lpVtbl -> get_hWnd(This,phwnd)

#define ICodeTipHighlight_get_TipText(This,Text)   \
    (This)->lpVtbl -> get_TipText(This,Text)

#define ICodeTipHighlight_put_TipText(This,Text)   \
    (This)->lpVtbl -> put_TipText(This,Text)

#define ICodeTipHighlight_get_TipTextLength(This,Length)   \
    (This)->lpVtbl -> get_TipTextLength(This,Length)

#define ICodeTipHighlight_Destroy(This)   \
    (This)->lpVtbl -> Destroy(This)

#define ICodeTipHighlight_get_TextColor(This,pcrColor)   \
    (This)->lpVtbl -> get_TextColor(This,pcrColor)

#define ICodeTipHighlight_put_TextColor(This,crColor)   \
    (This)->lpVtbl -> put_TextColor(This,crColor)

#define ICodeTipHighlight_get_BackColor(This,pcrColor)   \
    (This)->lpVtbl -> get_BackColor(This,pcrColor)

#define ICodeTipHighlight_put_BackColor(This,crColor)   \
    (This)->lpVtbl -> put_BackColor(This,crColor)

#define ICodeTipHighlight_putref_Font(This,pFont)   \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ICodeTipHighlight_put_Font(This,pFont)   \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ICodeTipHighlight_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)


#define ICodeTipHighlight_get_HighlightStartPos(This,lPos)   \
    (This)->lpVtbl -> get_HighlightStartPos(This,lPos)

#define ICodeTipHighlight_put_HighlightStartPos(This,lPos)   \
    (This)->lpVtbl -> put_HighlightStartPos(This,lPos)

#define ICodeTipHighlight_get_HighlightEndPos(This,lPos)   \
    (This)->lpVtbl -> get_HighlightEndPos(This,lPos)

#define ICodeTipHighlight_put_HighlightEndPos(This,lPos)   \
    (This)->lpVtbl -> put_HighlightEndPos(This,lPos)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTipHighlight_get_HighlightStartPos_Proxy(
    ICodeTipHighlight __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lPos);


void __RPC_STUB ICodeTipHighlight_get_HighlightStartPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTipHighlight_put_HighlightStartPos_Proxy(
    ICodeTipHighlight __RPC_FAR * This,
    /* [in] */ long lPos);


void __RPC_STUB ICodeTipHighlight_put_HighlightStartPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTipHighlight_get_HighlightEndPos_Proxy(
    ICodeTipHighlight __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lPos);


void __RPC_STUB ICodeTipHighlight_get_HighlightEndPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTipHighlight_put_HighlightEndPos_Proxy(
    ICodeTipHighlight __RPC_FAR * This,
    /* [in] */ long lPos);


void __RPC_STUB ICodeTipHighlight_put_HighlightEndPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ICodeTipHighlight_INTERFACE_DEFINED__ */


#ifndef __ICodeTipFuncHighlight_INTERFACE_DEFINED__
#define __ICodeTipFuncHighlight_INTERFACE_DEFINED__

/* interface ICodeTipFuncHighlight */
/* [nonextensible][dual][unique][helpstring][helpcontext][uuid][object] */


EXTERN_C const IID IID_ICodeTipFuncHighlight;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("BD846BB3-25EA-4566-82D3-4C1F790D6747")
    ICodeTipFuncHighlight : public ICodeTip
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Argument(
            /* [retval][out] */ long __RPC_FAR *lArgument) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Argument(
            /* [in] */ long lArgument) = 0;

    };

#else    /* C style interface */

    typedef struct ICodeTipFuncHighlightVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ICodeTipFuncHighlight __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ICodeTipFuncHighlight __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipText )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *Text);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TipText )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ BSTR Text);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipTextLength )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *Length);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Destroy )(
            ICodeTipFuncHighlight __RPC_FAR * This);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextColor )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TextColor )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BackColor )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BackColor )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Font )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Font )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Font )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Argument )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lArgument);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Argument )(
            ICodeTipFuncHighlight __RPC_FAR * This,
            /* [in] */ long lArgument);

        END_INTERFACE
    } ICodeTipFuncHighlightVtbl;

    interface ICodeTipFuncHighlight
    {
        CONST_VTBL struct ICodeTipFuncHighlightVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ICodeTipFuncHighlight_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeTipFuncHighlight_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ICodeTipFuncHighlight_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ICodeTipFuncHighlight_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeTipFuncHighlight_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeTipFuncHighlight_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeTipFuncHighlight_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeTipFuncHighlight_get_hWnd(This,phwnd)   \
    (This)->lpVtbl -> get_hWnd(This,phwnd)

#define ICodeTipFuncHighlight_get_TipText(This,Text)   \
    (This)->lpVtbl -> get_TipText(This,Text)

#define ICodeTipFuncHighlight_put_TipText(This,Text)   \
    (This)->lpVtbl -> put_TipText(This,Text)

#define ICodeTipFuncHighlight_get_TipTextLength(This,Length)   \
    (This)->lpVtbl -> get_TipTextLength(This,Length)

#define ICodeTipFuncHighlight_Destroy(This)   \
    (This)->lpVtbl -> Destroy(This)

#define ICodeTipFuncHighlight_get_TextColor(This,pcrColor)   \
    (This)->lpVtbl -> get_TextColor(This,pcrColor)

#define ICodeTipFuncHighlight_put_TextColor(This,crColor)   \
    (This)->lpVtbl -> put_TextColor(This,crColor)

#define ICodeTipFuncHighlight_get_BackColor(This,pcrColor)   \
    (This)->lpVtbl -> get_BackColor(This,pcrColor)

#define ICodeTipFuncHighlight_put_BackColor(This,crColor)   \
    (This)->lpVtbl -> put_BackColor(This,crColor)

#define ICodeTipFuncHighlight_putref_Font(This,pFont)   \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ICodeTipFuncHighlight_put_Font(This,pFont)   \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ICodeTipFuncHighlight_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)


#define ICodeTipFuncHighlight_get_Argument(This,lArgument)   \
    (This)->lpVtbl -> get_Argument(This,lArgument)

#define ICodeTipFuncHighlight_put_Argument(This,lArgument)   \
    (This)->lpVtbl -> put_Argument(This,lArgument)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTipFuncHighlight_get_Argument_Proxy(
    ICodeTipFuncHighlight __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lArgument);


void __RPC_STUB ICodeTipFuncHighlight_get_Argument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTipFuncHighlight_put_Argument_Proxy(
    ICodeTipFuncHighlight __RPC_FAR * This,
    /* [in] */ long lArgument);


void __RPC_STUB ICodeTipFuncHighlight_put_Argument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ICodeTipFuncHighlight_INTERFACE_DEFINED__ */


#ifndef __ICodeTipMultiFunc_INTERFACE_DEFINED__
#define __ICodeTipMultiFunc_INTERFACE_DEFINED__

/* interface ICodeTipMultiFunc */
/* [nonextensible][dual][unique][helpstring][helpcontext][uuid][object] */


EXTERN_C const IID IID_ICodeTipMultiFunc;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9708C966-49FD-44C1-90C7-3615BADB1215")
    ICodeTipMultiFunc : public ICodeTipFuncHighlight
    {
    public:
        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_FunctionCount(
            /* [retval][out] */ long __RPC_FAR *lCount) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_FunctionCount(
            /* [in] */ long lCount) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentFunction(
            /* [retval][out] */ long __RPC_FAR *lIndex) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentFunction(
            /* [in] */ long lIndex) = 0;

    };

#else    /* C style interface */

    typedef struct ICodeTipMultiFuncVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ICodeTipMultiFunc __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ICodeTipMultiFunc __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipText )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *Text);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TipText )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ BSTR Text);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TipTextLength )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *Length);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Destroy )(
            ICodeTipMultiFunc __RPC_FAR * This);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextColor )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TextColor )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BackColor )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BackColor )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Font )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Font )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Font )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Argument )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lArgument);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Argument )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ long lArgument);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FunctionCount )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lCount);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FunctionCount )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ long lCount);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentFunction )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *lIndex);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CurrentFunction )(
            ICodeTipMultiFunc __RPC_FAR * This,
            /* [in] */ long lIndex);

        END_INTERFACE
    } ICodeTipMultiFuncVtbl;

    interface ICodeTipMultiFunc
    {
        CONST_VTBL struct ICodeTipMultiFuncVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ICodeTipMultiFunc_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeTipMultiFunc_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ICodeTipMultiFunc_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ICodeTipMultiFunc_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeTipMultiFunc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeTipMultiFunc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeTipMultiFunc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeTipMultiFunc_get_hWnd(This,phwnd)   \
    (This)->lpVtbl -> get_hWnd(This,phwnd)

#define ICodeTipMultiFunc_get_TipText(This,Text)   \
    (This)->lpVtbl -> get_TipText(This,Text)

#define ICodeTipMultiFunc_put_TipText(This,Text)   \
    (This)->lpVtbl -> put_TipText(This,Text)

#define ICodeTipMultiFunc_get_TipTextLength(This,Length)   \
    (This)->lpVtbl -> get_TipTextLength(This,Length)

#define ICodeTipMultiFunc_Destroy(This)   \
    (This)->lpVtbl -> Destroy(This)

#define ICodeTipMultiFunc_get_TextColor(This,pcrColor)   \
    (This)->lpVtbl -> get_TextColor(This,pcrColor)

#define ICodeTipMultiFunc_put_TextColor(This,crColor)   \
    (This)->lpVtbl -> put_TextColor(This,crColor)

#define ICodeTipMultiFunc_get_BackColor(This,pcrColor)   \
    (This)->lpVtbl -> get_BackColor(This,pcrColor)

#define ICodeTipMultiFunc_put_BackColor(This,crColor)   \
    (This)->lpVtbl -> put_BackColor(This,crColor)

#define ICodeTipMultiFunc_putref_Font(This,pFont)   \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ICodeTipMultiFunc_put_Font(This,pFont)   \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ICodeTipMultiFunc_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)


#define ICodeTipMultiFunc_get_Argument(This,lArgument)   \
    (This)->lpVtbl -> get_Argument(This,lArgument)

#define ICodeTipMultiFunc_put_Argument(This,lArgument)   \
    (This)->lpVtbl -> put_Argument(This,lArgument)


#define ICodeTipMultiFunc_get_FunctionCount(This,lCount)   \
    (This)->lpVtbl -> get_FunctionCount(This,lCount)

#define ICodeTipMultiFunc_put_FunctionCount(This,lCount)   \
    (This)->lpVtbl -> put_FunctionCount(This,lCount)

#define ICodeTipMultiFunc_get_CurrentFunction(This,lIndex)   \
    (This)->lpVtbl -> get_CurrentFunction(This,lIndex)

#define ICodeTipMultiFunc_put_CurrentFunction(This,lIndex)   \
    (This)->lpVtbl -> put_CurrentFunction(This,lIndex)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTipMultiFunc_get_FunctionCount_Proxy(
    ICodeTipMultiFunc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lCount);


void __RPC_STUB ICodeTipMultiFunc_get_FunctionCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTipMultiFunc_put_FunctionCount_Proxy(
    ICodeTipMultiFunc __RPC_FAR * This,
    /* [in] */ long lCount);


void __RPC_STUB ICodeTipMultiFunc_put_FunctionCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeTipMultiFunc_get_CurrentFunction_Proxy(
    ICodeTipMultiFunc __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *lIndex);


void __RPC_STUB ICodeTipMultiFunc_get_CurrentFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeTipMultiFunc_put_CurrentFunction_Proxy(
    ICodeTipMultiFunc __RPC_FAR * This,
    /* [in] */ long lIndex);


void __RPC_STUB ICodeTipMultiFunc_put_CurrentFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ICodeTipMultiFunc_INTERFACE_DEFINED__ */


#ifndef __ICodeSense_INTERFACE_DEFINED__
#define __ICodeSense_INTERFACE_DEFINED__

/* interface ICodeSense */
/* [unique][helpstring][helpcontext][dual][uuid][object] */


EXTERN_C const IID IID_ICodeSense;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("515DB298-D15B-44BA-91C7-0134233EC305")
    ICodeSense : public IDispatch
    {
    public:
        virtual /* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Font(
            /* [in] */ IFontDisp __RPC_FAR *pFont) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Font(
            /* [in] */ IFontDisp __RPC_FAR *pFont) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Font(
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Window(
            /* [retval][out] */ long __RPC_FAR *phwnd) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Text(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Text(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ColorSyntax(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColorSyntax(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayWhitespace(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayWhitespace(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExpandTabs(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExpandTabs(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SmoothScrolling(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SmoothScrolling(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ReadOnly(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReadOnly(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineToolTips(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineToolTips(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_DisplayLeftMargin(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_DisplayLeftMargin(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnableColumnSel(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnableColumnSel(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnableDragDrop(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnableDragDrop(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Overtype(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Overtype(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SearchCaseSensitive(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SearchCaseSensitive(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ReplacePreserveCase(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReplacePreserveCase(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SearchWholeWord(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SearchWholeWord(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Modified(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Modified(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnableCRLF(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnableCRLF(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_GlobalProps(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_GlobalProps(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_VScrollVisible(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VScrollVisible(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_HScrollVisible(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HScrollVisible(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnableVSplitter(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnableVSplitter(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_EnableHSplitter(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_EnableHSplitter(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_Language(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_Language(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TabSize(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TabSize(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_TopIndex(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TopIndex(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_UndoLimit(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_UndoLimit(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentView(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextLength(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextLengthLogical(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineCount(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentWord(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentWordLength(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentWordLengthLogical(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_AutoIndentMode(
            /* [in] */ cmAutoIndentMode lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_AutoIndentMode(
            /* [retval][out] */ cmAutoIndentMode __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ViewCount(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_VSplitterPos(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_VSplitterPos(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_HSplitterPos(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HSplitterPos(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_FindText(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_FindText(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ReplText(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReplText(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineNumbering(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineNumbering(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineNumberStart(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineNumberStart(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineNumberStyle(
            /* [in] */ cmLineNumStyle lStyle) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineNumberStyle(
            /* [retval][out] */ cmLineNumStyle __RPC_FAR *plStyle) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetWord(
            /* [in] */ IPosition __RPC_FAR *Pos,
            /* [retval][out] */ BSTR __RPC_FAR *pstrWord) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetWordLength(
            /* [in] */ IPosition __RPC_FAR *Pos,
            /* [in] */ VARIANT_BOOL bLogical,
            /* [retval][out] */ long __RPC_FAR *plLen) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetTopIndex(
            /* [in] */ long lView,
            /* [retval][out] */ long __RPC_FAR *plLine) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetTopIndex(
            /* [in] */ long lView,
            /* [in] */ long lLine) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetSel(
            /* [in] */ VARIANT_BOOL bNormalized,
            /* [retval][out] */ IRange __RPC_FAR *__RPC_FAR *Sel) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetSel(
            /* [in] */ IRange __RPC_FAR *Sel,
            /* [in] */ VARIANT_BOOL bMakeVisible) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetSelFromPoint(
            /* [in] */ long lxClientPos,
            /* [in] */ long lyClientPos,
            /* [retval][out] */ IPosition __RPC_FAR *__RPC_FAR *Pos) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetVisibleLineCount(
            /* [in] */ long lView,
            /* [in] */ VARIANT_BOOL bFullyVisible,
            /* [retval][out] */ long __RPC_FAR *plCount) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetLine(
            /* [in] */ long lLine,
            /* [retval][out] */ BSTR __RPC_FAR *pstrLine) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetLineLength(
            /* [in] */ long lLine,
            /* [retval][out] */ long __RPC_FAR *plLen) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetColor(
            /* [in] */ cmColorItem lColorItem,
            /* [in] */ OLE_COLOR crColor) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetColor(
            /* [in] */ cmColorItem lColorItem,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE OpenFile(
            /* [in] */ BSTR strFileName) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SaveFile(
            /* [in] */ BSTR strFileName,
            /* [in] */ VARIANT_BOOL bClearUndo) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE InsertFile(
            /* [in] */ BSTR strFileName,
            /* [in] */ IPosition __RPC_FAR *InsertPos) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE InsertText(
            /* [in] */ BSTR strText,
            /* [in] */ IPosition __RPC_FAR *InsertPos) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ReplaceText(
            /* [in] */ BSTR strText,
            /* [in] */ IRange __RPC_FAR *Range) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE HitTest(
            /* [in] */ long lxClientPos,
            /* [in] */ long lyClientPos,
            /* [out] */ long __RPC_FAR *plView,
            /* [retval][out] */ cmHitTestCode __RPC_FAR *plHitTestCode) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE AddText(
            /* [in] */ BSTR strText) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE DeleteLine(
            /* [in] */ long lLine) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE InsertLine(
            /* [in] */ long lLine,
            /* [in] */ BSTR strLine) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SelectLine(
            /* [in] */ long lLine,
            /* [in] */ VARIANT_BOOL bMakeVisible) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE DeleteSel( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ReplaceSel(
            /* [in] */ BSTR strText) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ExecuteCmd(
            /* [in] */ cmCommand lCommand,
            /* [optional][in] */ VARIANT CmdData) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CanUndo(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CanRedo(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CanCut(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CanCopy(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE CanPaste(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Undo( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Redo( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Cut( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Copy( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE Paste( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ClearUndoBuffer( void) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IsRecordingMacro(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE IsPlayingMacro(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetDlgParent(
            /* [in] */ long lhWnd) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelText(
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SelText(
            /* [in] */ BSTR strVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SelBounds(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelBounds(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetFontStyle(
            /* [in] */ cmFontStyleItem lFontStyleItem,
            /* [in] */ cmFontStyle byFontStyle) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetFontStyle(
            /* [in] */ cmFontStyleItem lFontStyleItem,
            /* [retval][out] */ cmFontStyle __RPC_FAR *pbyFontStyle) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_SearchRegExp(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SearchRegExp(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetItemData(
            /* [in] */ long lLine,
            /* [in] */ long lData) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetItemData(
            /* [in] */ long lLine,
            /* [retval][out] */ long __RPC_FAR *plData) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetLineStyle(
            /* [in] */ long lLine,
            /* [in] */ long lStyle) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetLineStyle(
            /* [in] */ long lLine,
            /* [retval][out] */ long __RPC_FAR *plStyle) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetBookmark(
            /* [in] */ long lLine,
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetBookmark(
            /* [in] */ long lLine,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetAllBookmarks(
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Lines) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetAllBookmarks(
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Lines) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE PosFromChar(
            /* [in] */ IPosition __RPC_FAR *pPos,
            /* [retval][out] */ IRect __RPC_FAR *__RPC_FAR *Rect) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_HideSel(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HideSel(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_HighlightedLine(
            /* [in] */ long lLine) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_HighlightedLine(
            /* [retval][out] */ long __RPC_FAR *plLine) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_NormalizeCase(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_NormalizeCase(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetDivider(
            /* [in] */ long lLine,
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetDivider(
            /* [in] */ long lLine,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_OvertypeCaret(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_OvertypeCaret(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetMarginImages(
            /* [in] */ long lLine,
            /* [in] */ BYTE byImages) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetMarginImages(
            /* [in] */ long lLine,
            /* [retval][out] */ BYTE __RPC_FAR *pbyImages) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_hImageList(
            /* [in] */ OLE_HANDLE hImageList) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_hImageList(
            /* [retval][out] */ OLE_HANDLE __RPC_FAR *phImageList) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_ImageList(
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppImageList) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_ImageList(
            /* [in] */ IDispatch __RPC_FAR *pImageList) = 0;

        virtual /* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE putref_ImageList(
            /* [in] */ IDispatch __RPC_FAR *pImageList) = 0;

        virtual /* [helpcontext][helpstring][id][hidden] */ HRESULT STDMETHODCALLTYPE AboutBox( void) = 0;

        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE PrintContents(
            long hDC,
            long lFlags) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE SetCaretPos(
            /* [in] */ long lLine,
            /* [in] */ long lCol) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ViewColToBufferCol(
            /* [in] */ long lLine,
            /* [in] */ long lViewCol,
            /* [retval][out] */ long __RPC_FAR *plBuffCol) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE BufferColToViewCol(
            /* [in] */ long lLine,
            /* [in] */ long lBuffCol,
            /* [retval][out] */ long __RPC_FAR *plViewCol) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelLength(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_SelLengthLogical(
            /* [retval][out] */ long __RPC_FAR *plVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_BorderStyle(
            /* [in] */ cmBorderStyle lBorderStyle) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_BorderStyle(
            /* [retval][out] */ cmBorderStyle __RPC_FAR *plBorderStyle) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_hWnd(
            /* [retval][out] */ long __RPC_FAR *phwnd) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurrentView(
            /* [in] */ long lVal) = 0;

        virtual /* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE put_StealKeys(
            /* [in] */ VARIANT_BOOL bVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_StealKeys(
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal) = 0;

        virtual /* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentToken(
            /* [retval][out] */ cmTokenType __RPC_FAR *lpTokenType) = 0;

        virtual /* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE UpdateControlPositions( void) = 0;

    };

#else    /* C style interface */

    typedef struct ICodeSenseVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )(
            ICodeSense __RPC_FAR * This);

        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )(
            ICodeSense __RPC_FAR * This);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )(
            ICodeSense __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Font )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Font )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IFontDisp __RPC_FAR *pFont);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Font )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Enabled )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Enabled )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Window )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Text )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Text )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ColorSyntax )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ColorSyntax )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DisplayWhitespace )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DisplayWhitespace )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExpandTabs )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExpandTabs )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SmoothScrolling )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SmoothScrolling )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ReadOnly )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ReadOnly )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LineToolTips )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineToolTips )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DisplayLeftMargin )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DisplayLeftMargin )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EnableColumnSel )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnableColumnSel )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EnableDragDrop )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnableDragDrop )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Overtype )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Overtype )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SearchCaseSensitive )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SearchCaseSensitive )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ReplacePreserveCase )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ReplacePreserveCase )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SearchWholeWord )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SearchWholeWord )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Modified )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Modified )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EnableCRLF )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnableCRLF )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GlobalProps )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GlobalProps )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VScrollVisible )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VScrollVisible )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HScrollVisible )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HScrollVisible )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EnableVSplitter )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnableVSplitter )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EnableHSplitter )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EnableHSplitter )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Language )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Language )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TabSize )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TabSize )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TopIndex )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TopIndex )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UndoLimit )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UndoLimit )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentView )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextLength )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TextLengthLogical )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineCount )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentWord )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentWordLength )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentWordLengthLogical )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AutoIndentMode )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmAutoIndentMode lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AutoIndentMode )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ cmAutoIndentMode __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ViewCount )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_VSplitterPos )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VSplitterPos )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HSplitterPos )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HSplitterPos )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FindText )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FindText )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ReplText )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ReplText )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LineNumbering )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineNumbering )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LineNumberStart )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineNumberStart )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LineNumberStyle )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmLineNumStyle lStyle);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LineNumberStyle )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ cmLineNumStyle __RPC_FAR *plStyle);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWord )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IPosition __RPC_FAR *Pos,
            /* [retval][out] */ BSTR __RPC_FAR *pstrWord);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetWordLength )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IPosition __RPC_FAR *Pos,
            /* [in] */ VARIANT_BOOL bLogical,
            /* [retval][out] */ long __RPC_FAR *plLen);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTopIndex )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lView,
            /* [retval][out] */ long __RPC_FAR *plLine);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetTopIndex )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lView,
            /* [in] */ long lLine);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSel )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bNormalized,
            /* [retval][out] */ IRange __RPC_FAR *__RPC_FAR *Sel);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSel )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IRange __RPC_FAR *Sel,
            /* [in] */ VARIANT_BOOL bMakeVisible);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelFromPoint )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lxClientPos,
            /* [in] */ long lyClientPos,
            /* [retval][out] */ IPosition __RPC_FAR *__RPC_FAR *Pos);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVisibleLineCount )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lView,
            /* [in] */ VARIANT_BOOL bFullyVisible,
            /* [retval][out] */ long __RPC_FAR *plCount);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLine )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ BSTR __RPC_FAR *pstrLine);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLineLength )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ long __RPC_FAR *plLen);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetColor )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmColorItem lColorItem,
            /* [in] */ OLE_COLOR crColor);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetColor )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmColorItem lColorItem,
            /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenFile )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strFileName);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveFile )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strFileName,
            /* [in] */ VARIANT_BOOL bClearUndo);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertFile )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strFileName,
            /* [in] */ IPosition __RPC_FAR *InsertPos);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertText )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strText,
            /* [in] */ IPosition __RPC_FAR *InsertPos);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReplaceText )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strText,
            /* [in] */ IRange __RPC_FAR *Range);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HitTest )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lxClientPos,
            /* [in] */ long lyClientPos,
            /* [out] */ long __RPC_FAR *plView,
            /* [retval][out] */ cmHitTestCode __RPC_FAR *plHitTestCode);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddText )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strText);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteLine )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InsertLine )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ BSTR strLine);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SelectLine )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ VARIANT_BOOL bMakeVisible);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteSel )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReplaceSel )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strText);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExecuteCmd )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmCommand lCommand,
            /* [optional][in] */ VARIANT CmdData);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanUndo )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanRedo )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanCut )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanCopy )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CanPaste )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Undo )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Redo )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Cut )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Copy )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Paste )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearUndoBuffer )(
            ICodeSense __RPC_FAR * This);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsRecordingMacro )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPlayingMacro )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDlgParent )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lhWnd);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelText )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pstrVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelText )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ BSTR strVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SelBounds )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelBounds )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetFontStyle )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmFontStyleItem lFontStyleItem,
            /* [in] */ cmFontStyle byFontStyle);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFontStyle )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmFontStyleItem lFontStyleItem,
            /* [retval][out] */ cmFontStyle __RPC_FAR *pbyFontStyle);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SearchRegExp )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SearchRegExp )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetItemData )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ long lData);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetItemData )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ long __RPC_FAR *plData);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetLineStyle )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ long lStyle);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetLineStyle )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ long __RPC_FAR *plStyle);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBookmark )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBookmark )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAllBookmarks )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Lines);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllBookmarks )(
            ICodeSense __RPC_FAR * This,
            /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Lines);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PosFromChar )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IPosition __RPC_FAR *pPos,
            /* [retval][out] */ IRect __RPC_FAR *__RPC_FAR *Rect);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HideSel )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HideSel )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HighlightedLine )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HighlightedLine )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plLine);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NormalizeCase )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NormalizeCase )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDivider )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDivider )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OvertypeCaret )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OvertypeCaret )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetMarginImages )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ BYTE byImages);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMarginImages )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [retval][out] */ BYTE __RPC_FAR *pbyImages);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_hImageList )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ OLE_HANDLE hImageList);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hImageList )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ OLE_HANDLE __RPC_FAR *phImageList);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ImageList )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppImageList);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ImageList )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pImageList);

        /* [helpstring][helpcontext][id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_ImageList )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pImageList);

        /* [helpcontext][helpstring][id][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AboutBox )(
            ICodeSense __RPC_FAR * This);

        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PrintContents )(
            ICodeSense __RPC_FAR * This,
            long hDC,
            long lFlags);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetCaretPos )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ long lCol);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ViewColToBufferCol )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ long lViewCol,
            /* [retval][out] */ long __RPC_FAR *plBuffCol);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BufferColToViewCol )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lLine,
            /* [in] */ long lBuffCol,
            /* [retval][out] */ long __RPC_FAR *plViewCol);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelLength )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SelLengthLogical )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *plVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BorderStyle )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ cmBorderStyle lBorderStyle);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BorderStyle )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ cmBorderStyle __RPC_FAR *plBorderStyle);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_hWnd )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *phwnd);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CurrentView )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ long lVal);

        /* [helpstring][helpcontext][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_StealKeys )(
            ICodeSense __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_StealKeys )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);

        /* [helpstring][helpcontext][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentToken )(
            ICodeSense __RPC_FAR * This,
            /* [retval][out] */ cmTokenType __RPC_FAR *lpTokenType);

        /* [helpstring][helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateControlPositions )(
            ICodeSense __RPC_FAR * This);

        END_INTERFACE
    } ICodeSenseVtbl;

    interface ICodeSense
    {
        CONST_VTBL struct ICodeSenseVtbl __RPC_FAR *lpVtbl;
    };



#ifdef COBJMACROS


#define ICodeSense_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICodeSense_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ICodeSense_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ICodeSense_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICodeSense_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICodeSense_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICodeSense_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICodeSense_putref_Font(This,pFont)   \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ICodeSense_put_Font(This,pFont)   \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ICodeSense_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)

#define ICodeSense_put_Enabled(This,bVal)   \
    (This)->lpVtbl -> put_Enabled(This,bVal)

#define ICodeSense_get_Enabled(This,pbVal)   \
    (This)->lpVtbl -> get_Enabled(This,pbVal)

#define ICodeSense_get_Window(This,phwnd)   \
    (This)->lpVtbl -> get_Window(This,phwnd)

#define ICodeSense_put_Text(This,strVal)   \
    (This)->lpVtbl -> put_Text(This,strVal)

#define ICodeSense_get_Text(This,pstrVal)   \
    (This)->lpVtbl -> get_Text(This,pstrVal)

#define ICodeSense_put_ColorSyntax(This,bVal)   \
    (This)->lpVtbl -> put_ColorSyntax(This,bVal)

#define ICodeSense_get_ColorSyntax(This,pbVal)   \
    (This)->lpVtbl -> get_ColorSyntax(This,pbVal)

#define ICodeSense_put_DisplayWhitespace(This,bVal)   \
    (This)->lpVtbl -> put_DisplayWhitespace(This,bVal)

#define ICodeSense_get_DisplayWhitespace(This,pbVal)   \
    (This)->lpVtbl -> get_DisplayWhitespace(This,pbVal)

#define ICodeSense_put_ExpandTabs(This,bVal)   \
    (This)->lpVtbl -> put_ExpandTabs(This,bVal)

#define ICodeSense_get_ExpandTabs(This,pbVal)   \
    (This)->lpVtbl -> get_ExpandTabs(This,pbVal)

#define ICodeSense_put_SmoothScrolling(This,bVal)   \
    (This)->lpVtbl -> put_SmoothScrolling(This,bVal)

#define ICodeSense_get_SmoothScrolling(This,pbVal)   \
    (This)->lpVtbl -> get_SmoothScrolling(This,pbVal)

#define ICodeSense_put_ReadOnly(This,bVal)   \
    (This)->lpVtbl -> put_ReadOnly(This,bVal)

#define ICodeSense_get_ReadOnly(This,pbVal)   \
    (This)->lpVtbl -> get_ReadOnly(This,pbVal)

#define ICodeSense_put_LineToolTips(This,bVal)   \
    (This)->lpVtbl -> put_LineToolTips(This,bVal)

#define ICodeSense_get_LineToolTips(This,pbVal)   \
    (This)->lpVtbl -> get_LineToolTips(This,pbVal)

#define ICodeSense_put_DisplayLeftMargin(This,bVal)   \
    (This)->lpVtbl -> put_DisplayLeftMargin(This,bVal)

#define ICodeSense_get_DisplayLeftMargin(This,pbVal)   \
    (This)->lpVtbl -> get_DisplayLeftMargin(This,pbVal)

#define ICodeSense_put_EnableColumnSel(This,bVal)   \
    (This)->lpVtbl -> put_EnableColumnSel(This,bVal)

#define ICodeSense_get_EnableColumnSel(This,pbVal)   \
    (This)->lpVtbl -> get_EnableColumnSel(This,pbVal)

#define ICodeSense_put_EnableDragDrop(This,bVal)   \
    (This)->lpVtbl -> put_EnableDragDrop(This,bVal)

#define ICodeSense_get_EnableDragDrop(This,pbVal)   \
    (This)->lpVtbl -> get_EnableDragDrop(This,pbVal)

#define ICodeSense_put_Overtype(This,bVal)   \
    (This)->lpVtbl -> put_Overtype(This,bVal)

#define ICodeSense_get_Overtype(This,pbVal)   \
    (This)->lpVtbl -> get_Overtype(This,pbVal)

#define ICodeSense_put_SearchCaseSensitive(This,bVal)   \
    (This)->lpVtbl -> put_SearchCaseSensitive(This,bVal)

#define ICodeSense_get_SearchCaseSensitive(This,pbVal)   \
    (This)->lpVtbl -> get_SearchCaseSensitive(This,pbVal)

#define ICodeSense_put_ReplacePreserveCase(This,bVal)   \
    (This)->lpVtbl -> put_ReplacePreserveCase(This,bVal)

#define ICodeSense_get_ReplacePreserveCase(This,pbVal)   \
    (This)->lpVtbl -> get_ReplacePreserveCase(This,pbVal)

#define ICodeSense_put_SearchWholeWord(This,bVal)   \
    (This)->lpVtbl -> put_SearchWholeWord(This,bVal)

#define ICodeSense_get_SearchWholeWord(This,pbVal)   \
    (This)->lpVtbl -> get_SearchWholeWord(This,pbVal)

#define ICodeSense_put_Modified(This,bVal)   \
    (This)->lpVtbl -> put_Modified(This,bVal)

#define ICodeSense_get_Modified(This,pbVal)   \
    (This)->lpVtbl -> get_Modified(This,pbVal)

#define ICodeSense_put_EnableCRLF(This,bVal)   \
    (This)->lpVtbl -> put_EnableCRLF(This,bVal)

#define ICodeSense_get_EnableCRLF(This,pbVal)   \
    (This)->lpVtbl -> get_EnableCRLF(This,pbVal)

#define ICodeSense_put_GlobalProps(This,bVal)   \
    (This)->lpVtbl -> put_GlobalProps(This,bVal)

#define ICodeSense_get_GlobalProps(This,pbVal)   \
    (This)->lpVtbl -> get_GlobalProps(This,pbVal)

#define ICodeSense_put_VScrollVisible(This,bVal)   \
    (This)->lpVtbl -> put_VScrollVisible(This,bVal)

#define ICodeSense_get_VScrollVisible(This,pbVal)   \
    (This)->lpVtbl -> get_VScrollVisible(This,pbVal)

#define ICodeSense_put_HScrollVisible(This,bVal)   \
    (This)->lpVtbl -> put_HScrollVisible(This,bVal)

#define ICodeSense_get_HScrollVisible(This,pbVal)   \
    (This)->lpVtbl -> get_HScrollVisible(This,pbVal)

#define ICodeSense_put_EnableVSplitter(This,bVal)   \
    (This)->lpVtbl -> put_EnableVSplitter(This,bVal)

#define ICodeSense_get_EnableVSplitter(This,pbVal)   \
    (This)->lpVtbl -> get_EnableVSplitter(This,pbVal)

#define ICodeSense_put_EnableHSplitter(This,bVal)   \
    (This)->lpVtbl -> put_EnableHSplitter(This,bVal)

#define ICodeSense_get_EnableHSplitter(This,pbVal)   \
    (This)->lpVtbl -> get_EnableHSplitter(This,pbVal)

#define ICodeSense_put_Language(This,strVal)   \
    (This)->lpVtbl -> put_Language(This,strVal)

#define ICodeSense_get_Language(This,pstrVal)   \
    (This)->lpVtbl -> get_Language(This,pstrVal)

#define ICodeSense_put_TabSize(This,lVal)   \
    (This)->lpVtbl -> put_TabSize(This,lVal)

#define ICodeSense_get_TabSize(This,plVal)   \
    (This)->lpVtbl -> get_TabSize(This,plVal)

#define ICodeSense_put_TopIndex(This,lVal)   \
    (This)->lpVtbl -> put_TopIndex(This,lVal)

#define ICodeSense_get_TopIndex(This,plVal)   \
    (This)->lpVtbl -> get_TopIndex(This,plVal)

#define ICodeSense_put_UndoLimit(This,lVal)   \
    (This)->lpVtbl -> put_UndoLimit(This,lVal)

#define ICodeSense_get_UndoLimit(This,plVal)   \
    (This)->lpVtbl -> get_UndoLimit(This,plVal)

#define ICodeSense_get_CurrentView(This,plVal)   \
    (This)->lpVtbl -> get_CurrentView(This,plVal)

#define ICodeSense_get_TextLength(This,plVal)   \
    (This)->lpVtbl -> get_TextLength(This,plVal)

#define ICodeSense_get_TextLengthLogical(This,plVal)   \
    (This)->lpVtbl -> get_TextLengthLogical(This,plVal)

#define ICodeSense_get_LineCount(This,plVal)   \
    (This)->lpVtbl -> get_LineCount(This,plVal)

#define ICodeSense_get_CurrentWord(This,pstrVal)   \
    (This)->lpVtbl -> get_CurrentWord(This,pstrVal)

#define ICodeSense_get_CurrentWordLength(This,plVal)   \
    (This)->lpVtbl -> get_CurrentWordLength(This,plVal)

#define ICodeSense_get_CurrentWordLengthLogical(This,plVal)   \
    (This)->lpVtbl -> get_CurrentWordLengthLogical(This,plVal)

#define ICodeSense_put_AutoIndentMode(This,lVal)   \
    (This)->lpVtbl -> put_AutoIndentMode(This,lVal)

#define ICodeSense_get_AutoIndentMode(This,plVal)   \
    (This)->lpVtbl -> get_AutoIndentMode(This,plVal)

#define ICodeSense_get_ViewCount(This,plVal)   \
    (This)->lpVtbl -> get_ViewCount(This,plVal)

#define ICodeSense_put_VSplitterPos(This,lVal)   \
    (This)->lpVtbl -> put_VSplitterPos(This,lVal)

#define ICodeSense_get_VSplitterPos(This,plVal)   \
    (This)->lpVtbl -> get_VSplitterPos(This,plVal)

#define ICodeSense_put_HSplitterPos(This,lVal)   \
    (This)->lpVtbl -> put_HSplitterPos(This,lVal)

#define ICodeSense_get_HSplitterPos(This,plVal)   \
    (This)->lpVtbl -> get_HSplitterPos(This,plVal)

#define ICodeSense_put_FindText(This,strVal)   \
    (This)->lpVtbl -> put_FindText(This,strVal)

#define ICodeSense_get_FindText(This,pstrVal)   \
    (This)->lpVtbl -> get_FindText(This,pstrVal)

#define ICodeSense_put_ReplText(This,strVal)   \
    (This)->lpVtbl -> put_ReplText(This,strVal)

#define ICodeSense_get_ReplText(This,pstrVal)   \
    (This)->lpVtbl -> get_ReplText(This,pstrVal)

#define ICodeSense_put_LineNumbering(This,bVal)   \
    (This)->lpVtbl -> put_LineNumbering(This,bVal)

#define ICodeSense_get_LineNumbering(This,pbVal)   \
    (This)->lpVtbl -> get_LineNumbering(This,pbVal)

#define ICodeSense_put_LineNumberStart(This,lVal)   \
    (This)->lpVtbl -> put_LineNumberStart(This,lVal)

#define ICodeSense_get_LineNumberStart(This,plVal)   \
    (This)->lpVtbl -> get_LineNumberStart(This,plVal)

#define ICodeSense_put_LineNumberStyle(This,lStyle)   \
    (This)->lpVtbl -> put_LineNumberStyle(This,lStyle)

#define ICodeSense_get_LineNumberStyle(This,plStyle)   \
    (This)->lpVtbl -> get_LineNumberStyle(This,plStyle)

#define ICodeSense_GetWord(This,Pos,pstrWord)   \
    (This)->lpVtbl -> GetWord(This,Pos,pstrWord)

#define ICodeSense_GetWordLength(This,Pos,bLogical,plLen)   \
    (This)->lpVtbl -> GetWordLength(This,Pos,bLogical,plLen)

#define ICodeSense_GetTopIndex(This,lView,plLine)   \
    (This)->lpVtbl -> GetTopIndex(This,lView,plLine)

#define ICodeSense_SetTopIndex(This,lView,lLine)   \
    (This)->lpVtbl -> SetTopIndex(This,lView,lLine)

#define ICodeSense_GetSel(This,bNormalized,Sel)   \
    (This)->lpVtbl -> GetSel(This,bNormalized,Sel)

#define ICodeSense_SetSel(This,Sel,bMakeVisible)   \
    (This)->lpVtbl -> SetSel(This,Sel,bMakeVisible)

#define ICodeSense_GetSelFromPoint(This,lxClientPos,lyClientPos,Pos)   \
    (This)->lpVtbl -> GetSelFromPoint(This,lxClientPos,lyClientPos,Pos)

#define ICodeSense_GetVisibleLineCount(This,lView,bFullyVisible,plCount)   \
    (This)->lpVtbl -> GetVisibleLineCount(This,lView,bFullyVisible,plCount)

#define ICodeSense_GetLine(This,lLine,pstrLine)   \
    (This)->lpVtbl -> GetLine(This,lLine,pstrLine)

#define ICodeSense_GetLineLength(This,lLine,plLen)   \
    (This)->lpVtbl -> GetLineLength(This,lLine,plLen)

#define ICodeSense_SetColor(This,lColorItem,crColor)   \
    (This)->lpVtbl -> SetColor(This,lColorItem,crColor)

#define ICodeSense_GetColor(This,lColorItem,pcrColor)   \
    (This)->lpVtbl -> GetColor(This,lColorItem,pcrColor)

#define ICodeSense_OpenFile(This,strFileName)   \
    (This)->lpVtbl -> OpenFile(This,strFileName)

#define ICodeSense_SaveFile(This,strFileName,bClearUndo)   \
    (This)->lpVtbl -> SaveFile(This,strFileName,bClearUndo)

#define ICodeSense_InsertFile(This,strFileName,InsertPos)   \
    (This)->lpVtbl -> InsertFile(This,strFileName,InsertPos)

#define ICodeSense_InsertText(This,strText,InsertPos)   \
    (This)->lpVtbl -> InsertText(This,strText,InsertPos)

#define ICodeSense_ReplaceText(This,strText,Range)   \
    (This)->lpVtbl -> ReplaceText(This,strText,Range)

#define ICodeSense_HitTest(This,lxClientPos,lyClientPos,plView,plHitTestCode)   \
    (This)->lpVtbl -> HitTest(This,lxClientPos,lyClientPos,plView,plHitTestCode)

#define ICodeSense_AddText(This,strText)   \
    (This)->lpVtbl -> AddText(This,strText)

#define ICodeSense_DeleteLine(This,lLine)   \
    (This)->lpVtbl -> DeleteLine(This,lLine)

#define ICodeSense_InsertLine(This,lLine,strLine)   \
    (This)->lpVtbl -> InsertLine(This,lLine,strLine)

#define ICodeSense_SelectLine(This,lLine,bMakeVisible)   \
    (This)->lpVtbl -> SelectLine(This,lLine,bMakeVisible)

#define ICodeSense_DeleteSel(This)   \
    (This)->lpVtbl -> DeleteSel(This)

#define ICodeSense_ReplaceSel(This,strText)   \
    (This)->lpVtbl -> ReplaceSel(This,strText)

#define ICodeSense_ExecuteCmd(This,lCommand,CmdData)   \
    (This)->lpVtbl -> ExecuteCmd(This,lCommand,CmdData)

#define ICodeSense_CanUndo(This,pbAnswer)   \
    (This)->lpVtbl -> CanUndo(This,pbAnswer)

#define ICodeSense_CanRedo(This,pbAnswer)   \
    (This)->lpVtbl -> CanRedo(This,pbAnswer)

#define ICodeSense_CanCut(This,pbAnswer)   \
    (This)->lpVtbl -> CanCut(This,pbAnswer)

#define ICodeSense_CanCopy(This,pbAnswer)   \
    (This)->lpVtbl -> CanCopy(This,pbAnswer)

#define ICodeSense_CanPaste(This,pbAnswer)   \
    (This)->lpVtbl -> CanPaste(This,pbAnswer)

#define ICodeSense_Undo(This)   \
    (This)->lpVtbl -> Undo(This)

#define ICodeSense_Redo(This)   \
    (This)->lpVtbl -> Redo(This)

#define ICodeSense_Cut(This)   \
    (This)->lpVtbl -> Cut(This)

#define ICodeSense_Copy(This)   \
    (This)->lpVtbl -> Copy(This)

#define ICodeSense_Paste(This)   \
    (This)->lpVtbl -> Paste(This)

#define ICodeSense_ClearUndoBuffer(This)   \
    (This)->lpVtbl -> ClearUndoBuffer(This)

#define ICodeSense_IsRecordingMacro(This,pbAnswer)   \
    (This)->lpVtbl -> IsRecordingMacro(This,pbAnswer)

#define ICodeSense_IsPlayingMacro(This,pbAnswer)   \
    (This)->lpVtbl -> IsPlayingMacro(This,pbAnswer)

#define ICodeSense_SetDlgParent(This,lhWnd)   \
    (This)->lpVtbl -> SetDlgParent(This,lhWnd)

#define ICodeSense_get_SelText(This,pstrVal)   \
    (This)->lpVtbl -> get_SelText(This,pstrVal)

#define ICodeSense_put_SelText(This,strVal)   \
    (This)->lpVtbl -> put_SelText(This,strVal)

#define ICodeSense_put_SelBounds(This,bVal)   \
    (This)->lpVtbl -> put_SelBounds(This,bVal)

#define ICodeSense_get_SelBounds(This,pbVal)   \
    (This)->lpVtbl -> get_SelBounds(This,pbVal)

#define ICodeSense_SetFontStyle(This,lFontStyleItem,byFontStyle)   \
    (This)->lpVtbl -> SetFontStyle(This,lFontStyleItem,byFontStyle)

#define ICodeSense_GetFontStyle(This,lFontStyleItem,pbyFontStyle)   \
    (This)->lpVtbl -> GetFontStyle(This,lFontStyleItem,pbyFontStyle)

#define ICodeSense_put_SearchRegExp(This,bVal)   \
    (This)->lpVtbl -> put_SearchRegExp(This,bVal)

#define ICodeSense_get_SearchRegExp(This,pbVal)   \
    (This)->lpVtbl -> get_SearchRegExp(This,pbVal)

#define ICodeSense_SetItemData(This,lLine,lData)   \
    (This)->lpVtbl -> SetItemData(This,lLine,lData)

#define ICodeSense_GetItemData(This,lLine,plData)   \
    (This)->lpVtbl -> GetItemData(This,lLine,plData)

#define ICodeSense_SetLineStyle(This,lLine,lStyle)   \
    (This)->lpVtbl -> SetLineStyle(This,lLine,lStyle)

#define ICodeSense_GetLineStyle(This,lLine,plStyle)   \
    (This)->lpVtbl -> GetLineStyle(This,lLine,plStyle)

#define ICodeSense_SetBookmark(This,lLine,bVal)   \
    (This)->lpVtbl -> SetBookmark(This,lLine,bVal)

#define ICodeSense_GetBookmark(This,lLine,pbVal)   \
    (This)->lpVtbl -> GetBookmark(This,lLine,pbVal)

#define ICodeSense_SetAllBookmarks(This,Lines)   \
    (This)->lpVtbl -> SetAllBookmarks(This,Lines)

#define ICodeSense_GetAllBookmarks(This,Lines)   \
    (This)->lpVtbl -> GetAllBookmarks(This,Lines)

#define ICodeSense_PosFromChar(This,pPos,Rect)   \
    (This)->lpVtbl -> PosFromChar(This,pPos,Rect)

#define ICodeSense_put_HideSel(This,bVal)   \
    (This)->lpVtbl -> put_HideSel(This,bVal)

#define ICodeSense_get_HideSel(This,pbVal)   \
    (This)->lpVtbl -> get_HideSel(This,pbVal)

#define ICodeSense_put_HighlightedLine(This,lLine)   \
    (This)->lpVtbl -> put_HighlightedLine(This,lLine)

#define ICodeSense_get_HighlightedLine(This,plLine)   \
    (This)->lpVtbl -> get_HighlightedLine(This,plLine)

#define ICodeSense_put_NormalizeCase(This,bVal)   \
    (This)->lpVtbl -> put_NormalizeCase(This,bVal)

#define ICodeSense_get_NormalizeCase(This,pbVal)   \
    (This)->lpVtbl -> get_NormalizeCase(This,pbVal)

#define ICodeSense_SetDivider(This,lLine,bVal)   \
    (This)->lpVtbl -> SetDivider(This,lLine,bVal)

#define ICodeSense_GetDivider(This,lLine,pbVal)   \
    (This)->lpVtbl -> GetDivider(This,lLine,pbVal)

#define ICodeSense_put_OvertypeCaret(This,bVal)   \
    (This)->lpVtbl -> put_OvertypeCaret(This,bVal)

#define ICodeSense_get_OvertypeCaret(This,pbVal)   \
    (This)->lpVtbl -> get_OvertypeCaret(This,pbVal)

#define ICodeSense_SetMarginImages(This,lLine,byImages)   \
    (This)->lpVtbl -> SetMarginImages(This,lLine,byImages)

#define ICodeSense_GetMarginImages(This,lLine,pbyImages)   \
    (This)->lpVtbl -> GetMarginImages(This,lLine,pbyImages)

#define ICodeSense_put_hImageList(This,hImageList)   \
    (This)->lpVtbl -> put_hImageList(This,hImageList)

#define ICodeSense_get_hImageList(This,phImageList)   \
    (This)->lpVtbl -> get_hImageList(This,phImageList)

#define ICodeSense_get_ImageList(This,ppImageList)   \
    (This)->lpVtbl -> get_ImageList(This,ppImageList)

#define ICodeSense_put_ImageList(This,pImageList)   \
    (This)->lpVtbl -> put_ImageList(This,pImageList)

#define ICodeSense_putref_ImageList(This,pImageList)   \
    (This)->lpVtbl -> putref_ImageList(This,pImageList)

#define ICodeSense_AboutBox(This)   \
    (This)->lpVtbl -> AboutBox(This)

#define ICodeSense_PrintContents(This,hDC,lFlags)   \
    (This)->lpVtbl -> PrintContents(This,hDC,lFlags)

#define ICodeSense_SetCaretPos(This,lLine,lCol)   \
    (This)->lpVtbl -> SetCaretPos(This,lLine,lCol)

#define ICodeSense_ViewColToBufferCol(This,lLine,lViewCol,plBuffCol)   \
    (This)->lpVtbl -> ViewColToBufferCol(This,lLine,lViewCol,plBuffCol)

#define ICodeSense_BufferColToViewCol(This,lLine,lBuffCol,plViewCol)   \
    (This)->lpVtbl -> BufferColToViewCol(This,lLine,lBuffCol,plViewCol)

#define ICodeSense_get_SelLength(This,plVal)   \
    (This)->lpVtbl -> get_SelLength(This,plVal)

#define ICodeSense_get_SelLengthLogical(This,plVal)   \
    (This)->lpVtbl -> get_SelLengthLogical(This,plVal)

#define ICodeSense_put_BorderStyle(This,lBorderStyle)   \
    (This)->lpVtbl -> put_BorderStyle(This,lBorderStyle)

#define ICodeSense_get_BorderStyle(This,plBorderStyle)   \
    (This)->lpVtbl -> get_BorderStyle(This,plBorderStyle)

#define ICodeSense_get_hWnd(This,phwnd)   \
    (This)->lpVtbl -> get_hWnd(This,phwnd)

#define ICodeSense_put_CurrentView(This,lVal)   \
    (This)->lpVtbl -> put_CurrentView(This,lVal)

#define ICodeSense_put_StealKeys(This,bVal)   \
    (This)->lpVtbl -> put_StealKeys(This,bVal)

#define ICodeSense_get_StealKeys(This,pbVal)   \
    (This)->lpVtbl -> get_StealKeys(This,pbVal)

#define ICodeSense_get_CurrentToken(This,lpTokenType)   \
    (This)->lpVtbl -> get_CurrentToken(This,lpTokenType)

#define ICodeSense_UpdateControlPositions(This)   \
    (This)->lpVtbl -> UpdateControlPositions(This)

#endif /* COBJMACROS */


#endif    /* C style interface */



/* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE ICodeSense_putref_Font_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ICodeSense_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_Font_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ICodeSense_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Font_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);


void __RPC_STUB ICodeSense_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_Enabled_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Enabled_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Window_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *phwnd);


void __RPC_STUB ICodeSense_get_Window_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_Text_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ICodeSense_put_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Text_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ICodeSense_get_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_ColorSyntax_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_ColorSyntax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ColorSyntax_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_ColorSyntax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_DisplayWhitespace_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_DisplayWhitespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_DisplayWhitespace_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_DisplayWhitespace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_ExpandTabs_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_ExpandTabs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ExpandTabs_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_ExpandTabs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_SmoothScrolling_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_SmoothScrolling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SmoothScrolling_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_SmoothScrolling_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_ReadOnly_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_ReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ReadOnly_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_ReadOnly_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_LineToolTips_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_LineToolTips_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_LineToolTips_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_LineToolTips_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_DisplayLeftMargin_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_DisplayLeftMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_DisplayLeftMargin_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_DisplayLeftMargin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_EnableColumnSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_EnableColumnSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_EnableColumnSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_EnableColumnSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_EnableDragDrop_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_EnableDragDrop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_EnableDragDrop_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_EnableDragDrop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_Overtype_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_Overtype_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Overtype_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_Overtype_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_SearchCaseSensitive_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_SearchCaseSensitive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SearchCaseSensitive_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_SearchCaseSensitive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_ReplacePreserveCase_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_ReplacePreserveCase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ReplacePreserveCase_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_ReplacePreserveCase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_SearchWholeWord_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_SearchWholeWord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SearchWholeWord_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_SearchWholeWord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_Modified_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_Modified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Modified_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_Modified_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_EnableCRLF_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_EnableCRLF_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_EnableCRLF_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_EnableCRLF_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_GlobalProps_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_GlobalProps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_GlobalProps_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_GlobalProps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_VScrollVisible_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_VScrollVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_VScrollVisible_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_VScrollVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_HScrollVisible_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_HScrollVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_HScrollVisible_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_HScrollVisible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_EnableVSplitter_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_EnableVSplitter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_EnableVSplitter_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_EnableVSplitter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_EnableHSplitter_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_EnableHSplitter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_EnableHSplitter_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_EnableHSplitter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_Language_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ICodeSense_put_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_Language_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ICodeSense_get_Language_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_TabSize_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_TabSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_TabSize_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_TabSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_TopIndex_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_TopIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_TopIndex_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_TopIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_UndoLimit_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_UndoLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_UndoLimit_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_UndoLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_CurrentView_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_CurrentView_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_TextLength_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_TextLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_TextLengthLogical_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_TextLengthLogical_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_LineCount_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_LineCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_CurrentWord_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ICodeSense_get_CurrentWord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_CurrentWordLength_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_CurrentWordLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_CurrentWordLengthLogical_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_CurrentWordLengthLogical_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_AutoIndentMode_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmAutoIndentMode lVal);


void __RPC_STUB ICodeSense_put_AutoIndentMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_AutoIndentMode_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ cmAutoIndentMode __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_AutoIndentMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ViewCount_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_ViewCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_VSplitterPos_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_VSplitterPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_VSplitterPos_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_VSplitterPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_HSplitterPos_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_HSplitterPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_HSplitterPos_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_HSplitterPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_FindText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ICodeSense_put_FindText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_FindText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ICodeSense_get_FindText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_ReplText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ICodeSense_put_ReplText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ReplText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ICodeSense_get_ReplText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_LineNumbering_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_LineNumbering_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_LineNumbering_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_LineNumbering_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_LineNumberStart_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_LineNumberStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_LineNumberStart_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_LineNumberStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_LineNumberStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmLineNumStyle lStyle);


void __RPC_STUB ICodeSense_put_LineNumberStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_LineNumberStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ cmLineNumStyle __RPC_FAR *plStyle);


void __RPC_STUB ICodeSense_get_LineNumberStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetWord_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IPosition __RPC_FAR *Pos,
    /* [retval][out] */ BSTR __RPC_FAR *pstrWord);


void __RPC_STUB ICodeSense_GetWord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetWordLength_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IPosition __RPC_FAR *Pos,
    /* [in] */ VARIANT_BOOL bLogical,
    /* [retval][out] */ long __RPC_FAR *plLen);


void __RPC_STUB ICodeSense_GetWordLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetTopIndex_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lView,
    /* [retval][out] */ long __RPC_FAR *plLine);


void __RPC_STUB ICodeSense_GetTopIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetTopIndex_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lView,
    /* [in] */ long lLine);


void __RPC_STUB ICodeSense_SetTopIndex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bNormalized,
    /* [retval][out] */ IRange __RPC_FAR *__RPC_FAR *Sel);


void __RPC_STUB ICodeSense_GetSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IRange __RPC_FAR *Sel,
    /* [in] */ VARIANT_BOOL bMakeVisible);


void __RPC_STUB ICodeSense_SetSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetSelFromPoint_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lxClientPos,
    /* [in] */ long lyClientPos,
    /* [retval][out] */ IPosition __RPC_FAR *__RPC_FAR *Pos);


void __RPC_STUB ICodeSense_GetSelFromPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetVisibleLineCount_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lView,
    /* [in] */ VARIANT_BOOL bFullyVisible,
    /* [retval][out] */ long __RPC_FAR *plCount);


void __RPC_STUB ICodeSense_GetVisibleLineCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetLine_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ BSTR __RPC_FAR *pstrLine);


void __RPC_STUB ICodeSense_GetLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetLineLength_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ long __RPC_FAR *plLen);


void __RPC_STUB ICodeSense_GetLineLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetColor_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmColorItem lColorItem,
    /* [in] */ OLE_COLOR crColor);


void __RPC_STUB ICodeSense_SetColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetColor_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmColorItem lColorItem,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pcrColor);


void __RPC_STUB ICodeSense_GetColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_OpenFile_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strFileName);


void __RPC_STUB ICodeSense_OpenFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SaveFile_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strFileName,
    /* [in] */ VARIANT_BOOL bClearUndo);


void __RPC_STUB ICodeSense_SaveFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_InsertFile_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strFileName,
    /* [in] */ IPosition __RPC_FAR *InsertPos);


void __RPC_STUB ICodeSense_InsertFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_InsertText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strText,
    /* [in] */ IPosition __RPC_FAR *InsertPos);


void __RPC_STUB ICodeSense_InsertText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_ReplaceText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strText,
    /* [in] */ IRange __RPC_FAR *Range);


void __RPC_STUB ICodeSense_ReplaceText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_HitTest_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lxClientPos,
    /* [in] */ long lyClientPos,
    /* [out] */ long __RPC_FAR *plView,
    /* [retval][out] */ cmHitTestCode __RPC_FAR *plHitTestCode);


void __RPC_STUB ICodeSense_HitTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_AddText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strText);


void __RPC_STUB ICodeSense_AddText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_DeleteLine_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine);


void __RPC_STUB ICodeSense_DeleteLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_InsertLine_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ BSTR strLine);


void __RPC_STUB ICodeSense_InsertLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SelectLine_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ VARIANT_BOOL bMakeVisible);


void __RPC_STUB ICodeSense_SelectLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_DeleteSel_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_DeleteSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_ReplaceSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strText);


void __RPC_STUB ICodeSense_ReplaceSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_ExecuteCmd_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmCommand lCommand,
    /* [optional][in] */ VARIANT CmdData);


void __RPC_STUB ICodeSense_ExecuteCmd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_CanUndo_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_CanUndo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_CanRedo_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_CanRedo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_CanCut_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_CanCut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_CanCopy_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_CanCopy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_CanPaste_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_CanPaste_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_Undo_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_Undo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_Redo_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_Redo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_Cut_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_Cut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_Copy_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_Copy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_Paste_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_Paste_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_ClearUndoBuffer_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_ClearUndoBuffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_IsRecordingMacro_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_IsRecordingMacro_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_IsPlayingMacro_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAnswer);


void __RPC_STUB ICodeSense_IsPlayingMacro_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetDlgParent_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lhWnd);


void __RPC_STUB ICodeSense_SetDlgParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SelText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrVal);


void __RPC_STUB ICodeSense_get_SelText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_SelText_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ BSTR strVal);


void __RPC_STUB ICodeSense_put_SelText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_SelBounds_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_SelBounds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SelBounds_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_SelBounds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetFontStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmFontStyleItem lFontStyleItem,
    /* [in] */ cmFontStyle byFontStyle);


void __RPC_STUB ICodeSense_SetFontStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetFontStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmFontStyleItem lFontStyleItem,
    /* [retval][out] */ cmFontStyle __RPC_FAR *pbyFontStyle);


void __RPC_STUB ICodeSense_GetFontStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_SearchRegExp_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_SearchRegExp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SearchRegExp_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_SearchRegExp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetItemData_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ long lData);


void __RPC_STUB ICodeSense_SetItemData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetItemData_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ long __RPC_FAR *plData);


void __RPC_STUB ICodeSense_GetItemData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetLineStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ long lStyle);


void __RPC_STUB ICodeSense_SetLineStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetLineStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ long __RPC_FAR *plStyle);


void __RPC_STUB ICodeSense_GetLineStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetBookmark_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_SetBookmark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetBookmark_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_GetBookmark_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetAllBookmarks_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Lines);


void __RPC_STUB ICodeSense_SetAllBookmarks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetAllBookmarks_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [out] */ SAFEARRAY __RPC_FAR * __RPC_FAR *Lines);


void __RPC_STUB ICodeSense_GetAllBookmarks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_PosFromChar_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IPosition __RPC_FAR *pPos,
    /* [retval][out] */ IRect __RPC_FAR *__RPC_FAR *Rect);


void __RPC_STUB ICodeSense_PosFromChar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_HideSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_HideSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_HideSel_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_HideSel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_HighlightedLine_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine);


void __RPC_STUB ICodeSense_put_HighlightedLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_HighlightedLine_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plLine);


void __RPC_STUB ICodeSense_get_HighlightedLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_NormalizeCase_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_NormalizeCase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_NormalizeCase_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_NormalizeCase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetDivider_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_SetDivider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetDivider_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_GetDivider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_OvertypeCaret_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_OvertypeCaret_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_OvertypeCaret_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_OvertypeCaret_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetMarginImages_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ BYTE byImages);


void __RPC_STUB ICodeSense_SetMarginImages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_GetMarginImages_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [retval][out] */ BYTE __RPC_FAR *pbyImages);


void __RPC_STUB ICodeSense_GetMarginImages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_hImageList_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ OLE_HANDLE hImageList);


void __RPC_STUB ICodeSense_put_hImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_hImageList_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ OLE_HANDLE __RPC_FAR *phImageList);


void __RPC_STUB ICodeSense_get_hImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_ImageList_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppImageList);


void __RPC_STUB ICodeSense_get_ImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_ImageList_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pImageList);


void __RPC_STUB ICodeSense_put_ImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propputref] */ HRESULT STDMETHODCALLTYPE ICodeSense_putref_ImageList_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pImageList);


void __RPC_STUB ICodeSense_putref_ImageList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring][id][hidden] */ HRESULT STDMETHODCALLTYPE ICodeSense_AboutBox_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_AboutBox_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_PrintContents_Proxy(
    ICodeSense __RPC_FAR * This,
    long hDC,
    long lFlags);


void __RPC_STUB ICodeSense_PrintContents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_SetCaretPos_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ long lCol);


void __RPC_STUB ICodeSense_SetCaretPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_ViewColToBufferCol_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ long lViewCol,
    /* [retval][out] */ long __RPC_FAR *plBuffCol);


void __RPC_STUB ICodeSense_ViewColToBufferCol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_BufferColToViewCol_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lLine,
    /* [in] */ long lBuffCol,
    /* [retval][out] */ long __RPC_FAR *plViewCol);


void __RPC_STUB ICodeSense_BufferColToViewCol_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SelLength_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_SelLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_SelLengthLogical_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plVal);


void __RPC_STUB ICodeSense_get_SelLengthLogical_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_BorderStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ cmBorderStyle lBorderStyle);


void __RPC_STUB ICodeSense_put_BorderStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_BorderStyle_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ cmBorderStyle __RPC_FAR *plBorderStyle);


void __RPC_STUB ICodeSense_get_BorderStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_hWnd_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *phwnd);


void __RPC_STUB ICodeSense_get_hWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_CurrentView_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ long lVal);


void __RPC_STUB ICodeSense_put_CurrentView_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propput] */ HRESULT STDMETHODCALLTYPE ICodeSense_put_StealKeys_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bVal);


void __RPC_STUB ICodeSense_put_StealKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_StealKeys_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbVal);


void __RPC_STUB ICodeSense_get_StealKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id][propget] */ HRESULT STDMETHODCALLTYPE ICodeSense_get_CurrentToken_Proxy(
    ICodeSense __RPC_FAR * This,
    /* [retval][out] */ cmTokenType __RPC_FAR *lpTokenType);


void __RPC_STUB ICodeSense_get_CurrentToken_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][helpcontext][id] */ HRESULT STDMETHODCALLTYPE ICodeSense_UpdateControlPositions_Proxy(
    ICodeSense __RPC_FAR * This);


void __RPC_STUB ICodeSense_UpdateControlPositions_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif    /* __ICodeSense_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * );
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * );

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long __RPC_FAR *, unsigned long            , LPSAFEARRAY __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, LPSAFEARRAY __RPC_FAR * );
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long __RPC_FAR *, LPSAFEARRAY __RPC_FAR * );

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * );
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * );
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
