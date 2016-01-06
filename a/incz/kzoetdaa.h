//KZOETDAA.H - typdef's for object Engine pointer

/*
CHANGE LOG

97/05/07    DGC
   Removed code dealing with ZEIDON.XMD

98/01/20    DGC
   Cleaned up a little.

1999.04.19  RS
   Commentlines required for Rose.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose

typedef struct AnchorStruct AnchorRecord;
typedef AnchorRecord *LPANCHOR;

typedef struct DataHeaderStruct DataHeaderRecord;
typedef DataHeaderRecord *LPDATAHEADER;

typedef struct FileHeaderStruct FileHeaderRecord;
typedef FileHeaderRecord *LPFILEHEADER;

typedef struct InternationalStruct InternationalRecord;
typedef InternationalRecord *LPINTERNATIONAL;

typedef struct SubtaskStruct SubtaskRecord;
typedef SubtaskRecord *LPSUBTASK;

typedef struct TaskDBHandlerStruct TaskDBHandlerRecord;
typedef TaskDBHandlerRecord *LPTASKDBHANDLER;

typedef struct TaskGKHandlerStruct TaskGKHandlerRecord;
typedef TaskGKHandlerRecord *LPTASKGKHANDLER;

typedef struct TaskAppStruct TaskAppRecord;
typedef TaskAppRecord *LPTASKAPP;

typedef struct MQInterfaceStruct MQInterfaceRecord;
typedef MQInterfaceRecord *LPMQINTERFACE;

typedef struct ViewOI_Struct ViewOI_Record;
typedef ViewOI_Record *LPVIEWOI;

// NOTE: This typedef also occurs in kzoengaa.h for ActivateEmptyObjectIns
//typedef struct ViewStruct ViewRecord;
//typedef ViewRecord *LPVIEW;

typedef struct ViewCsrStruct ViewCsrRecord;
typedef ViewCsrRecord *LPVIEWCSR;

typedef struct ViewEntityCsrStruct ViewEntityCsrRecord;
typedef ViewEntityCsrRecord *LPVIEWENTITYCSR;

typedef struct ViewSubobjectStruct ViewSubobjectRecord;
typedef ViewSubobjectRecord *LPVIEWSUBOBJECT;

typedef struct EntityInstanceStruct EntityInstanceRecord;
typedef EntityInstanceRecord *LPENTITYINSTANCE;

// NOTE: the following typedef duplicated in kzoessaa.h for
//       the internal prototype SysLoadLibrary, SysGetProcAddr
//typedef struct LibraryStruct LibraryRecord;
//typedef LibraryRecord *LPLIBRARY;

typedef struct OpenFileStruct OpenFileRecord;
typedef OpenFileRecord *LPOPENFILE;

typedef struct FreespaceStruct FreespaceRecord;
typedef FreespaceRecord *LPFREESPACE;

typedef struct ErrorStruct ErrorRecord;
typedef ErrorRecord *LPERROR;

typedef struct SelectedInstanceStruct SelectedInstanceRecord;
typedef SelectedInstanceRecord *LPSELECTEDINSTANCE;

typedef struct DateTimeInternalStruct DateTimeInternalRecord;
typedef DateTimeInternalRecord *LPDTINTERNAL;
