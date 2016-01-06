/**************************************************************
   Generated wrapper for ActiveX library named: PVDATE2Lib
   Generated at:   Friday, November 05, 1999
**************************************************************/



#define VC_EXTRALEAN
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions (including VB)
#include <afxpriv.h>
#include <afxpriv2.h>
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC OLE automation classes
#include <vfw.h>

#include <kzoegdef.h>

#include "Zdr.h"
#include "Zdctl.h"

// Definitions for Interfaces other than IDispatch
// Forward Declarations
class _DPVDate2;
// End Forward Declarations

/*
Constants for Text Alignment
*/
typedef enum tagenumDateAlignment {
   enumDateAlignment_pvdLeft = 0,
   enumDateAlignment_pvdCenter = 1,
   enumDateAlignment_pvdRight = 2
} enumDateAlignment;

/*
Constants for Display style
*/
typedef enum tagenumDateDisplayStyle {
   enumDateDisplayStyle_pvdNormal = 0,
   enumDateDisplayStyle_pvdFormatted = 1,
   enumDateDisplayStyle_pvdOdometer = 2,
   enumDateDisplayStyle_pvdOdomLight = 3,
   enumDateDisplayStyle_pvdOdomInvert = 4,
   enumDateDisplayStyle_pvdLED = 5,
   enumDateDisplayStyle_pvdLEDBold = 6
} enumDateDisplayStyle;

/*
Constants for Border
*/
typedef enum tagenumDateBorder {
   enumDateBorder_pvdNone = 0,
   enumDateBorder_pvdFlat = 1,
   enumDateBorder_pvd3D = 2
} enumDateBorder;

/*
Constants for Calendar Format
*/
typedef enum tagenumCalendarFormat {
   enumCalendarFormat_pvdOne = 1,
   enumCalendarFormat_pvdThree = 2,
   enumCalendarFormat_pvdSix = 3
} enumCalendarFormat;

/*
Constants for Date Format
*/
typedef enum tagenumDateFormat {
   enumDateFormat_pvdShort = 0,
   enumDateFormat_pvdLong = 1,
   enumDateFormat_pvdMd = 2,
   enumDateFormat_pvdMmdd = 3,
   enumDateFormat_pvdDm = 4,
   enumDateFormat_pvdDdmm = 5,
   enumDateFormat_pvdMdyy = 6,
   enumDateFormat_pvdMmddyy = 7,
   enumDateFormat_pvdDmyy = 8,
   enumDateFormat_pvdDdmmyy = 9,
   enumDateFormat_pvdMdyyyy = 10,
   enumDateFormat_pvdMmddyyyy = 11,
   enumDateFormat_pvdDmyyyy = 12,
   enumDateFormat_pvdDdmmyyyy = 13,
   enumDateFormat_pvdLmdy = 14,
   enumDateFormat_pvdLmddy = 15,
   enumDateFormat_pvdLdmy = 16,
   enumDateFormat_pvdLddmy = 17,
   enumDateFormat_pvdLwmdy = 18,
   enumDateFormat_pvdLwmddy = 19,
   enumDateFormat_pvdLwdmy = 20,
   enumDateFormat_pvdLwddmy = 21
} enumDateFormat;

/*
Constants for Date Status
*/
typedef enum tagenumDateStatus {
   enumDateStatus_pvdValid = 0,
   enumDateStatus_pvdInvalid = 1,
   enumDateStatus_pvdNull = 2
} enumDateStatus;

/*
Constants for Date Status
*/
typedef enum tagenumDateInitial {
   enumDateInitial_pvdCurrent = 0,
   enumDateInitial_pvdCustom = 1
} enumDateInitial;

/*
Dispatch interface for PVDate2 Control
*/
class AFX_CLASS_EXPORT _DPVDate2 : public  COleDispatchDriver
{
public:
   _DPVDate2() {}
   _DPVDate2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   _DPVDate2(const _DPVDate2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

public: // methods for setting and getting poperties

   IFontDisp*   GetFont();
   void   SetFont( IFontDisp* propVal );

   OLE_COLOR   GetBackColor();
   void   SetBackColor( OLE_COLOR propVal );

   OLE_COLOR   GetForeColor();
   void   SetForeColor( OLE_COLOR propVal );

   short   GetAlignment();
   void   SetAlignment( short propVal );

   short   GetDisplayFormat();
   void   SetDisplayFormat( short propVal );

   BOOL   GetHighlightInverted();
   void   SetHighlightInverted( BOOL propVal );

   OLE_COLOR   GetHighlightColor();
   void   SetHighlightColor( OLE_COLOR propVal );

   DATE   GetValue();
   void   SetValue( DATE propVal );

   DATE   Get_Value();
   void   Set_Value( DATE propVal );

   short   GetDay();
   void   SetDay( short propVal );

   short   GetMonth();
   void   SetMonth( short propVal );

   short   GetYear();
   void   SetYear( short propVal );

   short   GetDateFormat();
   void   SetDateFormat( short propVal );

   BSTR   GetSeparator();
   void   SetSeparator( BSTR propVal );

   BOOL   GetEnabled();
   void   SetEnabled( BOOL propVal );

   OLE_COLOR   GetDisabledColor();
   void   SetDisabledColor( OLE_COLOR propVal );

   BOOL   GetTimeStore();
   void   SetTimeStore( BOOL propVal );

   BOOL   GetModified();
   void   SetModified( BOOL propVal );

   BSTR   GetDateString();
   void   SetDateString( BSTR propVal );

   BOOL   GetSeparatorDefault();
   void   SetSeparatorDefault( BOOL propVal );

   short   GetBorder();
   void   SetBorder( short propVal );

   short   GetEditMode();
   void   SetEditMode( short propVal );

   BOOL   GetEditModeChange();
   void   SetEditModeChange( BOOL propVal );

   short   GetCalendarFormat();
   void   SetCalendarFormat( short propVal );

   short   GetStatus();
   void   SetStatus( short propVal );

   short   GetDayOfWeek();
   void   SetDayOfWeek( short propVal );

   short   GetDayOfYear();
   void   SetDayOfYear( short propVal );

   short   GetWeekOfYear();
   void   SetWeekOfYear( short propVal );

   short   GetValueReset();
   void   SetValueReset( short propVal );


public: // methods of the current Dispinterface

   void   AboutBox(  );
};


#ifdef __cplusplus
extern "C"
{
#endif

zOPER_EXPORT short OPERATION    _DPVDate2_GetValueReset( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetValueReset( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetWeekOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetWeekOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetDayOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDayOfYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetDayOfWeek( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDayOfWeek( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetStatus( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetStatus( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetCalendarFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetCalendarFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetEditModeChange( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetEditModeChange( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetEditMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetEditMode( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetBorder( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetBorder( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetSeparatorDefault( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetSeparatorDefault( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BSTR OPERATION    _DPVDate2_GetDateString( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDateString( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal );

zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetModified( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetTimeStore( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetTimeStore( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetDisabledColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDisabledColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal );

zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetEnabled( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT BSTR OPERATION    _DPVDate2_GetSeparator( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetSeparator( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BSTR propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetDateFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDateFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetYear( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetMonth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetMonth( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetDay( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDay( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT DATE OPERATION    _DPVDate2_Get_Value( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_Set_Value( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, DATE propVal );

zOPER_EXPORT DATE OPERATION    _DPVDate2_GetValue( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetValue( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, DATE propVal );

zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetHighlightColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetHighlightColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal );

zOPER_EXPORT BOOL OPERATION    _DPVDate2_GetHighlightInverted( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetHighlightInverted( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, BOOL propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetDisplayFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetDisplayFormat( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT short OPERATION    _DPVDate2_GetAlignment( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetAlignment( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, short propVal );

zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetForeColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal );

zOPER_EXPORT OLE_COLOR OPERATION    _DPVDate2_GetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetBackColor( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, OLE_COLOR propVal );

zOPER_EXPORT IFontDisp* OPERATION    _DPVDate2_GetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag );
zOPER_EXPORT void OPERATION    _DPVDate2_SetFont( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, IFontDisp* propVal );

   zOPER_EXPORT void OPERATION _DPVDate2_AboutBox( zVIEW vSubtask, zCPCHAR  cpcCtrlTag  );

#ifdef __cplusplus
}
#endif
