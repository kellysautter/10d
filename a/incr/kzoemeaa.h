//KZOEMEAA.H - Structures for external meta def's

/*
CHANGE LOG
2002.01.11  HH
   added DBH_SetUser.

2001.04.19  DGC  10b
   Added ORDER BY stuff.

2001.04.18  DGC  10b
   Made a change to ViewOD struct for DropViewObject() logic.

2001.02.26  DGC  10b
   Added "JOIN" field to DataRecord.

2000.07.17  DGC
   Added flag DBH_NewObject.

2000.04.18  DGC
   Added bKeepAlive in App struct to allow us to keep an application loaded
   in core even if there are no tasks referencing it.

2000.01.07  DGC  10b
   Added flags in ViewEntityStruct for dealing with abstract entities.

   Removed nMultiRootLimit.

1999.10.26  DGC  10a
   Made table/column names 256 bytes long.

1999.10.16  DGC  10a
   Added lpPrev/NextSibling view entity pointers to ViewEntityStruct.

1999.06.24  DGC  10a
   Made changes requested by HH to compile on MVS.

1999.06.01  DGC  10a
   Added tracing stuff.

1999.05.07  DGC  10a
   Added ER_DATE attribute to the XOD object so we can check for LODs that
   are out of sync.

1999.04.19  RS
   Commentlines required for Rose.

1999.04.09  DGC  10a
   Added support for internal derived attribute concat functions.

1999.04.07  DGC  10a
   Added set-attribute-from-attribute domain stuff.

1999.02.11  DGC
   Started adding attribute flag stuff.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoeenaa.h> //required for Rose
//#include <kzoengaa.h> //required for Rose

#define zCOREMEM zCHAR * *
// DB handler messages;
#define DBH_Init           0x1000         // Function messages...
#define DBH_Term           0x2000
#define DBH_Object         0x4000
#define DBH_Entity         0x8000
#define DBH_TaskCloseAll   0x0100
#define DBH_Database       0x0200

#define DBH_TranBegin      0x0001         // Operation msgs for Object...
#define DBH_Commit         0x0002
#define DBH_Rollback       0x0004
#define DBH_NewObject      0x0008

#define DBH_Insert         0x0001         // Operation msgs for Entity...
#define DBH_InsertRel      0x0002
#define DBH_Delete         0x0004
#define DBH_DeleteAll      0x0400
#define DBH_DeleteRel      0x0008
#define DBH_Update         0x0010
#define DBH_UpdateSeq      0x0800
#define DBH_Load           0x0020
#define DBH_StartGenkey    0x0040
#define DBH_GetGenkey      0x0080

#define DBH_SetUser        0x0001         // Operation msgs for Database...

#define DBH_LoadOI         0x0001         // DB handler indicators...
#define DBH_StoreOI        0x0002

#define zDBH_RAIMADM               'R'
#define zDBHANDLER_WORK_VIEW_NAME  "Zeidon DBHandler Work Object"

#define zDBH_ONE_TO_MANY           'O'
#define zDBH_MANY_TO_ONE           'M'
#define zDBH_MANY_TO_MANY          '2'

#define zDBH_PARENT_IS_OWNER       'O'
#define zDBH_PARENT_IS_MEMBER      'M'

#define zDBH_PARENT_IS_SOURCE      'O'
#define zDBH_CHILD_IS_SOURCE       'M'

typedef struct LibraryStruct LibraryRecord;
typedef LibraryRecord *LPLIBRARY;
typedef LPLIBRARY *LPLPLIBRARY;
#define LPLIBRARY_DEF

typedef struct AppStruct AppRecord;
typedef AppRecord *LPAPP;
#define LPAPP_DEF

typedef struct LNameStruct LNameRecord;
typedef LNameRecord *LPLNAME;
#define LPLNAME_DEF

typedef struct DomainStruct DomainRecord;
typedef DomainRecord *LPDOMAIN;
#define LPDOMAIN_DEF

typedef struct ViewOD_Struct ViewOD_Record;
typedef ViewOD_Record *LPVIEWOD;
#define LPVIEWOD_DEF

typedef struct DataRecordStruct DataRecordRecord;
typedef DataRecordRecord *LPDATARECORD;

typedef struct DataFieldStruct DataFieldRecord;
typedef DataFieldRecord *LPDATAFIELD;

typedef struct RelRecordStruct RelRecordRecord;
typedef RelRecordRecord *LPRELRECORD;

typedef struct RelFieldStruct RelFieldRecord;
typedef RelFieldRecord *LPRELFIELD;

typedef struct ViewEntityStruct ViewEntityRecord;
typedef ViewEntityRecord *LPVIEWENTITY;
#define LPVIEWENTITY_DEF

typedef struct ViewAttribStruct ViewAttribRecord;
typedef ViewAttribRecord *LPVIEWATTRIB;
#define LPVIEWATTRIB_DEF

typedef struct HandlerStruct HandlerRecord;
typedef HandlerRecord *LPHANDLER;

typedef struct DBHandlerStruct DBHandlerRecord;
typedef DBHandlerRecord *LPDBHANDLER;

typedef struct GKHandlerStruct GKHandlerRecord;
typedef GKHandlerRecord *LPGKHANDLER;

typedef struct ViewNameStruct ViewNameRecord;
typedef ViewNameRecord *LPVIEWNAME;

typedef struct { zCHAR zDateTime[ 6 ]; } DateTimeRecord;
typedef DateTimeRecord *LPDATETIME;
#define LPDATETIME_DEF

typedef struct ContextStruct ContextRecord;
typedef ContextRecord * zLPCONTEXT;
#define LPCONTEXT_DEF

typedef struct TaskOperationStruct TaskOperationRecord;
typedef TaskOperationRecord *LPTASKOPER;
#define LPTASKOPER_DEF

typedef struct TaskDomainStruct TaskDomainRecord;
typedef TaskDomainRecord *LPTASKDOMAIN;
#define LPTASKDOMAIN_DEF

typedef struct TableEntryStruct TableEntryRecord;
typedef TableEntryRecord * LPTABLEENTRY;
#define LPTABLEENTRY_DEF

typedef struct NLS_TableEntryStruct NLS_TableEntryRecord;
typedef NLS_TableEntryRecord * LPNLSTABLEENTRY;
#define LPNLSTABLEENTRY_DEF

typedef struct RegExpressionStruct RegExpressionRecord;
typedef RegExpressionRecord * LPREGEXPRESSION;
#define LPREGEXPRESSION_DEF

typedef int (POPERATION zPOBJOPER)( zVIEW        lpView,
                                    LPVIEWENTITY lpViewEntity,
                                    LPVIEWATTRIB lpViewAttribute,
                                    zSHORT       nEntryType );

typedef zSHORT (POPERATION zPDBHOPER)( zLONG        hTask,
                                       zUSHORT      uMessage,
                                       int          Indicators,
                                       LPVIEWOD     lpViewOD,
                                       LPVIEWENTITY lpViewEntity,
                                       zVIEW        lpView,
                                       zVIEW        lpQualView,
                                       zPVOID       lpPtr );
typedef zPDBHOPER *zlpPDBHOPER;

typedef zSHORT (POPERATION zPGKHOPER)( zSHORT       nMessage,
                                       zVIEW        lpView,
                                       LPVIEWOD     lpViewOD,
                                       LPVIEWENTITY lpViewEntity,
                                       zVIEW        lpQualView );
typedef zPGKHOPER *zlpPGKHOPER;

typedef int (POPERATION zPOCEOPER) ( zVIEW    lpView,
                                     zSHORT   nEvent,
                                     zSHORT   nState );
typedef zPOCEOPER *zlpPOCEOPER;

typedef int (POPERATION zPECEOPER) ( zVIEW    lpView,
                                     zCPCHAR  szEntityName,
                                     zSHORT   nEvent,
                                     zSHORT   nState );
typedef zPECEOPER *zlpPECEOPER;

LPVIEWENTITY OPERATION
MiGetViewEntityForView( zVIEW lpView, zCPCHAR  szEntityName );

// Application Logical rename structure
struct LNameStruct
{
   zSHORT               nTableID;
   LPLNAME              hNextLName;
   zSHORT               nType;
   zCHAR                szLName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR                szPName[ zZEIDON_NAME_LTH + 1 ];
};

// Application Definition Structure
struct AppStruct
{
   zSHORT               nTableID;
   LPAPP                hNextApp;
   zCHAR                szName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR                szDefaultDialog[ zZEIDON_NAME_LTH + 1 ];
   LPDOMAIN             hFirstDomain;
   LPDBHANDLER          hFirstDBHandler;
   LPGKHANDLER          hFirstGKHandler;
   LPVIEWOD             hFirstViewOD;   // First loaded Object def
   zSHORT               nVersion;
   zCHAR                szLibraryDir[ zMAX_FILENAME_LTH + 1 ];
   zCHAR                szObjectDir[ zMAX_FILENAME_LTH + 1 ];
   zCHAR                szLocalDir[ zMAX_FILENAME_LTH + 1 ];
   zCHAR                szSharedDir[ zMAX_FILENAME_LTH + 1 ];
   zCHAR                szQLPLR_Dir[ zMAX_FILENAME_LTH + 1 ];
   zCHAR                szQXODS_Dir[ zMAX_FILENAME_LTH + 1 ];
   zCHAR                szSourceDir[ zMAX_FILENAME_LTH + 1 ];
   zVIEW                hFirstView;      // First view for Application
   LPVIEWNAME           hFirstViewName;  // First named view for Application
   LPLNAME              hFirstLName;     // First logical name struct
   zPVOID               hApplicationTask; // Task to be alive as long as app
   unsigned int         bKeepAlive       : 1;  // Never close App.
};

// Domain Structure
struct DomainStruct
{
   zSHORT                  nTableID;
   LPDOMAIN                hNextDomain;
   zCHAR                   szName[ zZEIDON_NAME_LTH + 1 ]; // Domain Name
   zLONG                   lApDmTok;      // Application Domain Token
   LPAPP                   hApp;          // Application owning this Domain
   zLPCONTEXT              hFirstContext; // First Context associated w/Domain
   LPTASKDOMAIN            hFirstTaskDomain; // 1st TaskDomain assoc. w/domain
   LPTASKDOMAIN            hLastTaskDomain;  // last TaskDomain assoc. w/domain
   zULONG                  ulMaxStringLth;   // MaxStringLth of domain
   zCHAR                   cType;            // Datatype of domain
   zCHAR                   cDecimal;         // Decimal precision
   zCHAR                   cDomainType;      // Domain type
   zCHAR                   cContextRestriction; // Context
   zCHAR                   szLibraryName[ zZEIDON_NAME_LTH + 1 ]; // Domain Library Name
   zCHAR                   szDomainOper[ zZEIDON_NAME_LTH + 1 ];  // Domain Operation function
   zLONG                   lDecimalFormat;
   zLONG                   lDefaultContextZKey; // Default Context Token
   unsigned int            bRestricted   : 1; //
   zUSHORT                 uOperNumber;   // Seq no of each domain with oper
};

// Context Definition
struct ContextStruct
{
   zSHORT                  nTableID;
   zLPCONTEXT              hNextContext;
   zCHAR                   szName[ zZEIDON_NAME_LTH + 1 ]; // Context Name
   zLONG                   lContextTok;         // Context Token
   LPDOMAIN                hDomain;             // Domain for this context
   LPTABLEENTRY            hFirstTableEntry;    // First TableEntry
   LPTABLEENTRY            hLastTableEntry;     // Last  TableEntry
   LPREGEXPRESSION         hFirstRegExpression; // First RegularExpression
   zPCHAR                  hEditString;         // Edit string
   zLONG                   lDecimalFormat;
   zSHORT                  nDisplayLength;      // display lth using edit str
};

// TableEntry Definition
struct TableEntryStruct
{
   zSHORT                  nTableID;
   LPTABLEENTRY            hNextTableEntry;
   LPTABLEENTRY            hPrevTableEntry;
   zLONG                   lTableEntryTok;   // TableEntry Token
   zLPCONTEXT              hContext;         // Context for this Table Entry
   zPCHAR                  hExternalValue;
   LPNLSTABLEENTRY         hFirstNLS_TableEntry;
   union
   {
      zLONG                lInteger;
      zPCHAR               hStringValue;
   } unInternal;
};

// NLS_TableEntry Definition
struct NLS_TableEntryStruct
{
   zSHORT                  nTableID;
   LPNLSTABLEENTRY         hNextNLS_TableEntry;
   zPCHAR                  pszNLS_Text;
   zSHORT                  nLanguageCode;
};

// RegularExpression Definition
struct RegExpressionStruct
{
   zSHORT                  nTableID;
   LPREGEXPRESSION         hNextRegExpression;
   zLONG                   lRegExpressionTok;     // RegularExpression Token
   zLPCONTEXT              hContext;   // Context for this RegularExpression
};

// View Object Definition Structure
struct ViewOD_Struct
{
   zSHORT                  nTableID;
   LPVIEWOD                hNextViewOD;  // Next ViewOD For App
   LPAPP                   hApp;         // Owning application
   zCHAR                   szName[ zZEIDON_NAME_LTH + 1 ]; // View Object Definition Name
   zCHAR                   szDfltDBName[ zZEIDON_NAME_LTH + 1 ];  // Database Name
   zCHAR                   szDfltServer[ zZEIDON_NAME_LTH + 1 ];  // Server Name
   zCHAR                   szNetwork[ zZEIDON_NAME_LTH + 1 ];     // Network name (?)
   zCHAR                   szOperLibname[ zZEIDON_NAME_LTH + 1 ]; // Operation library name
   zCHAR                   szOCEOpername[ zZEIDON_NAME_LTH + 1 ]; // Object Constraint Event Handler
                                          //    Operation Name
   LPDBHANDLER             hDBHandler;   // Database Handler
   LPGKHANDLER             hGKHandler;   // Genkey Handler
   LPTASKOPER              hFirstTaskOperation;
   LPTASKOPER              hLastTaskOperation;
   LPVIEWENTITY            hFirstOD_Entity;   // Root of structure
   zSHORT                  nOfRecID;      // Overflow record id
   zSHORT                  nOfBufSz;      // Overflow buffer size
   zSHORT                  nDummy;        // No longer used.
   zUSHORT                 uEntities;     // Number of entities in OD.
   zBYTE                   nLock;         // Locking type for LOD.
   zBYTE                   nEntityLock;   // Locking type for entity.
   zCHAR                   cCacheNbr;     // # of ent. insts. to cache.
   zCHAR                   cSironDB_Type; // Physical DB type for Siron DB Handler
   unsigned int            bHasDupInstance  : 1,  // Def has dup inst
                           bHasDupRel       : 1,  // Def has dup rels
                           bHasDerivedPath  : 1,  // Def has derived paths

                           // Active Object Contraints
                           bActivateConstraint      : 1,
                           bActivateEmptyConstraint : 1,
                           bCommitConstraint        : 1,
                           bDropOIConstraint        : 1,
                           bLogicalServerName       : 1,
                           bLogicalDBName           : 1,

                           // Following flags match ViewEntity flags.  If any
                           // of the following flags is 1, then at least one
                           // entity exists with the flag set to 1.
                           bGenkey                  : 1, // Def has ent w/ GK.
                           bAutoSeq                 : 1, // Def has ent w/ Autoseq.

                           // If following is TRUE then next ActivateViewObject
                           // reloads the XOD file.
                           bDeprecated              : 1,
                           bNetCompress             : 1;

   // SQL stuff.
   unsigned int            bUseOrderBy: 1;  // Entities will be sorted using ORDER BY.
   zPVOID                  hDBH_Data;     // Pointer to DBH-specific data.

   // Following is used to ensure that LODs are in sync.  The date represents
   // the last update date for the ER.  Any LODs that are included should have
   // the same ER date.
   zCHAR                   szER_Date[ 20 ];
};

// View Entity Structure
struct ViewEntityStruct
{
   zSHORT                  nTableID;
   LPVIEWENTITY            hNextHier;         // Next hierarchical
   LPVIEWENTITY            hPrevHier;         // Previous hierarchical
   LPVIEWENTITY            hParent;           // Parent Entity
   zCHAR                   szName[ zZEIDON_NAME_LTH + 1 ];        // Entity Name
   zCHAR                   szXML_Name[ zZEIDON_NAME_LTH + 1 ];    // XML Entity Name (not unique)
   zCHAR                   szDBName[ zZEIDON_NAME_LTH + 1 ];      // Database Name
   zCHAR                   szServer[ zZEIDON_NAME_LTH + 1 ];      // Server Name
   zCHAR                   szECEOpername[ zZEIDON_NAME_LTH + 1 ]; // Entity Constraint Handler
                                                                  //    Operation Name
   LPVIEWATTRIB            hFirstOD_Attrib;
   LPTASKOPER              hFirstTaskOperation;
   LPTASKOPER              hLastTaskOperation;
   LPDATARECORD            hFirstDataRecord;   // First Data Record
   zLONG                   lEREntTok;          // Token for Entity
   zLONG                   lERRelTok;          // Token for rel to parent
   zULONG                  ulMaxLth;           // Max lth of all attrs.
   zULONG                  ulRecordSize;       // Entity record size
   zULONG                  ulNonPersistSize;   // Entity NonP record size
   zSHORT                  nLevel;             // Level of Entity
   zSHORT                  nHierNbr;           // Hierarchical number
   zUSHORT                 uCardMin;           // Minimum cardinality
   zUSHORT                 uCardMax;           // Maximum cardinality
   zUCHAR                  cMappings;          // Number of dbms mappings
   zUSHORT                 uECEOperNumber;     // Oper no for Entity Cnstrnt
   zLONG                   lActivateLimit;     // Max # of entities to activate.
   zBYTE                   nLock;              // Locking level.
   LPVIEWENTITY            hCacheViewEntity;   // Used to load/cache EIs.

   // Update rules and indicators
   unsigned int            bCreate       : 1,  // Entity can be created
                           bDelete       : 1,  // Entity can be deleted
                           bUpdate       : 1,  // Entity can be deleted
                           bInclude      : 1,  // Entity can be created by inc
                           bExclude      : 1,  // Entity can be deleted by exc
                           bInclSrc      : 1,  // May be source of include
                           bHidden       : 1,  // Entity is hidden as the
                                               // result of a delete/excl call
                           bPDelete      : 1,  // Entity deleted with parent
                           bPRestrict    : 1,  // Restrict parent delete
                           bCheckRestrict: 1,  // Ent has children w/ PRestrict
                           bRecursiveSt  : 1,  // Recursive structure
                           bRecursive    : 1,  // Recursive behaviour
                           bAutoCreate   : 1,  // Entity is Autocreate
                           bDerived      : 1,  // Entity is derived
                           bDerivedPath  : 1,  // 1=Relationship derived OR
                                               // Child of derived rel
                           bGenkey       : 1,  // Has a generated key
                           bSingleKey    : 1,  // Has a single (not composite) key
                           bAttrInit     : 1,  // Has initialized Attr(s)
                           bAttrOrder    : 1,  // Ent is ordered by attribute
                           bAttrOrderChild : 1, // Ent is ordered by child attrs
                           bAutoSeq      : 1,  // Ent has autosequencing.
                           bIncrLoad     : 1,  // To be incrementally loaded
                           bFullPersist  : 1,  // Contains Full persist attrs
                           bSharesParRec : 1,  // Shrs parents primary record
                           bSharesChdRec : 1,  // Shrs rec w/1 or more chldrn
                           bDupInstance  : 1,  // Dup inst, relink after load
                           bDupRel       : 1,  // Dup Rel, re-incl after load
                           bDupRelPath   : 1,  // Dup Rel or child of DupRel
                           bRelLink1     : 1,  // Rel Link Direction ( 1 or 0 )
                           bRecursivePar : 1,  // Recursive parent entity

                           bAutoLoadFromParent : 1,  // Info for ent'y in Parent
                           bContainsBlob       : 1,  // 1 if ent'y has blob attr
                           bHasDB_Oper         : 1,  // 1 if ent'y has db oper

                           // Activate Entity Constraints
                           bCreateConstraint   : 1,
                           bDeleteConstraint   : 1,
                           bIncludeConstraint  : 1,
                           bExcludeConstraint  : 1,
                           bAcceptConstraint   : 1,
                           bCancelConstraint   : 1,

                           bEnqShare           : 1,  // Auto enqueuing
                           bEnqExclusive       : 1,
                           bRequiredChild      : 1,  // child w/uMinCard > 0
                           bHangingFKey        : 1,

                           // Flags for OOP protocols (e.g. XML)
                           bAbstract           : 1,  // Abstract entity.
                           bAbstractChild      : 1,  // Entity is child of abstract entity.

                           bDebugChange        : 1;  // Show msg when attr chgs.

   LPVIEWENTITY            hPrevSibling;
   LPVIEWENTITY            hNextSibling;

};

#define VIEWENTITY_OD
typedef struct ViewEntityStruct VIEWENTITY;

// View Attribute Structure
struct ViewAttribStruct
{
   zSHORT                  nTableID;
   LPVIEWATTRIB            hNextOD_Attrib;
   LPVIEWENTITY            hViewEntity;       // Owning View Entity
   zCHAR                   szName[ zZEIDON_NAME_LTH + 1 ];             // Attribute Name
   zCHAR                   szXML_SimpleName[ zZEIDON_NAME_LTH + 1 ];   // XML Simple Attribute Name (may contain special characters)
   zCHAR                   szXML_ExternalName[ 101 ];   // XML External Attribute Name (may contain special characters)
   zLONG                   lERAttTok;          // Used for sorting.
   zLONG                   lXVAAttTok;         // Used to match Data + Rel flds
   LPTASKOPER              hFirstTaskOperation;
   LPTASKOPER              hLastTaskOperation;
   // Indicators
   unsigned int            bPersist       : 1, // Attr is persistent
                           bKey           : 1, // Is an Ident of the Entity
                           bForeignKey    : 1, // Is an Ident of related Entity
                           bGenkey        : 1, // Is mapped to DBMS
                           bRequired      : 1, // Attribute is Required
                           bHidden        : 1, // Attribute is Hidden
                           bCaseSens      : 1, // Attr compares case sensitive
                           bCrDate        : 1, // System type creation date
                           bUpDate        : 1, // System type last update date
                           bUserID        : 1, // System type last update user
                           bAutoSeq       : 1, // Automatic sequencing attr
                           bSequencingD   : 1, // Sequencing is descending
                           bNoUpdate      : 1, // Attr can not be updated.
                           bNoPersistUpd  : 1, // Attr can't be upd if persist
                           bDebugChange   : 1, // Show msg when attr changes.
                           bDerivedIntrnl : 1; // Derived oper is internal.
   zUCHAR                  cSequencing;        // Sequencing number
   LPDOMAIN                hDomain;            // Attribute Domain
   zCHAR                   cType;              // Internal datatype
   zULONG                  ulLth;              // Length of attr
   zCHAR                   cDecimal;           // Decimal precision
   zUCHAR                  cMapping;           // Mapping number for attr
   zPCHAR                  hInit;              // Attribute Init value
   zCHAR                   szDerivedOper[ zZEIDON_NAME_LTH + 1 ]; // Derived Op Name
   zPCHAR                  hDerivedExpr;       // Internal Derived Op expression.
   zULONG                  ulRecordOffset;     // Offset in internal record
   zCHAR                   cDB_Oper;           // maX, Min, etc.
   zPCHAR                  hDerivedXPG;        // XPG for derived function.
};

typedef struct ViewAttribStruct VIEWATTRIB;

// Data Record structure
struct DataRecordStruct
{
   zSHORT                  nTableID;
   LPDATARECORD            hNextDataRecord;   // Next data rec for entity
   LPRELRECORD             hFirstRelRecord;   // Rel Record to get to rec
   LPDATAFIELD             hFirstDataField;   // First data field for this rec
   zCHAR                   cRecordType;       // 'P' primary, 'S' Secondary
   zCHAR                   szRecordName[ 256 ]; // Record name
   zLONG                   lRecordID;         // Record ID
   zCHAR                   cJoin;             // If 'Y' then load via a join.
   zPVOID                  hDBH_Data;         // Pointer to DBH-specific data.
};

// Data Field structure
struct DataFieldStruct
{
   zSHORT                  nTableID;
   LPDATAFIELD             hNextDataField;    // Next data field
   LPVIEWATTRIB            hViewAttrib;       // Src View Attrib for Field
   zCHAR                   szFldName[ 256 ];  // Mapping name for field
   zLONG                   lFldID;            // DBMS Field identifier
   zCHAR                   cFldType;          // DBMS Field Type
   zUSHORT                 uFldLth;           // Record Field Length
   zUSHORT                 uFldOffset;        // Record Field Offset
   zPVOID                  hDBH_Data;         // Pointer to DBH-specific data.
};

// Relationship Record structure
struct RelRecordStruct
{
   zSHORT                  nTableID;
   LPRELRECORD             hNextRelRecord;    // Next rel rec for entity
   LPRELFIELD              hFirstRelField;    // First rel field for this rec
   zCHAR                   cOwnerMember;      // Owner, Member, or NULL
   zCHAR                   szRecordName[ 256 ]; // Record name
   zLONG                   lRecordID;         // Record ID
   zCHAR                   szSetName1[ 256 ]; // Owner or member set OR
   zLONG                   lSetID1;           //  Parent set owner for (m,m)
   zCHAR                   szSetName2[ 256 ]; // Unused unless (m,m), Child
   zLONG                   lSetID2;           //  set owner for (m,m)
   zPVOID                  hDBH_Data;         // Pointer to DBH-specific data.
};

// Rel Field structure
struct RelFieldStruct
{
   zSHORT                  nTableID;
   LPRELFIELD              hNextRelField;     // Next relationship field
   LPVIEWATTRIB            hViewAttrib;       // Src View Attrib for Field
   LPDATAFIELD             hSrcDataField;     // Source of key value.
   LPDATAFIELD             hRelDataField;     // Relationship attribute.
   zCHAR                   szFldName[ 256 ];  // Field name in Rel record
   zPVOID                  hDBH_Data;         // Pointer to DBH-specific data.
};

// Domain operation interface.  Items should only be added to the end.
struct _zDomainOperData
{
   zLONG        lID;
   zCHAR        cType;
   zPVOID       lpData;
   zPCHAR       pszContextName;
   zVIEW        zView;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttribute;
   zULONG       uMaxStringLength;  // ulMaxStringLth;
   zVIEW        zSrcView;
   LPVIEWENTITY lpSrcViewEntity;
   LPVIEWATTRIB lpSrcViewAttribute;
};

typedef struct _zDomainOperData zDomainOperData;
typedef zDomainOperData *LPDOMAINDATA;

typedef zLONG (POPERATION zPDMOPER) ( zLONG        lEntryType,
                                      LPDOMAINDATA lpDomainData );

// Flags for each of the attribute instances.
typedef struct AttribFlagsStruct AttribFlagsRecord;
typedef AttribFlagsRecord *LPATTRIBFLAGS;
struct AttribFlagsStruct
{
   union
   {
      unsigned int uFlags;
      struct
      {
         // Attribute indicators.            Flags mean that attribute was...
         unsigned int bActivated      : 1,  // loaded from the database
                      bUpdated        : 1,  // changed
                      bNotNull        : 1;  // not null
      } bFlags;
   } u;
};

// Flags for determining tracing.
typedef struct TracingFlagsStruct TracingFlagsRecord;
typedef TracingFlagsRecord *LPTRACINGFLAGS;
struct TracingFlagsStruct
{
   unsigned bOI_Times       : 1,  // Times for commit/activate.
            bTaskMsgs       : 1,
            bOpenFile       : 1;  // Trace when file is opened.
};
