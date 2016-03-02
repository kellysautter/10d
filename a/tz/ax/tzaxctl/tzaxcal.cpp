/////////////////////////////////////////////////////////////////////////////
// Generated wrapper for ActiveX library named: ICalendar
// Generated on:   Friday, November 05, 1999
/////////////////////////////////////////////////////////////////////////////

#include "zstdafx.h"

#include "Zdr.h"
#include "TzAxGbl.h"

#include "TzAxCtl.h"
#include "TzAxCal.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DISPID_CalendarAfterUpdate  0x01
#define DISPID_CalendarBeforeUpdate 0x02
#define DISPID_CalendarNewMonth     0x03
#define DISPID_CalendarNewYear      0x04

/////////////////////////////////////////////////////////////////////////////
// ICalendar properties

/////////////////////////////////////////////////////////////////////////////
// ICalendar operations

/*
Calendar Control
*/
/*------------------------------------------------------
**   Wrapper for invoking methods of dispatch interface
**   ICalendar
**------------------------------------------------------*/

OLE_COLOR   ICalendar::GetBackColor(  )
{
   OLE_COLOR result;

   InvokeHelper( DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_USERDEFINED, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetBackColor( OLE_COLOR Value )
{
   static BYTE parms[] =
      VTS_USERDEFINED;
   InvokeHelper( DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

short   ICalendar::GetDay(  )
{
   short result;

   InvokeHelper( 0x11, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetDay( short Value )
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

IFontDisp*   ICalendar::GetDayFont(  )
{
   IFontDisp* result;

   InvokeHelper( 0x1, DISPATCH_PROPERTYGET, VT_PTR, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetDayFont( IFontDisp* Value )
{
   static BYTE parms[] =
      VTS_PUSERDEFINED;
   InvokeHelper( 0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

OLE_COLOR   ICalendar::GetDayFontColor(  )
{
   OLE_COLOR result;

   InvokeHelper( 0x2, DISPATCH_PROPERTYGET, VT_USERDEFINED, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetDayFontColor( OLE_COLOR Value )
{
   static BYTE parms[] =
      VTS_USERDEFINED;
   InvokeHelper( 0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

short   ICalendar::GetDayLength(  )
{
   short result;

   InvokeHelper( 0x12, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetDayLength( short Value )
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

short   ICalendar::GetFirstDay(  )
{
   short result;

   InvokeHelper( 0x13, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetFirstDay( short Value )
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

long   ICalendar::GetGridCellEffect(  )
{
   long result;

   InvokeHelper( 0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetGridCellEffect( long Value )
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper( 0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

IFontDisp*   ICalendar::GetGridFont(  )
{
   IFontDisp* result;

   InvokeHelper( 0x3, DISPATCH_PROPERTYGET, VT_PTR, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetGridFont( IFontDisp* Value )
{
   static BYTE parms[] =
      VTS_PUSERDEFINED;
   InvokeHelper( 0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

OLE_COLOR   ICalendar::GetGridFontColor(  )
{
   OLE_COLOR result;

   InvokeHelper( 0x4, DISPATCH_PROPERTYGET, VT_USERDEFINED, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetGridFontColor( OLE_COLOR Value )
{
   static BYTE parms[] =
      VTS_USERDEFINED;
   InvokeHelper( 0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

OLE_COLOR   ICalendar::GetGridLinesColor(  )
{
   OLE_COLOR result;

   InvokeHelper( 0xd, DISPATCH_PROPERTYGET, VT_USERDEFINED, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetGridLinesColor( OLE_COLOR Value )
{
   static BYTE parms[] =
      VTS_USERDEFINED;
   InvokeHelper( 0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

short   ICalendar::GetMonth(  )
{
   short result;

   InvokeHelper( 0x10, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetMonth( short Value )
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

short   ICalendar::GetMonthLength(  )
{
   short result;

   InvokeHelper( 0x15, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetMonthLength( short Value )
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

BOOL   ICalendar::GetShowDateSelectors(  )
{
   BOOL result;

   InvokeHelper( 0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetShowDateSelectors( BOOL Value )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

BOOL   ICalendar::GetShowDays(  )
{
   BOOL result;

   InvokeHelper( 0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetShowDays( BOOL Value )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

BOOL   ICalendar::GetShowHorizontalGrid(  )
{
   BOOL result;

   InvokeHelper( 0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetShowHorizontalGrid( BOOL Value )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

BOOL   ICalendar::GetShowTitle(  )
{
   BOOL result;

   InvokeHelper( 0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetShowTitle( BOOL Value )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

BOOL   ICalendar::GetShowVerticalGrid(  )
{
   BOOL result;

   InvokeHelper( 0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetShowVerticalGrid( BOOL Value )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

IFontDisp*   ICalendar::GetTitleFont(  )
{
   IFontDisp* result;

   InvokeHelper( 0xa, DISPATCH_PROPERTYGET, VT_PTR, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetTitleFont( IFontDisp* Value )
{
   static BYTE parms[] =
      VTS_PUSERDEFINED;
   InvokeHelper( 0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

OLE_COLOR   ICalendar::GetTitleFontColor(  )
{
   OLE_COLOR result;

   InvokeHelper( 0xb, DISPATCH_PROPERTYGET, VT_USERDEFINED, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetTitleFontColor( OLE_COLOR Value )
{
   static BYTE parms[] =
      VTS_USERDEFINED;
   InvokeHelper( 0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

VARIANT   ICalendar::GetValue(  )
{
   VARIANT result;

   InvokeHelper( 0xc, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetValue( VARIANT Value )
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper( 0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}





BOOL   ICalendar::GetValueIsNull(  )
{
   BOOL result;

   InvokeHelper( 0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetValueIsNull( BOOL Value )
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper( 0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

short   ICalendar::GetYear(  )
{
   short result;

   InvokeHelper( 0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL );
   return result;
}

void   ICalendar::SetYear( short Value )
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper( 0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
      Value );
}

void   ICalendar::NextDay(  )
{

   InvokeHelper( 0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::NextMonth(  )
{

   InvokeHelper( 0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::NextWeek(  )
{

   InvokeHelper( 0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::NextYear(  )
{

   InvokeHelper( 0x19, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::PreviousDay(  )
{

   InvokeHelper( 0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::PreviousMonth(  )
{

   InvokeHelper( 0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::PreviousWeek(  )
{

   InvokeHelper( 0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::PreviousYear(  )
{

   InvokeHelper( 0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::Refresh(  )
{

   InvokeHelper( DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::Today(  )
{

   InvokeHelper( 0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   ICalendar::AboutBox(  )
{

   InvokeHelper( DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

/*
Event interface for Calendar control
*/
/*------------------------------------------------------
**   Wrapper for invoking methods of dispatch interface
**   DCalendarEvents
**------------------------------------------------------*/


void   DCalendarEvents::Click(  )
{

   InvokeHelper( DISPID_CLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   DCalendarEvents::DblClick(  )
{

   InvokeHelper( DISPID_DBLCLICK, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   DCalendarEvents::KeyDown( short* KeyCode, short Shift )
{
   static BYTE parms[] =
      VTS_PI2 VTS_I2;
   InvokeHelper( DISPID_KEYDOWN, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      KeyCode, Shift );
}

void   DCalendarEvents::KeyPress( short* KeyAscii )
{
   static BYTE parms[] =
      VTS_PI2;
   InvokeHelper( DISPID_KEYPRESS, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      KeyAscii );
}

void   DCalendarEvents::KeyUp( short* KeyCode, short Shift )
{
   static BYTE parms[] =
      VTS_PI2 VTS_I2;
   InvokeHelper( DISPID_KEYUP, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      KeyCode, Shift );
}

void   DCalendarEvents::BeforeUpdate( short* Cancel )
{
   static BYTE parms[] =
      VTS_PI2;
   InvokeHelper( 0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
      Cancel );
}

void   DCalendarEvents::AfterUpdate(  )
{

   InvokeHelper( 0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   DCalendarEvents::NewMonth(  )
{

   InvokeHelper( 0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

void   DCalendarEvents::NewYear(  )
{

   InvokeHelper( 0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL );
}

/*
Calendar control
*/

extern "C" zOPER_EXPORT zLONG OPERATION
CalendarScript( ZMapAct *pzma, zLONG lFlags, zULONG ulParm, zLONG lRequest )
{
   #define ZCLASS_NAME ICalendar
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
            // user handles DISPID_CalendarNewYear in current DLL
            ((ZActiveX *) pzma)->HandleEvent( DISPID_CalendarNewYear );
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
         TraceLineS( "Calendar View Name: ", *(pzma->m_pzsVName) );
         TraceLineS( "Calendar Entity Name: ", *(pzma->m_pzsEName) );
         TraceLineS( "Calendar Attribute Name: ", *(pzma->m_pzsAName) );
#endif
         GetStringFromAttribute( szDate, zsizeof( szDate ), vApp,
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
         TraceLineS( "Calendar View Name: ", *(pzma->m_pzsVName) );
         TraceLineS( "Calendar Entity Name: ", *(pzma->m_pzsEName) );
         TraceLineS( "Calendar Attribute Name: ", *(pzma->m_pzsAName) );
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
      return( pzma->ZMapAct::SetZCtrlState( lFlags, ulParm ) );
   }
   else
   if ( lRequest == zACTIVEX_GET_CTRL_STATE )
   {
      // handle GetCtrlState here
      return( pzma->ZMapAct::GetZCtrlState( lFlags ) );
   }
   else
   if ( lRequest == zACTIVEX_HANDLE_EVENT )
   {
      // If we get a zACTIVEX_HANDLE_EVENT lFlags contains the dispatch id of the
      // event. This is one of the events passed to the Driver via HandleEvent(...)
      // in current sample DISPID_CalendarNewYear.
      zSHORT nRC = 0;
      switch ( lFlags )
      {
         case DISPID_CalendarNewYear:
            // Add additional code to handle current event.
            TraceLineI( "UserDLL handles ActiveX Event: ", lFlags );

            nRC = 0;    // ==> NOT done ... call user code as well
            // nRC = 1; // ==> all done ... do NOT call user code
            break;

         default:
            nRC = 0;    // ==> NOT done ... call user code in addition
            break;
      }

      return( nRC );
   }

   return( nRC );
}
