/**************************************************************
   Generated wrapper for ActiveX library named: MSDBGrid
   Generated at:   Wednesday, March 29, 2000
**************************************************************/

/*
CHANGE LOG - most recent first order

   200y.mm.dd    xxx
      Note ...

   2000.06.15    TMV
      Implement routines for manual navigation by using row and col indexes

   2000.05.29    TMV
      call InitMapping whenever MapFromOI is called to allow
      dynamic mapping

   2000.05.05    TMV
      add exception handling when call several
      dispatch methods
*/

#define zGLOBAL_DATA
#include "MSDBGrid.h"// include Zeidon generated Header

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL IsVariantNULL( const VARIANT v )
{
   return ( v.vt == VT_NULL || v.vt == VT_EMPTY );
}

/*
extern "C" int APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
   UNREFERENCED_PARAMETER( lpReserved );

   if ( dwReason == DLL_PROCESS_ATTACH )
   {
      g_hInstanceDLL = hInstance;
      if ( AfxInitExtensionModule( extensionDLL, hInstance ) == 0 )
         return( 0 );
   }
   else
   if ( dwReason == DLL_PROCESS_DETACH )
   {
      AfxTermExtensionModule( extensionDLL );
   }
   return( 1 );   // ok
}
*/

extern "C"
void WINAPI
ZeidonInitMSDBGrid( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                    zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
   new CDynLinkLibrary( extensionDLL );
}

extern "C" zOPER_EXPORT zSHORT OPERATION
MsDBGridScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
   #define ZCLASS_NAME IMsDgridCtrl
   zSHORT nRC = 0;
   CWnd *pWnd = NULL;

   if ( pzma == NULL )
   {
      if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )
      {
         pWnd = ( CWnd* ) ulParm;
         if ( !pWnd )
         {
            return(0);
         }
      }
      else
         return (0);
   }

   if ( lRequest == zACTIVEX_INITIALIZE )
   {
      // Default Zeidon Implementation
      // Never change this code unless you want to crash!!!!!
      IDispatch *pIDispatch = pzma->GetDispatchInterface();
      if ( pIDispatch )
      {
         ZCLASS_NAME *pWrapperInstance = new ZCLASS_NAME( pIDispatch );
         if ( pWrapperInstance )
         {
            pzma->SetWrapperInstance( pWrapperInstance );
            pWrapperInstance->m_pZActiveX = (ZActiveX*)pzma;
            nRC = 0;
         }

         if ( nRC == 0 )
         {
            TRY
            {
               // Zeidon requires specific Datamode
               enumDataModeConstants DataMode = pWrapperInstance->GetDataMode( );
               if ( DataMode != enumDataModeConstants__Unbound )
               {
                  CString strMsg = "Data mode must be set to \"Unbound\". Mapping code will not be called!";
                  MessagePrompt( pzma->m_pZSubtask->m_vDialog,
                                 "MSDBGRID SCR00001",
                                 "Invalid Property",
                                 strMsg,
                                 TRUE,
                                 zBUTTONS_OK,
                                 0,
                                 zICON_ERROR );
               }
               pWrapperInstance->SetBorderStyle( enumBorderStyleConstants__FixedSingle );
            }
            CATCH( COleException, e )
            {
               e->ReportError();
               // clean up
               delete pWrapperInstance;
               pzma->SetWrapperInstance( NULL );
               CString strMsg = "Unable to request Data mode. Mapping code will not be called!";
               MessagePrompt( pzma->m_pZSubtask->m_vDialog,
                              "MSDBGRID SCR00002",
                              "Dispatch Driver",
                              strMsg,
                              TRUE,
                              zBUTTONS_OK,
                              0,
                              zICON_ERROR );
               nRC = -1;
            }
            END_CATCH

         }
         nRC = 0;
      }
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance();
      if( pWrapperInstance )
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
      ZCLASS_NAME * pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance();
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
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // add code to display OI's data with this control
      // TraceLineI( "Calling MapFromOI: ", lFlags );
      pWrapperInstance->MapFromOI( );
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_TO_OI )
   {
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // add code to update OI's data with control's data
      pWrapperInstance->MapToOI( );
      // TraceLineI( "Transfer_Data_To_OI called: ", lFlags );
      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )
   {
      ZCLASS_NAME *pWrapperInstance = NULL;

      if ( pzma )
      {
         pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );
      }
      else
      {
         IDispatch *pIDispatch = NULL;
         LPUNKNOWN pUnk = pWnd->GetControlUnknown();
         if ( pUnk )
         {
            pUnk->QueryInterface( IID_IDispatch, (LPVOID *)&pIDispatch);
            if ( pIDispatch )
            {
               pWrapperInstance = new ZCLASS_NAME( pIDispatch );
            }
         }
      }

      zLONG lColumnCount = 0;

      if ( !pWrapperInstance )
         return ( ( zSHORT )lColumnCount );

      // to do:
      // return number of attributes to map here
      LPDISPATCH lpDispColumns = pWrapperInstance->GetColumns(  );
      if ( lpDispColumns )
      {
         Columns cols ( lpDispColumns );
         lColumnCount = cols.GetCount();
         cols.DetachDispatch();
      }

      if ( !pzma )
      {
         delete pWrapperInstance;
         pWrapperInstance = NULL;
      }

      return ( ( zSHORT )lColumnCount );
   }
   else
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )
   {
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // handle get focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_LOSE_FOCUS )
   {
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );
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
      return( ( zSHORT )pzma->ZMapAct::SetZCtrlState( lFlags, ulParm ) );
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
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      return pWrapperInstance->HandleEvent( (DISPID)lFlags);
   }
   return( nRC );
}





#ifdef __cplusplus
extern "C"
{
#endif

zOPER_EXPORT LPCTSTR OPERATION    IMsDgridCtrl_GetText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   LPCTSTR ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetText();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, LPCTSTR propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetText( propVal );
}


zOPER_EXPORT enumAddNewModeConstants OPERATION    IMsDgridCtrl_GetAddNewMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumAddNewModeConstants ReturnValue = enumAddNewModeConstants__NoAddNew; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAddNewMode();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAddNewMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumAddNewModeConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAddNewMode( propVal );
}


zOPER_EXPORT BSTR OPERATION    IMsDgridCtrl_GetErrorText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BSTR ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetErrorText();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetErrorText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetErrorText( propVal );
}


zOPER_EXPORT long OPERATION    IMsDgridCtrl_GetApproxCount( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   long ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetApproxCount();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetApproxCount( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetApproxCount( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetWrapCellPointer( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetWrapCellPointer();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetWrapCellPointer( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetWrapCellPointer( propVal );
}


zOPER_EXPORT enumTabActionConstants OPERATION    IMsDgridCtrl_GetTabAction( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumTabActionConstants ReturnValue = enumTabActionConstants__ControlNavigation; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetTabAction();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetTabAction( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumTabActionConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetTabAction( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetTabAcrossSplits( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetTabAcrossSplits();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetTabAcrossSplits( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetTabAcrossSplits( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowArrows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAllowArrows();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowArrows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAllowArrows( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetLeftCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetLeftCol();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetLeftCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetLeftCol( propVal );
}


zOPER_EXPORT IFontDisp* OPERATION    IMsDgridCtrl_GetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   IFontDisp* ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetFont();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, IFontDisp* propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetFont( propVal );
}


zOPER_EXPORT BSTR OPERATION    IMsDgridCtrl_GetCaption( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BSTR ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetCaption();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCaption( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetCaption( propVal );
}


zOPER_EXPORT OLE_COLOR OPERATION    IMsDgridCtrl_GetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_COLOR ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetForeColor();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetForeColor( propVal );
}


zOPER_EXPORT OLE_COLOR OPERATION    IMsDgridCtrl_GetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_COLOR ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetBackColor();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetBackColor( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetVisibleRows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetVisibleRows();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetVisibleRows( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetVisibleRows( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetVisibleCols( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetVisibleCols();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetVisibleCols( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetVisibleCols( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetSplit( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSplit();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSplit( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSplit( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetSelEndCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSelEndCol();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelEndCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSelEndCol( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetSelStartCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSelStartCol();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelStartCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSelStartCol( propVal );
}


zOPER_EXPORT BSTR OPERATION    IMsDgridCtrl_GetSelText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BSTR ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSelText();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelText( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSelText( propVal );
}


zOPER_EXPORT long OPERATION    IMsDgridCtrl_GetSelStart( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   long ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSelStart();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelStart( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSelStart( propVal );
}


zOPER_EXPORT long OPERATION    IMsDgridCtrl_GetSelLength( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   long ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSelLength();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetSelLength( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, long propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSelLength( propVal );
}


zOPER_EXPORT enumScrollBarsConstants OPERATION    IMsDgridCtrl_GetScrollBars( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumScrollBarsConstants ReturnValue = enumScrollBarsConstants__Automatic; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetScrollBars();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetScrollBars( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumScrollBarsConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetScrollBars( propVal );
}


zOPER_EXPORT float OPERATION    IMsDgridCtrl_GetRowHeight( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   float ReturnValue = 0.0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetRowHeight();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRowHeight( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetRowHeight( propVal );
}


zOPER_EXPORT enumDividerStyleConstants OPERATION    IMsDgridCtrl_GetRowDividerStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumDividerStyleConstants ReturnValue = enumDividerStyleConstants__BlackLine; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetRowDividerStyle();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRowDividerStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumDividerStyleConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetRowDividerStyle( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetRow();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetRow( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetRecordSelectors( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetRecordSelectors();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetRecordSelectors( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetRecordSelectors( propVal );
}


zOPER_EXPORT enumMarqueeStyleConstants OPERATION    IMsDgridCtrl_GetMarqueeStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumMarqueeStyleConstants ReturnValue = enumMarqueeStyleConstants__DottedCellBorder; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetMarqueeStyle();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetMarqueeStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumMarqueeStyleConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetMarqueeStyle( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetMarqueeUnique( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetMarqueeUnique();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetMarqueeUnique( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetMarqueeUnique( propVal );
}


zOPER_EXPORT OLE_HANDLE OPERATION    IMsDgridCtrl_GethWndEditor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_HANDLE ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GethWndEditor();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SethWndEditor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_HANDLE propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SethWndEditor( propVal );
}


zOPER_EXPORT OLE_HANDLE OPERATION    IMsDgridCtrl_GethWnd( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_HANDLE ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GethWnd();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SethWnd( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_HANDLE propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SethWnd( propVal );
}


zOPER_EXPORT float OPERATION    IMsDgridCtrl_GetHeadLines( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   float ReturnValue = 0.0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetHeadLines();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetHeadLines( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetHeadLines( propVal );
}


zOPER_EXPORT IFontDisp* OPERATION    IMsDgridCtrl_GetHeadFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   IFontDisp* ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetHeadFont();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetHeadFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, IFontDisp* propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetHeadFont( propVal );
}


zOPER_EXPORT VARIANT OPERATION    IMsDgridCtrl_GetFirstRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   VARIANT ReturnValue; // to do: initialize as necessary
   ReturnValue.vt = VT_NULL;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetFirstRow();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetFirstRow( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, VARIANT propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetFirstRow( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetEnabled();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetEnabled( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetEditActive( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetEditActive();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetEditActive( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetEditActive( propVal );
}


zOPER_EXPORT float OPERATION    IMsDgridCtrl_GetDefColWidth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   float ReturnValue = 0.0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDefColWidth();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDefColWidth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, float propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDefColWidth( propVal );
}


zOPER_EXPORT ICursor* OPERATION    IMsDgridCtrl_GetDataSource( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   ICursor* ReturnValue = NULL; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDataSource();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDataSource( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, ICursor* propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDataSource( propVal );
}


zOPER_EXPORT enumDataModeConstants OPERATION    IMsDgridCtrl_GetDataMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumDataModeConstants ReturnValue = enumDataModeConstants__Unbound; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDataMode();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDataMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumDataModeConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDataMode( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetDataChanged( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDataChanged();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetDataChanged( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDataChanged( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetCurrentCellModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetCurrentCellModified();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCurrentCellModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetCurrentCellModified( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetCurrentCellVisible( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetCurrentCellVisible();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCurrentCellVisible( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetCurrentCellVisible( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetColumnHeaders( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetColumnHeaders();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetColumnHeaders( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetColumnHeaders( propVal );
}


zOPER_EXPORT short OPERATION    IMsDgridCtrl_GetCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetCol();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetCol( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetCol( propVal );
}


zOPER_EXPORT VARIANT OPERATION    IMsDgridCtrl_GetBookmark( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   VARIANT ReturnValue; // to do: initialize as necessary
   ReturnValue.vt = VT_NULL;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetBookmark();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetBookmark( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, VARIANT propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetBookmark( propVal );
}


zOPER_EXPORT enumBorderStyleConstants OPERATION    IMsDgridCtrl_GetBorderStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumBorderStyleConstants ReturnValue = enumBorderStyleConstants__FixedSingle; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetBorderStyle();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetBorderStyle( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumBorderStyleConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetBorderStyle( propVal );
}


zOPER_EXPORT enumAppearanceConstants OPERATION    IMsDgridCtrl_GetAppearance( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   enumAppearanceConstants ReturnValue = enumAppearanceConstants__3D; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAppearance();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAppearance( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, enumAppearanceConstants propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAppearance( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowUpdate( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAllowUpdate();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowUpdate( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAllowUpdate( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowRowSizing( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAllowRowSizing();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowRowSizing( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAllowRowSizing( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowAddNew( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAllowAddNew();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowAddNew( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAllowAddNew( propVal );
}


zOPER_EXPORT BOOL OPERATION    IMsDgridCtrl_GetAllowDelete( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BOOL ReturnValue = FALSE; // to do: initialize as necessary
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAllowDelete();
}

zOPER_EXPORT void OPERATION    IMsDgridCtrl_SetAllowDelete( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAllowDelete( propVal );
}


   zOPER_EXPORT short OPERATION IMsDgridCtrl_SplitContaining( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , float X, float Y )
{
   short  ReturnValue = 0;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->SplitContaining( X, Y );
   return  ReturnValue ;
}


   zOPER_EXPORT void OPERATION IMsDgridCtrl_HoldFields( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->HoldFields(  );
   return ;
}


   zOPER_EXPORT void OPERATION IMsDgridCtrl_ClearFields( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->ClearFields(  );
   return ;
}


   zOPER_EXPORT void OPERATION IMsDgridCtrl_ClearSelCols( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->ClearSelCols(  );
   return ;
}


/*
   zOPER_EXPORT IPictureDisp OPERATION IMsDgridCtrl_CaptureImage( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{
   IPictureDisp  ReturnValue = NULL;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->CaptureImage(  );
   return  ReturnValue ;
}
*/


   zOPER_EXPORT void OPERATION IMsDgridCtrl_Refresh( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->Refresh(  );
   return ;
}


   zOPER_EXPORT void OPERATION IMsDgridCtrl_Scroll( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , long Cols, long Rows )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->Scroll( Cols, Rows );
   return ;
}


   zOPER_EXPORT float OPERATION IMsDgridCtrl_RowTop( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , short RowNum )
{
   float ReturnValue = 0.0 ;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->RowTop( RowNum );
   return  ReturnValue ;
}


   zOPER_EXPORT short OPERATION IMsDgridCtrl_RowContaining( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , float Y )
{
   short  ReturnValue = 0; ;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->RowContaining( Y );
   return  ReturnValue ;
}


   zOPER_EXPORT VARIANT OPERATION IMsDgridCtrl_RowBookmark( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , short RowNum )
{
   VARIANT  ReturnValue ;
   ReturnValue.vt = VT_EMPTY;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->RowBookmark( RowNum );
   return  ReturnValue ;
}


   zOPER_EXPORT void OPERATION IMsDgridCtrl_ReBind( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->ReBind(  );
   return ;
}


   zOPER_EXPORT VARIANT OPERATION IMsDgridCtrl_GetBookmark0( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , long RowNum )
{
   VARIANT  ReturnValue ;
   ReturnValue.vt = VT_EMPTY;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->GetBookmark( RowNum );
   return  ReturnValue ;
}


   zOPER_EXPORT short OPERATION IMsDgridCtrl_ColContaining( zVIEW vSubtask, zCPCHAR  cpcCtrlTag , float X )
{
   short  ReturnValue = 0; ;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->ColContaining( X );
   return  ReturnValue ;
}


   zOPER_EXPORT VARIANT OPERATION IMsDgridCtrl_GetSelBookmarks( zVIEW vSubtask, zCPCHAR  cpcCtrlTag ,  const VARIANT & Index )
{
   VARIANT  ReturnValue ;
   ReturnValue.vt = VT_EMPTY;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->GetSelBookmarks( Index );
   return  ReturnValue ;
}


   zOPER_EXPORT IDispatch* OPERATION IMsDgridCtrl_GetSplits( zVIEW vSubtask, zCPCHAR  cpcCtrlTag ,  const VARIANT & Index )
{
   IDispatch*  ReturnValue = NULL;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->GetSplits( Index );
   return  ReturnValue ;
}


   zOPER_EXPORT IDispatch* OPERATION IMsDgridCtrl_GetColumns( zVIEW vSubtask, zCPCHAR  cpcCtrlTag ,  const VARIANT & Index )
{
   IDispatch*  ReturnValue = NULL;
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return (  ReturnValue );
    ReturnValue  =  pInstance->GetColumns( Index );
   return  ReturnValue ;
}


   zOPER_EXPORT void OPERATION IMsDgridCtrl_AboutBox( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->AboutBox(  );
   return ;
}

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_LockColumn( zVIEW vSubtask, zCPCHAR cpcCtrlTag, zSHORT nIndex, zBOOL bLock )
{
   LPDISPATCH lpDispatchColumns = NULL;
   zSHORT nRC = 0;

   // validate parameters
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( !pInstance || nIndex  < 0 )
      nRC = -1;

   if ( nRC >= 0 )
   {
      lpDispatchColumns = pInstance->GetColumns();
      if ( !lpDispatchColumns )
      {
         nRC = -2;
      }
   }

   if ( nRC >= 0 )
   {
      Columns cols ( lpDispatchColumns );
      TRY
      {
         LPDISPATCH lpDispatchCol = NULL;

         if ( cols.GetCount() < nIndex )
         {
            nRC = -3;
            return nRC;
         }

         COleVariant vItem = nIndex;
         vItem.ChangeType( VT_I4 );
         lpDispatchCol = cols.GetItem( vItem );
         if ( !lpDispatchCol )
            nRC = -3;

         if ( nRC >= 0 )
         {
            Column col( lpDispatchCol );
            col.SetLocked( bLock );
         }
      }
      CATCH_ALL( e )
      {
         CString strErrorMessage;
         LPSTR   lpErrorMessage = strErrorMessage.GetBufferSetLength( 1024 );
         e->GetErrorMessage( lpErrorMessage, 1024 );
         TraceLineS( "MsDBGrid - Exception Handling IMsDgridCtrl_LockColumn ", lpErrorMessage  );
         nRC = -4;
         strErrorMessage.ReleaseBuffer();
      }
      END_CATCH_ALL
   }
   return nRC;
}

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_SetCellData( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zCPCHAR  pchData, zLONG lRowNbr, zLONG lColNbr )
{
   CString strErrorMessage;
   LPDISPATCH lpDispatchColumns = NULL;
   zSHORT nRC = 0;

   // validate parameters
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( !pInstance )
      nRC = -1;
   else
      nRC = pInstance->SetCellData( pchData, lRowNbr, lColNbr );

   return nRC;
}

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_GetCellData( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zPCHAR  pchData, zLONG lRowNbr, zLONG lColNbr )
{
   CString strErrorMessage;
   LPDISPATCH lpDispatchColumns = NULL;
   zSHORT nRC = 0;

   // validate parameters
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( !pInstance )
      nRC = -1;
   else
      nRC = pInstance->GetCellData( pchData, lRowNbr, lColNbr );

   return nRC;
}

zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_GetActiveCell( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zPLONG lRowNbr, zPLONG lColNbr )
{
   CString strErrorMessage;
   LPDISPATCH lpDispatchColumns = NULL;
   zSHORT nRC = 0;

   // validate parameters
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( !pInstance )
      nRC = -1;
   else
      nRC = pInstance->GetActiveCell( lRowNbr, lColNbr );
   return nRC;
}


zOPER_EXPORT zSHORT OPERATION IMsDgridCtrl_SetActiveCell( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, zLONG lRowNbr, zLONG lColNbr )
{
   CString strErrorMessage;
   LPDISPATCH lpDispatchColumns = NULL;
   zSHORT nRC = 0;

   // validate parameters
   IMsDgridCtrl * pInstance  = (IMsDgridCtrl*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( !pInstance )
      nRC = -1;
   else
      nRC = pInstance->SetCurrentCell( lRowNbr, lColNbr, TRUE );
   return nRC;
}



#ifdef __cplusplus
}
#endif

/*------------------------------------------------------
**   Setting and getting properties of dispatch interface
**   IMsDgridCtrl
**------------------------------------------------------*/

/*
Enables interactive record deletion
*/

BOOL IMsDgridCtrl::GetAllowDelete()
{
    BOOL result;
       GetProperty(0x1, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAllowDelete(BOOL propVal)
{
       SetProperty(0x1, VT_BOOL, propVal);
}

/*
Enables interactive record addition
*/

BOOL IMsDgridCtrl::GetAllowAddNew()
{
    BOOL result;
       GetProperty(0x2, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAllowAddNew(BOOL propVal)
{
       SetProperty(0x2, VT_BOOL, propVal);
}

/*
Enables interactive row resizing
*/

BOOL IMsDgridCtrl::GetAllowRowSizing()
{
    BOOL result;
       GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAllowRowSizing(BOOL propVal)
{
       SetProperty(0x3, VT_BOOL, propVal);
}

/*
Enables or disables record updatability
*/

BOOL IMsDgridCtrl::GetAllowUpdate()
{
    BOOL result;
       GetProperty(0x4, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAllowUpdate(BOOL propVal)
{
       SetProperty(0x4, VT_BOOL, propVal);
}

/*
Controls appearance of column headings, caption, and record selectors
*/

enumAppearanceConstants IMsDgridCtrl::GetAppearance()
{
    enumAppearanceConstants result;
       GetProperty(0xfffffdf8, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAppearance(enumAppearanceConstants propVal)
{
       SetProperty(0xfffffdf8, VT_I4, propVal);
}

/*
Sets/returns style for grid border
*/

enumBorderStyleConstants IMsDgridCtrl::GetBorderStyle()
{
    enumBorderStyleConstants result;
       GetProperty(0xfffffe08, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetBorderStyle(enumBorderStyleConstants propVal)
{
       SetProperty(0xfffffe08, VT_I4, propVal);
}

/*
Sets/returns bookmark of current row
*/

VARIANT IMsDgridCtrl::GetBookmark()
{
    VARIANT result;
       GetProperty(0x5, VT_VARIANT, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetBookmark(const VARIANT& propVal)
{
       SetProperty(0x5, VT_VARIANT, &propVal);
}

/*
Sets/returns current column number
*/

short IMsDgridCtrl::GetCol()
{
    short result;
       GetProperty(0x6, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetCol(short propVal)
{
       SetProperty(0x6, VT_I2, propVal);
}

/*
Turns column headings on or off
*/

BOOL IMsDgridCtrl::GetColumnHeaders()
{
    BOOL result;
       GetProperty(0x7, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetColumnHeaders(BOOL propVal)
{
       SetProperty(0x7, VT_BOOL, propVal);
}

/*
Sets/returns the visibility of the current cell
*/

BOOL IMsDgridCtrl::GetCurrentCellVisible()
{
    BOOL result;
       GetProperty(0x48, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetCurrentCellVisible(BOOL propVal)
{
       SetProperty(0x48, VT_BOOL, propVal);
}

/*
Sets/returns modification status of the current cell
*/

BOOL IMsDgridCtrl::GetCurrentCellModified()
{
    BOOL result;
       GetProperty(0x40, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetCurrentCellModified(BOOL propVal)
{
       SetProperty(0x40, VT_BOOL, propVal);
}

/*
Sets/returns modification status of the current row
*/

BOOL IMsDgridCtrl::GetDataChanged()
{
    BOOL result;
       GetProperty(0x46, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetDataChanged(BOOL propVal)
{
       SetProperty(0x46, VT_BOOL, propVal);
}

/*
Specifies bound or unbound mode
*/

enumDataModeConstants IMsDgridCtrl::GetDataMode()
{
    enumDataModeConstants result;
       GetProperty(0x25, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetDataMode(enumDataModeConstants propVal)
{
       SetProperty(0x25, VT_I4, propVal);
}

/*
Specifies source of grid data
*/

ICursor* IMsDgridCtrl::GetDataSource()
{
    ICursor* result;
       GetProperty(0x9, VT_PTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetDataSource(ICursor* propVal)
{
       SetProperty(0x9, VT_PTR, propVal);
}

/*
Specifies column width for auto-created columns
*/

float IMsDgridCtrl::GetDefColWidth()
{
    float result;
       GetProperty(0xa, VT_R4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetDefColWidth(float propVal)
{
       SetProperty(0xa, VT_R4, propVal);
}

/*
Returns status or enters/exits the cell editor
*/

BOOL IMsDgridCtrl::GetEditActive()
{
    BOOL result;
       GetProperty(0x3f, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetEditActive(BOOL propVal)
{
       SetProperty(0x3f, VT_BOOL, propVal);
}

/*
Enables or disables user interaction
*/

BOOL IMsDgridCtrl::GetEnabled()
{
    BOOL result;
       GetProperty(0xfffffdfe, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetEnabled(BOOL propVal)
{
       SetProperty(0xfffffdfe, VT_BOOL, propVal);
}

/*
Bookmark of row occupying first display line
*/

VARIANT IMsDgridCtrl::GetFirstRow()
{
   VARIANT result;
   GetProperty(0x16, VT_VARIANT, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetFirstRow( const VARIANT& propVal )
{
   SetProperty(0x16, VT_VARIANT, &propVal);
}

/*
Specifies the heading and caption font
*/

IFontDisp* IMsDgridCtrl::GetHeadFont()
{
    IFontDisp* result;
       GetProperty(0xb, VT_PTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetHeadFont(IFontDisp* propVal)
{
       SetProperty(0xb, VT_PTR, propVal);
}

/*
Number of lines allocated for heading text
*/

float IMsDgridCtrl::GetHeadLines()
{
    float result;
       GetProperty(0xc, VT_R4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetHeadLines(float propVal)
{
       SetProperty(0xc, VT_R4, propVal);
}

/*
Returns the window handle of the grid
*/

OLE_HANDLE IMsDgridCtrl::GethWnd()
{
    OLE_HANDLE result;
       GetProperty(0xfffffdfd, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SethWnd(OLE_HANDLE propVal)
{
       SetProperty(0xfffffdfd, VT_I4, propVal);
}

/*
Returns the window handle of the grid's editor
*/

OLE_HANDLE IMsDgridCtrl::GethWndEditor()
{
    OLE_HANDLE result;
       GetProperty(0x28, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SethWndEditor(OLE_HANDLE propVal)
{
       SetProperty(0x28, VT_I4, propVal);
}

/*
Restricts display of marquee to current split
*/

BOOL IMsDgridCtrl::GetMarqueeUnique()
{
    BOOL result;
       GetProperty(0x4a, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetMarqueeUnique(BOOL propVal)
{
       SetProperty(0x4a, VT_BOOL, propVal);
}

/*
Returns current split marquee style, sets all splits
*/

enumMarqueeStyleConstants IMsDgridCtrl::GetMarqueeStyle()
{
    enumMarqueeStyleConstants result;
       GetProperty(0x4b, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetMarqueeStyle(enumMarqueeStyleConstants propVal)
{
       SetProperty(0x4b, VT_I4, propVal);
}

/*
Shows/hides selection panel at left border
*/

BOOL IMsDgridCtrl::GetRecordSelectors()
{
    BOOL result;
       GetProperty(0xd, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetRecordSelectors(BOOL propVal)
{
       SetProperty(0xd, VT_BOOL, propVal);
}

/*
Specifies display line of current data row
*/

short IMsDgridCtrl::GetRow()
{
    short result;
       GetProperty(0xe, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetRow(short propVal)
{
       SetProperty(0xe, VT_I2, propVal);
}

/*
Selects style of row divider lines
*/

enumDividerStyleConstants IMsDgridCtrl::GetRowDividerStyle()
{
    enumDividerStyleConstants result;
       GetProperty(0xf, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetRowDividerStyle(enumDividerStyleConstants propVal)
{
       SetProperty(0xf, VT_I4, propVal);
}

/*
Specifies height of all grid rows
*/

float IMsDgridCtrl::GetRowHeight()
{
    float result;
       GetProperty(0x10, VT_R4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetRowHeight(float propVal)
{
       SetProperty(0x10, VT_R4, propVal);
}

/*
Sets/returns style for grid scrollbars
*/

enumScrollBarsConstants IMsDgridCtrl::GetScrollBars()
{
    enumScrollBarsConstants result;
       GetProperty(0x20, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetScrollBars(enumScrollBarsConstants propVal)
{
       SetProperty(0x20, VT_I4, propVal);
}

/*
Sets/returns length of selected text
*/

long IMsDgridCtrl::GetSelLength()
{
    long result;
       GetProperty(0x41, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetSelLength(long propVal)
{
       SetProperty(0x41, VT_I4, propVal);
}

/*
Sets/returns start of selected text
*/

long IMsDgridCtrl::GetSelStart()
{
    long result;
       GetProperty(0x42, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetSelStart(long propVal)
{
       SetProperty(0x42, VT_I4, propVal);
}

/*
Sets/returns the selected text
*/

BSTR IMsDgridCtrl::GetSelText()
{
    BSTR result;
       GetProperty(0x43, VT_BSTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetSelText(BSTR propVal)
{
       SetProperty(0x43, VT_BSTR, propVal);
}

/*
Sets/returns leftmost selected column of the current split
*/

short IMsDgridCtrl::GetSelStartCol()
{
    short result;
       GetProperty(0x11, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetSelStartCol(short propVal)
{
       SetProperty(0x11, VT_I2, propVal);
}

/*
Sets/returns rightmost selected column of the current split
*/

short IMsDgridCtrl::GetSelEndCol()
{
    short result;
       GetProperty(0x12, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetSelEndCol(short propVal)
{
       SetProperty(0x12, VT_I2, propVal);
}

/*
Sets/returns current split number
*/

short IMsDgridCtrl::GetSplit()
{
    short result;
       GetProperty(0x2b, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetSplit(short propVal)
{
       SetProperty(0x2b, VT_I2, propVal);
}

/*
Returns number of visible columns
*/

short IMsDgridCtrl::GetVisibleCols()
{
    short result;
       GetProperty(0x21, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetVisibleCols(short propVal)
{
       SetProperty(0x21, VT_I2, propVal);
}

/*
Returns number of visible display rows
*/

short IMsDgridCtrl::GetVisibleRows()
{
    short result;
       GetProperty(0x13, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetVisibleRows(short propVal)
{
       SetProperty(0x13, VT_I2, propVal);
}

/*
Sets/returns grid background color
*/

OLE_COLOR IMsDgridCtrl::GetBackColor()
{
    OLE_COLOR result;
       GetProperty(0xfffffe0b, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetBackColor(OLE_COLOR propVal)
{
       SetProperty(0xfffffe0b, VT_I4, propVal);
}

/*
Sets/returns grid foreground color
*/

OLE_COLOR IMsDgridCtrl::GetForeColor()
{
    OLE_COLOR result;
       GetProperty(0xfffffdff, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetForeColor(OLE_COLOR propVal)
{
       SetProperty(0xfffffdff, VT_I4, propVal);
}

/*
Sets/returns grid caption text
*/

BSTR IMsDgridCtrl::GetCaption()
{
    BSTR result;
       GetProperty(0xfffffdfa, VT_BSTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetCaption(BSTR propVal)
{
       SetProperty(0xfffffdfa, VT_BSTR, propVal);
}

/*
Specifies overall grid font object
*/

IFontDisp* IMsDgridCtrl::GetFont()
{
    IFontDisp* result;
       GetProperty(0xfffffe00, VT_PTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetFont(IFontDisp* propVal)
{
       SetProperty(0xfffffe00, VT_PTR, propVal);
}

/*
Sets/returns leftmost visible column
*/

short IMsDgridCtrl::GetLeftCol()
{
    short result;
       GetProperty(0x17, VT_I2, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetLeftCol(short propVal)
{
       SetProperty(0x17, VT_I2, propVal);
}

/*
Enables use of arrow keys for grid navigation
*/

BOOL IMsDgridCtrl::GetAllowArrows()
{
    BOOL result;
       GetProperty(0x2f, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAllowArrows(BOOL propVal)
{
       SetProperty(0x2f, VT_BOOL, propVal);
}

/*
Allows tab and arrow keys to cross split boundaries
*/

BOOL IMsDgridCtrl::GetTabAcrossSplits()
{
    BOOL result;
       GetProperty(0x31, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetTabAcrossSplits(BOOL propVal)
{
       SetProperty(0x31, VT_BOOL, propVal);
}

/*
Defines behavior of the tab key
*/

enumTabActionConstants IMsDgridCtrl::GetTabAction()
{
    enumTabActionConstants result;
       GetProperty(0x32, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetTabAction(enumTabActionConstants propVal)
{
       SetProperty(0x32, VT_I4, propVal);
}

/*
Defines behavior of tab and arrow keys at row boundaries
*/

BOOL IMsDgridCtrl::GetWrapCellPointer()
{
    BOOL result;
       GetProperty(0x33, VT_BOOL, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetWrapCellPointer(BOOL propVal)
{
       SetProperty(0x33, VT_BOOL, propVal);
}

/*
Sets/returns the approximate number of rows
*/

long IMsDgridCtrl::GetApproxCount()
{
    long result;
       GetProperty(0x51, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetApproxCount(long propVal)
{
       SetProperty(0x51, VT_I4, propVal);
}

/*
Returns the Error Text associated with an Error Event
*/

BSTR IMsDgridCtrl::GetErrorText()
{
    BSTR result;
       GetProperty(0x52, VT_BSTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetErrorText(BSTR propVal)
{
       SetProperty(0x52, VT_BSTR, propVal);
}

/*
Returns grid AddNew status
*/

enumAddNewModeConstants IMsDgridCtrl::GetAddNewMode()
{
    enumAddNewModeConstants result;
       GetProperty(0x54, VT_I4, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetAddNewMode(enumAddNewModeConstants propVal)
{
       SetProperty(0x54, VT_I4, propVal);
}

/*
Sets/returns the text of the current cell
*/

CString IMsDgridCtrl::GetText()
{
   CString result;
       GetProperty(0x0, VT_BSTR, (void*)&result);
   return result;
}
void IMsDgridCtrl::SetText( LPCTSTR propVal)
{
       SetProperty(0x0, VT_BSTR, propVal);
}
/*------------------------------------------------------
**   Wrapper for invoking methods of dispatch interface
**   IMsDgridCtrl
**------------------------------------------------------*/

/*
Displays the About box
*/
void   IMsDgridCtrl::AboutBox(  )
{

   InvokeHelper( DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
Contains a collection of grid columns
*/
IDispatch*   IMsDgridCtrl::GetColumns( const VARIANT & Index )
{
   IDispatch* result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper( 0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index );
   return result;
}

IDispatch*   IMsDgridCtrl::GetColumns( )
{
   IDispatch* result;
   InvokeHelper( 0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL, NULL );
   return result;
}


/*
Contains a collection of grid splits
*/
IDispatch*   IMsDgridCtrl::GetSplits( const VARIANT & Index )
{
   IDispatch* result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper( 0x23, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index );
   return result;
}
/*
Contains a collection of selected row bookmarks
*/
VARIANT   IMsDgridCtrl::GetSelBookmarks( const VARIANT & Index )
{
   VARIANT result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper( 0x15, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
      &Index );
   return result;
}

VARIANT   IMsDgridCtrl::GetSelBookmarks( )
{
   VARIANT result;

   InvokeHelper( 0x15, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL,
      NULL );
   return result;
}


/*
Identifies a column under an X coordinate
*/
short   IMsDgridCtrl::ColContaining( float X )
{
   short result;
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper( 0x18, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      X );
   return result;
}
/*
Returns a bookmark relative to the current row
*/
VARIANT   IMsDgridCtrl::GetBookmark( long RowNum )
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper( 0x19, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      RowNum );
   return result;
}
/*
Reinitializes grid from source data table
*/
void   IMsDgridCtrl::ReBind(  )
{

   InvokeHelper( 0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
Returns bookmark corresponding to a display row
*/
VARIANT   IMsDgridCtrl::RowBookmark( short RowNum )
{
   VARIANT result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x1b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
      RowNum );
   return result;
}
/*
Identifies a row under a Y coordinate
*/
short   IMsDgridCtrl::RowContaining( float Y )
{
   short result;
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper( 0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      Y );
   return result;
}
/*
Returns the Y position of a row's top border
*/
float   IMsDgridCtrl::RowTop( short RowNum )
{
   float result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x1d, DISPATCH_METHOD, VT_R4, (void*)&result, parms,
      RowNum );
   return result;
}
/*
Scrolls the grid data area
*/
void   IMsDgridCtrl::Scroll( long Cols, long Rows )
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper( 0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      Cols, Rows );
}
/*
Updates the grid's screen display
*/
void   IMsDgridCtrl::Refresh(  )
{

   InvokeHelper( DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
Returns a captured image of the grid's display
*/
IPictureDisp   IMsDgridCtrl::CaptureImage(  )
{
   LPDISPATCH result;

   InvokeHelper( 0x24, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL );
   return IPictureDisp ( result );
}
/*
Deselects all selected columns in the current split
*/
void   IMsDgridCtrl::ClearSelCols(  )
{

   InvokeHelper( 0x4c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
Clears the current column/field layout
*/
void   IMsDgridCtrl::ClearFields(  )
{

   InvokeHelper( 0x4d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
Holds the current column/field layout
*/
void   IMsDgridCtrl::HoldFields(  )
{

   InvokeHelper( 0x59, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
/*
Identifies the split under X,Y coordinates
*/
short   IMsDgridCtrl::SplitContaining( float X, float Y )
{
   short result;
   static BYTE parms[] =
      VTS_R4 VTS_R4;
   InvokeHelper( 0x53, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
      X, Y );
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

/////////////////////////////////////////////////////////////////////////////
// Column operations

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

void Columns::SetAdd(short Index)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Index);
}

LPDISPATCH Columns::GetAdd(short Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
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


/////////////////////////////////////////////////////////////////////////////
// RowBuffer properties

long RowBuffer::GetRowCount()
{
   long result;
   GetProperty(0x2, VT_I4, (void*)&result);
   return result;
}

void RowBuffer::SetRowCount(long propVal)
{
   SetProperty(0x2, VT_I4, propVal);
}

short RowBuffer::GetColumnCount()
{
   short result;
   GetProperty(0x3, VT_I2, (void*)&result);
   return result;
}

/////////////////////////////////////////////////////////////////////////////
// RowBuffer operations

void RowBuffer::SetBookmark(long Row, const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_VARIANT;
   InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Row, &newValue);
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

CString RowBuffer::GetColumnName(short Col)
{
   CString result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
      Col);
   return result;
}

void RowBuffer::SetValue(long Row, short Col, const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_I2 VTS_VARIANT;
   InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Row, Col, &newValue);
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

void Splits::SetAdd(short Index)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Index);
}

LPDISPATCH Splits::GetAdd(short Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
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

/////////////////////////////////////////////////////////////////////////////
// Split operations

LPDISPATCH Split::GetColumns(const VARIANT& Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      &Index);
   return result;
}

void Split::ClearSelCols()
{
   InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


