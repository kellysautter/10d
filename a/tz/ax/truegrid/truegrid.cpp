/********************************************************************************************/
//
// File:            truegrid.cpp
// Copyright:       Ton Beller AG2000
// Autor:           TMV
// Datum:           6. September 2000
// describtion:
//        Default implementation of script function
//
//
/* Change log most recent first order:

   2001.11.26   TMV   2000   ActiveX
      add Twips to pixel conversion routines

   2001.7.3   TMV   2000   ActiveX
      Implement handler for loose focus


   2001.03.26   DKS   Cosmetic

   2001.01.31   TMV   2000   ActiveX
      adding MFC macros for debug purpose

   2000.09.18   TMV   2000   ActiveX
      do inline documentation

*/
/*********************************************************************************************/

#include "zstdafx.h"
#include "tdbg6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**************************************************************************/
// Function name   : IsVariantNULL
// Description     : checks a variant whether it represents a NULL value
// Return type     : BOOL
// Argument        : const VARIANT v
/**************************************************************************/
BOOL IsVariantNULL( const VARIANT v )
{
   return ( v.vt == VT_NULL || v.vt == VT_EMPTY );
}

/**************************************************************************/
// Function name   : tgGetDomainType
// Description     : gets the domaintype of an attribute
// Return type     : zSHORT OPERATION
// Argument        : zVIEW   v
// Argument        : zCPCHAR szEntity
// Argument        : zCPCHAR szAttribute
/**************************************************************************/
zSHORT OPERATION tgGetDomainType( zVIEW v, zCPCHAR szEntity, zCPCHAR szAttribute )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPDOMAIN     lpDomain;

   lpViewEntity = MiGetViewEntityForView( v, szEntity );
   if ( !lpViewEntity )
      return( 0 );

   // find the attribute
   lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );
   while ( lpViewAttrib )
   {
      if ( zstrcmpi( lpViewAttrib->szName, szAttribute ) == 0 )
         break;
      lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib );
   }

   if ( !lpViewAttrib )
      return( 0 );

   lpDomain = (LPDOMAIN) zGETPTR( lpViewAttrib->hDomain );
   return (zSHORT) lpDomain->cDomainType;
}

//////////////////////////////////////////////////////////////////////////////////
//
// twips/pixel conversion routines
//
static zSHORT lpx = 0;
static zSHORT lpy = 0;

// called once to get constants
void GetDeviceConstants(  )
{
   HDC hdc = GetDC(NULL);

   //pixel in one logical inch
   lpx = GetDeviceCaps(hdc, LOGPIXELSX);
   lpy = GetDeviceCaps(hdc, LOGPIXELSY);

   ReleaseDC(NULL, hdc);
}

zLONG XTwipsToPixels( zLONG twips )
{
   if (!lpx)
      GetDeviceConstants();

   return MulDiv(lpx, (int) twips, 1440);
 // Total pixel in Inch = Total pixel in twips/1440
}

zLONG YTwipsToPixels( zLONG twips )
{
   if (!lpy)
      GetDeviceConstants();

   return MulDiv(lpy, (int)twips, 1440);
}

zLONG XPixelsToTwips( zLONG pixels )
{
   if (!lpx)
      GetDeviceConstants();

   return MulDiv(pixels, 1440, lpx);
}

zLONG YPixelsToTwips( zLONG pixels)
{
   if (!lpx) GetDeviceConstants();
      return MulDiv(pixels, 1440, lpy);
}

/////////////////////////////////////////////////////////////////////////////////////

extern "C" void WINAPI
ZeidonInitTrueGrid( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                    zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
   new CDynLinkLibrary( extensionDLL );
}

/*****************************************************************************/
// Function name   : TrueGridScript
// Description     : Drivers entrypoint into the grid implementation
// Return type     : zSHORT
// Argument        : ZMapAct *pzma  - pointer back to Zeidon Control instance
// Argument        : zLONG lFlags   - used to pass additional information
//                                    for SetCtrlState
// Argument        : zULONG ulParm  - used to pass request specific data
// Argument        : zLONG lRequest - request of script invocation
/*****************************************************************************/
extern "C" zOPER_EXPORT zSHORT OPERATION
TrueGridScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
   zSHORT nRC = 0;
   CWnd *pWnd = NULL;

// TraceLineS("<TrueGrid UsrDll> ",  "TrueGridScript");
// TraceLineI("   lFlags  = ",  lFlags );
// TraceLineI("   ulParm  = ",  ulParm );
// TraceLineI("   Request = ",  lRequest );

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
         ITrueDBGridCtrl *pWrapperInstance =
                        new ITrueDBGridCtrl( (ZActiveX *) pzma, pIDispatch );
         if ( pWrapperInstance )
         {
            nRC = 0;
         }
      }

      ITrueDBGridCtrl *pWrapperInstance =
                             (ITrueDBGridCtrl *) pzma->GetWrapperInstance( );
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
      ITrueDBGridCtrl *pWrapperInstance =
                             (ITrueDBGridCtrl *) pzma->GetWrapperInstance( );
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
      ITrueDBGridCtrl *pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
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
      ITrueDBGridCtrl *pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
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
      ITrueDBGridCtrl *pWrapperInstance = NULL;

      if ( pzma )
      {
         pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
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
               pWrapperInstance = new ITrueDBGridCtrl( NULL, pIDispatch );
               if( pWrapperInstance )
                  pWrapperInstance->SetDataMode( dbgUnboundSt ); // datamode = storage
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
   if( lRequest == zACTIVEX_CREATE_ZEIDON_CONTROL )
   {
      return nRC;
   }
   else
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )
   {
      ITrueDBGridCtrl *pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // handle get focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_LOSE_FOCUS )
   {
      ITrueDBGridCtrl *pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // handle lose focus here
      pWrapperInstance->OnLooseFocus();
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_SET_CTRL_STATE )
   {
      // handle SetCtrlState here
      zSHORT nRC = 0;

      ITrueDBGridCtrl *pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( nRC );

      nRC = (zSHORT) pWrapperInstance->SetZCtrlState( lFlags, ulParm );
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
      ITrueDBGridCtrl *pWrapperInstance = (ITrueDBGridCtrl *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      return pWrapperInstance->HandleEvent( (DISPID)lFlags);
   }
   return( nRC );
}



/*****************************************************************************/
// Function name   : TBDropDownScript
// Description     : Drivers entrypoint into the grid implementation
// Return type     : zSHORT
// Argument        : ZMapAct *pzma  - pointer back to Zeidon Control instance
// Argument        : zLONG lFlags   - used to pass additional information
//                                    for SetCtrlState
// Argument        : zULONG ulParm  - used to pass request specific data
// Argument        : zLONG lRequest - request of script invocation
/*****************************************************************************/
#ifdef TB_DROPDOWN_IMPLEMENT
extern "C" zOPER_EXPORT zSHORT OPERATION
TBDropDownScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
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
         ITrueDBDropDownCtrl *pWrapperInstance = new ITrueDBDropDownCtrl( (ZActiveX*)pzma, pIDispatch );
         if ( pWrapperInstance )
         {
            nRC = 0;
         }

      }
      ITrueDBDropDownCtrl *pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance();
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
      ITrueDBDropDownCtrl * pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance();
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
      ITrueDBDropDownCtrl *pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance( );
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
      ITrueDBDropDownCtrl *pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance( );
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
      ITrueDBDropDownCtrl *pWrapperInstance = NULL;

      if ( pzma )
      {
         pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance( );
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
               pWrapperInstance = new ITrueDBDropDownCtrl( NULL, pIDispatch );
               if( pWrapperInstance )
                  pWrapperInstance->SetDataMode( dbgUnboundSt ); // datamode = storage
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
      ITrueDBDropDownCtrl *pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      // to do:
      // handle get focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_LOSE_FOCUS )
   {
      ITrueDBDropDownCtrl *pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance( );
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
      ITrueDBDropDownCtrl *pWrapperInstance = (ITrueDBDropDownCtrl *)pzma->GetWrapperInstance( );
      if ( !pWrapperInstance )
         return ( 0 );

      return pWrapperInstance->HandleEvent( (DISPID)lFlags);
   }

   return( nRC );
}
#endif //TB_DROPDOWN_IMPLEMENT
