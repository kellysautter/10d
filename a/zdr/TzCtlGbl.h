/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Application
// FILE:         tzctlgbl.h
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
//
// 2001.03.26    TMV   2000
//    Added global declaration for szlLKey
//
// 2001.01.29    TMV
//    Added global declaration for szlProperties
//
// 1999.04.19    RS
//    Commentlines required for Rose.
//

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose

#include <kzoegdef.h>

mGLOBAL_DECLARE( HINSTANCE, g_hInstanceDLL, 0 );
mGLOBAL_DECLARE( zLONG, g_lDlgUnitCharX, 0 );
mGLOBAL_DECLARE( zLONG, g_lDlgUnitCharY, 0 );
mGLOBAL_DECLARE( zLONG, g_lMapDlgCharX, 0 );
mGLOBAL_DECLARE( zLONG, g_lMapDlgCharY, 0 );
mGLOBAL_DECLARE( zLONG, g_lPixelsPerInchX, 0 );
mGLOBAL_DECLARE( zLONG, g_lPixelsPerInchY, 0 );
mGLOBAL_DECLARE( zLONG, g_lDblClkX, 0 );
mGLOBAL_DECLARE( zLONG, g_lDblClkY, 0 );
zEXTERN TEXTMETRIC TextMetrics;

mGLOBAL_DECLARE( zPCHAR, szlAction, "Action" );
mGLOBAL_DECLARE( zPCHAR, szlCOMMA, "," );
mGLOBAL_DECLARE( zPCHAR, szlCaption, "Caption" );
mGLOBAL_DECLARE( zPCHAR, szlCardMax, "CardMax" );
mGLOBAL_DECLARE( zPCHAR, szlCardMin, "CardMin" );
mGLOBAL_DECLARE( zPCHAR, szlClass, "Class" );
mGLOBAL_DECLARE( zPCHAR, szlControl, "Control" );
mGLOBAL_DECLARE( zPCHAR, szlControlDef, "ControlDef" );
mGLOBAL_DECLARE( zPCHAR, szlCtrl, "Ctrl" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlBOI, "CtrlBOI" );
mGLOBAL_DECLARE( zPCHAR, szlProperties, "Properties" );
mGLOBAL_DECLARE( zPCHAR, szlLKey, "LKey" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlBkgdColor, "CtrlBkgdColor" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlCtrl, "CtrlCtrl" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlEventDef, "CtrlEventDef" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlFgdColor, "CtrlFgdColor" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlFont, "CtrlFont" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMap, "CtrlMap" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMapContext, "CtrlMapContext" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMapER_Attribute, "CtrlMapER_Attribute" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMapLOD_Attribute, "CtrlMapLOD_Attribute" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMapLOD_Entity, "CtrlMapLOD_Entity" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMapRelatedEntity, "CtrlMapRelatedEntity" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMapView, "CtrlMapView" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlPhysOverride, "CtrlPhysOverride" );
mGLOBAL_DECLARE( zPCHAR, szlDIL_Active, "DIL_Active" );
mGLOBAL_DECLARE( zPCHAR, szlDesc, "Desc" );
mGLOBAL_DECLARE( zPCHAR, szlDfltMenu, "DfltMenu" );
mGLOBAL_DECLARE( zPCHAR, szlDialog, "Dialog" );
mGLOBAL_DECLARE( zPCHAR, szlDisplayText, "DisplayText" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramBendPosX, "ER_DiagramBendPosX" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramBendPosY, "ER_DiagramBendPosY" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramE1PosX, "ER_DiagramE1PosX" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramE1PosY, "ER_DiagramE1PosY" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramE2PosX, "ER_DiagramE2PosX" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramE2PosY, "ER_DiagramE2PosY" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramPosX, "ER_DiagramPosX" );
mGLOBAL_DECLARE( zPCHAR, szlER_DiagramPosY, "ER_DiagramPosY" );
mGLOBAL_DECLARE( zPCHAR, szlER_Entity, "ER_Entity" );
mGLOBAL_DECLARE( zPCHAR, szlER_Entity_2, "ER_Entity_2" );
mGLOBAL_DECLARE( zPCHAR, szlER_PortFactor, "ER_PortFactor" );
mGLOBAL_DECLARE( zPCHAR, szlER_RelLink_2, "ER_RelLink_2" );
mGLOBAL_DECLARE( zPCHAR, szlER_RelType, "ER_RelType" );
mGLOBAL_DECLARE( zPCHAR, szlEvent, "Event" );
mGLOBAL_DECLARE( zPCHAR, szlEventAct, "EventAct" );
mGLOBAL_DECLARE( zPCHAR, szlEventDef, "EventDef" );
mGLOBAL_DECLARE( zPCHAR, szlGroup, "Group" );
mGLOBAL_DECLARE( zPCHAR, szlGroupSet, "GroupSet" );
mGLOBAL_DECLARE( zPCHAR, szlHD_Node, "HD_Node" );
mGLOBAL_DECLARE( zPCHAR, szlID, "ID" );
mGLOBAL_DECLARE( zPCHAR, szlIdNbr, "IdNbr" );
mGLOBAL_DECLARE( zPCHAR, szlKey, "Key" );
mGLOBAL_DECLARE( zPCHAR, szlLIST_MENU, "LIST_MENU" );
mGLOBAL_DECLARE( zPCHAR, szlLPAREN, " (" );
mGLOBAL_DECLARE( zPCHAR, szlLevel, "Level" );
mGLOBAL_DECLARE( zPCHAR, szlMaintenanceDialog, "MaintenanceDialog" );
mGLOBAL_DECLARE( zPCHAR, szlMaintenanceWindow, "MaintenanceWindow" );
mGLOBAL_DECLARE( zPCHAR, szlMenu, "Menu" );
mGLOBAL_DECLARE( zPCHAR, szlNULL, "NULL" );
mGLOBAL_DECLARE( zPCHAR, szlName, "Name" );
mGLOBAL_DECLARE( zPCHAR, szlNew, "New" );
mGLOBAL_DECLARE( zPCHAR, szlNewFile, "NewFile" );
mGLOBAL_DECLARE( zPCHAR, szlOpenedFile, "OpenedFile" );
mGLOBAL_DECLARE( zPCHAR, szlOptAct, "OptAct" );
mGLOBAL_DECLARE( zPCHAR, szlOptMap, "OptMap" );
mGLOBAL_DECLARE( zPCHAR, szlOptOpt, "OptOpt" );
mGLOBAL_DECLARE( zPCHAR, szlOption, "Option" );
mGLOBAL_DECLARE( zPCHAR, szlPAGEL, "PAGEL" );
mGLOBAL_DECLARE( zPCHAR, szlPage, "Page" );
mGLOBAL_DECLARE( zPCHAR, szlPSDLG_X, "PSDLG_X" );
mGLOBAL_DECLARE( zPCHAR, szlPSDLG_Y, "PSDLG_Y" );
mGLOBAL_DECLARE( zPCHAR, szlPainterBitmap, "PainterBitmap" );
mGLOBAL_DECLARE( zPCHAR, szlPainterC_OperationName, "PainterC_OperationName" );
mGLOBAL_DECLARE( zPCHAR, szlPainterDLL_Name, "PainterDLL_Name" );
mGLOBAL_DECLARE( zPCHAR, szlPalette, "Palette" );
mGLOBAL_DECLARE( zPCHAR, szlPointer, "Pointer" );
mGLOBAL_DECLARE( zPCHAR, szlProfileXFER, "ProfileXFER" );
mGLOBAL_DECLARE( zPCHAR, szlPurpose, "Purpose" );
mGLOBAL_DECLARE( zPCHAR, szlReport, "Report" );
mGLOBAL_DECLARE( zPCHAR, szlRPAREN, ")" );
mGLOBAL_DECLARE( zPCHAR, szlRoot, "Root" );
mGLOBAL_DECLARE( zPCHAR, szlSizeX, "SizeX" );
mGLOBAL_DECLARE( zPCHAR, szlSizeY, "SizeY" );
mGLOBAL_DECLARE( zPCHAR, szlSZDLG_X, "SZDLG_X" );
mGLOBAL_DECLARE( zPCHAR, szlSZDLG_Y, "SZDLG_Y" );
mGLOBAL_DECLARE( zPCHAR, szlSeparator, "Separator" );
mGLOBAL_DECLARE( zPCHAR, szlStyle, "Style" );
mGLOBAL_DECLARE( zPCHAR, szlSubtype, "Subtype" );
mGLOBAL_DECLARE( zPCHAR, szlSyncKey, "SyncKey" );
mGLOBAL_DECLARE( zPCHAR, szlTZCONTROL, "TZCONTROL" );
mGLOBAL_DECLARE( zPCHAR, szlTZDIALOGS, "TZDIALOGS" );
mGLOBAL_DECLARE( zPCHAR, szlTZPESRCO, "TZPESRCO" );
mGLOBAL_DECLARE( zPCHAR, szlTZPNEVWO, "TZPNEVWO" );
mGLOBAL_DECLARE( zPCHAR, szlTZPNOPWO, "TZPNOPWO" );
mGLOBAL_DECLARE( zPCHAR, szlTZPNTRAD, "TZPNTRAD" );
mGLOBAL_DECLARE( zPCHAR, szlTZPNTROO, "TZPNTROO" );
mGLOBAL_DECLARE( zPCHAR, szlTZRELTGT, "TZRELTGT" );
mGLOBAL_DECLARE( zPCHAR, szlTZWDLGSO, "TZWDLGSO" );
mGLOBAL_DECLARE( zPCHAR, szlTZWINDOW, "TZWINDOW" );
mGLOBAL_DECLARE( zPCHAR, szlTZWINDOWL, "TZWINDOWL" );
mGLOBAL_DECLARE( zPCHAR, szlTZWINOPT, "TZWINOPT" );
mGLOBAL_DECLARE( zPCHAR, szlTZWND_LIST, "TZWND_LIST" );
mGLOBAL_DECLARE( zPCHAR, szlTag, "Tag" );
mGLOBAL_DECLARE( zPCHAR, szlText, "Text" );
mGLOBAL_DECLARE( zPCHAR, szlType, "Type" );
mGLOBAL_DECLARE( zPCHAR, szlUPD_MENU, "UPD_MENU" );
mGLOBAL_DECLARE( zPCHAR, szlWND_UPD, "WND_UPD" );
mGLOBAL_DECLARE( zPCHAR, szlW_MetaDef, "W_MetaDef" );
mGLOBAL_DECLARE( zPCHAR, szlWindow, "Window" );
mGLOBAL_DECLARE( zPCHAR, szlWindowStyle, "WindowStyle" );
mGLOBAL_DECLARE( zPCHAR, szlWndAct, "WndAct" );
mGLOBAL_DECLARE( zPCHAR, szlWndEvent, "WndEvent" );
mGLOBAL_DECLARE( zPCHAR, szlXSLT, "XSLT" );
mGLOBAL_DECLARE( zPCHAR, szlXSLTL, "XSLTL" );
mGLOBAL_DECLARE( zPCHAR, szlZKey, "ZKey" );
