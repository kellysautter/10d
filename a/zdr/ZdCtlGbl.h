/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlgbl.h
// AUTHOR:
//
// OVERVIEW
// ========
// Global definitions.
//

//
// Change Log
//
//
// CHANGE LOG - most recent first order
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 1999.04.19    RS
//    includes required for Rose
//    Commentlines required for Rose.
//

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose
//#include <zdctl.h> //required for Rose

#include <kzoegdef.h>

mGLOBAL_DECLARE( zPCHAR, szlDrvr, "(drvr) " );

mGLOBAL_DECLARE( HINSTANCE, g_hInstanceDLL, 0 );
mGLOBAL_DECLARE( zLONG, g_lDlgUnitCharX, 0 );
mGLOBAL_DECLARE( zLONG, g_lDlgUnitCharY, 0 );
mGLOBAL_DECLARE( zLONG, g_lMapDlgCharX, 0 );
mGLOBAL_DECLARE( zLONG, g_lMapDlgCharY, 0 );
mGLOBAL_DECLARE( zLONG, g_lPixelsPerInchX, 0 );
mGLOBAL_DECLARE( zLONG, g_lPixelsPerInchY, 0 );
mGLOBAL_DECLARE( zLONG, g_lDblClkX, 0 );
mGLOBAL_DECLARE( zLONG, g_lDblClkY, 0 );

mGLOBAL_DECLARE( zPCHAR, szlAN, "AN" );
mGLOBAL_DECLARE( zPCHAR, szlBUTTON, "BUTTON" );
mGLOBAL_DECLARE( zPCHAR, szlCOMBOBOX, "COMBOBOX" );
mGLOBAL_DECLARE( zPCHAR, szlCOMMA  , "," );
mGLOBAL_DECLARE( zPCHAR, szlCardMax, "CardMax" );
mGLOBAL_DECLARE( zPCHAR, szlCardMin, "CardMin" );
mGLOBAL_DECLARE( zPCHAR, szlCollapsed, "Collapsed" );
mGLOBAL_DECLARE( zPCHAR, szlColor, "Color" );
mGLOBAL_DECLARE( zPCHAR, szlCtrl, "Ctrl" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlBOI, "CtrlBOI" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlCtrl, "CtrlCtrl" );
mGLOBAL_DECLARE( zPCHAR, szlCtrlMap, "CtrlMap" );
mGLOBAL_DECLARE( zPCHAR, szlDIL, "DIL" );
mGLOBAL_DECLARE( zPCHAR, szlDfltBtn, "DfltBtn" );
mGLOBAL_DECLARE( zPCHAR, szlEDIT, "EDIT" );
mGLOBAL_DECLARE( zPCHAR, szlEN, "EN" );
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
mGLOBAL_DECLARE( zPCHAR, szlER_RelLink_2, "ER_RelLink_2" );
mGLOBAL_DECLARE( zPCHAR, szlER_RelType, "ER_RelType" );
mGLOBAL_DECLARE( zPCHAR, szlHD_ChildNode, "HD_ChildNode" );
mGLOBAL_DECLARE( zPCHAR, szlHD_Diagram, "HD_Diagram" );
mGLOBAL_DECLARE( zPCHAR, szlHD_Node, "HD_Node" );
mGLOBAL_DECLARE( zPCHAR, szlHidden, "Hidden" );
mGLOBAL_DECLARE( zPCHAR, szlKey, "Key" );
mGLOBAL_DECLARE( zPCHAR, szlLISTBOX, "LISTBOX" );
mGLOBAL_DECLARE( zPCHAR, szlLPAREN, " (" );
mGLOBAL_DECLARE( zPCHAR, szlName, "Name" );
mGLOBAL_DECLARE( zPCHAR, szlOrientation, "Orientation" );
mGLOBAL_DECLARE( zPCHAR, szlOutsideText1, "OutsideText1" );
mGLOBAL_DECLARE( zPCHAR, szlOutsideText2, "OutsideText2" );
mGLOBAL_DECLARE( zPCHAR, szlOutsideText3, "OutsideText3" );
mGLOBAL_DECLARE( zPCHAR, szlOutsideText4, "OutsideText4" );
mGLOBAL_DECLARE( zPCHAR, szlOutsideText5, "OutsideText5" );
mGLOBAL_DECLARE( zPCHAR, szlPosX, "PosX" );
mGLOBAL_DECLARE( zPCHAR, szlPosY, "PosY" );
mGLOBAL_DECLARE( zPCHAR, szlPrintBox, "PrintBox" );
mGLOBAL_DECLARE( zPCHAR, szlPrintTitle, "PrintTitle" );
mGLOBAL_DECLARE( zPCHAR, szlProperties, "Properties" );
mGLOBAL_DECLARE( zPCHAR, szlPurpose, "Purpose" );
mGLOBAL_DECLARE( zPCHAR, szlRPAREN, ")" );
mGLOBAL_DECLARE( zPCHAR, szlSTATIC, "STATIC" );
mGLOBAL_DECLARE( zPCHAR, szlSubtype, "Subtype" );
mGLOBAL_DECLARE( zPCHAR, szlTZRELTGT, "TZRELTGT" );
mGLOBAL_DECLARE( zPCHAR, szlTag, "Tag" );
mGLOBAL_DECLARE( zPCHAR, szlText, "Text" );
mGLOBAL_DECLARE( zPCHAR, szlTitle, "Title" );
mGLOBAL_DECLARE( zPCHAR, szlType, "Type" );
mGLOBAL_DECLARE( zPCHAR, szlVN, "VN" );
mGLOBAL_DECLARE( zPCHAR, szlWnd, "Wnd" );
mGLOBAL_DECLARE( zPCHAR, szlZKey, "ZKey" );
mGLOBAL_DECLARE( zPCHAR, szlZeidonOL, "ZeidonOL" );
