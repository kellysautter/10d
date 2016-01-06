// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICalendar wrapper class

/*
Calendar Control
*/
class ICalendar : public  COleDispatchDriver
{
public:
   ICalendar() {}
   ICalendar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   ICalendar(const ICalendar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

public: // methods for setting and getting poperties


public: // methods of the current Dispinterface

   OLE_COLOR GetBackColor( );
   void   SetBackColor( OLE_COLOR Value );
   short  GetDay( );
   void   SetDay( short Value );
   IFontDisp * GetDayFont( );
   void   SetDayFont( IFontDisp* Value );
   OLE_COLOR GetDayFontColor( );
   void   SetDayFontColor( OLE_COLOR Value );
   short  GetDayLength( );
   void   SetDayLength( short Value );
   short  GetFirstDay( );
   void   SetFirstDay( short Value );
   long   GetGridCellEffect( );
   void   SetGridCellEffect( long Value );
   IFontDisp * GetGridFont( );
   void   SetGridFont( IFontDisp* Value );
   OLE_COLOR GetGridFontColor( );
   void   SetGridFontColor( OLE_COLOR Value );
   OLE_COLOR GetGridLinesColor( );
   void   SetGridLinesColor( OLE_COLOR Value );
   short  GetMonth( );
   void   SetMonth( short Value );
   short  GetMonthLength( );
   void   SetMonthLength( short Value );
   BOOL   GetShowDateSelectors( );
   void   SetShowDateSelectors( BOOL Value );
   BOOL   GetShowDays( );
   void   SetShowDays( BOOL Value );
   BOOL   GetShowHorizontalGrid( );
   void   SetShowHorizontalGrid( BOOL Value );
   BOOL   GetShowTitle( );
   void   SetShowTitle( BOOL Value );
   BOOL   GetShowVerticalGrid( );
   void   SetShowVerticalGrid( BOOL Value );
   IFontDisp * GetTitleFont( );
   void   SetTitleFont( IFontDisp* Value );
   OLE_COLOR GetTitleFontColor( );
   void   SetTitleFontColor( OLE_COLOR Value );
   VARIANT GetValue( );
   void   SetValue( VARIANT Value );
   BOOL   GetValueIsNull( );
   void   SetValueIsNull( BOOL Value );
   short  GetYear( );
   void   SetYear( short Value );
   void   NextDay( );
   void   NextMonth( );
   void   NextWeek( );
   void   NextYear( );
   void   PreviousDay( );
   void   PreviousMonth( );
   void   PreviousWeek( );
   void   PreviousYear( );
   void   Refresh( );
   void   Today( );
   void   AboutBox( );
};

/*
Event interface for Calendar control
*/
class DCalendarEvents : public  COleDispatchDriver
{
public:
   DCalendarEvents() {}
   DCalendarEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DCalendarEvents(const DCalendarEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

public: // methods for setting and getting poperties


public: // methods of the current Dispinterface

   void   Click( );
   void   DblClick( );
   void   KeyDown( short* KeyCode, short Shift );
   void   KeyPress( short* KeyAscii );
   void   KeyUp( short* KeyCode, short Shift );
   void   BeforeUpdate( short* Cancel );
   void   AfterUpdate( );
   void   NewMonth( );
   void   NewYear( );
};

/*
Calendar control
*/
