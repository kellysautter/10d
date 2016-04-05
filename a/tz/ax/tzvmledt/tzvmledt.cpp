/********************************************************************************************/
//
// File:            tzvmledt.cpp
// *******************************************************************
// * Copyright © 2002 - 2016 QuinSoft, Inc. and TONBELLER AG.        *
// * All rights reserved.                                            *
// * Confidential and Proprietary material subject to license -      *
// * do not reproduce or disclose. This material is an unpublished   *
// * work and is considered a trade secret belonging to the          *
// * copyright holder.                                               *
// *******************************************************************
// Autor:           TMV
// Datum:           12. März 2002
// description:
//
//
/* Change log most recent first order:


*/
/*********************************************************************************************/
#include "zstdafx.h"
#include "tzvmledt.h"
#include "tzvmlcst.h" // constants, enums, etc
#include "zstring.h"

#include "tzlodopr.h"
#include "tz__oprs.h"


#ifdef __cplusplus
extern "C"
{
#endif
   #include "zeidonop.h"   //to force explicit c-linkage
#ifdef __cplusplus
}
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static zCPCHAR TZOPSIGK_FILE     = "TZOPSIGK.XGO";

static zCPCHAR g_pszFilter = "VML files (*.VML)|*.vml|"\
                              "C/C++ files (*.cpp,*.c,*.h,*.hpp)|*.cpp;*.c;*.h;*.hpp|"\
                              "Java files (*.java)|*.java|"\
                              "Web files (*.htm,*.html,*.htx,*.asp,*.alx,*.stm,*.shtml)|*.htm;*.html;*.htx;*.asp;*.alx;*.stm;*.shtml|"\
                              "XML files (*.xml)|*.xml|"\
                              "Basic files (*.bas,*.inc)|*.bas;*.inc|"\
                              "Pascal files (*.pas)|*.pas|"\
                              "SQL files (*.sql,*.ddl)|*.sql;*.ddl|"\
                              "Text files (*.txt,*.asc,*.csv,*.tab)|*.txt;*.asc;*.csv;*.tab|"\
                              "All files (*.*)|*.*||";


//===========================================================================
// Following structure contains a list of all standard VML insert strings.
// It is used only by VML_BuildTextList
//===========================================================================


extern "C" void WINAPI
ZeidonInitTZVMLEDIT( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                     zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
   new CDynLinkLibrary( extensionDLL );
}

//./ ADD NAME=TZVMLEditScript
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:      TZVMLEditScript
//                                            3/13/2002 Modified
//  PURPOSE:    Drivers entrypoint into the codesense implementation
//
//  PARAMETERS: ZMapAct *pzma  - pointer back to Zeidon Control instance
//              zLONG lFlags   - used to pass additional information
//                               for SetCtrlState
//              zULONG ulParm  - used to pass request specific data
//              zLONG lRequest - request of script invocation
//
//  RETURN    : zSHORT
/**************************************************************************/
//./END + 2
extern "C" zOPER_EXPORT zSHORT OPERATION
TZVMLEditScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
   zSHORT nRC = 0;
   CWnd *pWnd = NULL;

   if ( pzma == NULL )
   {
      switch ( lRequest)
      {
         case zACTIVEX_GET_ATTRIBUTE_COUNT:
         case zACTIVEX_CREATE_ZEIDON_CONTROL:
            pWnd = ( CWnd* ) ulParm;
            if ( !pWnd )
            {
               return(0);
            }
            break;
         default:
            return (0);
            break;
      }
   }

   if ( lRequest == zACTIVEX_INITIALIZE )
   {
      // Default Zeidon Implementation
      // Never change this code unless you want to crash!!!!!
      IDispatch *pIDispatch = pzma->GetDispatchInterface();
      if ( pIDispatch )
      {
         LPVMLWRAPPER pWrapperInstance =
                        new ZVMLWrap( (ZActiveX *) pzma, pIDispatch );
         if ( pWrapperInstance )
         {

            nRC = 0;
         }
      }

      LPVMLWRAPPER pWrapperInstance =
                             (LPVMLWRAPPER ) pzma->GetWrapperInstance( );
      if ( pWrapperInstance )
      {
         pWrapperInstance->WantHandleEvents( );
      }

      return nRC;
   }
   else
   if ( lRequest == zACTIVEX_UNINITIALIZE )
   {
      // Default Zeidon Implementation
      // Never change this code unless you want to crash!!!!!
      // delete wrapper class instance
      LPVMLWRAPPER pWrapperInstance =
                             (LPVMLWRAPPER ) pzma->GetWrapperInstance( );
      if ( pWrapperInstance )
      {
         delete pWrapperInstance;
         pWrapperInstance = NULL;
         pzma->SetWrapperInstance( NULL );
         return( 0 );
      }
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_FROM_OI )
   {
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_TO_OI )
   {
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )
   {
      return ( ( zSHORT ) 0 );
   }
   else
   if ( lRequest == zACTIVEX_CREATE_ZEIDON_CONTROL )
   {
      return nRC;
   }
   else
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )
   {
      LPVMLWRAPPER pWrapperInstance = (LPVMLWRAPPER )pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // handle get focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_LOSE_FOCUS )
   {
      LPVMLWRAPPER pWrapperInstance = (LPVMLWRAPPER )pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // handle lose focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_SET_CTRL_STATE )
   {
      // handle SetCtrlState here
      zSHORT nRC = 0;

      LPVMLWRAPPER pWrapperInstance = (LPVMLWRAPPER )pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( nRC );

//      nRC = (zSHORT) pWrapperInstance->SetZCtrlState( lFlags, ulParm );
      return nRC;

   }
   else
   if ( lRequest == zACTIVEX_GET_CTRL_STATE )
   {
      // handle GetCtrlState here
      return( ( zSHORT )pzma->ZMapAct::GetZCtrlState( lFlags ) );
   }
   else
   if ( lRequest == zACTIVEX_HANDLE_EVENT )
   {
      // if we get a zACTIVEX_HANDLE_EVENT lFlags contains the dispatch id of the event.
      LPVMLWRAPPER pWrapperInstance = (LPVMLWRAPPER )pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      return pWrapperInstance->HandleEvent( (DISPID)lFlags);
   }
   return( nRC );
}


ZVMLWrap::~ZVMLWrap()
{
   ResetCommands();
   ResetLods();
}


void
ZVMLWrap::Init()
{

   m_bFindFromCode = FALSE;
   m_bFindFromCodeWrapped = FALSE;

   m_lCommand   = 0;
   m_lErrorCode = 0;

   m_pCL = NULL;
   m_pCurrentLod = NULL;
   m_pCurrentEntity = NULL;

   m_ListState = ZVMLWrap::ListFunctions;

   m_strCompletionType = "F";

   if ( m_pZActiveX )
      m_pZActiveX ->SetWrapperInstance( this );

   GetModuleFileName( NULL, m_strIniPath.GetBuffer( MAX_PATH ), MAX_PATH );
   m_strIniPath.ReleaseBuffer();

   int i = m_strIniPath.ReverseFind( L'\\' );
   ASSERT( i != -1 );

   m_strIniPath = m_strIniPath.Left( i + 1 ) + _T("VMLLANG.INI");

#ifdef _DEBUG

   CFileFind f;

   // If this ASSERT fails, you need to make sure the functions.ini file is
   // in the same directory as the application executable.
   ASSERT( f.FindFile( m_strIniPath ) );

#endif

   SetEnableCRLF( TRUE );
   RegisterCustomLanguage();

   TRY
   {
      m_posSel.CreateDispatch( _T("CodeSense.Position") );
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ZVMLWrap::Init()");
   }
   END_CATCH_ALL

}

void
ZVMLWrap::InitEventList()
{
}


zSHORT
ZVMLWrap::HandleEvent( DISPID dispEvent )
{

   zSHORT nRC = zACTIVEX_EVENT_COMPLETED;

   zLONG lRowColChange = 0;

   AFX_EVENT *pEvent = (AFX_EVENT *) ((zCTRL_EVENT*)(m_pZActiveX->m_pZSubtask->m_pEventParms))->m_pEvent;

   if ( pEvent->m_eventKind != AFX_EVENT::event )
   {
      //TRACE0("SHIT HAPPENS\n");
      return zACTIVEX_EVENT_COMPLETED; // nothing to do
   }

   switch ( dispEvent )
   {
   case DISPID_Change           :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_HScroll          :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_VScroll          :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_SelChange        :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_ViewChange       :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_ModifiedChange   :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_ShowProps        :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_PropsChange      :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_Create           :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_Destroy          :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_SetFocus         :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_KillFocus        :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_Click            :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_DblClick         :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_RDblClick        :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_Return           :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_RClick           :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_DeleteLine       :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_CmdFailure       :
      {
         LPDISPATCH Control = pEvent->m_pDispParams->rgvarg[3].pdispVal;
         zLONG lCmd         = pEvent->m_pDispParams->rgvarg[2].lVal;
         zLONG lErrCode     = pEvent->m_pDispParams->rgvarg[1].lVal;
         zPSHORT pbStop     = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCmdFailureCodesense(Control, lCmd, lErrCode, pbStop);
      }
      break;
   case DISPID_RegisteredCmd    :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_KeyDown          :
      {
         LPDISPATCH Control = pEvent->m_pDispParams->rgvarg[3].pdispVal;
         zLONG      KeyCode = pEvent->m_pDispParams->rgvarg[2].lVal;
         zLONG      Shift   = pEvent->m_pDispParams->rgvarg[1].lVal;
         zPSHORT    pbStop  = pEvent->m_pDispParams->rgvarg[0].piVal;

         nRC = OnKeyDownCodesense(Control, KeyCode,  Shift, pbStop);
      }
      break;
   case DISPID_KeyUp            :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_KeyPress         :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_MouseDown        :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_MouseMove        :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_MouseUp          :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_OvertypeChange   :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_FindWrapped      :
      {
         LPDISPATCH Control = pEvent->m_pDispParams->rgvarg[3].pdispVal;
         zLONG lCmd         = pEvent->m_pDispParams->rgvarg[2].lVal;
         BOOL bForward      = pEvent->m_pDispParams->rgvarg[1].iVal;
         zPSHORT pbStop     = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnFindWrappedCodesense( Control, lCmd, bForward, pbStop );
      }
      break;
   case DISPID_CodeList         :
      {
         LPDISPATCH Control    = pEvent->m_pDispParams->rgvarg[2].pdispVal;
         LPDISPATCH ListCtrl   = pEvent->m_pDispParams->rgvarg[1].pdispVal;
         zPSHORT    pbContinue = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCodeListCodesense(Control, ListCtrl, pbContinue);
      }
      break;
   case DISPID_CodeListSelMade  :
      {
         LPDISPATCH Control    = pEvent->m_pDispParams->rgvarg[2].pdispVal;
         LPDISPATCH ListCtrl   = pEvent->m_pDispParams->rgvarg[1].pdispVal;
         zPSHORT    pbStop     = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCodeListSelMadeCodesense  (Control, ListCtrl, pbStop);
      }
      break;
   case DISPID_CodeListCancel   :
      {
         LPDISPATCH Control      = pEvent->m_pDispParams->rgvarg[2].pdispVal;
         LPDISPATCH ListCtrl     = pEvent->m_pDispParams->rgvarg[1].pdispVal;
         zPSHORT    bKeepVisible = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCodeListCancelCodesense   (Control, ListCtrl, bKeepVisible);
      }
      break;
   case DISPID_CodeListChar     :
      {
         LPDISPATCH Control   = pEvent->m_pDispParams->rgvarg[4].pdispVal;
         LPDISPATCH ListCtrl  = pEvent->m_pDispParams->rgvarg[3].pdispVal;
         zLONG      wChar     = pEvent->m_pDispParams->rgvarg[2].lVal;
         zLONG      lKeyData  = pEvent->m_pDispParams->rgvarg[1].lVal;
         zPSHORT    pbHandled = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCodeListCharCodesense (Control, ListCtrl, wChar, lKeyData, pbHandled);
      }
      break;
   case DISPID_CodeTip          :
      {
         LPDISPATCH Control     = pEvent->m_pDispParams->rgvarg[1].pdispVal;
         zPLONG     ToolTipType = pEvent->m_pDispParams->rgvarg[0].plVal;
         nRC = OnCodeTipCodesense (Control, ToolTipType);
      }
      break;
   case DISPID_CodeTipCancel    :
      {
         LPDISPATCH Control      = pEvent->m_pDispParams->rgvarg[2].pdispVal;
         LPDISPATCH ToolTipCtrl  = pEvent->m_pDispParams->rgvarg[1].pdispVal;
         zPSHORT    bKeepVisible = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCodeTipCancelCodesense    (Control, ToolTipCtrl, bKeepVisible);
      }
      break;
   case DISPID_CodeTipUpdate    :
      {
         LPDISPATCH Control      = pEvent->m_pDispParams->rgvarg[1].pdispVal;
         LPDISPATCH ToolTipCtrl  = pEvent->m_pDispParams->rgvarg[0].pdispVal;
         nRC = OnCodeTipUpdateCodesense (Control, ToolTipCtrl);
      }
      break;
   case DISPID_CodeListSelWord  :
      {
         LPDISPATCH Control          = pEvent->m_pDispParams->rgvarg[3].pdispVal;
         LPDISPATCH ListCtrl         = pEvent->m_pDispParams->rgvarg[2].pdispVal;
         zLONG      lItem            = pEvent->m_pDispParams->rgvarg[1].lVal;
         zPSHORT    pbAllowSelection = pEvent->m_pDispParams->rgvarg[0].piVal;
         nRC = OnCodeListSelWordCodesense  (Control, ListCtrl, lItem, pbAllowSelection);
      }
      break;
   case DISPID_CodeListSelChange:
      {
         LPDISPATCH Control     = pEvent->m_pDispParams->rgvarg[3].pdispVal;
         LPDISPATCH ListCtrl    = pEvent->m_pDispParams->rgvarg[2].pdispVal;
         zLONG      lItem       = pEvent->m_pDispParams->rgvarg[1].lVal;
         BSTR       *strTipText = pEvent->m_pDispParams->rgvarg[0].pbstrVal;
         nRC = OnCodeListSelChangeCodesense(Control, ListCtrl, lItem, strTipText);
      }
      break;
   case DISPID_CodeListHotTrack :
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   case DISPID_CodeTipInitialize:
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
      break;
   default:
      nRC = zACTIVEX_EVENT_CONTINUE; // call user provided code
   break;
   }




   return nRC;
}


void
ZVMLWrap::WantHandleEvents()
{
   // this DLL handles the events below
   m_pZActiveX->RegisterEvent( DISPID_Change            );
   m_pZActiveX->RegisterEvent( DISPID_HScroll           );
   m_pZActiveX->RegisterEvent( DISPID_VScroll           );
   m_pZActiveX->RegisterEvent( DISPID_SelChange         );
   m_pZActiveX->RegisterEvent( DISPID_ViewChange        );
   m_pZActiveX->RegisterEvent( DISPID_ModifiedChange    );
   m_pZActiveX->RegisterEvent( DISPID_ShowProps         );
   m_pZActiveX->RegisterEvent( DISPID_PropsChange       );
   m_pZActiveX->RegisterEvent( DISPID_Create            );
   m_pZActiveX->RegisterEvent( DISPID_Destroy           );
   m_pZActiveX->RegisterEvent( DISPID_SetFocus          );
   m_pZActiveX->RegisterEvent( DISPID_KillFocus         );
   m_pZActiveX->RegisterEvent( DISPID_Click             );
   m_pZActiveX->RegisterEvent( DISPID_DblClick          );
   m_pZActiveX->RegisterEvent( DISPID_RDblClick         );
   m_pZActiveX->RegisterEvent( DISPID_Return            );
   m_pZActiveX->RegisterEvent( DISPID_RClick            );
   m_pZActiveX->RegisterEvent( DISPID_DeleteLine        );
   m_pZActiveX->RegisterEvent( DISPID_CmdFailure        );
   m_pZActiveX->RegisterEvent( DISPID_RegisteredCmd     );
   m_pZActiveX->RegisterEvent( DISPID_KeyDown           );
   m_pZActiveX->RegisterEvent( DISPID_KeyUp             );
   m_pZActiveX->RegisterEvent( DISPID_KeyPress          );
   m_pZActiveX->RegisterEvent( DISPID_MouseDown         );
   m_pZActiveX->RegisterEvent( DISPID_MouseMove         );
   m_pZActiveX->RegisterEvent( DISPID_MouseUp           );
   m_pZActiveX->RegisterEvent( DISPID_OvertypeChange    );
   m_pZActiveX->RegisterEvent( DISPID_FindWrapped       );
   m_pZActiveX->RegisterEvent( DISPID_CodeList          );
   m_pZActiveX->RegisterEvent( DISPID_CodeListSelMade   );
   m_pZActiveX->RegisterEvent( DISPID_CodeListCancel    );
   m_pZActiveX->RegisterEvent( DISPID_CodeListChar      );
   m_pZActiveX->RegisterEvent( DISPID_CodeTip           );
   m_pZActiveX->RegisterEvent( DISPID_CodeTipCancel     );
   m_pZActiveX->RegisterEvent( DISPID_CodeTipUpdate     );
   m_pZActiveX->RegisterEvent( DISPID_CodeListSelWord   );
   m_pZActiveX->RegisterEvent( DISPID_CodeListSelChange );
   m_pZActiveX->RegisterEvent( DISPID_CodeListHotTrack  );
   m_pZActiveX->RegisterEvent( DISPID_CodeTipInitialize );
}


//./ ADD NAME=ZVMLWrap::RegisterCustomLanguage
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::RegisterCustomLanguage
//                                            3/13/2002 Modified
//  PURPOSE:
//
//  PARAMETERS:
//
//  RETURN    :
/**************************************************************************/
//./END + 1
void ZVMLWrap::RegisterCustomLanguage()
{
   IGlobals    *cm_Globals  = NULL;
   ILanguage   *cm_Language = NULL;

   TRY
   {
      cm_Language = new ILanguage(  );
      if ( cm_Language )
         cm_Language->CreateDispatch( _T("CodeSense.Language")  );

      cm_Globals = new IGlobals();
      if ( cm_Globals )
      {
         cm_Globals->CreateDispatch( _T("CodeSense.Globals") );
      }

      if ( cm_Globals && cm_Language )
      {
         RegisterVML( cm_Globals, cm_Language );
      }

      if ( cm_Globals )
         delete cm_Globals;
      if ( cm_Language )
         delete cm_Language;
   }
   CATCH_ALL( e )
   {
      e->ReportError();
   }
   END_CATCH_ALL
}


//./ ADD NAME=ZVMLWrap::RegisterVML
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::RegisterVML
//                                            3/13/2002 Modified
//  PURPOSE:
//
//  PARAMETERS:  CGlobals *cm_Globals -
//              CLanguage *cm_Language -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
void ZVMLWrap::RegisterVML( IGlobals *cm_Globals, ILanguage *cm_Language )
{
   CString strKeyWords =  GetListFromINI( _T("Keyword"), m_strIniPath );
   CString strOperators = GetListFromINI( _T("Operator"), m_strIniPath );

   CString strSingleLineComment = GetString( "Comment", "SingleLine", m_strIniPath );
   CString strMultiLineCommentStart = GetString( "Comment", "MultiLineStart", m_strIniPath );
   CString strMultiLineCommentEnd   = GetString( "Comment", "MultiLineEnd", m_strIniPath );

   CString strStringDelimiter = GetString( "String", "Delimiter", m_strIniPath );

   // Number of start scope keywords must match
   // the number end scope key words
   // otherwise the language will not register
   CString strScopeKeyWordsStart = GetScopeKeyWordsStart( m_strIniPath );
   CString strScopeKeyWordsEnd   = GetScopeKeyWordsEnd( m_strIniPath );


   CString strCase = GetString( "Common", "CaseSensitive", m_strIniPath );
   cm_Language->SetCaseSensitive( (strCase.CompareNoCase("Y") == 0) ? TRUE : FALSE );
   CString strLanguageStyle = GetString( "Common", "LanguageStyle", m_strIniPath );
   cm_Language->SetStyle( (strLanguageStyle.CompareNoCase("P") == 0)
                            ? cmLangStyleProcedural
                            : cmLangStyleSGML );

   cm_Language->SetKeywords( strKeyWords );
   cm_Language->SetOperators( strOperators );
   cm_Language->SetSingleLineComments( strSingleLineComment );
   cm_Language->SetMultiLineComments1( strMultiLineCommentStart );
   cm_Language->SetMultiLineComments2( strMultiLineCommentEnd );

   cm_Language->SetStringDelims( strStringDelimiter );
   cm_Language->SetScopeKeywords1( GetScopeKeyWordsStart( m_strIniPath ));
   cm_Language->SetScopeKeywords2( GetScopeKeyWordsEnd( m_strIniPath ));

   cm_Language->SetTerminatorChar("");

   CString strLanguageName = GetString( "Common", "Name", m_strIniPath );
   cm_Globals->RegisterLanguage( strLanguageName, cm_Language->m_lpDispatch );
}


//./ ADD NAME=ZVMLWrap::GetListFromINI
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetListFromINI
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS:  zCPCHAR szSection -
//              zCPCHAR szPath -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
CString ZVMLWrap::GetListFromINI( zCPCHAR szSection, zCPCHAR szPath )
{
   CString key;
   CString strFormat = szSection;
   CString strReturn;
   strFormat+=_T("%d");


   CString strValue = " ";
   for( int iIndex = 1; !strValue.IsEmpty(); iIndex++ )
   {
      key.Format( strFormat, iIndex );
      strValue = GetString( szSection, key, szPath );
      if ( !strValue.IsEmpty() )
      {
         if ( !strReturn.IsEmpty() )
         {
            strReturn += _T("\n");
            strReturn += strValue;
         }
         else
            strReturn += strValue;
      }
   }
   return strReturn;
}


//./ ADD NAME=ZVMLWrap::GetString
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetString
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: zCPCHAR lpszSection -
//              zCPCHAR lpszKey -
//              zCPCHAR szPath -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
CString ZVMLWrap::GetString( zCPCHAR lpszSection, zCPCHAR lpszKey, zCPCHAR szPath )
{
   CString strValue = " ";
   GetPrivateProfileString( lpszSection, lpszKey, _T(""),
                            strValue.GetBuffer( 1000 ), 1000, szPath );
   strValue.ReleaseBuffer();
   return strValue;
}



//./ ADD NAME=ZVMLWrap::SetLanguageBasedOnFileType
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::SetLanguageBasedOnFileType
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: zCPCHAR szPath -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
void ZVMLWrap::SetLanguageBasedOnFileType( zCPCHAR szPath )
{
   // View Manipilation Language
   zCPCHAR pszLang = NULL;

   // based on the file name extension, set the language
   zCPCHAR pszDot = _tcsrchr( szPath, '.' );
   if ( pszDot )
   {

      TCHAR szExt[ _MAX_PATH ];
      _tcscpy( szExt, pszDot + 1 );
      CharUpper( szExt );

      if ( !_tcscmp( szExt, _T("C") ) || !_tcscmp( szExt, _T("CPP") ) ||
          !_tcscmp( szExt, _T("H") ) || !_tcscmp( szExt, _T("HPP") ) )
      {
         // C/C++
         pszLang = CMLANG_CPP;
      }
      else if ( !_tcscmp( szExt, _T("JAVA") ) )
      {
         // Java
         pszLang = CMLANG_JAVA;
      }
      else if ( !_tcscmp( szExt, _T("BAS") ) || !_tcscmp( szExt, _T("INC") ) )
      {
         // Basic
         pszLang = CMLANG_BASIC;
      }
      else if ( !_tcscmp( szExt, _T("PAS") ) )
      {
         // Pascal
         pszLang = CMLANG_PASCAL;
      }
      else if ( !_tcscmp( szExt, _T("SQL") ) || !_tcscmp( szExt, _T("DDL") ) )
      {
         // SQL
         pszLang = CMLANG_SQL;
      }
      else if ( !_tcscmp( szExt, _T("HTM") ) || !_tcscmp( szExt, _T("HTML") ) ||
                !_tcscmp( szExt, _T("HTX") ) || !_tcscmp( szExt, _T("ASP") ) ||
                !_tcscmp( szExt, _T("ALX") ) || !_tcscmp( szExt, _T("STM") ) ||
                !_tcscmp( szExt, _T("SHTML") ) )
      {
         // HTML
         pszLang = CMLANG_HTML;
      }
      else if ( !_tcscmp( szExt, _T("XML") ) )
      {
         // XML
         pszLang = CMLANG_XML;
      }
      else if ( !_tcscmp( szExt, _T("VML") ) )
      {
         // View Manipilation Language
         pszLang = CMLANG_VML;
      }
   }
   if ( ::_tcsclen( szPath ) == 0 )
      pszLang = CMLANG_VML;

   if ( pszLang )
   {
      SetLanguage( pszLang );
      SetColorSyntax( TRUE );
      if ( _tcscmp( pszLang, CMLANG_VML ) == 0 )
         SetNormalizeCase( FALSE );
   }
}


//./ ADD NAME=ZVMLWrap::GetScopeKeyWordsStart
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetScopeKeyWordsStart
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: CString strFileName -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
CString ZVMLWrap::GetScopeKeyWordsStart( CString strFileName )
{
   CString strScopeSection = "ScopeKeyWord";
   CString key;
   CString strFormat = strScopeSection;
   CString strReturn;
   CString strKeyWordSection;
   strFormat+=_T("%d");


   strKeyWordSection = " ";
   for( int iIndex = 1; !strKeyWordSection.IsEmpty(); iIndex++ )
   {
      key.Format( strFormat, iIndex );
      strKeyWordSection = GetString( strScopeSection, key, strFileName );
      if ( !strKeyWordSection.IsEmpty() )
      {
         CString strValue = GetString( strKeyWordSection, "KeyWordStart", strFileName );
         if ( !strReturn.IsEmpty() )
         {
            strReturn += _T("\n");
            strReturn += strValue;
         }
         else
            strReturn += strValue;
      }
   }
   return strReturn;
}


//./ ADD NAME=ZVMLWrap::GetScopeKeyWordsEnd
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetScopeKeyWordsEnd
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: CString strFileName -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
CString
ZVMLWrap::GetScopeKeyWordsEnd( CString strFileName )
{
   CString strScopeSection = "ScopeKeyWord";
   CString key;
   CString strFormat = strScopeSection;
   CString strReturn;
   CString strKeyWordSection;
   strFormat+=_T("%d");


   strKeyWordSection = " ";
   for( int iIndex = 1; !strKeyWordSection.IsEmpty(); iIndex++ )
   {
      key.Format( strFormat, iIndex );
      strKeyWordSection = GetString( strScopeSection, key, strFileName );
      if ( !strKeyWordSection.IsEmpty() )
      {
         CString strValue = GetString( strKeyWordSection, "KeyWordEnd", strFileName );
         if ( !strReturn.IsEmpty() )
         {
            strReturn += _T("\n");
            strReturn += strValue;
         }
         else
            strReturn += strValue;
      }
   }
   return strReturn;

}

void
ZVMLWrap::GetCursorPosition( zPLONG lMoveBaseLine,
                             zPLONG lMoveBaseColumn,
                             zPLONG lMoveBaseIndex)
{
   IRange range( GetSel( TRUE ) );
   *lMoveBaseLine = range.GetStartLineNo();
   *lMoveBaseColumn = range.GetStartColNo();
   *lMoveBaseIndex = 0;
}

void
ZVMLWrap::SetCursorPositionByLine(zLONG lMoveBaseLine,
                                  zLONG lMoveBaseColumn )
{
   TRY
   {
      SetCaretPos( lMoveBaseLine, lMoveBaseColumn );
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ZVMLWrap::SetCursorPositionByLine" );
   }
   END_CATCH_ALL

}

zBOOL ZVMLWrap::FileOpen( zPCHAR strFileName)
{
   zBOOL bOpen = FALSE;

   TRY
   {
      OpenFile( (zCPCHAR) strFileName );
      m_strFileName = strFileName;
      SetLanguageBasedOnFileType( m_strFileName );
      bOpen = TRUE;
   }
   CATCH_ALL( e )
   {
      m_strFileName.Empty();
      TraceExceptionError( e, "ZVMLWrap::FileOpen" );
   }
   END_CATCH_ALL

   return bOpen;
}

zBOOL ZVMLWrap::FileSave( )
{
   zBOOL bSaved = FALSE;

   TRY
   {
      ICodeSense::SaveFile( m_strFileName, TRUE );
      bSaved = TRUE;
   }
   CATCH_ALL( e )
   {
      TraceExceptionError( e, "ZVMLWrap::FileSave" );
   }
   END_CATCH_ALL

   return bSaved;
}

void ZVMLWrap::TraceExceptionError(CException *e, CString strExtra )
{
   CString strTitle = "<ZVMLWrap Exception> " + strExtra + " ";

   CString strErrorMessage;
   LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
   e->GetErrorMessage( lpErrorMessage, 1024 );
   strErrorMessage.ReleaseBuffer();
   strErrorMessage = "   " + strErrorMessage;

   TraceLineS( strTitle, GetControlTag() );
   TraceLineS( strErrorMessage, "");
}


/**************************************************************************/
// Function name   : ZVMLWrap::GetControlTag
// Description     : returns the controls tag
// Return type     : LPCSTR
/**************************************************************************/
LPCSTR ZVMLWrap::GetControlTag()
{
   CString strHelp = "<Unknown>";
   if ( m_pZActiveX )
   {
      strHelp = *m_pZActiveX->m_pzsTag;
   }
   return strHelp;
}

void ZVMLWrap::GetObjectName( zPCHAR pchFileName )
{
   strcpy_s( pchFileName, MAX_PATH + 1, m_strFileName );
}


void ZVMLWrap::PropertyDialog( )
{
   CRect rect;

   LPUNKNOWN pUnknown = NULL;
   LPOLEOBJECT lpObject = NULL;

   if ( lpObject == NULL )
   {
      if ( m_lpDispatch )
      {
         HRESULT hr = m_lpDispatch->QueryInterface(IID_IOleObject, (LPVOID *)&lpObject);
         if (!SUCCEEDED (hr))
            lpObject = NULL;
      }
   }

   if ( lpObject )
   {
      ((CWnd*)(m_pZActiveX))->GetClientRect( &rect );
      lpObject->DoVerb( OLEIVERB_PROPERTIES, 0, 0, 0,
                          ((CWnd*)(m_pZActiveX))->m_hWnd, &rect );
      if ( lpObject )
      {
         lpObject->Release();
         lpObject = NULL;
      }
   }
}

void ZVMLWrap::SelectAll()
{
   COleVariant v;
   ICodeSense::ExecuteCmd( cmCmdSelectAll, v );
}


zSHORT
ZVMLWrap::OnFindWrappedCodesense(LPDISPATCH Control,
                                zLONG lCmd,
                                BOOL bForward,
                                zPSHORT pbStop)
{
   // TODO: Add your control notification handler code here
   if ( m_bFindFromCode )
   {
      m_bFindFromCodeWrapped  = TRUE;
      *pbStop = TRUE;
   }
   return zACTIVEX_EVENT_CONTINUE;
}


zSHORT
ZVMLWrap::OnCmdFailureCodesense(LPDISPATCH Control,
                                zLONG lCmd,
                                zLONG lErrCode,
                                zPSHORT pbStop)
{
   // TODO: Add your control notification handler code here
   *pbStop = TRUE;
   m_lCommand   = lCmd;
   m_lErrorCode = lErrCode;
   return zACTIVEX_EVENT_CONTINUE;
}

zBOOL
ZVMLWrap::GetFindFromCode()
{
   return m_bFindFromCode;
}

void
ZVMLWrap::SetFindFromCode( zBOOL bSet )
{
   m_bFindFromCode = bSet;
}


zBOOL
ZVMLWrap::GetFindFromCodeWrapped()
{
   return m_bFindFromCodeWrapped;
}


void
ZVMLWrap::SetFindFromCodeWrapped( zBOOL bSet )
{
   m_bFindFromCodeWrapped = bSet;
}

void
ZVMLWrap::SetRedraw( BOOL bRedraw )
{
   if ( m_pZActiveX )
      m_pZActiveX->SetRedraw( bRedraw );
   if ( bRedraw )
   {
      SetHScrollVisible(FALSE);
      SetHScrollVisible(TRUE);
      SetVScrollVisible(FALSE);
      SetVScrollVisible(TRUE);
      m_pZActiveX->m_pWndParent->Invalidate();
      m_pZActiveX->Invalidate();
      m_pZActiveX->UpdateWindow();
      m_pZActiveX->m_pWndParent->UpdateWindow();
   }
}

LPZLOD
ZVMLWrap::LoadLodDefinition( CString strObjectName )
{
   LPZLOD lpLod = NULL;
   zSHORT nRC;
   zVIEW  vLOD_List;
   CHAR szLodName[ 255 ];
   CHAR szLodDesc[ 5000 ];

   zVIEW vSubtask = GetDialogView( );

   // Create a view that lists all LODs.
   RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
   if ( vLOD_List == 0 )
   {
      CString strHelpMessage = "Unable to retrieve LOD definitions. Check LPLR!";
      TraceLineS("TZVMLEDT", strHelpMessage );
   }
   else
   {
      // Loop through each of the LOD's WMetaType.Type= zREFER_LOD_META
      //
      BOOL bFound = FALSE;
      nRC = SetCursorFirstEntity( vLOD_List, "W_MetaDef", "" );
      while( nRC >= zCURSOR_SET && bFound == FALSE )
      {

         GetStringFromAttribute( szLodName, zsizeof( szLodName ), vLOD_List, "W_MetaDef", "Name" );
         if ( strObjectName.CompareNoCase( szLodName ) == 0 )
            bFound = TRUE;
         else
            nRC = SetCursorNextEntity( vLOD_List, "W_MetaDef", "" );
      }

      if ( bFound )
      {
         GetStringFromAttribute( szLodDesc, zsizeof( szLodDesc ), vLOD_List, "W_MetaDef", "Desc" );

         lpLod = new ZLod( szLodName, szLodDesc );
         m_mapLods[lpLod->m_strName] = lpLod;

         zVIEW vLOD = NULL;

         nRC = ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zCURRENT_OI );
         if ( nRC >= 0 )
         {
            // Copy Entity and Attribute names from LOD to vEdWrk.
            nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
            while ( nRC == zCURSOR_SET )
            {
               CHAR szEntityName[255];
               CHAR szEntityDesc[5000];
               GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vLOD, "LOD_Entity", "Name" );
               GetStringFromAttribute( szEntityDesc, zsizeof( szEntityDesc ), vLOD, "LOD_Entity", "Desc" );

               LPZENTITY lpEntity = new ZEntity( szEntityName, szEntityDesc);
               // append entity to lod
               lpLod->m_mapEntities[lpEntity->m_strName] = lpEntity;

               nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
               while ( nRC == zCURSOR_SET )
               {
                  CHAR szAttributeName[255];
                  CHAR szAttributeDesc[5000];
                  GetStringFromAttribute( szAttributeName, zsizeof( szAttributeName ), vLOD, "ER_Attribute", "Name" );
                  GetStringFromAttribute( szAttributeDesc, zsizeof( szAttributeDesc ), vLOD, "ER_Attribute", "Desc" );

                  LPZATTRIBUTE lpAttribute = new ZAttribute( szAttributeName, szAttributeDesc );
                  // append attribute to entity
                  lpEntity->m_mapAttributes[lpAttribute->m_strName] = lpAttribute;

                  nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
               }

               nRC = SetCursorNextEntity( vLOD, "LOD_Entity", "" );
            }
            DropMetaOI( vSubtask, vLOD );
         }
      }
   }

   return( lpLod );
}


LPZLOD
ZVMLWrap::GetLodDefinition( CString strLodName )
{
   LPZLOD lpLod = NULL;

   // get definition from cache
   // if not cached load definition from lplr and cache it
   if ( !m_mapLods.Lookup( strLodName, (CObject* &)lpLod ) )
   {
      lpLod = LoadLodDefinition( strLodName );
   }

   return lpLod;
}

//./ ADD NAME=ZVMLWrap::OnKeyDownCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnKeyDownCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              zLONG KeyCode -
//              zLONG Shift -
//              zPSHORT  pbStop -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnKeyDownCodesense(LPDISPATCH Control, zLONG KeyCode, zLONG Shift, zPSHORT  pbStop)
{
   // TODO: Add your control notification handler code here
   if ( KeyCode == VK_SPACE && Shift == 0 )
   {
      // check if previous word is a keyword
      // set it to upper case
      zLONG lToken = -1;
      CString strHelp =  GetCurrentWord();

      TRY
      {
         lToken =  GetCurrentToken();
         switch ( lToken )
         {
         case cmTokenTypeKeyword  :
            *pbStop = !CompleteCommand( strHelp );
            break;
         case cmTokenTypeOperator :
         case cmTokenTypeString   :
         case cmTokenTypeSingleLineComment :
         case cmTokenTypeMultiLineComment :
         case cmTokenTypeNumber :
         case cmTokenTypeScopeBegin :
         case cmTokenTypeScopeEnd :
            break;
         case cmTokenTypeText :
         case cmTokenTypeUnknown :
            *pbStop = !CompleteCommand( strHelp );
            break;
         default:
            break;
         }

      }
      CATCH( COleException, e )
      {
         lToken = -1; //newer interface
      }
      END_CATCH
   }
   else if ( KeyCode == LOBYTE(VkKeyScan('(')) && Shift == HIBYTE(VkKeyScan('(')) )
   {
      ExecuteCmd( cmCmdCodeTip );
   }
   else   if ( KeyCode == LOBYTE(VkKeyScan('.')) && Shift == HIBYTE(VkKeyScan('.')) )
   {
      ShowEntityAttribute();
      *pbStop = FALSE;
   }

   return zACTIVEX_EVENT_COMPLETED;
}



//./ ADD NAME=ZVMLWrap::OnCodeListCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeListCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ListCtrl -
//              zPSHORT  bContinue -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeListCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zPSHORT  bContinue)
{
   // TODO: Add your control notification handler code here

   switch( m_ListState )
   {
   case ZVMLWrap::ListFunctions:
      *bContinue = PopulateFunctions( ListCtrl );
      break;
   case ZVMLWrap::ListEntities:
      *bContinue = PopulateEntities( ListCtrl );
      break;
   case ZVMLWrap::ListAttributes:
      *bContinue = PopulateAttributes( ListCtrl );
      break;
   default:
      *bContinue = FALSE;
      break;
   }

   return zACTIVEX_EVENT_COMPLETED;
}

zBOOL
ZVMLWrap::PopulateFunctions( LPDISPATCH ListCtrl )
{
   zVIEW vTZOPSIGK;
   zVIEW vSubtask;
   short nRC = 0;
   BOOL bReturn = TRUE;

   // Get list of Zeidon operations.
   zVIEW  vT;
   zCHAR  szXGO_FileName[ 129 ];

   nRC = SfCreateSubtask( &vT, 0, "Zeidon System" );
   if ( nRC == zCALL_ERROR )
   {
      bReturn = FALSE;
      return( bReturn );
   }

   nRC = GetApplDirectoryFromView( szXGO_FileName, vT, zAPPL_DIR_OBJECT, 125 );
   nRC = SfDropSubtask( vT, 0 );
   CString strHelp = szXGO_FileName;
   strHelp += TZOPSIGK_FILE;

   vSubtask = GetDialogView();

   if ( !vSubtask )
   {
      bReturn = FALSE;
      return( bReturn );
   }

   nRC = ActivateOI_FromFile( &vTZOPSIGK, "TZOPGRPO", vSubtask,
                              strHelp,
                              zSINGLE | zLEVEL_TASK );

   if ( nRC >= 0 )
      SetNameForView( vTZOPSIGK, "TZOPSIGK", vSubtask, zLEVEL_TASK );

    // Attach the list view control to our member variable so that we can use
   // it to update the selection as the user types.
   if ( m_pCL )
      delete m_pCL;

   m_pCL = new ICodeList( ListCtrl );
   m_pCL->m_bAutoRelease = FALSE;
   m_pCL->SetHImageList( long( m_ImageList.m_hImageList ) );

   // Attach our imagelist to the CodeList control.  Note that the list
   // control has the LVS_SHAREIMAGELISTS style set by default, so we don't
   // have to worry about detaching the imagelist later on.
   //
   // m_CodeList.SetImageList( &m_ImageList, LVSIL_SMALL );

   // Read the operation names from tzopsigk
   int i;
   for ( i = 0, nRC = SetCursorFirstEntity( vTZOPSIGK, "Operation", 0 );
         nRC == zCURSOR_SET && vTZOPSIGK != 0 ;
         nRC = SetCursorNextEntity( vTZOPSIGK, "Operation", 0 ), i++ )
   {
      char szFunc[255];
      GetStringFromAttribute( szFunc, zsizeof( szFunc ), vTZOPSIGK, "Operation", "Name" );
      COleVariant v;
      COleVariant vImageIndex( LONG(i%2) );
      m_pCL->AddItem( szFunc, vImageIndex, v );
   }

   // Try to select the item that matches any function name entered in the
   // CodeMax control window.
   if ( i > 0 )
   {
      SelectClosestItemInCodeList( m_pCL );
      // Allow CodeList control to be displayed
      bReturn = TRUE;
   }
   else
      bReturn = FALSE;

   return( bReturn );
}


zBOOL ZVMLWrap::PopulateEntities( LPDISPATCH ListCtrl )
{
   zBOOL bReturn = TRUE;

   if ( m_pCurrentLod == NULL )
   {
      bReturn = FALSE;
      return( bReturn );
   }

    // Attach the list view control to our member variable so that we can use
   // it to update the selection as the user types.
   if ( m_pCL )
      delete m_pCL;

   m_pCL = new ICodeList( ListCtrl );
   m_pCL->m_bAutoRelease = FALSE;
   // Attach our imagelist to the CodeList control.  Note that the list
   // control has the LVS_SHAREIMAGELISTS style set by default, so we don't
   // have to worry about detaching the imagelist later on.
   m_pCL->SetHImageList( long( m_ImageList.m_hImageList ) );


   // Read entity names from current lod object
   int i = 0;
   POSITION pos = m_pCurrentLod->m_mapEntities.GetStartPosition( );
   while (pos != NULL)
   {
      CString strKey;
      LPZENTITY lpEntity;
      m_pCurrentLod->m_mapEntities.GetNextAssoc( pos, strKey, (CObject*&)lpEntity );
      COleVariant v;
      COleVariant vImageIndex( LONG(i%2) );
      m_pCL->AddItem( lpEntity->m_strName, vImageIndex, v );
      i++;
   }

   // Try to select the item that matches any function name entered in the
   // CodeMax control window.
   if ( i > 0 )
   {
      SelectClosestItemInCodeList( m_pCL );
      // Allow CodeList control to be displayed
      bReturn = TRUE;
   }
   else
      bReturn = FALSE;

   return( bReturn );
}


zBOOL ZVMLWrap::PopulateAttributes( LPDISPATCH ListCtrl )
{
   zBOOL bReturn = TRUE;

   if ( m_pCurrentEntity == NULL )
   {
      bReturn = FALSE;
      return( bReturn );
   }

    // Attach the list view control to our member variable so that we can use
   // it to update the selection as the user types.
   if ( m_pCL )
      delete m_pCL;

   m_pCL = new ICodeList( ListCtrl );
   m_pCL->m_bAutoRelease = FALSE;
   m_pCL->SetHImageList( long( m_ImageList.m_hImageList ) );

   // Attach our imagelist to the CodeList control.  Note that the list
   // control has the LVS_SHAREIMAGELISTS style set by default, so we don't
   // have to worry about detaching the imagelist later on.

   // Read attribute names from current entity object
   int i = 0;
   POSITION pos = m_pCurrentEntity->m_mapAttributes.GetStartPosition( );
   while (pos != NULL)
   {
      CString strKey;
      LPZATTRIBUTE lpAttribute;
      m_pCurrentEntity->m_mapAttributes.GetNextAssoc( pos, strKey, (CObject*&)lpAttribute );
      COleVariant v;
      COleVariant vImageIndex( LONG(i%2) );
      m_pCL->AddItem( lpAttribute->m_strName, vImageIndex, v );
      i++;
   }

   // Try to select the item that matches any function name entered in the
   // CodeMax control window.
   if ( i > 0 )
   {
      SelectClosestItemInCodeList( m_pCL );
      // Allow CodeList control to be displayed
      bReturn = TRUE;
   }
   else
      bReturn = FALSE;

   return( bReturn );
}

//./ ADD NAME=ZVMLWrap::OnCodeListCancelCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeListCancelCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ListCtrl -
//              zPSHORT  bKeepVisible -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeListCancelCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zPSHORT  bKeepVisible)
{
   // TODO: Add your control notification handler code here
   if ( m_ListState == ZVMLWrap::ListAttributes )
      m_ListState = ZVMLWrap::ListFunctions;
   *bKeepVisible = FALSE;

   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeListCharCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeListCharCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ListCtrl -
//              zLONG wChar -
//              zLONG lKeyData -
//              zPSHORT  bHandled -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeListCharCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zLONG wChar, zLONG lKeyData, zPSHORT  bHandled)
{
   // TODO: Add your control notification handler code here
   *bHandled = FALSE;
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeListSelChangeCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeListSelChangeCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ListCtrl -
//              zLONG lItem -
//              BSTR FAR* strTipText -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeListSelChangeCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zLONG lItem, BSTR FAR* strTipText)
{
   // TODO: Add your control notification handler code here
   CString strItem, strToolTip;
   IRange range;

   TRY
   {
      if ( m_pCL )
      {
         // Determine which item was selected in the list
         zLONG lItem = m_pCL->GetSelectedItem();
         strItem = m_pCL->GetItemText( lItem );

         // Get the tooltip text...
         if ( m_ListState == ZVMLWrap::ListFunctions )
         {
            zVIEW vSubtask = GetDialogView();

            if ( vSubtask )
            {
               zVIEW vApp = 0;
               short nRC = GetViewByName( &vApp, "TZOPSIGK", vSubtask, zLEVEL_ANY );

               if ( nRC >= 0 )
                  nRC =  SetCursorFirstEntityByString( vApp, "Operation",
                                                       "Name", strItem, "" );
               if ( nRC == zCURSOR_SET )
               {
                  zULONG lDisplayLength;
                  GetAttributeDisplayLength( &lDisplayLength, vApp,
                                 "Operation", "Desc", 0 );
                  if ( lDisplayLength  < 1 )
                     lDisplayLength = 255;

                  LPSTR lpBuffer = strToolTip.GetBufferSetLength( lDisplayLength );

                  GetVariableFromAttribute( lpBuffer, 0, zTYPE_STRING,
                                                     lDisplayLength,
                                                     vApp,
                                                     "Operation", "Desc", "",
                                                     0 );
                  strToolTip.ReleaseBuffer();
               }
            }
         }
         if ( strToolTip.IsEmpty() )
            strToolTip = strItem;

         // set the tooltip text to be displayed ...
         *strTipText = strToolTip.AllocSysString( );
      }
   }
   CATCH_ALL( e )
   {
      e->ReportError();
   }
   END_CATCH_ALL
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeListSelMadeCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeListSelMadeCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ListCtrl -
//              zPSHORT  bStop -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeListSelMadeCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zPSHORT  bStop)
{
   // TODO: Add your control notification handler code here
   CString strItem;
   IRange range;
   *bStop = FALSE;

   SelectCurrentFunction();
   TRY
   {
      if ( m_pCL )
      {
         // Determine which item was selected in the list
         zLONG lItem = m_pCL->GetSelectedItem();
         strItem = m_pCL->GetItemText( lItem );

         // Replace current selection
          ReplaceSel (strItem);

         //Get new selection
         range =  GetSel( TRUE );

         // Update range to end of newly inserted text
         range.SetStartColNo( range.GetStartColNo() + strItem.GetLength() );
         range.SetEndColNo( range.GetStartColNo() );
         range.SetEndLineNo( range.GetStartLineNo() );

         // Move cursor
          SetSel( range, TRUE );
      }
   }
   CATCH_ALL( e )
   {
      e->ReportError();
   }
   END_CATCH_ALL

   delete m_pCL;
   m_pCL = NULL;

   if ( m_ListState == ZVMLWrap::ListAttributes )
      m_ListState = ZVMLWrap::ListFunctions;

   // Don't prevent list view control from being hidden
   *bStop = FALSE;
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeListSelWordCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeListSelWordCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ListCtrl -
//              zLONG lItem -
//              zPSHORT  bAllowSelection -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeListSelWordCodesense(LPDISPATCH Control, LPDISPATCH ListCtrl, zLONG lItem, zPSHORT  bAllowSelection)
{
   // TODO: Add your control notification handler code here
   // Allow the CodeList control to automatically select the item in the
   // list that most closely matches the current word.
   *bAllowSelection = TRUE;
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeTipCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeTipCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              zLONG FAR* ToolTipType -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeTipCodesense(LPDISPATCH Control, zLONG FAR* ToolTipType)
{
   // TODO: Add your control notification handler code here
   // We don't want to display a tip inside quoted or commented-out lines...

   if ((  GetCurrentToken() == cmTokenTypeText )
      || (  GetCurrentToken() == cmTokenTypeKeyword ))
     // We want to use the tooltip control that automatically
     // highlights the arguments in the function prototypes for us.
     // This type also provides support for overloaded function
     // prototypes.
     *ToolTipType = cmToolTipTypeMultiFunc;
   else
     // Don't display a tooltip
     *ToolTipType = cmToolTipTypeNone;
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeTipCancelCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeTipCancelCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ToolTipCtrl -
//              zPSHORT  bKeepVisible -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeTipCancelCodesense(LPDISPATCH Control, LPDISPATCH ToolTipCtrl, zPSHORT  bKeepVisible)
{
   // TODO: Add your control notification handler code here
   // set back to default
   m_ListState = ZVMLWrap::ListFunctions;
   *bKeepVisible = FALSE;
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::OnCodeTipUpdateCodesense
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::OnCodeTipUpdateCodesense
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: LPDISPATCH Control -
//              LPDISPATCH ToolTipCtrl -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zSHORT ZVMLWrap::OnCodeTipUpdateCodesense(LPDISPATCH Control, LPDISPATCH ToolTipCtrl)
{
   // TODO: Add your control notification handler code here
   ICodeTipMultiFunc ct( ToolTipCtrl );
   ct.m_bAutoRelease = FALSE;

   // See if the tooltip control has been initialized yet
   if ( 0 == ct.GetTipTextLength() )
   {
      // Save name of current function
      m_strTipFunc = GetCurrentFunction();
      if ( m_strTipFunc.IsEmpty() )
      {
         ct.Destroy();
         return zACTIVEX_EVENT_COMPLETED;
      }
      // Default to first function prototype
      ct.SetCurrentFunction( 0 );

      // Get first prototype for function
      CString strProto = GetPrototype( m_strTipFunc );
      if ( !strProto.IsEmpty() )
      {
         // Set tooltip text
         ct.SetTipText( strProto );

         // Default to first argument
         ct.SetArgument( 0 );

         // Get number of overloads, if any
         ct.SetFunctionCount( 0 );
      }
      else
      {
         ct.Destroy();
         return zACTIVEX_EVENT_COMPLETED;
      }

   }
   else
   {
      // Destroy the tooltip window if the caret is moved above or before
      // the starting point.
      //
      IRange range;

      range =  GetSel( TRUE );

      if ( range.GetEndLineNo()  <  m_posSel.GetLineNo() ||
         ( range.GetEndColNo()  <  m_posSel.GetColNo() &&
           range.GetEndLineNo() <= m_posSel.GetLineNo() ) )
      {
         // Caret moved too far up / back
         ct.Destroy();
      }
      else
      {
         // Determine which argument to highlight
         zLONG nArgument = GetCurrentArgument();
         ct.SetArgument( nArgument );
         if ( -1 == nArgument )
            ct.Destroy();// Right-paren found
         else if ( ct.GetFunctionCount() )
         {
            // Function is overloaded, so get current prototype
            CString strProto = GetPrototype( m_strTipFunc,
                                             ct.GetCurrentFunction() );

            // Set tooltip text
            if ( !strProto.IsEmpty() )
               ct.SetTipText( strProto );
         }
      }
   }
   return zACTIVEX_EVENT_COMPLETED;
}


//./ ADD NAME=ZVMLWrap::SelectClosestItemInCodeList
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::SelectClosestItemInCodeList
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: ICodeList * pCL -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
void ZVMLWrap::SelectClosestItemInCodeList( ICodeList * pCL)
{
   CString strFunc = GetCurrentFunction( false );
   COleVariant v;

   v.boolVal = 1;
   v.vt = VT_BOOL;

   if ( pCL )
   {
      if ( !strFunc.IsEmpty() )
      {
         int iItem = pCL->FindString( strFunc, v );

         if ( -1 != iItem )
         {
            pCL->SetSelectedItem( iItem );
         }
      }
   }
}



//./ ADD NAME=ZVMLWrap::SelectCurrentFunction
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::SelectCurrentFunction
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: //
//  RETURN    :
/**************************************************************************/
//./END + 1
void ZVMLWrap::SelectCurrentFunction()
{
   CString strLine;
   IRange range;


   if ( m_ListState != ZVMLWrap::ListFunctions )
      return;
   // Note:  We can't use CCodeMaxCtrl::GetWord() here, because the user
   // could have typed a function name followed by a space, and *then* the
   // left parenthesis, i.e. "MessageBox ("

   // Get the current line
   range =  GetSel( TRUE );
   strLine =  GetLine( range.GetEndLineNo() );

   // There's nothing for us to do if the line is empty
   if ( strLine.IsEmpty() )
      return;

   int nEnd = range.GetEndColNo() - 1;

   // Trim off trailing '(', if found
   if ( nEnd > 0 && nEnd < strLine.GetLength() )
   {
      if ( L'(' == strLine[ nEnd ] )
         nEnd --;

      // Trim off trailing whitespace
      while( nEnd > 0 && L' ' == strLine[nEnd] )
         nEnd --;
   }

   if ( nEnd < 0 )
      return;

   // The function name begins at the first alphanumeric character on line
   int nStart = min( nEnd, strLine.GetLength() );

   while( nStart > 0 && ( _istalnum( strLine[nStart - 1] ) ||
      L'_' == strLine[nStart - 1] ) )
   {
      --nStart;
   }

   range.SetStartColNo( nStart );
   range.SetEndColNo( nEnd + 1 );
    SetSel( range, FALSE );
}


//./ ADD NAME=ZVMLWrap::GetCurrentFunction
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetCurrentFunction
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: BOOL bMustExist -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
CString ZVMLWrap::GetCurrentFunction( BOOL bMustExist )
{
   IRange range;
   CString strLine, strFunc;

   // Note:  We can't use CCodeMaxCtrl::GetWord() here, because the user
   // could have typed a function name followed by a space, and *then* the
   // left parenthesis, i.e. "MessageBox ("

   // Get the current line
   range =  GetSel( TRUE );
   strLine =  GetLine( range.GetEndLineNo() );

   // There's nothing for us to do if the line is empty
   if ( strLine.IsEmpty() )
      return _T("");

   int nEnd = range.GetEndColNo() - 1;

   // Trim off trailing '(', if found
   if ( nEnd > 0 && nEnd < strLine.GetLength() )
   {
      if ( L'(' == strLine[ nEnd ] )
         nEnd --;

      // Trim off trailing whitespace
      while( nEnd > 0 && L' ' == strLine[nEnd] )
         nEnd --;
   }

   if ( nEnd < 0 )
      return _T("");

   // Save the starting position for use with the CodeTip.  This is so we
   // can destroy the tip window if the user moves the cursor back before or
   // above the starting point.
   //
   m_posSel.SetColNo( range.GetEndColNo() );
   m_posSel.SetLineNo( range.GetEndLineNo() );

   // The function name begins at the first alphanumeric character on line
   int nStart = min( nEnd, strLine.GetLength() );

   while( nStart > 0 && ( _istalnum( strLine[nStart - 1] ) ||
      L'_' == strLine[nStart - 1] ) )
   {
      --nStart;
   }

   // Extract the function name
   strFunc = strLine.Mid( nStart, nEnd - nStart + 1 );

   if ( strFunc.IsEmpty() )
      return _T("");

   // If we don't care whether or not the function actually exists in the
   // list, just return the string.
   if ( !bMustExist )
      return strFunc;

   zVIEW vSubtask = GetDialogView();

   if ( vSubtask )
   {
      zVIEW vApp = 0;
      short nRC = GetViewByName( &vApp, "TZOPSIGK", vSubtask, zLEVEL_ANY );

      if ( nRC >= 0 )
         nRC =  SetCursorFirstEntityByString( vApp, "Operation",
                                              "Name", strFunc, "" );
      if ( nRC >= zCURSOR_UNCHANGED )
         return strFunc;
   }


   return _T("");// Not found
}




//./ ADD NAME=ZVMLWrap::GetPrototype
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetPrototype
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS:  CString strFunc -
//              int iOverload -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
CString ZVMLWrap::GetPrototype( CString strFunc, int iOverload )
{
   CString strProto;
   int iIndex = 0;
   LPSTR szStr;
   CString strComp;

   strProto.Empty();
   // Is this function overloaded?
   if ( iOverload > 0 )
   {
      TRACE1("This is where overloaded funcions have to be handled %d\n", iOverload );
   }
   // Look for the function name in the INI file.

   // Get the tooltip text...
   zVIEW vSubtask = GetDialogView();

   if ( vSubtask )
   {
      zVIEW vApp = 0;
      short nRC = GetViewByName( &vApp, "TZOPSIGK", vSubtask, zLEVEL_ANY );
      if ( !vApp )
      {
         return strProto;
      }

      if ( nRC >= 0 )
         nRC =  SetCursorFirstEntityByString( vApp, "Operation",
                                              "Name", m_strTipFunc, "" );
      if ( nRC >= zCURSOR_UNCHANGED )
      {
         nRC = SetCursorFirstEntity( vApp, "Parameter", "" );
         while ( nRC == zCURSOR_SET )
         {
            if ( !strProto.IsEmpty() )
               strProto += ", ";

            BOOL bVML_File = (  GetLanguage() == CMLANG_VML );
            GetAddrForAttribute( &szStr, vApp, "Parameter", "PFlag" );
            if ( !bVML_File && *szStr == 'Y' )
               strProto += "PTR ";

            GetAddrForAttribute( &szStr, vApp, "Parameter", "UFlag" );
            if ( !bVML_File && *szStr == 'Y' )
               strProto += "UNSIGNED ";

            GetAddrForAttribute( &szStr, vApp, "Parameter", "DataType" );
            if ( *szStr )
            {
               if (  GetLanguage() == CMLANG_VML )
               {
                  switch ( *szStr )
                  {
                     case 'A':
                        strProto += "\"ATTRIBUTE_NAME\"";
                        break;

                     case 'E':
                        strProto +="\"ENTITY_NAME\"";
                        break;

                     case 'L':
                        strProto += "INTEGER";
                        break;

                     case 'M':
                        strProto += "DOUBLE";
                        break;

                     case 'N':
                        strProto += "INTEGER";
                        break;

                     case 'O':
                        strProto += "VOID";
                        break;

                     case 'S':
                        strProto += "STRING";
                        break;

                     case 'V':
                        strProto += "VIEW";
                        break;

                     case 'Y':
                        strProto += "STRING";
                        break;

                     default :
                        strProto += szStr;
                        break;

                  } // switch
               }
               else if (  GetLanguage() == CMLANG_CPP )
               {
                  switch ( *szStr )
                  {
                     case 'A':
                        strProto += "\"ATTRIBUTE_NAME\"";
                        break;

                     case 'E':
                        strProto += "\"ENTITY_NAME\"";
                        break;

                     case 'L':
                        strProto +="zLONG";
                        break;

                     case 'M':
                        strProto += "DOUBLE";
                        break;

                     case 'N':
                        strProto += "zSHORT";
                        break;

                     case 'O':
                        strProto += "void";
                        break;

                     case 'S':
                        strProto += "zCHAR";
                        break;

                     case 'V':
                        strProto += "zVIEW";
                        break;

                     case 'Y':
                        strProto += "zCHAR";
                        break;

                     default :
                        strProto += szStr;
                        break;
                  } // switch
               } // if ( bVML_File )...else...
            }
            GetAddrForAttribute( &szStr, vApp, "Parameter", "ShortDesc" );
            if ( *szStr )
            {
               strProto += " /* ";
               strProto += szStr;
               strProto += " */";
            }
            nRC = SetCursorNextEntity( vApp, "Parameter", "" );
         }
         strProto += ")";
      }
   }

   return strProto;
}


//./ ADD NAME=ZVMLWrap::GetCurrentArgument
// Source Module=tzvmledt.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::GetCurrentArgument
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS:
//  RETURN    :
/**************************************************************************/
//./END + 1
int ZVMLWrap::GetCurrentArgument()
{
   // Parse the buffer to determine which argument to highlight...
   //
   int iArg = 0;
   CString strLine;
   IRange range;

   range = GetSel( TRUE );

   enum
   {
      sNone,
      sQuote,
      sComment,
      sMultiComment,
      sEscape,
      sSubParen,

   } state = sNone;

   for( int nLine = m_posSel.GetLineNo();
            nLine <= range.GetEndLineNo(); nLine++ )
   {
      // Get next line from buffer
      strLine =  GetLine( nLine );

      if ( nLine == range.GetEndLineNo() )
      {
         // Trim off any excess beyond cursor pos so the argument with the
         // cursor in it will be highlighted.
         //
         int iTrim = min( range.GetEndColNo(), strLine.GetLength() + 1 );
         strLine = strLine.Left( iTrim );
      }

      if ( nLine == m_posSel.GetLineNo() )
      {
         // Strip off function name & open paren.
         int iTrim = strLine.Find( L'(' ) + 1;
         strLine = strLine.Mid( iTrim );
      }

      // Parse arguments from current line
      for( int iPos = 0, len = strLine.GetLength(); iPos < len; iPos++ )
      {
         switch( strLine[iPos] )
         {
         case L'(':// Sub-parenthesis
            {
               switch( state )
               {
               case sNone:
                  state = sSubParen;
                  break;
               case sEscape:
                  state = sQuote;
                  break;
               }
            }
            break;

         case L'"':// String begin/end
            {
               switch( state )
               {
               case sQuote:
                  state = sNone;
                  break;
               case sComment:
               case sMultiComment:
               case sSubParen:
                  break;
               default:
                  state = sQuote;
                  break;
               }
            }
            break;

         case L',':// Argument separator
            {
               switch( state )
               {
               case sNone:
                  iArg++;
                  break;
               case sEscape:
                  state = sQuote;
                  break;
               }
            }
            break;

         case L')':// End of function statement
            {
               switch( state )
               {
               case sNone:
                  return( -1 ); // Destroy tooltip on return
               case sEscape:
                  state = sQuote;
                  break;
               case sSubParen:
                  state = sNone;
                  break;
               }
            }
            break;

         case L'\\':// Escape sequence
            {
               switch( state )
               {
               case sQuote:
                  state = sEscape;
                  break;
               case sEscape:
                  state = sQuote;
                  break;
               }
            }
            break;

         case L'/':// Possible comment begin/end
            {
               switch( state )
               {
               case sNone:
                  {
                     if ( iPos + 1 < len )
                     {
                        TCHAR c = strLine[iPos + 1];

                        if ( L'/' == c )
                           state = sComment;
                        else if ( L'*' == c )
                        {
                           state = sMultiComment;
                           iPos++;
                        }
                     }
                  }
                  break;

               case sMultiComment:
                  {
                     if ( iPos && L'*' == strLine[iPos - 1] )
                        state = sNone;
                  }
                  break;

               case sEscape:
                  state = sQuote;
                  break;
               }
            }
            break;

         default:
            {
               if ( sEscape == state )
                  state = sQuote;
            }
            break;
         }

         // No point in scanning each and every character in comment line!
         if ( sComment == state )
            break;
      }

      // It's safe to clear this now that we're moving on to the next line
      if ( sComment == state )
         state = sNone;
   }

   return iArg;
}


/**************************************************************************/
// Function name   : ITrueDBGridCtrl::GetDialogView
// Description     : returns the view to the dialog
// Return type     : zVIEW
/**************************************************************************/
zVIEW ZVMLWrap::GetDialogView()
{
   if ( !m_pZActiveX )
   {
      TraceLineS( "Invalid Zeidon Control Pointer ", "" );
      return( 0 );
   }
   if ( !m_pZActiveX->m_pZSubtask )
   {
      TraceLineS( "Invalid Zeidon Subtask for control ", GetControlTag() );
      return( 0 );
   }

   return m_pZActiveX->m_pZSubtask->m_vDialog;
}

void
ZVMLWrap::ShowEntityAttribute( zCPCHAR lpszView )
{
   BOOL bShow = FALSE;

   CString strLastToken;
   if ( lpszView )
      strLastToken = lpszView;
   else
      strLastToken =  GetCurrentWord();

   ResetViewDeclarations();  // bombs performance
   if ( m_mapViewDeclarations.GetCount() < 1 )
      GetViewDeclarations( );

   int dots = ParseLineForListItems( );

   switch( dots )
   {
   case 0:
      m_ListState = ZVMLWrap::ListEntities;
      bShow = m_mapViewDeclarations.Lookup( strLastToken, (CObject *&)m_pCurrentLod );
      break;
   case 1:
      m_ListState = ZVMLWrap::ListAttributes;
      if ( m_pCurrentLod )
         bShow = m_pCurrentLod->m_mapEntities.Lookup( strLastToken, (CObject *&)m_pCurrentEntity );
      break;
   default:
      m_pCurrentLod = NULL;
      m_pCurrentEntity = NULL;
      break;
   }

   if ( bShow )
       ExecuteCmd( cmCmdCodeList );
   else
   {
      m_pCurrentLod = NULL;
      m_pCurrentEntity = NULL;
   }
}

int ZVMLWrap::ParseLineForListItems()
{
   IRange range =  GetSel(TRUE);
   LONG nCurrentLine = range.GetStartLineNo();
   CString strCurrentLine =  GetLine( nCurrentLine );

   CString strOperators =" \t";
   strOperators+= GetListFromINI( _T("Operator"), m_strIniPath );

   int pos = -1;
   int i = 0;
   for( i = 0; pos < 0 && i < strOperators.GetLength(); i++ )
   {
      pos = strCurrentLine.ReverseFind( strOperators.GetAt( i ) );
   }

   // if pos is greater than 0 there extract V.E.A from the end of the line
   CString strVEA;
   if ( pos >= 0 )
   {
      strVEA = strCurrentLine.Right( strCurrentLine.GetLength() - pos );
   }
   else
      strVEA = strCurrentLine;

   //count the "."
   TCHAR c;
   int dots = 0;
   for( i = 0; i < strVEA.GetLength(); i++ )
   {
      c = strVEA.GetAt( i );
      if ( c == '.' )
         dots++;
   }
   return dots;
}


//./ ADD NAME=ZVMLWrap::CompleteCommand
// Source Module=vmleditView.cpp
//**************************************************************************/
//
//  ENTRY:       ZVMLWrap::CompleteCommand
//                                            10/18/2001 Modified
//  PURPOSE:
//
//  PARAMETERS: CString strToken -
//
//  RETURN    :
/**************************************************************************/
//./END + 1
zBOOL
ZVMLWrap::CompleteCommand( CString strToken )
{
   BOOL bFirstWord = FALSE;
   zBOOL bReturn = TRUE;

   // check if token is the first word in the line
   IRange r =  GetSel( TRUE );
   CString strLine =  GetLine( r.GetStartLineNo() );

   if (  GetLanguage().CompareNoCase("VML") != 0 )
      return( bReturn );

   strLine.TrimLeft();
   if ( strLine.Find( strToken ) == 0 )
      bFirstWord = TRUE;


   LPZVMLCOMMAND lpCommand = NULL;
   strToken.MakeUpper();
   BOOL bIsCommand = m_mapCommandSet.Lookup( strToken, (CObject *&) lpCommand );

   if ( bIsCommand )
   {
      CString strCompleteCommand( _T("") );
//      CString strCompletionType = GetString( "Common", "CommandCompletion", m_strIniPath );
      if ( m_strCompletionType.CompareNoCase("S") == 0 )
         strCompleteCommand += lpCommand->m_strInsertText;
      else if ( m_strCompletionType.CompareNoCase("F") == 0 )
         strCompleteCommand += lpCommand->m_strInsertTextFull;

      if ( !bFirstWord && lpCommand->m_bCC_FirstChar )
         strCompleteCommand.Empty();
      if ( strCompleteCommand.IsEmpty() )
         bReturn = TRUE;
      else
      {
         LONG lStartCol = r.GetStartColNo();
         lStartCol-= strToken.GetLength();
         r.SetStartColNo( lStartCol );
         // when inserting multiple lines
         // a number of blanks have to be added to the items to be inserted
         ZString zstrCompleteCommand = strCompleteCommand;
         CStringArray sar;
         zstrCompleteCommand.Tokenize( "\r\n", sar );
         if ( sar.GetSize() > 1 )
         {
            CHAR tab =  GetExpandTabs() ? ' ' : '\t';
            CString strBlanks( tab, lStartCol );
            strCompleteCommand = sar[0];
            for ( int i = 1; i < sar.GetSize(); i++ )
            {
               strCompleteCommand+="\r\n";
               strCompleteCommand+=strBlanks;
               strCompleteCommand+=sar[i];
            }
         }

         LONG lCursorPos = strCompleteCommand.Find('&');
         if ( lCursorPos <= 0 )
            lCursorPos = lStartCol + strCompleteCommand.GetLength();
         else
         {
            //remove & character
            strCompleteCommand.Remove('&');
            lCursorPos+=lStartCol;
         }
          SetSel( r, FALSE );
          ReplaceSel( strCompleteCommand );
         r.SetStartColNo( lCursorPos - 1 );
         r.SetEndColNo( lCursorPos );
          SetSel( r, FALSE );
      }
   }

   return( bReturn );
}

BOOL
ZVMLWrap::GetViewDeclarations( )
{
   BOOL bReturn = FALSE;
   static LPSTR szOperationString = {"*OPERATION*"};
   SHORT     nViewType;
   ZString   strObjectName;
   ZString   strVarName;
   ZString   strSearchString;

   // Find the end of the current operation by searching for the beginning
   // of the next operation.  Since we want to ignore any operations that
   // might be commented out, keep searching until we are not in a comment.
   IRange currentRange =  GetSel( TRUE );
   IRange oprStartRange;
   IRange oprEndRange  =  GetSel( TRUE );
   IRange vDecl;

   SetRedraw( FALSE );// avoid scroling

   SetSearchRegExp( TRUE );
   SetSearchCaseSensitive( TRUE );
   SetFindFromCode( TRUE );
   SetFindFromCodeWrapped( FALSE );

   do
   {
      SetFindText( szOperationString );

      ExecuteCmd( cmCmdFindNext ); // this invokes the find forward
      if ( GetFindFromCodeWrapped() )
      {
         zLONG lLine = GetLineCount();
         zLONG lCol  = GetLineLength( lLine );
         oprEndRange.SetEndColNo( lCol );
         oprEndRange.SetEndLineNo( lLine );
         oprEndRange.SetStartColNo( lCol );
         oprEndRange.SetStartLineNo( lLine );
         break;
      }
      else
      {
         oprEndRange = GetSel( FALSE );
      }
   }
   while(  GetCurrentToken() != cmTokenTypeKeyword );
   TRACE2( "End of operation is at line %d, Column %d\n", oprEndRange.GetStartLineNo(), oprEndRange.GetStartColNo() );

   do
   {
      ExecuteCmd( cmCmdFindPrev ); // this invokes the find backward

      /* BLZ
      if ( GetLastCmd() == cmCmdFindPrev
      && GetLastCmdFailure() == cmErrNotFound )
      {
         SetRedraw( TRUE );
         return( FALSE );
      }
      */

      oprStartRange =  GetSel( TRUE );
   }
   while(  GetCurrentToken() != cmTokenTypeKeyword );
   TRACE2( "Start of Operation is at line %d, Column %d\n", oprStartRange.GetStartLineNo(), oprStartRange.GetStartColNo() );

   for ( nViewType = 0; nViewType < 2; nViewType++ )
   {
      // NOTE: If we go back to using regular expressions then we need to
      // change the logic for finding the variable name.  Look where we set
      // the cursor position for more.

      if ( nViewType == 0 )
      {  // searching for Based on lod
         strSearchString = "BASED ON LOD";
      }
      else
      if ( nViewType == 1 )
      {
         strSearchString = "REGISTERED AS";    // Set NOTE above.
      }

      SetSel(oprStartRange, TRUE );
      SetFindFromCodeWrapped( FALSE );
      BOOL bFinished = FALSE;
      do
      {
         SetFindText( strSearchString );
         ExecuteCmd( cmCmdFindNext ); // this invokes the find forward
         if ( GetFindFromCodeWrapped() )
            bFinished = TRUE;
         vDecl =  GetSel( TRUE );
         TRACE2( "vDecl is at line %d, Column %d\n", oprStartRange.GetStartLineNo(), oprStartRange.GetStartColNo() );
         if ( vDecl.GetStartLineNo() > oprEndRange.GetStartLineNo()
            ||vDecl.GetStartLineNo() <= oprStartRange.GetStartLineNo())
            bFinished = TRUE;

         if ( !bFinished &&  GetCurrentToken() == cmTokenTypeKeyword )
         {
            SHORT nRC = 0;
            zVIEW vVOR = 0;
            ZString str =  GetLine( vDecl.GetStartLineNo() );
            TRACE1( "%s\n", str );
            // tokenize str to get the variable name
            CStringArray sar;
            str.Tokenize( " \t", sar );
            // second entry is variable name e.g. VIEW CUSTOMER_LST BASED OND LOD CUSTOMER
            //                                         ============               ========
            //                                         strVarName                 strObjectName
            CString strVarName = sar[1];
            CString strObjectName = sar[sar.GetSize() - 1];
            for( int i = 0; i < sar.GetSize(); i++ )
            {
               if ( sar[i].CompareNoCase("VIEW") == 0 )
                  strVarName = sar[i+1];
               if ( nViewType == 0 && sar[i].CompareNoCase("LOD") == 0 )
                   strObjectName = sar[i+1];
               if ( nViewType == 1 && sar[i].CompareNoCase("AS") == 0 )
                   strObjectName = sar[i+1];
            }

            // If we are searching for registered views, then szObjectName is
            // really the VOR name.  Get the object name.

            // Add view variable and Object name to editor work object.
            if ( nViewType == 1 )
            {
               // Add view variable and Object name to editor work object.
               // First, get the VOR by name.
               zVIEW vSubtask = GetDialogView( );
               zVIEW vVor = 0;

               nRC = ActivateMetaOI_ByName( vSubtask, &vVor, vSubtask, zREFER_VOR_META,
                                            zSINGLE | zLEVEL_APPLICATION,
                                            (LPSTR)(LPCSTR)strObjectName, 0 );
               if ( nRC < 0 )
               {
                  ZString strHelpMessage = "Can't find Registered View named ";
                  strHelpMessage += strObjectName;
                  AfxMessageBox( strHelpMessage );
                  continue;
               }

               // Get Object name from VOR.
               GetStringFromAttribute( (LPSTR)(LPCSTR)strObjectName.GetBufferSetLength( 256 ), 256, vVor, "LOD", "Name" );
               DropMetaOI( vSubtask, vVor );
            }

            // If there is no entry with strVarName does NOT exist, create it.
            // If the variable exsits but the object name is different,
            // delete the attach the new LOD to the variable. If variable
            // name and object name are the same, then don't do anything.

            // check Lods definitions;
            // get the lod, create a object and attache it view variable
            LPZLOD lpLod = GetLodDefinition( strObjectName );
            if ( lpLod )
            {
               m_mapViewDeclarations[strVarName] = lpLod;
            }
            else
            {
               ZString strHelpMessage = "Unable to retrieve LOD definition for ";
               strHelpMessage += strObjectName;
               AfxMessageBox( strHelpMessage );
               continue;
            }
         }
      }while ( !bFinished );
   }

   SetRedraw( TRUE );
   SetSel( currentRange, FALSE );

   SetFindFromCode( FALSE );

   return( bReturn );
} // ZVMLWrap::GetViewDeclarations( )

void
ZVMLWrap::AddCommandToComplete( zCPCHAR strText,
                                zCPCHAR strType,
                                zCPCHAR strInsertText,
                                zCPCHAR strInsertTextFull,
                                zCPCHAR strCC_Code,
                                zBOOL   bCC_FirstChar )
{
   LPZVMLCOMMAND p = NULL;

   CString strKey = strCC_Code;
   strKey.MakeUpper();

   if ( !m_mapCommandSet.Lookup( strKey, (CObject *&)p ) )
   {
      p = new ZVMLCommand( strText,
                           strType,
                           strInsertText,
                           strInsertTextFull,
                           strCC_Code,
                           bCC_FirstChar );
      m_mapCommandSet[strCC_Code] = p;
   }
}

void ZVMLWrap::ResetCommands()
{
   POSITION pos = m_mapCommandSet.GetStartPosition( );
   while (pos != NULL)
   {
      CString strKey;
      LPZVMLCOMMAND p;
      m_mapCommandSet.GetNextAssoc( pos, strKey, (CObject*&)p);
      delete p;
   }
   m_mapCommandSet.RemoveAll( );
}

void ZVMLWrap::ResetLods()
{
   POSITION pos = m_mapLods.GetStartPosition( );
   while (pos != NULL)
   {
      CString strKey;
      LPZLOD p;
      m_mapLods.GetNextAssoc( pos, strKey, (CObject*&)p);
      delete p;
   }
   m_mapCommandSet.RemoveAll( );
}


void ZVMLWrap::ResetViewDeclarations()
{
   m_mapViewDeclarations.RemoveAll( );
}


void ZVMLWrap::ExecuteCmd(long lCommand)
{
   COleVariant v;
   ICodeSense::ExecuteCmd( lCommand, v );
}


void ZVMLWrap::SetCommandCompleteStyle( zCPCHAR szStyle )
{
   m_strCompletionType = szStyle;
   if ( m_strCompletionType.CompareNoCase( "F" ) != 0 )
      m_strCompletionType = "S";
}
