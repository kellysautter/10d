// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class

class _Application : public COleDispatchDriver
{
public:
   _Application() {}    // Calls COleDispatchDriver default constructor
   _Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   _Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   LPDISPATCH GetActiveMap();
   LPDISPATCH OpenMap(LPCTSTR FileName, BOOL AddToRecentFiles);
   LPDISPATCH NewMap(LPCTSTR Template);
   CString GetCaption();
   void SetCaption(LPCTSTR lpszNewValue);
   CString GetPath();
   CString GetDefaultFilePath();
   CString GetVersion();
   CString GetBuild();
   void Activate();
   long GetHeight();
   void SetHeight(long nNewValue);
   long GetLeft();
   void SetLeft(long nNewValue);
   long GetWidth();
   void SetWidth(long nNewValue);
   long GetTop();
   void SetTop(long nNewValue);
   BOOL GetVisible();
   void SetVisible(BOOL bNewValue);
   long GetWindowState();
   void SetWindowState(long nNewValue);
   void AddCommand(LPCTSTR DisplayName, LPCTSTR MethodName, LPDISPATCH Target);
   void RemoveCommands(LPDISPATCH Target);
   long GetUnits();
   void SetUnits(long nNewValue);
   long GetPaneState();
   void SetPaneState(long nNewValue);
   long GetRegion();
   void Quit();
   BOOL GetUserControl();
   void SetUserControl(BOOL bNewValue);
   BOOL GetItineraryVisible();
   void SetItineraryVisible(BOOL bNewValue);
   LPDISPATCH GetToolbars();
   CString GetActivePrinter();
   void SetActivePrinter(LPCTSTR lpszNewValue);
   long GetMousePointer();
   void SetMousePointer(long nNewValue);
   LPDISPATCH GetAddIns();
};
/////////////////////////////////////////////////////////////////////////////
// _Map wrapper class

class _Map : public COleDispatchDriver
{
public:
   _Map() {}      // Calls COleDispatchDriver default constructor
   _Map(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   _Map(const _Map& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   LPDISPATCH GetContainer();
   BOOL GetSaved();
   void SetSaved(BOOL bNewValue);
   CString GetFullName();
   CString GetPath();
   void SetRefLocation(LPDISPATCH newValue);
   LPDISPATCH GetLocation();
   double GetAltitude();
   void SetAltitude(double newValue);
   LPDISPATCH GetActiveRoute();
   LPDISPATCH GetDataSets();
   long GetMapStyle();
   void SetMapStyle(long nNewValue);
   long GetMapFont();
   void SetMapFont(long nNewValue);
   void Save();
   void SaveAs(LPCTSTR FileName, long SaveFormat, BOOL AddToRecentFiles);
   void PrintOut(LPCTSTR OutputFileName, LPCTSTR Title, long Copies, long PrintArea, long PrintQuality, long PrintOrientation, BOOL Collate, BOOL IncludeLegend, BOOL IncludeOverview, BOOL Faxable);
   LPDISPATCH FindPushpin(LPCTSTR Name);
   LPDISPATCH AddPushpin(LPDISPATCH AtLocation, LPCTSTR Name);
   void CopyMap();
   void CopyDirections();
   double Distance(LPDISPATCH StartLocation, LPDISPATCH EndLocation);
   LPDISPATCH GetLocation(double Latitude, double Longitude, double Altitude);
   LPDISPATCH GetSelection();
   LPDISPATCH GetShapes();
   LPDISPATCH GetPlaceCategories();
   LPDISPATCH FindResults(LPCTSTR Name);
   LPDISPATCH FindAddressResults(LPCTSTR Street, LPCTSTR City, LPCTSTR OtherCity, LPCTSTR Region, LPCTSTR PostalCode, const VARIANT& Country);
   LPDISPATCH ShowFindDialog(LPCTSTR Name, long FindState, long HWndParent, BOOL AutoConfirmExactMatch);
   long GetTop();
   long GetLeft();
   long GetHeight();
   long GetWidth();
   LPDISPATCH XYToLocation(long X, long Y);
   long LocationToX(LPDISPATCH Where);
   long LocationToY(LPDISPATCH Where);
   double GetPixelSize();
   void Pan(long PanDirection, double PanFactor);
   LPDISPATCH GetSelectedArea();
   LPDISPATCH Union(const VARIANT& ArrayOfLocations);
   LPDISPATCH ObjectsFromPoint(long X, long Y);
   void ZoomIn();
   void ZoomOut();
   void GoBack();
   void GoForward();
   void Paste();
   long GetProjection();
   void SetProjection(long nNewValue);
   LPDISPATCH GetPageSetup();
   BOOL GetAllowEdgePan();
   void SetAllowEdgePan(BOOL bNewValue);
   long GetDragBehavior();
   void SetDragBehavior(long nNewValue);
   LPDISPATCH GetSavedWebPages();
   LPDISPATCH ParseStreetAddress(LPCTSTR Address);
   LPDISPATCH GetSymbols();
   LPDISPATCH FindPlaceResults(LPCTSTR Name);
};
/////////////////////////////////////////////////////////////////////////////
// Location wrapper class

class Location : public COleDispatchDriver
{
public:
   Location() {}     // Calls COleDispatchDriver default constructor
   Location(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Location(const Location& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   void GoTo();
   LPDISPATCH GetLocation();
   LPDISPATCH GetPlaceCategory();
   LPDISPATCH FindNearby(double Distance);
   BOOL GetHighlight();
   void SetHighlight(BOOL bNewValue);
   BOOL GetIsHighlightable();
   double DistanceTo(LPDISPATCH From);
   void Select();
   void Copy();
   LPDISPATCH GetStreetAddress();
   long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// PlaceCategory wrapper class

class PlaceCategory : public COleDispatchDriver
{
public:
   PlaceCategory() {}      // Calls COleDispatchDriver default constructor
   PlaceCategory(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   PlaceCategory(const PlaceCategory& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   BOOL GetVisible();
   void SetVisible(BOOL bNewValue);
   long GetIndex();
};
/////////////////////////////////////////////////////////////////////////////
// PlaceCategories wrapper class

class PlaceCategories : public COleDispatchDriver
{
public:
   PlaceCategories() {}    // Calls COleDispatchDriver default constructor
   PlaceCategories(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   PlaceCategories(const PlaceCategories& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   void SetVisible(long nNewValue);
   long GetVisible();
};
/////////////////////////////////////////////////////////////////////////////
// FindResults wrapper class

class FindResults : public COleDispatchDriver
{
public:
   FindResults() {}     // Calls COleDispatchDriver default constructor
   FindResults(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   FindResults(const FindResults& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   long GetResultsQuality();
};
/////////////////////////////////////////////////////////////////////////////
// StreetAddress wrapper class

class StreetAddress : public COleDispatchDriver
{
public:
   StreetAddress() {}      // Calls COleDispatchDriver default constructor
   StreetAddress(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   StreetAddress(const StreetAddress& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetStreet();
   CString GetCity();
   CString GetOtherCity();
   CString GetRegion();
   CString GetPostalCode();
   long GetCountry();
   CString GetValue();
};
/////////////////////////////////////////////////////////////////////////////
// Route wrapper class

class Route : public COleDispatchDriver
{
public:
   Route() {}     // Calls COleDispatchDriver default constructor
   Route(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Route(const Route& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   LPDISPATCH GetWaypoints();
   BOOL GetIsCalculated();
   void Calculate();
   void Clear();
   double GetDistance();
   double GetDrivingTime();
   double GetTripTime();
   CY GetCost();
   LPDISPATCH GetDirections();
   LPDISPATCH GetDriverProfile();
};
/////////////////////////////////////////////////////////////////////////////
// Waypoints wrapper class

class Waypoints : public COleDispatchDriver
{
public:
   Waypoints() {}    // Calls COleDispatchDriver default constructor
   Waypoints(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Waypoints(const Waypoints& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   LPDISPATCH Add(LPDISPATCH Anchor, LPCTSTR Name);
   BOOL GetIsOptimized();
   void Optimize();
};
/////////////////////////////////////////////////////////////////////////////
// Waypoint wrapper class

class Waypoint : public COleDispatchDriver
{
public:
   Waypoint() {}     // Calls COleDispatchDriver default constructor
   Waypoint(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Waypoint(const Waypoint& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   void Delete();
   LPDISPATCH GetLocation();
   LPDISPATCH GetAnchor();
   void SetRefAnchor(LPDISPATCH newValue);
   DATE GetPreferredArrival();
   void SetPreferredArrival(DATE newValue);
   DATE GetPreferredDeparture();
   void SetPreferredDeparture(DATE newValue);
   double GetStopTime();
   void SetStopTime(double newValue);
   long GetOvernights();
   void SetOvernights(long nNewValue);
   long GetSegmentPreferences();
   void SetSegmentPreferences(long nNewValue);
   void SetListPosition(long nNewValue);
   long GetListPosition();
   void Reorder(long Order);
   LPDISPATCH GetSegmentDirections();
   void Select();
   void Copy();
   void Cut();
   long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// Directions wrapper class

class Directions : public COleDispatchDriver
{
public:
   Directions() {}      // Calls COleDispatchDriver default constructor
   Directions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Directions(const Directions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   void SetExpanded(long nNewValue);
   long GetExpanded();
   LPDISPATCH FindNearby(double Distance);
   double DistanceTo(LPDISPATCH From);
   LPDISPATCH GetLocation();
};
/////////////////////////////////////////////////////////////////////////////
// Direction wrapper class

class Direction : public COleDispatchDriver
{
public:
   Direction() {}    // Calls COleDispatchDriver default constructor
   Direction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Direction(const Direction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetInstruction();
   long GetIndex();
   LPDISPATCH GetWaypoint();
   long GetType();
   LPDISPATCH GetDirections();
   CString GetToward();
   long GetAction();
   double GetDistance();
   DATE GetStartTime();
   double GetElapsedDistance();
   LPDISPATCH FindNearby(double Distance);
   double DistanceTo(LPDISPATCH From);
   LPDISPATCH GetLocation();
   void Select();
};
/////////////////////////////////////////////////////////////////////////////
// DriverProfile wrapper class

class DriverProfile : public COleDispatchDriver
{
public:
   DriverProfile() {}      // Calls COleDispatchDriver default constructor
   DriverProfile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DriverProfile(const DriverProfile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   DATE GetStartTime();
   void SetStartTime(DATE newValue);
   DATE GetEndTime();
   void SetEndTime(DATE newValue);
   double GetTimeFlexibility();
   void SetTimeFlexibility(double newValue);
   BOOL GetShowConstruction();
   void SetShowConstruction(BOOL bNewValue);
   float GetFuelTankCapacity();
   void SetFuelTankCapacity(float newValue);
   long GetFuelTankUnits();
   void SetFuelTankUnits(long nNewValue);
   CY GetDrivingCost();
   void SetDrivingCost(const CY& newValue);
   long GetDrivingCostUnits();
   void SetDrivingCostUnits(long nNewValue);
   BOOL GetIncludeRefuelWarnings();
   void SetIncludeRefuelWarnings(BOOL bNewValue);
   long GetFuelConsumptionUnits();
   void SetFuelConsumptionUnits(long nNewValue);
   double GetTankStartLevel();
   void SetTankStartLevel(double newValue);
   double GetTankWarnLevel();
   void SetTankWarnLevel(double newValue);
   double GetFuelConsumptionCity();
   void SetFuelConsumptionCity(double newValue);
   double GetFuelConsumptionHighway();
   void SetFuelConsumptionHighway(double newValue);
   BOOL GetIncludeRestStops();
   void SetIncludeRestStops(BOOL bNewValue);
   double GetTimeBetweenRests();
   void SetTimeBetweenRests(double newValue);
   double GetRestStopDuration();
   void SetRestStopDuration(double newValue);
   void ResetSpeeds();
   void ResetPreferredRoads();
   long GetSpeed(long RoadType);
   void SetSpeed(long RoadType, long nNewValue);
   double GetPreferredRoads(long RoadClass);
   void SetPreferredRoads(long RoadClass, double newValue);
};
/////////////////////////////////////////////////////////////////////////////
// DataSets wrapper class

class DataSets : public COleDispatchDriver
{
public:
   DataSets() {}     // Calls COleDispatchDriver default constructor
   DataSets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataSets(const DataSets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   LPDISPATCH ImportData(LPCTSTR DataSourceMoniker, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags);
   LPDISPATCH LinkData(LPCTSTR DataSourceMoniker, const VARIANT& PrimaryKeyField, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags);
   void ZoomTo();
   LPDISPATCH GetDemographics(long Country);
   LPDISPATCH ShowImportWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags);
   LPDISPATCH ShowImportTerritoriesWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags);
   LPDISPATCH ShowLinkWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags);
   LPDISPATCH ShowLinkTerritoriesWizard(long HWndParent, LPCTSTR DataSourceMoniker, long Delimiter, long ImportFlags);
   LPDISPATCH ShowDataMappingWizard(long HWndParent, LPDISPATCH DataSetToUse, long StartPage);
   LPDISPATCH ImportTerritories(LPCTSTR DataSourceMoniker, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags);
   LPDISPATCH LinkTerritories(LPCTSTR DataSourceMoniker, const VARIANT& PrimaryKeyField, const VARIANT& ArrayOfFields, long Country, long Delimiter, long ImportFlags);
   LPDISPATCH AddPushpinSet(LPCTSTR Name);
};
/////////////////////////////////////////////////////////////////////////////
// DataSet wrapper class

class DataSet : public COleDispatchDriver
{
public:
   DataSet() {}      // Calls COleDispatchDriver default constructor
   DataSet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataSet(const DataSet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   void SetName(LPCTSTR lpszNewValue);
   void Delete();
   BOOL GetIsLinked();
   CString GetSourceFullName();
   short GetSymbol();
   void SetSymbol(short nNewValue);
   long GetRecordCount();
   long GetMatchedRecordCount();
   long GetUnmatchedRecordCount();
   long GetSkippedRecordCount();
   LPDISPATCH GetFields();
   LPDISPATCH QueryAllRecords();
   LPDISPATCH QueryCircle(LPDISPATCH Center, double Radius);
   long GetDataMapType();
   void DisplayPushpinMap();
   LPDISPATCH GetDataMap();
   void UpdateLink();
   void ZoomTo();
   LPDISPATCH QueryPolygon(const VARIANT& ArrayOfLocations);
   LPDISPATCH QueryShape(LPDISPATCH Shape);
   LPDISPATCH DisplayDataMap(long DataMapType, const VARIANT& DataField, long ShowDataBy, long CombineDataBy, long DataRangeType, long DataRangeOrder, long ColorScheme, long DataRangeCount, const VARIANT& ArrayOfCustomValues,
      const VARIANT& ArrayOfCustomNames, const VARIANT& DivideByField, const VARIANT& ArrayOfDataFieldLabels, const VARIANT& ArrayOfPushpinSymbols);
   void Select();
   void Copy();
   void Cut();
   long GetHyperlinkType();
   void SetHyperlinkType(long nNewValue);
   CString GetHyperlinkCalculated();
   void SetHyperlinkCalculated(LPCTSTR lpszNewValue);
   LPDISPATCH GetHyperlinkField();
   void SetHyperlinkField(LPDISPATCH newValue);
   BOOL GetFieldNamesVisibleInBalloon();
   void SetFieldNamesVisibleInBalloon(BOOL bNewValue);
   void SetFieldsVisibleInBalloon(const VARIANT& ArrayOfFields);
   long GetHowCreated();
   long GetMatchingMethod();
   long GetCountry();
   BOOL Paste();
};
/////////////////////////////////////////////////////////////////////////////
// Fields wrapper class

class Fields : public COleDispatchDriver
{
public:
   Fields() {}    // Calls COleDispatchDriver default constructor
   Fields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Fields(const Fields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
};
/////////////////////////////////////////////////////////////////////////////
// Field wrapper class

class Field : public COleDispatchDriver
{
public:
   Field() {}     // Calls COleDispatchDriver default constructor
   Field(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Field(const Field& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   VARIANT GetValue();
   long GetType();
   long GetGeographicFieldType();
   BOOL GetIsPrimaryKey();
   BOOL GetVisibleInBalloon();
};
/////////////////////////////////////////////////////////////////////////////
// Recordset wrapper class

class Recordset : public COleDispatchDriver
{
public:
   Recordset() {}    // Calls COleDispatchDriver default constructor
   Recordset(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Recordset(const Recordset& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   LPDISPATCH GetPushpin();
   LPDISPATCH GetLocation();
   LPDISPATCH GetFields();
   BOOL GetBof();
   BOOL GetEof();
   void MoveFirst();
   void MoveNext();
   void MoveToPushpin(LPDISPATCH Pushpin);
   BOOL GetIsMatched();
   void MatchRecord(BOOL ShowDialogIfAmbiguous, long HWndParent);
   long GetMatchingMethod();
   BOOL CallMatchMethod(LPCTSTR MethodName, LPDISPATCH MethodObject);
};
/////////////////////////////////////////////////////////////////////////////
// Pushpin wrapper class

class Pushpin : public COleDispatchDriver
{
public:
   Pushpin() {}      // Calls COleDispatchDriver default constructor
   Pushpin(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Pushpin(const Pushpin& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   void SetName(LPCTSTR lpszNewValue);
   void Delete();
   LPDISPATCH GetLocation();
   void SetRefLocation(LPDISPATCH newValue);
   CString GetNote();
   void SetNote(LPCTSTR lpszNewValue);
   BOOL GetHighlight();
   void SetHighlight(BOOL bNewValue);
   short GetSymbol();
   void SetSymbol(short nNewValue);
   long GetBalloonState();
   void SetBalloonState(long nNewValue);
   void Select();
   void Copy();
   void Cut();
   LPDISPATCH GetHyperlink();
   BOOL MoveTo(LPDISPATCH NewDataSet);
};
/////////////////////////////////////////////////////////////////////////////
// Hyperlink wrapper class

class Hyperlink : public COleDispatchDriver
{
public:
   Hyperlink() {}    // Calls COleDispatchDriver default constructor
   Hyperlink(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Hyperlink(const Hyperlink& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetAddress();
   CString GetScreenTip();
   CString GetSubaddress();
   CString GetTextToDisplay();
   void Follow();
};
/////////////////////////////////////////////////////////////////////////////
// DataMap wrapper class

class DataMap : public COleDispatchDriver
{
public:
   DataMap() {}      // Calls COleDispatchDriver default constructor
   DataMap(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataMap(const DataMap& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetRecordCount();
   CString GetLegendTitle();
   void SetLegendTitle(LPCTSTR lpszNewValue);
   long GetShowDataBy();
   long GetCombineDataBy();
   LPDISPATCH GetDataRanges();
   LPDISPATCH GetDivideByField();
   BOOL GetIsGeographyForEveryZoomLevel();
   VARIANT GetDataFields();
   VARIANT GetDataFieldLabels();
};
/////////////////////////////////////////////////////////////////////////////
// DataRanges wrapper class

class DataRanges : public COleDispatchDriver
{
public:
   DataRanges() {}      // Calls COleDispatchDriver default constructor
   DataRanges(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataRanges(const DataRanges& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   long GetDataRangeType();
   long GetDataRangeOrder();
   long GetColorScheme();
   BOOL GetIsCustomized();
};
/////////////////////////////////////////////////////////////////////////////
// DataRange wrapper class

class DataRange : public COleDispatchDriver
{
public:
   DataRange() {}    // Calls COleDispatchDriver default constructor
   DataRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   DataRange(const DataRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   VARIANT GetLowValue();
   VARIANT GetHighValue();
   long GetValueCount();
   short GetSymbol();
};
/////////////////////////////////////////////////////////////////////////////
// Shape wrapper class

class Shape : public COleDispatchDriver
{
public:
   Shape() {}     // Calls COleDispatchDriver default constructor
   Shape(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Shape(const Shape& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   void SetName(LPCTSTR lpszNewValue);
   void Delete();
   LPDISPATCH GetLocation();
   void SetRefLocation(LPDISPATCH newValue);
   VARIANT GetVertices();
   void SetVertices(const VARIANT& newValue);
   LPDISPATCH GetAdjustments();
   double GetHeight();
   void SetHeight(double newValue);
   double GetWidth();
   void SetWidth(double newValue);
   void ZOrder(long ZOrderCmd);
   long GetZOrderPosition();
   void SetZOrderPosition(long nNewValue);
   LPDISPATCH GetLine();
   LPDISPATCH GetFill();
   long GetType();
   long GetAutoShapeType();
   BOOL GetSizeVisible();
   void SetSizeVisible(BOOL bNewValue);
   CString GetText();
   void SetText(LPCTSTR lpszNewValue);
   long GetFontColor();
   void SetFontColor(long nNewValue);
   BOOL GetAvoided();
   void SetAvoided(BOOL bNewValue);
   void Select();
   void Copy();
   void Cut();
};
/////////////////////////////////////////////////////////////////////////////
// Shapes wrapper class

class Shapes : public COleDispatchDriver
{
public:
   Shapes() {}    // Calls COleDispatchDriver default constructor
   Shapes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Shapes(const Shapes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   LPDISPATCH AddLine(LPDISPATCH BeginLocation, LPDISPATCH EndLocation);
   LPDISPATCH AddPolyline(const VARIANT& ArrayOfLocations);
   LPDISPATCH AddShape(long Type, LPDISPATCH Center, double Width, double Height);
   LPDISPATCH AddDrivetimeZone(LPDISPATCH Center, double Time);
   LPDISPATCH AddTextbox(LPDISPATCH Center, double Width, double Height);
};
/////////////////////////////////////////////////////////////////////////////
// Adjustments wrapper class

class Adjustments : public COleDispatchDriver
{
public:
   Adjustments() {}     // Calls COleDispatchDriver default constructor
   Adjustments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Adjustments(const Adjustments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   double GetItem(VARIANT* Index);
   void SetItem(VARIANT* Index, double newValue);
};
/////////////////////////////////////////////////////////////////////////////
// LineFormat wrapper class

class LineFormat : public COleDispatchDriver
{
public:
   LineFormat() {}      // Calls COleDispatchDriver default constructor
   LineFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   LineFormat(const LineFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetForeColor();
   void SetForeColor(long nNewValue);
   BOOL GetVisible();
   void SetVisible(BOOL bNewValue);
   long GetWeight();
   void SetWeight(long nNewValue);
   BOOL GetBeginArrowhead();
   void SetBeginArrowhead(BOOL bNewValue);
   BOOL GetEndArrowhead();
   void SetEndArrowhead(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// FillFormat wrapper class

class FillFormat : public COleDispatchDriver
{
public:
   FillFormat() {}      // Calls COleDispatchDriver default constructor
   FillFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   FillFormat(const FillFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetForeColor();
   void SetForeColor(long nNewValue);
   BOOL GetVisible();
   void SetVisible(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// SelectedArea wrapper class

class SelectedArea : public COleDispatchDriver
{
public:
   SelectedArea() {}    // Calls COleDispatchDriver default constructor
   SelectedArea(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   SelectedArea(const SelectedArea& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetTop();
   long GetLeft();
   long GetWidth();
   long GetHeight();
   LPDISPATCH GetLocation();
   void Copy();
   void SelectArea(long Top, long Left, long Width, long Height);
};
/////////////////////////////////////////////////////////////////////////////
// PageSetup wrapper class

class PageSetup : public COleDispatchDriver
{
public:
   PageSetup() {}    // Calls COleDispatchDriver default constructor
   PageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   PageSetup(const PageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetPaperSize();
   void SetPaperSize(long nNewValue);
   double GetRightMargin();
   void SetRightMargin(double newValue);
   double GetLeftMargin();
   void SetLeftMargin(double newValue);
   double GetTopMargin();
   void SetTopMargin(double newValue);
   double GetBottomMargin();
   void SetBottomMargin(double newValue);
   BOOL GetIncludeRouteSummary();
   void SetIncludeRouteSummary(BOOL bNewValue);
   long GetRoutePageBreakFrequency();
   void SetRoutePageBreakFrequency(long nNewValue);
   double GetRoutePageBreakDistance();
   void SetRoutePageBreakDistance(double newValue);
   double GetRoutePageBreakTime();
   void SetRoutePageBreakTime(double newValue);
};
/////////////////////////////////////////////////////////////////////////////
// SavedWebPages wrapper class

class SavedWebPages : public COleDispatchDriver
{
public:
   SavedWebPages() {}      // Calls COleDispatchDriver default constructor
   SavedWebPages(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   SavedWebPages(const SavedWebPages& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   void Save();
   void Delete();
   LPDISPATCH Add(LPCTSTR FileName, LPDISPATCH Location, LPCTSTR Title, BOOL IncludeMap, BOOL IncludeThumbnail, BOOL IncludeHyperlinks, long Width, long Height, BOOL IncludeDirections, BOOL IncludeLegend, BOOL IncludeOverview,
      BOOL IncludeSourceFile);
};
/////////////////////////////////////////////////////////////////////////////
// SavedWebPage wrapper class

class SavedWebPage : public COleDispatchDriver
{
public:
   SavedWebPage() {}    // Calls COleDispatchDriver default constructor
   SavedWebPage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   SavedWebPage(const SavedWebPage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetFileName();
   void SetFileName(LPCTSTR lpszNewValue);
   LPDISPATCH GetLocation();
   void SetLocation(LPDISPATCH newValue);
   BOOL GetIncludeMap();
   void SetIncludeMap(BOOL bNewValue);
   BOOL GetIncludeDirections();
   void SetIncludeDirections(BOOL bNewValue);
   BOOL GetIncludeLegend();
   void SetIncludeLegend(BOOL bNewValue);
   BOOL GetIncludeOverview();
   void SetIncludeOverview(BOOL bNewValue);
   BOOL GetIncludeHyperlinks();
   void SetIncludeHyperlinks(BOOL bNewValue);
   BOOL GetIncludeThumbnail();
   void SetIncludeThumbnail(BOOL bNewValue);
   CString GetTitle();
   void SetTitle(LPCTSTR lpszNewValue);
   BOOL GetIncludeSourceFile();
   void SetIncludeSourceFile(BOOL bNewValue);
   long GetWidth();
   void SetWidth(long nNewValue);
   long GetHeight();
   void SetHeight(long nNewValue);
   void Delete();
   void SetAutoResave(BOOL bNewValue);
   BOOL GetAutoResave();
   void Save();
};
/////////////////////////////////////////////////////////////////////////////
// Symbols wrapper class

class Symbols : public COleDispatchDriver
{
public:
   Symbols() {}      // Calls COleDispatchDriver default constructor
   Symbols(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Symbols(const Symbols& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   LPDISPATCH Add(LPCTSTR FileName);
   LPDISPATCH ItemByID(short ID);
};
/////////////////////////////////////////////////////////////////////////////
// Symbol wrapper class

class Symbol : public COleDispatchDriver
{
public:
   Symbol() {}    // Calls COleDispatchDriver default constructor
   Symbol(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Symbol(const Symbol& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   void SetName(LPCTSTR lpszNewValue);
   short GetId();
   void Delete();
   BOOL GetIsCustom();
   void SetUseTransparency(BOOL bNewValue);
   BOOL GetUseTransparency();
};
/////////////////////////////////////////////////////////////////////////////
// Toolbars wrapper class

class Toolbars : public COleDispatchDriver
{
public:
   Toolbars() {}     // Calls COleDispatchDriver default constructor
   Toolbars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Toolbars(const Toolbars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
};
/////////////////////////////////////////////////////////////////////////////
// Toolbar wrapper class

class Toolbar : public COleDispatchDriver
{
public:
   Toolbar() {}      // Calls COleDispatchDriver default constructor
   Toolbar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   Toolbar(const Toolbar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   CString GetName();
   BOOL GetVisible();
   void SetVisible(BOOL bNewValue);
   long GetIndex();
};
/////////////////////////////////////////////////////////////////////////////
// AddIns wrapper class

class AddIns : public COleDispatchDriver
{
public:
   AddIns() {}    // Calls COleDispatchDriver default constructor
   AddIns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
   AddIns(const AddIns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
   LPDISPATCH GetApplication();
   LPDISPATCH GetParent();
   long GetCount();
   LPDISPATCH GetItem(VARIANT* Index);
   LPDISPATCH Connect(LPCTSTR ProgId);
   void Disconnect(const VARIANT& ObjectOrProgId);
};
