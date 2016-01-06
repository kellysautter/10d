/////////////////////////////////////////////////////////////////////////////
// Generated wrapper for ActiveX library named: PVDATE2Lib
// Generated on:   Friday, November 05, 1999
/////////////////////////////////////////////////////////////////////////////

#include "zstdafx.h"

#include "Zdr.h"
#include "TzAxGbl.h"

#include "TzAxCtl.h"
#include "TzAxDate.h"// include Zeidon generated Header


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" zOPER_EXPORT zLONG OPERATION
DateEditScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
   #define ZCLASS_NAME _DPVDate2
   zSHORT nRC = 0;

   if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )
   {
      // return number of attributes to map here
      return( 1 );
   }

   if ( pzma == 0 ) // return immediate
      return( 0 );

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
            nRC = 0;
         }

         if ( nRC == 0 )
         {
            // add additional initialization code here
            // user handles DISPID_??? in current DLL
         // ((ZActiveX *) pzma)->HandleEvent( DISPID_??? );
            return( 0 );
         }
      }
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_FROM_OI )
   {
      // add code to display OI's data with this control
      ZCLASS_NAME *pWrapperInstance =
                                 (ZCLASS_NAME *) pzma->GetWrapperInstance( );
      if ( pWrapperInstance == 0 )
         return( 0 );

      zVIEW vApp;

      if ( pzma->m_pzsVName && pzma->m_pzsEName && pzma->m_pzsAName &&
           GetViewByName( &vApp, *(pzma->m_pzsVName),
                          pzma->m_pZSubtask->m_vDialog,
                          zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vApp, *(pzma->m_pzsEName) ) == 0 )
      {
         char  szDate[ 64 ];

#ifdef DEBUG_ALL
         TraceLineS( "DateEdit View Name: ", *(pzma->m_pzsVName) );
         TraceLineS( "DateEdit Entity Name: ", *(pzma->m_pzsEName) );
         TraceLineS( "DateEdit Attribute Name: ", *(pzma->m_pzsAName) );
#endif
         GetStringFromAttribute( szDate, sizeof( szDate ), vApp,
                                 *(pzma->m_pzsEName),
                                 *(pzma->m_pzsAName) );

      // szDate looks like this:  YYYYMMDDHHMMSSFFF
      // pWrapperInstance->SetTenths( atoi( szDate + 14 ) );
      // szDate[ 14 ] = 0;
      // pWrapperInstance->SetSecond( atoi( szDate + 12 ) );
      // szDate[ 12 ] = 0;
      // pWrapperInstance->SetMinute( atoi( szDate + 10 ) );
      // szDate[ 10 ] = 0;
      // pWrapperInstance->SetHour( atoi( szDate + 8 ) );
         szDate[ 8 ] = 0;
         pWrapperInstance->SetDay( atoi( szDate + 6 ) );
         szDate[ 6 ] = 0;
         pWrapperInstance->SetMonth( atoi( szDate + 4 ) );
         szDate[ 4 ] = 0;
         pWrapperInstance->SetYear( atoi( szDate ) );
      }

      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_TRANSFER_DATA_TO_OI )
   {
      // add code to update OI's data with control's data
      ZCLASS_NAME *pWrapperInstance =
                                 (ZCLASS_NAME *) pzma->GetWrapperInstance( );
      if ( pWrapperInstance == 0 )
         return( 0 );

      int nYear = pWrapperInstance->GetYear( );
      if ( nYear < 100 )
      {
         if ( nYear < 50 )
            nYear += 2000;
         else
            nYear += 1900;

         pWrapperInstance->SetYear( nYear );
      }

      int nMonth = pWrapperInstance->GetMonth( );
      int nDay = pWrapperInstance->GetDay( );
      int nHour = 0;   // pWrapperInstance->GetHour( );
      int nMinute = 0; // pWrapperInstance->GetMinute( );
      int nSecond = 0; // pWrapperInstance->GetSecond( );
      int nTenths = 0; // pWrapperInstance->GetSecond( );

   // TraceLineI( "   Year  = ", nYear );
   // TraceLineI( "   Month = ", nMonth );
   // TraceLineI( "   Day   = ", nDay );

      zVIEW vApp;

      if ( pzma &&
           pzma->m_pzsVName && pzma->m_pzsEName && pzma->m_pzsAName &&
           GetViewByName( &vApp, *(pzma->m_pzsVName),
                          pzma->m_pZSubtask->m_vDialog,
                          zLEVEL_ANY ) > 0 &&
           CheckExistenceOfEntity( vApp, *(pzma->m_pzsEName) ) == 0 )
      {
         char   szDate[ 64 ];

#ifdef DEBUG_ALL
         TraceLineS( "DateEdit View Name: ", *(pzma->m_pzsVName) );
         TraceLineS( "DateEdit Entity Name: ", *(pzma->m_pzsEName) );
         TraceLineS( "DateEdit Attribute Name: ", *(pzma->m_pzsAName) );
#endif
         // szDate looks like this:  YYYYMMDDHHMMSSFFF
         zsprintf( szDate, "%04d%02d%02d%02d%02d%02d%03d",
                   nYear, nMonth, nDay, nHour, nMinute, nSecond, nTenths );
         nRC = SetAttributeFromString( vApp, *(pzma->m_pzsEName),
                                       *(pzma->m_pzsAName), szDate );
      }

      return( 0 );
   }
   else
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )
   {
      ZCLASS_NAME *pWrapperInstance =
                                 (ZCLASS_NAME *) pzma->GetWrapperInstance( );
      if ( pWrapperInstance == 0 )
         return ( 0 );

      // handle get focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_LOSE_FOCUS )
   {
      ZCLASS_NAME *pWrapperInstance =
                                 (ZCLASS_NAME *) pzma->GetWrapperInstance( );
      if ( pWrapperInstance == 0 )
         return( 0 );

      // handle lose focus here
      return( 1 );
   }
   else
   if ( lRequest == zACTIVEX_SET_CTRL_STATE )
   {
      // handle SetCtrlState here
      return( pzma->SetZCtrlState( lFlags, ulParm ) );
   }
   else
   if ( lRequest == zACTIVEX_GET_CTRL_STATE )
   {
      // handle GetCtrlState here
      return( pzma->GetZCtrlState( lFlags ) );
   }
   else
   if ( lRequest == zACTIVEX_HANDLE_EVENT )
   {
      // If we get a zACTIVEX_HANDLE_EVENT lFlags contains the dispatch id of the
      // event. This is one of the events passed to the Driver via HandleEvent(...)
      // in current sample DISPID_???.
      zSHORT nRC = 0;
#if 0
      switch ( lFlags )
      {
         case DISPID_???:
            // Add additional code to handle current event.
             TraceLineI( "UserDLL handles ActiveX Event: ", lFlags );

            nRC = 0;    // ==> NOT done ... call user code as well
            // nRC = 1; // ==> all done ... do NOT call user code
            break;

         default:
            nRC = 0;    // ==> NOT done ... call user code in addition
            break;
      }
#endif
      return( nRC );
   }

   return( nRC );
}

#ifdef __cplusplus
extern "C"
{
#endif

zOPER_EXPORT short OPERATION    _DPVDate2_GetValueReset( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetValueReset();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetValueReset( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetValueReset( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetWeekOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetWeekOfYear();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetWeekOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetWeekOfYear( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetDayOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDayOfYear();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDayOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDayOfYear( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetDayOfWeek( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDayOfWeek();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDayOfWeek( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDayOfWeek( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetStatus( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetStatus();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetStatus( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetStatus( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetCalendarFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetCalendarFormat();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetCalendarFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetCalendarFormat( propVal );
}


zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetEditModeChange( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = FALSE; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetEditModeChange();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetEditModeChange( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetEditModeChange( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetEditMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetEditMode();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetEditMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetEditMode( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetBorder( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetBorder();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetBorder( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetBorder( propVal );
}


zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetSeparatorDefault( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = FALSE; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSeparatorDefault();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetSeparatorDefault( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSeparatorDefault( propVal );
}


zOPER_EXPORT BSTR OPERATION    _DPVDate2_GetDateString( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BSTR ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDateString();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDateString( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDateString( propVal );
}


zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = FALSE; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetModified();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetModified( propVal );
}


zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetTimeStore( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = FALSE; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetTimeStore();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetTimeStore( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetTimeStore( propVal );
}


zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetDisabledColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_COLOR ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDisabledColor();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDisabledColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDisabledColor( propVal );
}


zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = FALSE; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetEnabled();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetEnabled( propVal );
}


zOPER_EXPORT BSTR OPERATION    _DPVDate2_GetSeparator( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   BSTR ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetSeparator();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetSeparator( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetSeparator( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetDateFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDateFormat();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDateFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDateFormat( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetYear();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetYear( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetMonth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetMonth();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetMonth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetMonth( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetDay( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDay();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDay( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDay( propVal );
}


zOPER_EXPORT DATE OPERATION    _DPVDate2_Get_Value( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   DATE ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->Get_Value();
}

zOPER_EXPORT void OPERATION    _DPVDate2_Set_Value( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, DATE propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->Set_Value( propVal );
}


zOPER_EXPORT DATE OPERATION    _DPVDate2_GetValue( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   DATE ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetValue();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetValue( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, DATE propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetValue( propVal );
}


zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetHighlightColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_COLOR ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetHighlightColor();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetHighlightColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetHighlightColor( propVal );
}


zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetHighlightInverted( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = FALSE; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetHighlightInverted();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetHighlightInverted( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetHighlightInverted( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetDisplayFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetDisplayFormat();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetDisplayFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetDisplayFormat( propVal );
}


zOPER_EXPORT short OPERATION    _DPVDate2_GetAlignment( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   short ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetAlignment();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetAlignment( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetAlignment( propVal );
}


zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_COLOR ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetForeColor();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetForeColor( propVal );
}


zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   OLE_COLOR ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetBackColor();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetBackColor( propVal );
}


zOPER_EXPORT IFontDisp* OPERATION    _DPVDate2_GetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )
{
   IFontDisp* ReturnValue = 0; // to do: initialize as necessary
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return( ReturnValue );

   return pInstance->GetFont();
}

zOPER_EXPORT void OPERATION    _DPVDate2_SetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, IFontDisp* propVal )
{
   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return;

   pInstance->SetFont( propVal );
}


   zOPER_EXPORT void OPERATION _DPVDate2_AboutBox( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  )
{

   _DPVDate2 * pInstance  = (_DPVDate2*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );
   if ( pInstance == NULL )
      return ;
    pInstance->AboutBox(  );
   return ;
}



#ifdef __cplusplus
}
#endif

/*------------------------------------------------------
**   Setting and getting properties of dispatch interface
**   _DPVDate2
**------------------------------------------------------*/



IFontDisp* _DPVDate2::GetFont()
{
    IFontDisp* result;
       GetProperty(0xfffffe00, VT_PTR, (void*)&result);
   return result;
}
void _DPVDate2::SetFont(IFontDisp* propVal)
{
       SetProperty(0xfffffe00, VT_PTR, propVal);
}



OLE_COLOR _DPVDate2::GetBackColor()
{
    OLE_COLOR result;
       GetProperty(0xfffffe0b, VT_USERDEFINED, (void*)&result);
   return result;
}
void _DPVDate2::SetBackColor(OLE_COLOR propVal)
{
       SetProperty(0xfffffe0b, VT_USERDEFINED, propVal);
}



OLE_COLOR _DPVDate2::GetForeColor()
{
    OLE_COLOR result;
       GetProperty(0xfffffdff, VT_USERDEFINED, (void*)&result);
   return result;
}
void _DPVDate2::SetForeColor(OLE_COLOR propVal)
{
       SetProperty(0xfffffdff, VT_USERDEFINED, propVal);
}



short _DPVDate2::GetAlignment()
{
    short result;
       GetProperty(0x1, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetAlignment(short propVal)
{
       SetProperty(0x1, VT_I2, propVal);
}



short _DPVDate2::GetDisplayFormat()
{
    short result;
       GetProperty(0x2, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetDisplayFormat(short propVal)
{
       SetProperty(0x2, VT_I2, propVal);
}



BOOL _DPVDate2::GetHighlightInverted()
{
    BOOL result;
       GetProperty(0x3, VT_BOOL, (void*)&result);
   return result;
}
void _DPVDate2::SetHighlightInverted(BOOL propVal)
{
       SetProperty(0x3, VT_BOOL, propVal);
}



OLE_COLOR _DPVDate2::GetHighlightColor()
{
    OLE_COLOR result;
       GetProperty(0x4, VT_USERDEFINED, (void*)&result);
   return result;
}
void _DPVDate2::SetHighlightColor(OLE_COLOR propVal)
{
       SetProperty(0x4, VT_USERDEFINED, propVal);
}



DATE _DPVDate2::GetValue()
{
    DATE result;
       GetProperty(0xf, VT_DATE, (void*)&result);
   return result;
}
void _DPVDate2::SetValue(DATE propVal)
{
       SetProperty(0xf, VT_DATE, propVal);
}



DATE _DPVDate2::Get_Value()
{
    DATE result;
       GetProperty(0x0, VT_DATE, (void*)&result);
   return result;
}
void _DPVDate2::Set_Value(DATE propVal)
{
       SetProperty(0x0, VT_DATE, propVal);
}



short _DPVDate2::GetDay()
{
    short result;
       GetProperty(0x10, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetDay(short propVal)
{
       SetProperty(0x10, VT_I2, propVal);
}



short _DPVDate2::GetMonth()
{
    short result;
       GetProperty(0x11, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetMonth(short propVal)
{
       SetProperty(0x11, VT_I2, propVal);
}



short _DPVDate2::GetYear()
{
    short result;
       GetProperty(0x12, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetYear(short propVal)
{
       SetProperty(0x12, VT_I2, propVal);
}



short _DPVDate2::GetDateFormat()
{
    short result;
       GetProperty(0x5, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetDateFormat(short propVal)
{
       SetProperty(0x5, VT_I2, propVal);
}



BSTR _DPVDate2::GetSeparator()
{
    BSTR result;
       GetProperty(0x6, VT_BSTR, (void*)&result);
   return result;
}
void _DPVDate2::SetSeparator(BSTR propVal)
{
       SetProperty(0x6, VT_BSTR, propVal);
}



BOOL _DPVDate2::GetEnabled()
{
    BOOL result;
       GetProperty(0xfffffdfe, VT_BOOL, (void*)&result);
   return result;
}
void _DPVDate2::SetEnabled(BOOL propVal)
{
       SetProperty(0xfffffdfe, VT_BOOL, propVal);
}



OLE_COLOR _DPVDate2::GetDisabledColor()
{
    OLE_COLOR result;
       GetProperty(0x7, VT_USERDEFINED, (void*)&result);
   return result;
}
void _DPVDate2::SetDisabledColor(OLE_COLOR propVal)
{
       SetProperty(0x7, VT_USERDEFINED, propVal);
}



BOOL _DPVDate2::GetTimeStore()
{
    BOOL result;
       GetProperty(0x8, VT_BOOL, (void*)&result);
   return result;
}
void _DPVDate2::SetTimeStore(BOOL propVal)
{
       SetProperty(0x8, VT_BOOL, propVal);
}



BOOL _DPVDate2::GetModified()
{
    BOOL result;
       GetProperty(0x9, VT_BOOL, (void*)&result);
   return result;
}
void _DPVDate2::SetModified(BOOL propVal)
{
       SetProperty(0x9, VT_BOOL, propVal);
}



BSTR _DPVDate2::GetDateString()
{
    BSTR result;
       GetProperty(0x13, VT_BSTR, (void*)&result);
   return result;
}
void _DPVDate2::SetDateString(BSTR propVal)
{
       SetProperty(0x13, VT_BSTR, propVal);
}



BOOL _DPVDate2::GetSeparatorDefault()
{
    BOOL result;
       GetProperty(0xa, VT_BOOL, (void*)&result);
   return result;
}
void _DPVDate2::SetSeparatorDefault(BOOL propVal)
{
       SetProperty(0xa, VT_BOOL, propVal);
}



short _DPVDate2::GetBorder()
{
    short result;
       GetProperty(0xb, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetBorder(short propVal)
{
       SetProperty(0xb, VT_I2, propVal);
}



short _DPVDate2::GetEditMode()
{
    short result;
       GetProperty(0xc, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetEditMode(short propVal)
{
       SetProperty(0xc, VT_I2, propVal);
}



BOOL _DPVDate2::GetEditModeChange()
{
    BOOL result;
       GetProperty(0xd, VT_BOOL, (void*)&result);
   return result;
}
void _DPVDate2::SetEditModeChange(BOOL propVal)
{
       SetProperty(0xd, VT_BOOL, propVal);
}



short _DPVDate2::GetCalendarFormat()
{
    short result;
       GetProperty(0x14, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetCalendarFormat(short propVal)
{
       SetProperty(0x14, VT_I2, propVal);
}



short _DPVDate2::GetStatus()
{
    short result;
       GetProperty(0x15, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetStatus(short propVal)
{
       SetProperty(0x15, VT_I2, propVal);
}



short _DPVDate2::GetDayOfWeek()
{
    short result;
       GetProperty(0x16, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetDayOfWeek(short propVal)
{
       SetProperty(0x16, VT_I2, propVal);
}



short _DPVDate2::GetDayOfYear()
{
    short result;
       GetProperty(0x17, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetDayOfYear(short propVal)
{
       SetProperty(0x17, VT_I2, propVal);
}



short _DPVDate2::GetWeekOfYear()
{
    short result;
       GetProperty(0x18, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetWeekOfYear(short propVal)
{
       SetProperty(0x18, VT_I2, propVal);
}



short _DPVDate2::GetValueReset()
{
    short result;
       GetProperty(0xe, VT_I2, (void*)&result);
   return result;
}
void _DPVDate2::SetValueReset(short propVal)
{
       SetProperty(0xe, VT_I2, propVal);
}
/*------------------------------------------------------
**   Wrapper for invoking methods of dispatch interface
**   _DPVDate2
**------------------------------------------------------*/

void   _DPVDate2::AboutBox(  )
{

   InvokeHelper( DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}
