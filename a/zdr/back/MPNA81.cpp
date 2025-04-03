// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "zstdafx.h"
#include "mpna81.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Application properties

/////////////////////////////////////////////////////////////////////////////
// _Application operations

LPDISPATCH _Application::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Application::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString _Application::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Application::GetActiveMap()
{
   LPDISPATCH result;
   InvokeHelper(0x6000101, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Application::OpenMap(LPCTSTR FileName, BOOL AddToRecentFiles)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_BOOL;
   InvokeHelper(0x6020103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      FileName, AddToRecentFiles);
   return result;
}

LPDISPATCH _Application::NewMap(LPCTSTR Template)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6020104, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Template);
   return result;
}

CString _Application::GetCaption()
{
   CString result;
   InvokeHelper(0x6000106, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void _Application::SetCaption(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6000106, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

CString _Application::GetPath()
{
   CString result;
   InvokeHelper(0x6000015, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString _Application::GetDefaultFilePath()
{
   CString result;
   InvokeHelper(0x6000107, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString _Application::GetVersion()
{
   CString result;
   InvokeHelper(0x6000108, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString _Application::GetBuild()
{
   CString result;
   InvokeHelper(0x6000109, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void _Application::Activate()
{
   InvokeHelper(0x6020119, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Application::GetHeight()
{
   long result;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetHeight(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long _Application::GetLeft()
{
   long result;
   InvokeHelper(0x6000113, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetLeft(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000113, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long _Application::GetWidth()
{
   long result;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetWidth(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long _Application::GetTop()
{
   long result;
   InvokeHelper(0x6000115, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetTop(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000115, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

BOOL _Application::GetVisible()
{
   BOOL result;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void _Application::SetVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long _Application::GetWindowState()
{
   long result;
   InvokeHelper(0x6000117, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetWindowState(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000117, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

void _Application::AddCommand(LPCTSTR DisplayName, LPCTSTR MethodName, LPDISPATCH Target)
{
   static BYTE parms[] =
      VTS_BSTR VTS_BSTR VTS_DISPATCH;
   InvokeHelper(0x6020120, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       DisplayName, MethodName, Target);
}

void _Application::RemoveCommands(LPDISPATCH Target)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6020121, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Target);
}

long _Application::GetUnits()
{
   long result;
   InvokeHelper(0x6000102, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetUnits(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000102, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long _Application::GetPaneState()
{
   long result;
   InvokeHelper(0x6000118, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetPaneState(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000118, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long _Application::GetRegion()
{
   long result;
   InvokeHelper(0x6000111, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::Quit()
{
   InvokeHelper(0x6020105, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Application::GetUserControl()
{
   BOOL result;
   InvokeHelper(0x600010a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void _Application::SetUserControl(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x600010a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL _Application::GetItineraryVisible()
{
   BOOL result;
   InvokeHelper(0x600010b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void _Application::SetItineraryVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x600010b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

LPDISPATCH _Application::GetToolbars()
{
   LPDISPATCH result;
   InvokeHelper(0x600010c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString _Application::GetActivePrinter()
{
   CString result;
   InvokeHelper(0x600010d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void _Application::SetActivePrinter(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x600010d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long _Application::GetMousePointer()
{
   long result;
   InvokeHelper(0x600022c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Application::SetMousePointer(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x600022c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

LPDISPATCH _Application::GetAddIns()
{
   LPDISPATCH result;
   InvokeHelper(0x600010e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// _Map properties

/////////////////////////////////////////////////////////////////////////////
// _Map operations

LPDISPATCH _Map::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString _Map::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::GetContainer()
{
   LPDISPATCH result;
   InvokeHelper(0x6000211, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL _Map::GetSaved()
{
   BOOL result;
   InvokeHelper(0x6000201, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void _Map::SetSaved(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000201, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

CString _Map::GetFullName()
{
   CString result;
   InvokeHelper(0x6000014, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString _Map::GetPath()
{
   CString result;
   InvokeHelper(0x6000015, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void _Map::SetRefLocation(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH _Map::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

double _Map::GetAltitude()
{
   double result;
   InvokeHelper(0x6000212, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void _Map::SetAltitude(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6000212, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

LPDISPATCH _Map::GetActiveRoute()
{
   LPDISPATCH result;
   InvokeHelper(0x6000214, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::GetDataSets()
{
   LPDISPATCH result;
   InvokeHelper(0x6000215, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long _Map::GetMapStyle()
{
   long result;
   InvokeHelper(0x6000216, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Map::SetMapStyle(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000216, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long _Map::GetMapFont()
{
   long result;
   InvokeHelper(0x6000217, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Map::SetMapFont(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000217, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

void _Map::Save()
{
   InvokeHelper(0x6020209, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Map::SaveAs(LPCTSTR FileName, long SaveFormat, BOOL AddToRecentFiles)
{
   static BYTE parms[] =
      VTS_BSTR VTS_I4 VTS_BOOL;
   InvokeHelper(0x6020210, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       FileName, SaveFormat, AddToRecentFiles);
}

void _Map::PrintOut(LPCTSTR OutputFileName, LPCTSTR Title, long Copies, long PrintArea, long PrintQuality, long PrintOrientation, BOOL Collate, BOOL IncludeLegend, BOOL IncludeOverview, BOOL Faxable)
{
   static BYTE parms[] =
      VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
   InvokeHelper(0x6020208, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       OutputFileName, Title, Copies, PrintArea, PrintQuality, PrintOrientation, Collate, IncludeLegend, IncludeOverview, Faxable);
}

LPDISPATCH _Map::FindPushpin(LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6020206, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name);
   return result;
}

LPDISPATCH _Map::AddPushpin(LPDISPATCH AtLocation, LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_BSTR;
   InvokeHelper(0x6020220, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      AtLocation, Name);
   return result;
}

void _Map::CopyMap()
{
   InvokeHelper(0x6020202, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Map::CopyDirections()
{
   InvokeHelper(0x6020218, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

double _Map::Distance(LPDISPATCH StartLocation, LPDISPATCH EndLocation)
{
   double result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH;
   InvokeHelper(0x6020203, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
      StartLocation, EndLocation);
   return result;
}

LPDISPATCH _Map::GetLocation(double Latitude, double Longitude, double Altitude)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_R8 VTS_R8 VTS_R8;
   InvokeHelper(0x6020222, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Latitude, Longitude, Altitude);
   return result;
}

LPDISPATCH _Map::GetSelection()
{
   LPDISPATCH result;
   InvokeHelper(0x6000213, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::GetShapes()
{
   LPDISPATCH result;
   InvokeHelper(0x600020a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::GetPlaceCategories()
{
   LPDISPATCH result;
   InvokeHelper(0x600020b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::FindResults(LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x602020c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name);
   return result;
}

LPDISPATCH _Map::FindAddressResults(LPCTSTR Street, LPCTSTR City, LPCTSTR OtherCity, LPCTSTR Region, LPCTSTR PostalCode, const VARIANT& Country)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_VARIANT;
   InvokeHelper(0x602020d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Street, City, OtherCity, Region, PostalCode, &Country);
   return result;
}

LPDISPATCH _Map::ShowFindDialog(LPCTSTR Name, long FindState, long HWndParent, BOOL AutoConfirmExactMatch)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL;
   InvokeHelper(0x6020228, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name, FindState, HWndParent, AutoConfirmExactMatch);
   return result;
}

long _Map::GetTop()
{
   long result;
   InvokeHelper(0x6000115, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long _Map::GetLeft()
{
   long result;
   InvokeHelper(0x6000113, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long _Map::GetHeight()
{
   long result;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long _Map::GetWidth()
{
   long result;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::XYToLocation(long X, long Y)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x602020e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      X, Y);
   return result;
}

long _Map::LocationToX(LPDISPATCH Where)
{
   long result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x602020f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      Where);
   return result;
}

long _Map::LocationToY(LPDISPATCH Where)
{
   long result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x602021a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
      Where);
   return result;
}

double _Map::GetPixelSize()
{
   double result;
   InvokeHelper(0x6000225, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void _Map::Pan(long PanDirection, double PanFactor)
{
   static BYTE parms[] =
      VTS_I4 VTS_R8;
   InvokeHelper(0x602021b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       PanDirection, PanFactor);
}

LPDISPATCH _Map::GetSelectedArea()
{
   LPDISPATCH result;
   InvokeHelper(0x600022d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::Union(const VARIANT& ArrayOfLocations)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x602021d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      &ArrayOfLocations);
   return result;
}

LPDISPATCH _Map::ObjectsFromPoint(long X, long Y)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x6020226, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      X, Y);
   return result;
}

void _Map::ZoomIn()
{
   InvokeHelper(0x602021e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Map::ZoomOut()
{
   InvokeHelper(0x602021f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Map::GoBack()
{
   InvokeHelper(0x6020224, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Map::GoForward()
{
   InvokeHelper(0x6020223, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Map::Paste()
{
   InvokeHelper(0x602002c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long _Map::GetProjection()
{
   long result;
   InvokeHelper(0x6000227, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Map::SetProjection(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000227, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

LPDISPATCH _Map::GetPageSetup()
{
   LPDISPATCH result;
   InvokeHelper(0x6000229, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL _Map::GetAllowEdgePan()
{
   BOOL result;
   InvokeHelper(0x600022a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void _Map::SetAllowEdgePan(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x600022a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long _Map::GetDragBehavior()
{
   long result;
   InvokeHelper(0x600022b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void _Map::SetDragBehavior(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x600022b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

LPDISPATCH _Map::GetSavedWebPages()
{
   LPDISPATCH result;
   InvokeHelper(0x600022e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::ParseStreetAddress(LPCTSTR Address)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x600022f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Address);
   return result;
}

LPDISPATCH _Map::GetSymbols()
{
   LPDISPATCH result;
   InvokeHelper(0x6000230, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH _Map::FindPlaceResults(LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6020231, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Location properties

/////////////////////////////////////////////////////////////////////////////
// Location operations

LPDISPATCH Location::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Location::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Location::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Location::GoTo()
{
   InvokeHelper(0x6020018, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Location::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Location::GetPlaceCategory()
{
   LPDISPATCH result;
   InvokeHelper(0x6000901, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Location::FindNearby(double Distance)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x602001b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Distance);
   return result;
}

BOOL Location::GetHighlight()
{
   BOOL result;
   InvokeHelper(0x6000801, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Location::SetHighlight(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000801, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL Location::GetIsHighlightable()
{
   BOOL result;
   InvokeHelper(0x600001c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

double Location::DistanceTo(LPDISPATCH From)
{
   double result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x602001d, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
      From);
   return result;
}

void Location::Select()
{
   InvokeHelper(0x602001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Location::Copy()
{
   InvokeHelper(0x602002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Location::GetStreetAddress()
{
   LPDISPATCH result;
   InvokeHelper(0x6000902, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Location::GetType()
{
   long result;
   InvokeHelper(0x6000701, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// PlaceCategory properties

/////////////////////////////////////////////////////////////////////////////
// PlaceCategory operations

LPDISPATCH PlaceCategory::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH PlaceCategory::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString PlaceCategory::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

BOOL PlaceCategory::GetVisible()
{
   BOOL result;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void PlaceCategory::SetVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long PlaceCategory::GetIndex()
{
   long result;
   InvokeHelper(0x600001e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// PlaceCategories properties

/////////////////////////////////////////////////////////////////////////////
// PlaceCategories operations

LPDISPATCH PlaceCategories::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH PlaceCategories::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long PlaceCategories::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH PlaceCategories::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

void PlaceCategories::SetVisible(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long PlaceCategories::GetVisible()
{
   long result;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// FindResults properties

/////////////////////////////////////////////////////////////////////////////
// FindResults operations

LPDISPATCH FindResults::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH FindResults::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long FindResults::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH FindResults::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

long FindResults::GetResultsQuality()
{
   long result;
   InvokeHelper(0x6002201, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// StreetAddress properties

/////////////////////////////////////////////////////////////////////////////
// StreetAddress operations

LPDISPATCH StreetAddress::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH StreetAddress::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString StreetAddress::GetStreet()
{
   CString result;
   InvokeHelper(0x6002601, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString StreetAddress::GetCity()
{
   CString result;
   InvokeHelper(0x6002602, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString StreetAddress::GetOtherCity()
{
   CString result;
   InvokeHelper(0x6002603, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString StreetAddress::GetRegion()
{
   CString result;
   InvokeHelper(0x6002604, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString StreetAddress::GetPostalCode()
{
   CString result;
   InvokeHelper(0x6002605, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

long StreetAddress::GetCountry()
{
   long result;
   InvokeHelper(0x6000459, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString StreetAddress::GetValue()
{
   CString result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Route properties

/////////////////////////////////////////////////////////////////////////////
// Route operations

LPDISPATCH Route::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Route::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Route::GetWaypoints()
{
   LPDISPATCH result;
   InvokeHelper(0x6000a01, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL Route::GetIsCalculated()
{
   BOOL result;
   InvokeHelper(0x6000a02, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Route::Calculate()
{
   InvokeHelper(0x6020a07, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Route::Clear()
{
   InvokeHelper(0x6020a08, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

double Route::GetDistance()
{
   double result;
   InvokeHelper(0x6000a04, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

double Route::GetDrivingTime()
{
   double result;
   InvokeHelper(0x6000a05, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

double Route::GetTripTime()
{
   double result;
   InvokeHelper(0x6000a06, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

CY Route::GetCost()
{
   CY result;
   InvokeHelper(0x6000a09, DISPATCH_PROPERTYGET, VT_CY, (void*)&result, NULL);
   return result;
}

LPDISPATCH Route::GetDirections()
{
   LPDISPATCH result;
   InvokeHelper(0x6001b02, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Route::GetDriverProfile()
{
   LPDISPATCH result;
   InvokeHelper(0x6000a0a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Waypoints properties

/////////////////////////////////////////////////////////////////////////////
// Waypoints operations

LPDISPATCH Waypoints::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Waypoints::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Waypoints::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Waypoints::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

LPDISPATCH Waypoints::Add(LPDISPATCH Anchor, LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_BSTR;
   InvokeHelper(0x6020024, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Anchor, Name);
   return result;
}

BOOL Waypoints::GetIsOptimized()
{
   BOOL result;
   InvokeHelper(0x6020b01, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Waypoints::Optimize()
{
   InvokeHelper(0x6020b02, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Waypoint properties

/////////////////////////////////////////////////////////////////////////////
// Waypoint operations

LPDISPATCH Waypoint::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Waypoint::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Waypoint::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Waypoint::Delete()
{
   InvokeHelper(0xfffffcdf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Waypoint::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Waypoint::GetAnchor()
{
   LPDISPATCH result;
   InvokeHelper(0x6000c01, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void Waypoint::SetRefAnchor(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6000c01, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

DATE Waypoint::GetPreferredArrival()
{
   DATE result;
   InvokeHelper(0x6000c02, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
   return result;
}

void Waypoint::SetPreferredArrival(DATE newValue)
{
   static BYTE parms[] =
      VTS_DATE;
   InvokeHelper(0x6000c02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

DATE Waypoint::GetPreferredDeparture()
{
   DATE result;
   InvokeHelper(0x6000c03, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
   return result;
}

void Waypoint::SetPreferredDeparture(DATE newValue)
{
   static BYTE parms[] =
      VTS_DATE;
   InvokeHelper(0x6000c03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double Waypoint::GetStopTime()
{
   double result;
   InvokeHelper(0x6000c04, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void Waypoint::SetStopTime(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6000c04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

long Waypoint::GetOvernights()
{
   long result;
   InvokeHelper(0x6000c05, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void Waypoint::SetOvernights(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000c05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long Waypoint::GetSegmentPreferences()
{
   long result;
   InvokeHelper(0x6000c06, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void Waypoint::SetSegmentPreferences(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000c06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

void Waypoint::SetListPosition(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000c07, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long Waypoint::GetListPosition()
{
   long result;
   InvokeHelper(0x6000c07, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void Waypoint::Reorder(long Order)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6020c08, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Order);
}

LPDISPATCH Waypoint::GetSegmentDirections()
{
   LPDISPATCH result;
   InvokeHelper(0x6000c09, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void Waypoint::Select()
{
   InvokeHelper(0x602001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Waypoint::Copy()
{
   InvokeHelper(0x602002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Waypoint::Cut()
{
   InvokeHelper(0x602002a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long Waypoint::GetType()
{
   long result;
   InvokeHelper(0x6000701, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Directions properties

/////////////////////////////////////////////////////////////////////////////
// Directions operations

LPDISPATCH Directions::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Directions::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Directions::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Directions::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

void Directions::SetExpanded(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001a01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long Directions::GetExpanded()
{
   long result;
   InvokeHelper(0x6001a01, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Directions::FindNearby(double Distance)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x602001b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Distance);
   return result;
}

double Directions::DistanceTo(LPDISPATCH From)
{
   double result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x602001d, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
      From);
   return result;
}

LPDISPATCH Directions::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Direction properties

/////////////////////////////////////////////////////////////////////////////
// Direction operations

LPDISPATCH Direction::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Direction::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Direction::GetInstruction()
{
   CString result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

long Direction::GetIndex()
{
   long result;
   InvokeHelper(0x600001e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Direction::GetWaypoint()
{
   LPDISPATCH result;
   InvokeHelper(0x6001b07, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Direction::GetType()
{
   long result;
   InvokeHelper(0x6000701, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Direction::GetDirections()
{
   LPDISPATCH result;
   InvokeHelper(0x6001b02, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Direction::GetToward()
{
   CString result;
   InvokeHelper(0x6001b03, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

long Direction::GetAction()
{
   long result;
   InvokeHelper(0x6001b04, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

double Direction::GetDistance()
{
   double result;
   InvokeHelper(0x6000a04, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

DATE Direction::GetStartTime()
{
   DATE result;
   InvokeHelper(0x6001b05, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
   return result;
}

double Direction::GetElapsedDistance()
{
   double result;
   InvokeHelper(0x6001b06, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

LPDISPATCH Direction::FindNearby(double Distance)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x602001b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Distance);
   return result;
}

double Direction::DistanceTo(LPDISPATCH From)
{
   double result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x602001d, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
      From);
   return result;
}

LPDISPATCH Direction::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void Direction::Select()
{
   InvokeHelper(0x602001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// DriverProfile properties

/////////////////////////////////////////////////////////////////////////////
// DriverProfile operations

LPDISPATCH DriverProfile::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DriverProfile::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

DATE DriverProfile::GetStartTime()
{
   DATE result;
   InvokeHelper(0x6001b05, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetStartTime(DATE newValue)
{
   static BYTE parms[] =
      VTS_DATE;
   InvokeHelper(0x6001b05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

DATE DriverProfile::GetEndTime()
{
   DATE result;
   InvokeHelper(0x6001e02, DISPATCH_PROPERTYGET, VT_DATE, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetEndTime(DATE newValue)
{
   static BYTE parms[] =
      VTS_DATE;
   InvokeHelper(0x6001e02, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double DriverProfile::GetTimeFlexibility()
{
   double result;
   InvokeHelper(0x6001e03, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetTimeFlexibility(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e03, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

BOOL DriverProfile::GetShowConstruction()
{
   BOOL result;
   InvokeHelper(0x6001e04, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetShowConstruction(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6001e04, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

float DriverProfile::GetFuelTankCapacity()
{
   float result;
   InvokeHelper(0x6001e05, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetFuelTankCapacity(float newValue)
{
   static BYTE parms[] =
      VTS_R4;
   InvokeHelper(0x6001e05, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

long DriverProfile::GetFuelTankUnits()
{
   long result;
   InvokeHelper(0x6001e06, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetFuelTankUnits(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001e06, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

CY DriverProfile::GetDrivingCost()
{
   CY result;
   InvokeHelper(0x6001e07, DISPATCH_PROPERTYGET, VT_CY, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetDrivingCost(const CY& newValue)
{
   static BYTE parms[] =
      VTS_CY;
   InvokeHelper(0x6001e07, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       &newValue);
}

long DriverProfile::GetDrivingCostUnits()
{
   long result;
   InvokeHelper(0x6001e08, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetDrivingCostUnits(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001e08, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

BOOL DriverProfile::GetIncludeRefuelWarnings()
{
   BOOL result;
   InvokeHelper(0x6001e09, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetIncludeRefuelWarnings(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6001e09, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long DriverProfile::GetFuelConsumptionUnits()
{
   long result;
   InvokeHelper(0x6001e0a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetFuelConsumptionUnits(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001e0a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

double DriverProfile::GetTankStartLevel()
{
   double result;
   InvokeHelper(0x6001e0b, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetTankStartLevel(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e0b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double DriverProfile::GetTankWarnLevel()
{
   double result;
   InvokeHelper(0x6001e0c, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetTankWarnLevel(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double DriverProfile::GetFuelConsumptionCity()
{
   double result;
   InvokeHelper(0x6001e0d, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetFuelConsumptionCity(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e0d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double DriverProfile::GetFuelConsumptionHighway()
{
   double result;
   InvokeHelper(0x6001e0e, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetFuelConsumptionHighway(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e0e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

BOOL DriverProfile::GetIncludeRestStops()
{
   BOOL result;
   InvokeHelper(0x6001e0f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetIncludeRestStops(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6001e0f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

double DriverProfile::GetTimeBetweenRests()
{
   double result;
   InvokeHelper(0x6001e10, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetTimeBetweenRests(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double DriverProfile::GetRestStopDuration()
{
   double result;
   InvokeHelper(0x6001e11, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void DriverProfile::SetRestStopDuration(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6001e11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void DriverProfile::ResetSpeeds()
{
   InvokeHelper(0x6021e14, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void DriverProfile::ResetPreferredRoads()
{
   InvokeHelper(0x6021e15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long DriverProfile::GetSpeed(long RoadType)
{
   long result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001e12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
      RoadType);
   return result;
}

void DriverProfile::SetSpeed(long RoadType, long nNewValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4;
   InvokeHelper(0x6001e12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       RoadType, nNewValue);
}

double DriverProfile::GetPreferredRoads(long RoadClass)
{
   double result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001e13, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
      RoadClass);
   return result;
}

void DriverProfile::SetPreferredRoads(long RoadClass, double newValue)
{
   static BYTE parms[] =
      VTS_I4 VTS_R8;
   InvokeHelper(0x6001e13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       RoadClass, newValue);
}


/////////////////////////////////////////////////////////////////////////////
// DataSets properties

/////////////////////////////////////////////////////////////////////////////
// DataSets operations

LPDISPATCH DataSets::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataSets::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long DataSets::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataSets::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

LPDISPATCH DataSets::ImportData(LPCTSTR DataSourceMoniker, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x6020303, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      DataSourceMoniker, &ArrayOfFields, Country, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::LinkData(LPCTSTR DataSourceMoniker, const VARIANT& PrimaryKeyField, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x6020304, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      DataSourceMoniker, &PrimaryKeyField, &ArrayOfFields, Country, Delimiter, ImportFlags);
   return result;
}

void DataSets::ZoomTo()
{
   InvokeHelper(0x6020412, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH DataSets::GetDemographics(long Country)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6020305, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Country);
   return result;
}

LPDISPATCH DataSets::ShowImportWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
   InvokeHelper(0x6020306, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      HWndParent, DataSourceMoniker, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::ShowImportTerritoriesWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
   InvokeHelper(0x602030b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      HWndParent, DataSourceMoniker, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::ShowLinkWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
   InvokeHelper(0x6020307, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      HWndParent, DataSourceMoniker, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::ShowLinkTerritoriesWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
   InvokeHelper(0x602030c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      HWndParent, DataSourceMoniker, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::ShowDataMappingWizard(long HWndParent, LPDISPATCH DataSetToUse, long StartPage)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_DISPATCH VTS_I4;
   InvokeHelper(0x6020308, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      HWndParent, DataSetToUse, StartPage);
   return result;
}

LPDISPATCH DataSets::ImportTerritories(LPCTSTR DataSourceMoniker, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x6020309, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      DataSourceMoniker, &ArrayOfFields, Country, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::LinkTerritories(LPCTSTR DataSourceMoniker, const VARIANT& PrimaryKeyField, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x602030a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      DataSourceMoniker, &PrimaryKeyField, &ArrayOfFields, Country, Delimiter, ImportFlags);
   return result;
}

LPDISPATCH DataSets::AddPushpinSet(LPCTSTR Name)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x602030d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Name);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// DataSet properties

/////////////////////////////////////////////////////////////////////////////
// DataSet operations

LPDISPATCH DataSet::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataSet::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString DataSet::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void DataSet::SetName(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

void DataSet::Delete()
{
   InvokeHelper(0xfffffcdf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL DataSet::GetIsLinked()
{
   BOOL result;
   InvokeHelper(0x6000401, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

CString DataSet::GetSourceFullName()
{
   CString result;
   InvokeHelper(0x6000402, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

short DataSet::GetSymbol()
{
   short result;
   InvokeHelper(0x6000022, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
   return result;
}

void DataSet::SetSymbol(short nNewValue)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x6000022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long DataSet::GetRecordCount()
{
   long result;
   InvokeHelper(0x6000023, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataSet::GetMatchedRecordCount()
{
   long result;
   InvokeHelper(0x6000403, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataSet::GetUnmatchedRecordCount()
{
   long result;
   InvokeHelper(0x6000404, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataSet::GetSkippedRecordCount()
{
   long result;
   InvokeHelper(0x6000405, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataSet::GetFields()
{
   LPDISPATCH result;
   InvokeHelper(0x6000021, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataSet::QueryAllRecords()
{
   LPDISPATCH result;
   InvokeHelper(0x6020408, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataSet::QueryCircle(LPDISPATCH Center, double Radius)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_R8;
   InvokeHelper(0x6020410, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Center, Radius);
   return result;
}

long DataSet::GetDataMapType()
{
   long result;
   InvokeHelper(0x6000406, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void DataSet::DisplayPushpinMap()
{
   InvokeHelper(0x6020414, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH DataSet::GetDataMap()
{
   LPDISPATCH result;
   InvokeHelper(0x6000415, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void DataSet::UpdateLink()
{
   InvokeHelper(0x6020411, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void DataSet::ZoomTo()
{
   InvokeHelper(0x6020412, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH DataSet::QueryPolygon(const VARIANT& ArrayOfLocations)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6020450, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      &ArrayOfLocations);
   return result;
}

LPDISPATCH DataSet::QueryShape(LPDISPATCH Shape)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6020451, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Shape);
   return result;
}

LPDISPATCH DataSet::DisplayDataMap(long DataMapType, const VARIANT& DataField, long ShowDataBy, long CombineDataBy, long DataRangeType, long DataRangeOrder, long ColorScheme, long DataRangeCount, const VARIANT& ArrayOfCustomValues,
      const VARIANT& ArrayOfCustomNames, const VARIANT& DivideByField, const VARIANT& ArrayOfDataFieldLabels, const VARIANT& ArrayOfPushpinSymbols)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
   InvokeHelper(0x6020452, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      DataMapType, &DataField, ShowDataBy, CombineDataBy, DataRangeType, DataRangeOrder, ColorScheme, DataRangeCount, &ArrayOfCustomValues, &ArrayOfCustomNames, &DivideByField, &ArrayOfDataFieldLabels, &ArrayOfPushpinSymbols);
   return result;
}

void DataSet::Select()
{
   InvokeHelper(0x602001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void DataSet::Copy()
{
   InvokeHelper(0x602002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void DataSet::Cut()
{
   InvokeHelper(0x602002a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long DataSet::GetHyperlinkType()
{
   long result;
   InvokeHelper(0x6000453, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void DataSet::SetHyperlinkType(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000453, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

CString DataSet::GetHyperlinkCalculated()
{
   CString result;
   InvokeHelper(0x6000454, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void DataSet::SetHyperlinkCalculated(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6000454, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

LPDISPATCH DataSet::GetHyperlinkField()
{
   LPDISPATCH result;
   InvokeHelper(0x6000455, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void DataSet::SetHyperlinkField(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6000455, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

BOOL DataSet::GetFieldNamesVisibleInBalloon()
{
   BOOL result;
   InvokeHelper(0x6000456, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void DataSet::SetFieldNamesVisibleInBalloon(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000456, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

void DataSet::SetFieldsVisibleInBalloon(const VARIANT& ArrayOfFields)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6020457, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &ArrayOfFields);
}

long DataSet::GetHowCreated()
{
   long result;
   InvokeHelper(0x6000458, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataSet::GetMatchingMethod()
{
   long result;
   InvokeHelper(0x600002d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataSet::GetCountry()
{
   long result;
   InvokeHelper(0x6000459, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

BOOL DataSet::Paste()
{
   BOOL result;
   InvokeHelper(0x602002c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Fields properties

/////////////////////////////////////////////////////////////////////////////
// Fields operations

LPDISPATCH Fields::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Fields::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Fields::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Fields::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Field properties

/////////////////////////////////////////////////////////////////////////////
// Field operations

LPDISPATCH Field::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Field::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Field::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

VARIANT Field::GetValue()
{
   VARIANT result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
   return result;
}

long Field::GetType()
{
   long result;
   InvokeHelper(0x6000701, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long Field::GetGeographicFieldType()
{
   long result;
   InvokeHelper(0x6000702, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

BOOL Field::GetIsPrimaryKey()
{
   BOOL result;
   InvokeHelper(0x6000704, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL Field::GetVisibleInBalloon()
{
   BOOL result;
   InvokeHelper(0x6000705, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Recordset properties

/////////////////////////////////////////////////////////////////////////////
// Recordset operations

LPDISPATCH Recordset::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Recordset::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Recordset::GetPushpin()
{
   LPDISPATCH result;
   InvokeHelper(0x6000501, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Recordset::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Recordset::GetFields()
{
   LPDISPATCH result;
   InvokeHelper(0x6000021, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL Recordset::GetBof()
{
   BOOL result;
   InvokeHelper(0x6000502, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

BOOL Recordset::GetEof()
{
   BOOL result;
   InvokeHelper(0x6000503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Recordset::MoveFirst()
{
   InvokeHelper(0x6020505, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Recordset::MoveNext()
{
   InvokeHelper(0x6020506, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Recordset::MoveToPushpin(LPDISPATCH Pushpin)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6020507, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Pushpin);
}

BOOL Recordset::GetIsMatched()
{
   BOOL result;
   InvokeHelper(0x6000504, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Recordset::MatchRecord(BOOL ShowDialogIfAmbiguous, long HWndParent)
{
   static BYTE parms[] =
      VTS_BOOL VTS_I4;
   InvokeHelper(0x6020407, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       ShowDialogIfAmbiguous, HWndParent);
}

long Recordset::GetMatchingMethod()
{
   long result;
   InvokeHelper(0x600002d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

BOOL Recordset::CallMatchMethod(LPCTSTR MethodName, LPDISPATCH MethodObject)
{
   BOOL result;
   static BYTE parms[] =
      VTS_BSTR VTS_DISPATCH;
   InvokeHelper(0x6020508, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      MethodName, MethodObject);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Pushpin properties

/////////////////////////////////////////////////////////////////////////////
// Pushpin operations

LPDISPATCH Pushpin::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Pushpin::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Pushpin::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Pushpin::SetName(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

void Pushpin::Delete()
{
   InvokeHelper(0xfffffcdf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Pushpin::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void Pushpin::SetRefLocation(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

CString Pushpin::GetNote()
{
   CString result;
   InvokeHelper(0x6000802, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Pushpin::SetNote(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6000802, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

BOOL Pushpin::GetHighlight()
{
   BOOL result;
   InvokeHelper(0x6000801, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Pushpin::SetHighlight(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000801, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

short Pushpin::GetSymbol()
{
   short result;
   InvokeHelper(0x6000022, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
   return result;
}

void Pushpin::SetSymbol(short nNewValue)
{
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x6000022, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long Pushpin::GetBalloonState()
{
   long result;
   InvokeHelper(0x6000803, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void Pushpin::SetBalloonState(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000803, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

void Pushpin::Select()
{
   InvokeHelper(0x602001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Pushpin::Copy()
{
   InvokeHelper(0x602002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Pushpin::Cut()
{
   InvokeHelper(0x602002a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Pushpin::GetHyperlink()
{
   LPDISPATCH result;
   InvokeHelper(0x6000804, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL Pushpin::MoveTo(LPDISPATCH NewDataSet)
{
   BOOL result;
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6020805, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
      NewDataSet);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Hyperlink properties

/////////////////////////////////////////////////////////////////////////////
// Hyperlink operations

LPDISPATCH Hyperlink::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Hyperlink::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0x6000012, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Hyperlink::GetAddress()
{
   CString result;
   InvokeHelper(0x6002101, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString Hyperlink::GetScreenTip()
{
   CString result;
   InvokeHelper(0x6002102, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString Hyperlink::GetSubaddress()
{
   CString result;
   InvokeHelper(0x6002103, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

CString Hyperlink::GetTextToDisplay()
{
   CString result;
   InvokeHelper(0x6002104, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Hyperlink::Follow()
{
   InvokeHelper(0x6022105, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// DataMap properties

/////////////////////////////////////////////////////////////////////////////
// DataMap operations

LPDISPATCH DataMap::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataMap::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long DataMap::GetRecordCount()
{
   long result;
   InvokeHelper(0x6000023, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

CString DataMap::GetLegendTitle()
{
   CString result;
   InvokeHelper(0x6000d01, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void DataMap::SetLegendTitle(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6000d01, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long DataMap::GetShowDataBy()
{
   long result;
   InvokeHelper(0x6000d03, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataMap::GetCombineDataBy()
{
   long result;
   InvokeHelper(0x6000d04, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataMap::GetDataRanges()
{
   LPDISPATCH result;
   InvokeHelper(0x6000d05, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataMap::GetDivideByField()
{
   LPDISPATCH result;
   InvokeHelper(0x6000d06, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

BOOL DataMap::GetIsGeographyForEveryZoomLevel()
{
   BOOL result;
   InvokeHelper(0x6000d07, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

VARIANT DataMap::GetDataFields()
{
   VARIANT result;
   InvokeHelper(0x6000d08, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
   return result;
}

VARIANT DataMap::GetDataFieldLabels()
{
   VARIANT result;
   InvokeHelper(0x6000d09, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// DataRanges properties

/////////////////////////////////////////////////////////////////////////////
// DataRanges operations

LPDISPATCH DataRanges::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataRanges::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long DataRanges::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataRanges::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

long DataRanges::GetDataRangeType()
{
   long result;
   InvokeHelper(0x6000f01, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataRanges::GetDataRangeOrder()
{
   long result;
   InvokeHelper(0x6000f02, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long DataRanges::GetColorScheme()
{
   long result;
   InvokeHelper(0x6000f03, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

BOOL DataRanges::GetIsCustomized()
{
   BOOL result;
   InvokeHelper(0x6000f04, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// DataRange properties

/////////////////////////////////////////////////////////////////////////////
// DataRange operations

LPDISPATCH DataRange::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH DataRange::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString DataRange::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

VARIANT DataRange::GetLowValue()
{
   VARIANT result;
   InvokeHelper(0x6000e01, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
   return result;
}

VARIANT DataRange::GetHighValue()
{
   VARIANT result;
   InvokeHelper(0x6000e02, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
   return result;
}

long DataRange::GetValueCount()
{
   long result;
   InvokeHelper(0x6000e03, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

short DataRange::GetSymbol()
{
   short result;
   InvokeHelper(0x6000022, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Shape properties

/////////////////////////////////////////////////////////////////////////////
// Shape operations

LPDISPATCH Shape::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Shape::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Shape::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Shape::SetName(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

void Shape::Delete()
{
   InvokeHelper(0xfffffcdf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Shape::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void Shape::SetRefLocation(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms,
       newValue);
}

VARIANT Shape::GetVertices()
{
   VARIANT result;
   InvokeHelper(0x6001205, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
   return result;
}

void Shape::SetVertices(const VARIANT& newValue)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6001205, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       &newValue);
}

LPDISPATCH Shape::GetAdjustments()
{
   LPDISPATCH result;
   InvokeHelper(0x6001206, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

double Shape::GetHeight()
{
   double result;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void Shape::SetHeight(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double Shape::GetWidth()
{
   double result;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void Shape::SetWidth(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

void Shape::ZOrder(long ZOrderCmd)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6021201, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       ZOrderCmd);
}

long Shape::GetZOrderPosition()
{
   long result;
   InvokeHelper(0x6001202, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void Shape::SetZOrderPosition(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001202, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

LPDISPATCH Shape::GetLine()
{
   LPDISPATCH result;
   InvokeHelper(0x6001203, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Shape::GetFill()
{
   LPDISPATCH result;
   InvokeHelper(0x6001204, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Shape::GetType()
{
   long result;
   InvokeHelper(0x6000701, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long Shape::GetAutoShapeType()
{
   long result;
   InvokeHelper(0x6001207, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

BOOL Shape::GetSizeVisible()
{
   BOOL result;
   InvokeHelper(0x6001208, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Shape::SetSizeVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6001208, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

CString Shape::GetText()
{
   CString result;
   InvokeHelper(0x600120a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Shape::SetText(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x600120a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

long Shape::GetFontColor()
{
   long result;
   InvokeHelper(0x6001209, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void Shape::SetFontColor(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001209, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

BOOL Shape::GetAvoided()
{
   BOOL result;
   InvokeHelper(0x600120b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Shape::SetAvoided(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x600120b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

void Shape::Select()
{
   InvokeHelper(0x602001f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape::Copy()
{
   InvokeHelper(0x602002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Shape::Cut()
{
   InvokeHelper(0x602002a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Shapes properties

/////////////////////////////////////////////////////////////////////////////
// Shapes operations

LPDISPATCH Shapes::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Shapes::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Shapes::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Shapes::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

LPDISPATCH Shapes::AddLine(LPDISPATCH BeginLocation, LPDISPATCH EndLocation)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_DISPATCH;
   InvokeHelper(0x6021101, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      BeginLocation, EndLocation);
   return result;
}

LPDISPATCH Shapes::AddPolyline(const VARIANT& ArrayOfLocations)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6021102, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      &ArrayOfLocations);
   return result;
}

LPDISPATCH Shapes::AddShape(long Type, LPDISPATCH Center, double Width, double Height)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I4 VTS_DISPATCH VTS_R8 VTS_R8;
   InvokeHelper(0x6021103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Type, Center, Width, Height);
   return result;
}

LPDISPATCH Shapes::AddDrivetimeZone(LPDISPATCH Center, double Time)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_R8;
   InvokeHelper(0x6021104, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Center, Time);
   return result;
}

LPDISPATCH Shapes::AddTextbox(LPDISPATCH Center, double Width, double Height)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_DISPATCH VTS_R8 VTS_R8;
   InvokeHelper(0x6021105, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      Center, Width, Height);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Adjustments properties

/////////////////////////////////////////////////////////////////////////////
// Adjustments operations

LPDISPATCH Adjustments::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Adjustments::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Adjustments::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

double Adjustments::GetItem(VARIANT* Index)
{
   double result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms,
      Index);
   return result;
}

void Adjustments::SetItem(VARIANT* Index, double newValue)
{
   static BYTE parms[] =
      VTS_PVARIANT VTS_R8;
   InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       Index, newValue);
}


/////////////////////////////////////////////////////////////////////////////
// LineFormat properties

/////////////////////////////////////////////////////////////////////////////
// LineFormat operations

LPDISPATCH LineFormat::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH LineFormat::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long LineFormat::GetForeColor()
{
   long result;
   InvokeHelper(0x600001a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void LineFormat::SetForeColor(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x600001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

BOOL LineFormat::GetVisible()
{
   BOOL result;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void LineFormat::SetVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long LineFormat::GetWeight()
{
   long result;
   InvokeHelper(0x6001303, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void LineFormat::SetWeight(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6001303, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

BOOL LineFormat::GetBeginArrowhead()
{
   BOOL result;
   InvokeHelper(0x6001301, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void LineFormat::SetBeginArrowhead(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6001301, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL LineFormat::GetEndArrowhead()
{
   BOOL result;
   InvokeHelper(0x6001302, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void LineFormat::SetEndArrowhead(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6001302, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// FillFormat properties

/////////////////////////////////////////////////////////////////////////////
// FillFormat operations

LPDISPATCH FillFormat::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH FillFormat::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long FillFormat::GetForeColor()
{
   long result;
   InvokeHelper(0x600001a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void FillFormat::SetForeColor(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x600001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

BOOL FillFormat::GetVisible()
{
   BOOL result;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void FillFormat::SetVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// SelectedArea properties

/////////////////////////////////////////////////////////////////////////////
// SelectedArea operations

LPDISPATCH SelectedArea::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH SelectedArea::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long SelectedArea::GetTop()
{
   long result;
   InvokeHelper(0x6000115, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long SelectedArea::GetLeft()
{
   long result;
   InvokeHelper(0x6000113, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long SelectedArea::GetWidth()
{
   long result;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

long SelectedArea::GetHeight()
{
   long result;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH SelectedArea::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void SelectedArea::Copy()
{
   InvokeHelper(0x602002b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SelectedArea::SelectArea(long Top, long Left, long Width, long Height)
{
   static BYTE parms[] =
      VTS_I4 VTS_I4 VTS_I4 VTS_I4;
   InvokeHelper(0x602021c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       Top, Left, Width, Height);
}


/////////////////////////////////////////////////////////////////////////////
// PageSetup properties

/////////////////////////////////////////////////////////////////////////////
// PageSetup operations

LPDISPATCH PageSetup::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH PageSetup::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long PageSetup::GetPaperSize()
{
   long result;
   InvokeHelper(0x6002001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void PageSetup::SetPaperSize(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6002001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

double PageSetup::GetRightMargin()
{
   double result;
   InvokeHelper(0x6002003, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void PageSetup::SetRightMargin(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6002003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double PageSetup::GetLeftMargin()
{
   double result;
   InvokeHelper(0x6002004, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void PageSetup::SetLeftMargin(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6002004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double PageSetup::GetTopMargin()
{
   double result;
   InvokeHelper(0x6002005, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void PageSetup::SetTopMargin(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6002005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double PageSetup::GetBottomMargin()
{
   double result;
   InvokeHelper(0x6002006, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void PageSetup::SetBottomMargin(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x6002006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

BOOL PageSetup::GetIncludeRouteSummary()
{
   BOOL result;
   InvokeHelper(0x6002008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void PageSetup::SetIncludeRouteSummary(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long PageSetup::GetRoutePageBreakFrequency()
{
   long result;
   InvokeHelper(0x6002009, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void PageSetup::SetRoutePageBreakFrequency(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6002009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

double PageSetup::GetRoutePageBreakDistance()
{
   double result;
   InvokeHelper(0x600200a, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void PageSetup::SetRoutePageBreakDistance(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x600200a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

double PageSetup::GetRoutePageBreakTime()
{
   double result;
   InvokeHelper(0x600200b, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
   return result;
}

void PageSetup::SetRoutePageBreakTime(double newValue)
{
   static BYTE parms[] =
      VTS_R8;
   InvokeHelper(0x600200b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}


/////////////////////////////////////////////////////////////////////////////
// SavedWebPages properties

/////////////////////////////////////////////////////////////////////////////
// SavedWebPages operations

LPDISPATCH SavedWebPages::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH SavedWebPages::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long SavedWebPages::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH SavedWebPages::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

void SavedWebPages::Save()
{
   InvokeHelper(0x6020209, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SavedWebPages::Delete()
{
   InvokeHelper(0x6020017, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH SavedWebPages::Add(LPCTSTR FileName, LPDISPATCH Location, LPCTSTR Title, BOOL IncludeMap, BOOL IncludeThumbnail, BOOL IncludeHyperlinks, long Width, long Height, BOOL IncludeDirections, BOOL IncludeLegend, BOOL IncludeOverview,
      BOOL IncludeSourceFile)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR VTS_DISPATCH VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
   InvokeHelper(0x6020024, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      FileName, Location, Title, IncludeMap, IncludeThumbnail, IncludeHyperlinks, Width, Height, IncludeDirections, IncludeLegend, IncludeOverview, IncludeSourceFile);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// SavedWebPage properties

/////////////////////////////////////////////////////////////////////////////
// SavedWebPage operations

LPDISPATCH SavedWebPage::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH SavedWebPage::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString SavedWebPage::GetFileName()
{
   CString result;
   InvokeHelper(0x6002501, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetFileName(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6002501, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

LPDISPATCH SavedWebPage::GetLocation()
{
   LPDISPATCH result;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetLocation(LPDISPATCH newValue)
{
   static BYTE parms[] =
      VTS_DISPATCH;
   InvokeHelper(0x6000016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       newValue);
}

BOOL SavedWebPage::GetIncludeMap()
{
   BOOL result;
   InvokeHelper(0x6002502, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeMap(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002502, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL SavedWebPage::GetIncludeDirections()
{
   BOOL result;
   InvokeHelper(0x6002503, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeDirections(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002503, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL SavedWebPage::GetIncludeLegend()
{
   BOOL result;
   InvokeHelper(0x6002504, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeLegend(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002504, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL SavedWebPage::GetIncludeOverview()
{
   BOOL result;
   InvokeHelper(0x6002505, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeOverview(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002505, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL SavedWebPage::GetIncludeHyperlinks()
{
   BOOL result;
   InvokeHelper(0x6002506, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeHyperlinks(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002506, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL SavedWebPage::GetIncludeThumbnail()
{
   BOOL result;
   InvokeHelper(0x6002509, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeThumbnail(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002509, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

CString SavedWebPage::GetTitle()
{
   CString result;
   InvokeHelper(0x6002507, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetTitle(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6002507, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

BOOL SavedWebPage::GetIncludeSourceFile()
{
   BOOL result;
   InvokeHelper(0x6002508, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetIncludeSourceFile(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002508, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long SavedWebPage::GetWidth()
{
   long result;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetWidth(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

long SavedWebPage::GetHeight()
{
   long result;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

void SavedWebPage::SetHeight(long nNewValue)
{
   static BYTE parms[] =
      VTS_I4;
   InvokeHelper(0x6000112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       nNewValue);
}

void SavedWebPage::Delete()
{
   InvokeHelper(0xfffffcdf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void SavedWebPage::SetAutoResave(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x600250a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL SavedWebPage::GetAutoResave()
{
   BOOL result;
   InvokeHelper(0x600250a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void SavedWebPage::Save()
{
   InvokeHelper(0x6020209, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Symbols properties

/////////////////////////////////////////////////////////////////////////////
// Symbols operations

LPDISPATCH Symbols::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Symbols::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Symbols::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Symbols::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

LPDISPATCH Symbols::Add(LPCTSTR FileName)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6020024, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      FileName);
   return result;
}

LPDISPATCH Symbols::ItemByID(short ID)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_I2;
   InvokeHelper(0x6022701, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      ID);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Symbol properties

/////////////////////////////////////////////////////////////////////////////
// Symbol operations

LPDISPATCH Symbol::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Symbol::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Symbol::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

void Symbol::SetName(LPCTSTR lpszNewValue)
{
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       lpszNewValue);
}

short Symbol::GetId()
{
   short result;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
   return result;
}

void Symbol::Delete()
{
   InvokeHelper(0xfffffcdf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL Symbol::GetIsCustom()
{
   BOOL result;
   InvokeHelper(0x6002802, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Symbol::SetUseTransparency(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6002803, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

BOOL Symbol::GetUseTransparency()
{
   BOOL result;
   InvokeHelper(0x6002803, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Toolbars properties

/////////////////////////////////////////////////////////////////////////////
// Toolbars operations

LPDISPATCH Toolbars::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Toolbars::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long Toolbars::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH Toolbars::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// Toolbar properties

/////////////////////////////////////////////////////////////////////////////
// Toolbar operations

LPDISPATCH Toolbar::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH Toolbar::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

CString Toolbar::GetName()
{
   CString result;
   InvokeHelper(0xfffffce0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
   return result;
}

BOOL Toolbar::GetVisible()
{
   BOOL result;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
   return result;
}

void Toolbar::SetVisible(BOOL bNewValue)
{
   static BYTE parms[] =
      VTS_BOOL;
   InvokeHelper(0x6000116, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
       bNewValue);
}

long Toolbar::GetIndex()
{
   long result;
   InvokeHelper(0x600001e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}


/////////////////////////////////////////////////////////////////////////////
// AddIns properties

/////////////////////////////////////////////////////////////////////////////
// AddIns operations

LPDISPATCH AddIns::GetApplication()
{
   LPDISPATCH result;
   InvokeHelper(0x6000011, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

LPDISPATCH AddIns::GetParent()
{
   LPDISPATCH result;
   InvokeHelper(0xfffffcdd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
   return result;
}

long AddIns::GetCount()
{
   long result;
   InvokeHelper(0x6000019, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
   return result;
}

LPDISPATCH AddIns::GetItem(VARIANT* Index)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_PVARIANT;
   InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
      Index);
   return result;
}

LPDISPATCH AddIns::Connect(LPCTSTR ProgId)
{
   LPDISPATCH result;
   static BYTE parms[] =
      VTS_BSTR;
   InvokeHelper(0x6022901, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
      ProgId);
   return result;
}

void AddIns::Disconnect(const VARIANT& ObjectOrProgId)
{
   static BYTE parms[] =
      VTS_VARIANT;
   InvokeHelper(0x6022902, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
       &ObjectOrProgId);
}
