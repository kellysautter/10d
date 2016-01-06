/********************************************************************************************/
//
// File:            tzvmledt.h
// *******************************************************************
// * Copyright © 2002 QuinSoft Corporation and TONBELLER AG.         *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           12. März 2002
// describtion:
//
// list of classes :
// Name:             describtion:
// ------------------------------------------------------------------------------------
//
//
//
/* Change log most recent first order:


*/
/*********************************************************************************************/
#ifndef __TZVMLEDT_H__
#define __TZVMLEDT_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ZDCTL_CLASS
#define ZDCTL_CLASS AFX_EXT_CLASS
#endif

#define VC_EXTRALEAN
#include <kzoegdef.h>

zEXTERN HINSTANCE  g_hInstanceDLL;

static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };
#include "zdrvropr.h"   // Added by ClassView
#include "Zdr.h"
#include "Zdctl.h"

#include "tzcswrpr.h"
#include "zviewdef.h"
#include "zcmddef.h"   // Added by ClassView

class ZVMLWrap;

typedef ZVMLWrap            * LPVMLWRAPPER;

class ZDCTL_CLASS ZVMLWrap : public ICodeSense
{
public: // ctors
   ZVMLWrap () {}      // Calls COleDispatchDriver default constructor
   ZVMLWrap (LPDISPATCH pDispatch) : ICodeSense(pDispatch) {}
   ZVMLWrap (const ZVMLWrap & dispatchSrc) : ICodeSense(dispatchSrc) {}
   ZVMLWrap ( ZActiveX* pzma, LPDISPATCH pDispatch) : ICodeSense(pDispatch)
   {
      m_pZActiveX = pzma;
      Init();
      InitEventList();
   }
     ~ZVMLWrap ();

private: //debugging helpers
   void TraceExceptionError( CException *e, CString strExtra );
   LPCSTR GetControlTag();

private:

public: //Methods
   void ExecuteCmd(long lCommand );
   void ResetCommands();
   void ResetViewDeclarations();
   void ResetLods();
   void AddCommandToComplete( zCPCHAR strText,
                              zCPCHAR strType,
                              zCPCHAR strInsertText,
                              zCPCHAR strInsertTextFull,
                              zCPCHAR strCC_Code,
                              zBOOL   bCC_FirstChar );

   ZVMLCommandSet m_mapCommandSet;
   void SelectAll();
   void GetObjectName( zPCHAR lpszFileName );
   zBOOL FileOpen(zPCHAR strFileName);
   zBOOL FileSave();
   // used to call an event handler assigned to an event
   zSHORT   HandleEvent(  DISPID dispEvent );
   // tell Zeidon control which events are handled by this DLL
   void     WantHandleEvents( );

   // old stuff implemented for backward compatibility QUICK HACK
   void GetCursorPosition( zPLONG lMoveBaseLine,
                           zPLONG lMoveBaseColumn,
                           zPLONG lMoveBaseIndex);
   void SetCursorPositionByLine(zLONG lMoveBaseLine,
                                zLONG lMoveBaseColumn );
   void PropertyDialog();

   void SetFindFromCode( zBOOL bSet );
   zBOOL GetFindFromCode( );

   void SetFindFromCodeWrapped( zBOOL bSet );
   zBOOL GetFindFromCodeWrapped( );

   zLONG GetLastCmd(){ return m_lCommand; }
   zLONG GetLastCmdFailure(){ return m_lErrorCode; }

   void SetRedraw( BOOL bRedraw = TRUE );

   LPZLOD GetLodDefinition(CString strLodName);

   void SetCommandCompleteStyle( zCPCHAR szStyle );// "F" = full, anything else short

private: //methods
   void Init();
   void InitEventList();
   zBOOL PopulateFunctions( LPDISPATCH ListCtrl );

   void SelectClosestItemInCodeList( ICodeList *pCL );
   void SelectCurrentFunction();
   CString GetCurrentFunction( BOOL bMustExist = TRUE);
   int GetCurrentArgument();
   zBOOL CompleteCommand( CString strToken );

   // ini browsing functions
   CString GetListFromINI( zCPCHAR szSection, zCPCHAR szPath );
   CString GetScopeKeyWordsEnd( CString strFileName );
   CString GetScopeKeyWordsStart( CString strFileName );
   CString GetString( LPCTSTR lpszSection, zCPCHAR szKey, zCPCHAR szPath );

   // language registration
   void RegisterCustomLanguage();
   void RegisterVML( IGlobals *cm_Globals, ILanguage *cm_Language );
   void SetLanguageBasedOnFileType( zCPCHAR szPath );

   // LOD handling
   LPZLOD LoadLodDefinition  ( CString strObjectName );
   zBOOL  PopulateEntities   ( LPDISPATCH ListCtrl );
   zBOOL  PopulateAttributes ( LPDISPATCH ListCtrl );
   void   ShowEntityAttribute( zCPCHAR lpszView = NULL );

   zVIEW GetDialogView();
   CString GetPrototype( CString strFunc, int iOverload = 0 );
   BOOL GetViewDeclarations( );
   int ParseLineForListItems();

private: //Event Handlers

   zSHORT OnFindWrappedCodesense( LPDISPATCH Control, zLONG lCmd, BOOL  bForward, zPSHORT pbStop);
   zSHORT OnCmdFailureCodesense(  LPDISPATCH Control, zLONG lCmd, zLONG lErrCode, zPSHORT pbStop);
   zSHORT OnKeyDownCodesense(LPDISPATCH Control, zLONG KeyCode, zLONG Shift, zPSHORT pbStop);
   zSHORT OnCodeListCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zPSHORT bContinue);
   zSHORT OnCodeListCancelCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zPSHORT bKeepVisible);
   zSHORT OnCodeListCharCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zLONG wChar, zLONG lKeyData, zPSHORT bHandled);
   zSHORT OnCodeListSelChangeCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zLONG lItem, BSTR FAR* strTipText);
   zSHORT OnCodeListSelMadeCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zPSHORT bStop);
   zSHORT OnCodeListSelWordCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zLONG lItem, zPSHORT bAllowSelection);
   zSHORT OnCodeTipCodesense(LPDISPATCH Control, zPLONG ToolTipType);
   zSHORT OnCodeTipCancelCodesense(LPDISPATCH Control, LPDISPATCH ToolTipCtrl, zPSHORT bKeepVisible);
   zSHORT OnCodeTipUpdateCodesense(LPDISPATCH Control, LPDISPATCH ToolTipCtrl);


private: //variables
   CString m_strCompletionType;
   CString      m_strIniPath;  // full qualified path to language ini
   CString      m_strFileName; // name of the file being edited

   ZActiveX* m_pZActiveX; // pointer back to Zeidon control

   ICodeList *m_pCL;      // codelist listbox
    IPosition m_posSel;


   zBOOL m_bFindFromCode;
   zBOOL m_bFindFromCodeWrapped;
   zLONG m_lCommand;   // in case of an error situation last command invoked by ExecuteCmd
   zLONG m_lErrorCode; // in case of an error situation last error due command invokation

   CString    m_strTipFunc;
   CImageList m_ImageList;

   enum ListState {
       ListFunctions,
       ListEntities,
       ListAttributes,
   } m_ListState;

   ZLodSet     m_mapLods;     // a list of LODS form current project
   LPZLOD      m_pCurrentLod;
   LPZENTITY   m_pCurrentEntity;
   ZInstanceSet m_mapViewDeclarations;    // a list of variables referencing an Object instance (LOD)

};


#endif //__TZVMLEDT_H__
