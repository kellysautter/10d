/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrgbl.h
// AUTHOR:
//
// OVERVIEW
// ========
// Global definitions.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//    Change to use new operation GetAppOrWorkstationValue.
//
// 1999.04.19    RS
//    Commentlines required for Rose.
//    includes required for Rose
//

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> // required for Rose
//#include <zdrapp.h> // required for Rose

#include <kzoegdef.h>

#ifdef zGLOBAL_DATA
// ZPtrArray<ZSubtask *> g_SubtaskList( 50, 0, 25 );
#else
// zEXTERN ZPtrArray<ZSubtask *> g_SubtaskList;
#endif

mGLOBAL_DECLARE( HINSTANCE, g_hInstanceDLL, 0 );
mGLOBAL_DECLARE( ZFindDialog *, g_pSubtaskList, 0 );

#ifdef zREMOTE_SERVER
mGLOBAL_DECLARE( ZFindSocket *, g_pSocketList, 0 );
#endif

mGLOBAL_DECLARE( zVIEW, g_vRegister, 0 ); // Original view from RegisterZeidonApplication
mGLOBAL_DECLARE( ZMapAct *, g_pzmaDragDrop, 0 );
mGLOBAL_DECLARE( zLONG, g_lDlgUnitCharX, 0 );
mGLOBAL_DECLARE( zLONG, g_lDlgUnitCharY, 0 );
mGLOBAL_DECLARE( zLONG, g_lMapDlgCharX, 0 );
mGLOBAL_DECLARE( zLONG, g_lMapDlgCharY, 0 );
mGLOBAL_DECLARE( zLONG, g_lPixelsPerInchX, 0 );
mGLOBAL_DECLARE( zLONG, g_lPixelsPerInchY, 0 );
mGLOBAL_DECLARE( zLONG, g_lDblClkX, 0 );
mGLOBAL_DECLARE( zLONG, g_lDblClkY, 0 );
zEXTERN ZDragDrop g_DragDrop; // Drag drop structure

#ifdef zREMOTE_SERVER
mGLOBAL_DECLARE( zLONG, g_lMsgTag, 0 );
#endif

mGLOBAL_DECLARE( zPCHAR, szlPostedClose, "ZDR_POSTED_CLOSE" );
mGLOBAL_DECLARE( UINT, g_uMsgPostedClose,
                 RegisterWindowMessage( szlPostedClose ) );

mGLOBAL_DECLARE( zPCHAR, szlDrvr, "(drvr) " );
mGLOBAL_DECLARE( zPCHAR, szlAN, "AN" );
mGLOBAL_DECLARE( zPCHAR, szlAct, "Act" );
mGLOBAL_DECLARE( zPCHAR, szlActCtrl, "ActCtrl" );
mGLOBAL_DECLARE( zPCHAR, szlActHot, "ActHot" );
mGLOBAL_DECLARE( zPCHAR, szlActOpt, "ActOpt" );
mGLOBAL_DECLARE( zPCHAR, szlApp, "App" );
mGLOBAL_DECLARE( zPCHAR, szlAutoGray, "AutoGray" );
mGLOBAL_DECLARE( zPCHAR, szlCPP, "CPP" );
mGLOBAL_DECLARE( zPCHAR, szlClient, "Client" );
mGLOBAL_DECLARE( zPCHAR, szlCOP, "COP" );
mGLOBAL_DECLARE( zPCHAR, szlCtrl, "Ctrl" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlBOI, "CtrlBOI" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlColor, "CtrlColor" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlCtrl, "CtrlCtrl" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlFont, "CtrlFont" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMap, "CtrlMap" );
mGLOBAL_DECLARE( zPCHAR, szlD_Caption, "D_Caption" );
mGLOBAL_DECLARE( zPCHAR, szlD_Msg, "D_Msg" );
mGLOBAL_DECLARE( zPCHAR, szlD_Text, "D_Text" );
mGLOBAL_DECLARE( zPCHAR, szlDD_BOI, "DD_BOI" );
mGLOBAL_DECLARE( zPCHAR, szlDfltBtn, "DfltBtn" );
mGLOBAL_DECLARE( zPCHAR, szlDfltWnd, "DfltWnd" );
mGLOBAL_DECLARE( zPCHAR, szlDIL, "DIL" );
mGLOBAL_DECLARE( zPCHAR, szlDlg, "Dlg" );
mGLOBAL_DECLARE( zPCHAR, szlDLL, "DLL" );
mGLOBAL_DECLARE( zPCHAR, szlEN, "EN" );
mGLOBAL_DECLARE( zPCHAR, szlEvent, "Event" );
mGLOBAL_DECLARE( zPCHAR, szlEventAct, "EventAct" );
mGLOBAL_DECLARE( zPCHAR, szlHotkey, "Hotkey" );
mGLOBAL_DECLARE( zPCHAR, szlId, "Id" );
mGLOBAL_DECLARE( zPCHAR, szlIdNbr, "IdNbr" );
mGLOBAL_DECLARE( zPCHAR, szlKZWDLGXO, "KZWDLGXO" );
mGLOBAL_DECLARE( zPCHAR, szlKey, "Key" );
mGLOBAL_DECLARE( zPCHAR, szlLPFN, "LPFN" );
mGLOBAL_DECLARE( zPCHAR, szlMenu, "Menu" );
mGLOBAL_DECLARE( zPCHAR, szlOpt, "Opt" );
mGLOBAL_DECLARE( zPCHAR, szlOptAct, "OptAct" );
mGLOBAL_DECLARE( zPCHAR, szlOptCnt, "OptCnt" );
mGLOBAL_DECLARE( zPCHAR, szlOptMap, "OptMap" );
mGLOBAL_DECLARE( zPCHAR, szlOptOpt, "OptOpt" );
mGLOBAL_DECLARE( zPCHAR, szlProperties, "Properties" );
mGLOBAL_DECLARE( zPCHAR, szlQMsg, "QMsg" );
mGLOBAL_DECLARE( zPCHAR, szlRGB, "RGB" );
mGLOBAL_DECLARE( zPCHAR, szlStatus, "Status" );
mGLOBAL_DECLARE( zPCHAR, szlSubtask, "Subtask" );
mGLOBAL_DECLARE( zPCHAR, szlSyncIdx, "SyncIdx" );
mGLOBAL_DECLARE( zPCHAR, szlTag, "Tag" );
mGLOBAL_DECLARE( zPCHAR, szlTask, "Task" );
mGLOBAL_DECLARE( zPCHAR, szlTitle, "Title" );
mGLOBAL_DECLARE( zPCHAR, szlType, "Type" );
mGLOBAL_DECLARE( zPCHAR, szlVN, "VN" );
mGLOBAL_DECLARE( zPCHAR, szlWnd, "Wnd" );
mGLOBAL_DECLARE( zPCHAR, szlWndAct, "WndAct" );
mGLOBAL_DECLARE( zPCHAR, szlWndColor, "WndColor" );
mGLOBAL_DECLARE( zPCHAR, szlWndEvent, "WndEvent" );
mGLOBAL_DECLARE( zPCHAR, szlZeidon, "Zeidon" );

mGLOBAL_DECLARE( zPCHAR, szlTZPNTRAD, "TZPNTRAD" );
mGLOBAL_DECLARE( zPCHAR, szlKZMSGQOO, "KZMSGQOO" );
mGLOBAL_DECLARE( zPCHAR, szl__MSGQ, "__MSGQ" );

mGLOBAL_DECLARE( zPCHAR, szlKZPEXVOO, "KZPEXVOO" );
mGLOBAL_DECLARE( zPCHAR, szl__PE, "__PE" );
mGLOBAL_DECLARE( zPCHAR, szlZEIDON_XPE, "ZEIDON.XPE" );

mGLOBAL_DECLARE( zPCHAR, szlKZHKEYXO, "KZHKEYXO" );
mGLOBAL_DECLARE( zPCHAR, szl__HKEY, "__HKEY" );
mGLOBAL_DECLARE( zPCHAR, szlKZHKEYAA_POR, "ZEIDON.HKY" );
