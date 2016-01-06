/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctl.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.07.20   TMV
//    Implement setting Background color for MLE
//
// 2001.06.10    BL     WEB
//    Added LimitTextLength for a MLE
//
// 2001.05.29    BL     WEB
//   Bugifx for ListBox without Header Buttons
//   Bugfix for Tab Control (ActivateTab request)
//
// 2001.05.23    BL    RemoteServer
//   Implement SetRowColText and GetRowColText for Web
//
// 2001.05.15    BL    RemoteServer
//   Bugfix for Multi Selection in ListBox
//
// 2001.05.13    BL    RemoteServer
//    Added SelectStringByKeyAttributes, GetArrayWithKeyAttribute and
//    SearchStringByKeyAttributes for a ComboBox with Derived Attribute
//    Operation in Mapping Attribute
//
// 2001.04.28    BL    TREE
//    Added Parameter bEnsureVisible to SetTextColorForItemText and
//    SetTxtColorForItemText
//
// 2001.04.28    BL    TREE
//    Save Text Color for Tree Item in Array
//
// 2001.04.25    BL    TREE
//    Added new Function Added SetTextColorForItemText
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2001.04.12    BL    TREE
//    Added SetExpandStatusForEntity for bugfix for Expand all and Collapse all
//    for a Entity in a Lod with recursive Entities
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2001.03.26    TMV   2000   ActiveX
//    Retrieve license key from xwd and create control using this
//    related methods:
//       ZActiveX::ZActiveX
//       ZActiveX::CreateActiveXWithPersistProperties
//    new methods:
//       ZActiveX::BuildLicKeyString ( for debugging only )
//    see TZActiveX for further changes
//

// Don't remove the following commentlines. They are required for Rose.
//#include <afxtempl.h> //required for Rose
//#include <kzoengaa.h> //required for Rose
//#include <zdrapp.h> //required for Rose

#ifndef __zdctl_h__  // Sentry, use file only if it's not already included
#define __zdctl_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

/////////////////////////////////////////////////////////////////////////////
// Note: Use AFX_EXT_CLASS to export an entire class.  This avoids
// creating a .DEF file with all the decorated names for the class.
// Creating a .DEF file is more efficient since the names can be
// exported by ordinal.
//
// Here we use that method of exporting, much like MFC does itself, by
// implementing the following two lines and adding the appropriate lines
// to our .DEF file. All the public interfaces are exported in ZdCtl.DEF.
//

#ifdef ZDCTL_CLASS
// #undef  AFX_DATA
// #define AFX_DATA AFX_EXT_DATA
// #define ZDCTL_CLASS_PREDEFINED
#else
   #define ZDCTL_CLASS
#endif

#define zMAP_SIZE 256

extern "C" void WINAPI
ZeidonInitZDCTL( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                 zLONG lMapDlgCharX, zLONG lMapDlgCharY );
void
fnGetRootEntityName( zVIEW vObject, zPCHAR pchEntity, zLONG lMaxLth );
void
fnGetImageURL( ZSubtask *pZSubtask, zPCHAR pchImageURL, zSHORT nMaxLth );
void
fnEnableChildCtrls( ZMapAct *pzma, zBOOL bEnable, zBOOL bProcessChildren );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT BOOL OPERATION
PointWithinRange( CPoint& pt1, CPoint& pt2, int nRangeX, int nRangeY );

zOPER_EXPORT CString OPERATION
FormatTimeAmPm( COleDateTime& date );

zOPER_EXPORT void OPERATION
SetZeidonDateFromSysTime( zPCHAR pchDate, zLONG lMaxLth, SYSTEMTIME& SysTime );

zOPER_EXPORT BOOL OPERATION
SetSysTimeFromZeidon( SYSTEMTIME& SysTime, zCPCHAR cpcDate );

zOPER_EXPORT void OPERATION
SetZeidonDateFromOleDateTime( zPCHAR pchDate, zLONG lMaxLth, COleDateTime& DateTime );

zOPER_EXPORT void OPERATION
SetZeidonDateAttributeFromOleDateTime( COleDateTime& DateTime,
                                       zVIEW vApp,
                                       zCPCHAR cpcEntity,
                                       zCPCHAR cpcAttribute );

zOPER_EXPORT void OPERATION
SetOleDateTimeFromZeidonDate( COleDateTime& DateTime, zCPCHAR cpcDate );

zOPER_EXPORT void OPERATION
GetOleDateTimeFromZeidonDateAttribute( COleDateTime& DateTime,
                                       zVIEW vApp,
                                       zCPCHAR cpcEntity,
                                       zCPCHAR cpcAttribute );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zUSE_MS_TREE
#define zREMOTE_DATA_DELIMITER   '\t'

#define DISPATCH_MAX_PARAM       64
#define CRLF         _T( "\r\n" )
#define TAB_SPACE    _T( "   " )

#define zEDITBOX_LOWERCASE            0x00000001L // 1
#define zEDITBOX_UPPERCASE            0x00000002L // 2
#define zEDITBOX_PASSWORD             0x00000004L // 4
#define zEDITBOX_PIC                  0x00000008L // 8
#define zEDITBOX_MONEY                0x00000010L // 16

#define zLISTBOX_SINGLESEL            0x00000000L // default
#define zLISTBOX_MULTIPLESEL          0x00000001L // 1
#define zLISTBOX_EXTENDMULTIPLESEL    0x00000003L // 3  // mult+extended
#define zLISTBOX_FIXEDFONT            0x00000020L // 32
#define zLISTBOX_NOINTEGRALHEIGHT     0x00000040L // 64
#define zLISTBOX_LISTWHENPARENTSEL    0x00001000L // 4096
#define zLISTBOX_HILITEATPOSITION     0x00002000L // 8192
#define zLISTBOX_SCOPE_OI             0x00004000L // 16384
#define zLISTBOX_SORT_DISPLAY         0x00008000L // 32768
#define zLISTBOX_AUTODRAGSORT         zCONTROL_SPECIFIC_1  // 0x00040000L

#define zLISTBOX_LButtonPick          0x00000001L
#define zLISTBOX_LButtonEnter         0x00000002L
#define zLISTBOX_SetFocus             0x00000004L
#define zLISTBOX_KillFocus            0x00000005L
#define zLISTBOX_LButtonDown          0x00000008L
#define zLISTBOX_LButtonUp            0x00000009L
#define zLISTBOX_LButtonClick         0x0000000aL
#define zLISTBOX_LButtonClickPreSort  0x0000000bL
#define zLISTBOX_LButtonDrag          0x00000010L
#define zLISTBOX_LButtonDrop          0x00000011L
#define zLISTBOX_RButtonDown          0x0000000cL
#define zLISTBOX_RButtonUp            0x0000000dL
#define zLISTBOX_RButtonClick         0x0000000eL
#define zLISTBOX_RButtonDblclk        0x0000000fL
#define zLISTBOX_RButtonDrag          0x00000020L
#define zLISTBOX_RButtonDrop          0x00000022L
#define zLISTBOX_ItemChanging         0x00000100L
#define zLISTBOX_BeforePasteCell      0x00000200L
#define zLISTBOX_AfterPasteCell       0x00000400L

#define zCOMBOBOX_DROPDOWN_LIST       0x00000000L // default
#define zCOMBOBOX_DROPDOWN            0x00000001L // 1
#define zCOMBOBOX_SIMPLE              0x00000002L // 2
#define zCOMBOBOX_EXTENDED_LIST       0x00000004L // 4
#define zCOMBOBOX_SORT                0x00000010L // 16
#define zCOMBOBOX_DOMAIN_LIST         0x00000000L // default
#define zCOMBOBOX_SCOPE_OI            0x00000100L
#define zCOMBOBOX_NO_NULL             0x00000200L
#define zCOMBOBOX_EXTERNAL_DATA       0x00000800L
#define zCOMBOBOX_POSITION_ONLY       0x00001000L // 4096
#define zCOMBOBOX_SELECT              0x00002000L // 8192  //LIST_ENTITY
#define zCOMBOBOX_INCLUDE             0x00004000L // 16384 //LIST_ENTITY
#define zCOMBOBOX_FOREIGN_KEY         0x00008000L // 32768 //LIST_ENTITY

#define zRADIOBUTTON_DEFAULT          0x00000001L

#define zTOOLBOX_MULTIPLESEL          0x00000001L
#define zTOOLBOX_NOSTATE              0x00000002L
#define zTOOLBOX_TOOLBAR              0x00000004L

// these are Grid sub-control types
#define zGRID_EDITBOX                 0x00000000L
#define zGRID_COMBOBOX                0x00010000L
#define zGRID_DATE                    0x00020000L
#define zGRID_FLOAT                   0x00040000L
#define zGRID_INTEGER                 0x00080000L
#define zGRID_PIC                     0x00100000L
#define zGRID_PICTURE                 0x00200000L
#define zGRID_TEXT                    0x00400000L
#define zGRID_TIME                    0x00800000L
#define zGRID_BUTTON                  0x01000000L
#define zGRID_STATICTEXT              0x02000000L
#define zGRID_CHECKBOX                0x04000000L
#define zGRID_OWNERDRAW               0x80000000L
#define zGRID_ZEIDON                  0x0FFF0000L

#define zGRID_SCOPE_OI                0x00004000L // 16384

#define zLBN_NEWENTITY                0x00000100L // 256

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define  tzHDNODEX              9  // # of cells in width
#define  tzHDNODEY              5  // # of cells in height

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// If this structure changes, change the copy in tzpnolad.c.  This is
// the structure that defines each entity to be displayed by the
// tree control.

#pragma pack( push, __zdctl_h )
#pragma pack( 1 )

typedef struct tagOutlineItem
{
   zLONG lTagAttributeOffset; // Offset to attribute name used to identify entity
   zLONG lAttributeOffset;    // Offset to array of attributes (null terminated)
   zLONG lSiblingEntityOffset; // Offset to next sibling entity struct
   zLONG lChildEntityOffset;  // Offset to first child entity struct
   zLONG lFlags;              // Recursive entity indicator ...
                              // If children should be expanded on init
                              // If the parent view should be used for mapping
   zCHAR  szEntityName[ 33 ]; // Entity/attribute name (required)
   zCHAR  szScopeEntity[ 33 ]; // Scope for entity for szEntityName
   zCHAR  szRecursiveEntity[ 33 ]; // Recursive entity name

} zTREE_ITEM, *zLPOL_ITEM;

#pragma pack( pop, __zdctl_h )

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef struct tagListColumn
{
   zSHORT  nTabPos;   // Position of tab (first is always 0)
   zSHORT  nColLth;   // Length of list column
   zSHORT  nExtent;   // Extent of list column
   zSHORT  nColWidth; // Width of list column in 256ths of an inch
   zSHORT  nMinColWidth;
   zSHORT  nMaxColWidth;
   zSHORT  nJustify;  // Right/Left/Center justification
   zPCHAR  pchEntity;
   zPCHAR  pchAttrib;
   zPCHAR  pchContext;
   zPCHAR  pchTag;
   zPCHAR  pchText;
   zPCHAR  pchData;   // Data specific to subctrl's type (e.g. CheckBox)
   zPCHAR  pchMB_Text;
   zULONG  ulLth;
   zULONG  ulStyle;
   zULONG  ulSubtype;
   zPCHAR  pchEntityX; // Extra E/A specification (e.g. for CheckBox display)
   zPCHAR  pchAttribX;
   zPCHAR  pchContextX;
   zPCHAR  pchColorAttribute;

} zLB_COL, *zLPLB_COL;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// forward declarations

class ZDCTL_CLASS ZTypeDesc;
class ZDCTL_CLASS ZGlobalOperation;
class ZDCTL_CLASS ZGlobalDispatch;
class ZDCTL_CLASS ZOperation;
class ZDCTL_CLASS ZFunction;
class ZDCTL_CLASS ZDispinterface;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZCellId
{
// Attributes
public:
   long m_lRow;
   int  m_nCol;

// Operations
public:
   explicit ZCellId( long lRow = -1, int nCol = -1 ) :
            m_lRow( lRow ), m_nCol( nCol ) {}

   int IsValid( ) const { return( m_lRow >= 0 && m_nCol >= 0 ); }
   int operator==(const ZCellId& rhs) const
   {
      return( m_lRow == rhs.m_lRow && m_nCol == rhs.m_nCol );
   }

   int operator!=(const ZCellId& rhs) const
   {
      return( !operator==(rhs) );
   }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define ZTITLETIP_CLASSNAME _T( "ZTitleTip" )

/////////////////////////////////////////////////////////////////////////////
// ZTitleTip window

class ZTitleTip : public CWnd
{
// Construction
public:
   ZTitleTip( zLONG lHoverDelay );
   virtual ~ZTitleTip( );
   virtual BOOL Create( CWnd *pParentWnd );

// Attributes
public:
   CWnd  *m_pParentWnd;
   CRect m_rectTitle;

// Operations
public:
   void Show( CRect rectTitle, zCPCHAR cpcTitleText,
              int nOffsetX = 0, LPRECT pHoverRect = 0,
              const LOGFONT *pLogFont = 0,
              COLORREF clrText = CLR_DEFAULT,
              COLORREF clrBack = CLR_DEFAULT );
   void Hide( );
   void DrawTipText( int nId );

// Overrides
   // ClassWizard generated virtual function overrides.
   //{{AFX_VIRTUAL( ZTitleTip )
   public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL DestroyWindow( );
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRect   m_rectHover;
   DWORD   m_dwLastLButtonDown;
   DWORD   m_ulDblClickInterval;
   BOOL    m_bCreated;

   zLONG   m_lHoverDelay;
   UINT    m_uToolTipTimerId;
   CString m_csTitle;
   int     m_nOffsetX;
   COLORREF m_clrText;
   COLORREF m_clrBack;

   // Generated message map functions.
protected:
   //{{AFX_MSG(ZTitleTip)
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnTimer( UINT uIDEvent );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

class ZListTip : public CWnd
{
// Construction
public:
   ZListTip( zLONG lHoverDelay );
   virtual ~ZListTip( );
   virtual BOOL Create( CWnd *pParentWnd );

// Attributes
public:
   CWnd  *m_pParentWnd;
   CRect m_rectTitle;

// Operations
public:
   void Show( CRect rectTitle, zCPCHAR cpcTitleText,
              int nOffsetX = 0, LPRECT pHoverRect = 0,
              const LOGFONT *pLogFont = 0,
              COLORREF clrText = CLR_DEFAULT,
              COLORREF clrBack = CLR_DEFAULT );
   void Hide( );
   void DrawTipText( );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZListTip)
public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL DestroyWindow( );
   //}}AFX_VIRTUAL

// Implementation
   // Generated message map functions
protected:
   //{{AFX_MSG(ZListTip)
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnTimer( UINT uIDEvent );
   //}}AFX_MSG

private:
   zULONG  m_ulDblClickInterval;
   CPoint  m_ptStart;
   zLONG   m_lHoverDelay;
   UINT    m_uToolTipTimerId;
   CString m_csTitle;
   int     m_nOffsetX;

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDCTL_CLASS ZTlbBaseProperties : public CObject
{
   DECLARE_DYNAMIC( ZTlbBaseProperties )
public:
   CString GetDocumentationAsComment();
   ZTlbBaseProperties();
   virtual ~ZTlbBaseProperties();

   CString GetHelpfile();
   CString GetDocumentation();
   virtual CString GetName();

   zULONG GetHelpContext() { return m_ulHelpContext; }

   BSTR    m_bstrName;                     // Property name.
   BSTR    m_bstrDocumentation;            // Documentation.
   zULONG  m_ulHelpContext;                // Help Context.
   BSTR    m_bstrHelpFile;                 // Helpfile.
};

class ZDCTL_CLASS ZTypeLibrary : public ZTlbBaseProperties
{
   DECLARE_DYNAMIC( ZTypeLibrary )

public: // methods

   ZTypeLibrary();
   virtual ~ZTypeLibrary();

   CObList * GetDispTypeInfos() { return &m_pdispTypeInfos; }

   ZDispinterface * GetDispatchOfGUID( CString strGUID );
   zBOOL GenerateCode( zCPCHAR cpcFileName = NULL,
                       zCPCHAR cpcScriptFunctionName = NULL,
                       zCPCHAR cpcNewDefaultName = NULL,
                       zCPCHAR cpcGUID = NULL,
                       zBOOL   bAppend = FALSE );

   ZDispinterface * GetEventInterface( LPCSTR lpszGUID );
   ZDispinterface * GetDefaultDispatch( LPCSTR lpszGUID );
   POSITION GetFirstActiveX_OperationPosition();
   ZOperation * GetNextOperation( POSITION& pos );
   POSITION GetFirstGlobalDispatchPosition();
   ZGlobalDispatch * GetNextGlobalDispatch( POSITION& pos );
   POSITION GetFirstExternalClassPosition();
   CString GetNextExternalClass( POSITION& pos );
   void QueryExternalClasses();
   ZDispinterface * FindDispatchByName( CString strName );

   CString GetGUID_AsString();
   BOOL LoadTypeLibrary( LPCSTR lpLibraryName, LPCSTR lpszGUID = NULL );
   void DestroyContent();
   LCID GetLocalID() { return m_lcid; }
   void GetVersionInformation( PUSHORT pushMinorVersion,
                               PUSHORT pushMajorVersion )
   {
      *pushMinorVersion = m_wMinorVer;
      *pushMajorVersion = m_wMajorVer;
   }

   zULONG GetHelpContext() { return m_ulHelpContext; }
   GUID GetGUID() { return m_guid; }

private: // methods

   long BrowseLibComplete();
   BOOL WriteFileHeader( CFile * );
   BOOL CreateCPP( zCPCHAR cpcFileName );
   BOOL CreateHeader( zCPCHAR cpcFileName );
   BOOL CreateC( zCPCHAR cpcFileName, CString csScriptFunctionName );
   BOOL CreateDEF( zCPCHAR cpcFileName, zCPCHAR cpcZeidonScriptInit );
   int  GetMaxOrdinal( CFile *f, DWORD *dwOrdinalTokenPosition );
   void PopulateGlobalOperations( ZDispinterface *i );
   long PopulateGlobalOperations(); // called the first time when GetFirst ActiveX_OperationPosition is called
                                    // to set up the list of global operations
   long CheckDependencies();
   long BuildStringListOfReturnTypes( CStringList *sl, ZDispinterface *pI );

private: // members
   CString     m_csGUID;// the controls guid
   CString     m_csLibraryFileName;
   CString     m_csNewDefaultClassName;
   CString     m_csCPPFileName;
   CString     m_csHeaderFileName;
   CObList     m_pdispTypeInfos;         // List of typeinfos contained by this typelib
   LPTYPELIB   m_TypeLib;
   GUID        m_guid;                   // the typelibs GUID
   LCID        m_lcid;                   // Local ID
   USHORT      m_wMajorVer;              // Major version
   USHORT      m_wMinorVer;              // Minor version
   zBOOL       m_bAppend;                // Overwrite or append to existing code
   CObList     m_lGlobalOperations;      // list of global operations to be displayed in the edito
   CObList     m_lGlobalDispatch;        //
   CObList     m_lDependendInterfaces;   //
   CStringList m_lExternalClasses;       // list of classes that are not defined in the typelib
};

class ZDCTL_CLASS ZTypeInfo : public ZTlbBaseProperties
{
   DECLARE_DYNAMIC( ZTypeInfo )

public:
   ZTypeInfo( ) {}
   ZTypeInfo( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib );
   virtual ~ZTypeInfo();

// Operations
public:
   CString GetGUID_AsString();
   GUID GetGUID() { return m_guid; }
   ZTypeLibrary * GetTypeLibrary() { return m_pTypeLib; }

   TYPEKIND GetTypeKind() { return m_typekind; }

   BOOL InitTypeInfo( LPTYPEINFO ptinfo );

protected:
   short m_wTypeFlags;            // TYPEFLAGS

private: // methods

private: // members
   TYPEKIND m_typekind;           // Type of TypeInfo. See TYPEKIND enumeration.
   GUID m_guid;                   // GUID

   ZTypeLibrary *m_pTypeLib;      // pointer back to the Typelibrary where the info belongs to

};

class ZDCTL_CLASS ZInterface  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZInterface )

// Attributes
public:
   ZInterface() {};
   ZInterface( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib, int nImplFlag = 0 );
   virtual ~ZInterface();

// Operations
public:
   CObList     *GetFunctions() { return &m_pdispFunctions; }
   ZInterface  *GetBaseInterface() { return m_pdispBaseInterface; }
   CString     GetCPPDeclaration();

private: // methods
   long QueryFunctions();
   BOOL QueryBaseInterface();

private: // members
   int m_nImplFlag;
   LPTYPEINFO  m_ptinfo;              // ITypeInfo* of interface typeinfo.
   CObList     m_pdispFunctions;      // List of functions in interface.
   ZInterface  *m_pdispBaseInterface; // Base interface of this interface.
};

class ZDCTL_CLASS ZDispinterface  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZDispinterface )

// Attributes
public:
   ZDispinterface() {}
   ZDispinterface( LPTYPEINFO ptinfo, ZTypeLibrary * pTypeLib );
   virtual ~ZDispinterface();

   friend ZTypeLibrary;
public: // methods
   CString GetForwardDeclaration();
   CString GetCPPDefinition();
   CString GetCPPDeclaration();
   CObList *GetProperties() { return &m_pdispProperties; }
   CObList *GetMethods() { return &m_pdispMethods; }
   ZInterface *GetInterface() { return m_pInterface; }

protected:
   void Rename(CString strNewName);

private: // methods
   long LoadProperties();
   long LoadMethods();
   BOOL LoadInterface();

private: // members
   LPTYPEINFO  m_ptinfo;              // ITypeInfo* of dispinterface typeinfo.
   CObList     m_pdispProperties;     // List of properties in dispinterface
   CObList     m_pdispMethods;        // List of methods in dispinterface
   ZInterface  *m_pInterface;         // Interface part of dual interface, if this is a dual interface
};

class ZDCTL_CLASS ZModule  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZModule )

// Attributes
public:
   ZModule() {}
   ZModule( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib );
   virtual ~ZModule();

// Operations
public:
   CObList* GetFunctions() { return &m_pFunctions; }

private: // methods
   zLONG LoadFunctions();

private: // members
   LPTYPEINFO  m_ptinfo;               // ITypeInfo* of CModul.
   CObList     m_pFunctions;            // List of functions
};

class ZDCTL_CLASS ZCoClass  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZCoClass )

// Attributes
public:
   ZCoClass() {}
   ZCoClass( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib, int m_nImplFlag = 0 );
   virtual ~ZCoClass();

   friend ZTypeLibrary;

// Operations
public:
   CString GetEventInterfaceGUID();
   CString GetDefaultDispatchInterfaceGUID();
   CObList *GetInterfaces() { return &m_Interfaces; }
   BOOL IsEventSource( );
   BOOL IsDefault( );

private: // methods
   zLONG LoadInterfaces();

private: // members
   CObList m_Interfaces;
   LPTYPEINFO m_ptinfo;              // TYPEINFO of coclass typeinfo.
   int m_nImplFlag;
protected:
};

class ZDCTL_CLASS ZEnum  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZEnum )

// Attributes
public:
   ZEnum() {}
   ZEnum( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib );
   virtual ~ZEnum();

// Operations
public:
   CString GetCPPDeclaration( BOOL bTagged = TRUE );
   CObList * GetElements() { return &m_pElements; }

private: // methods
   zLONG LoadEnumElements();

private: // members
   LPTYPEINFO  m_ptinfo;         // enum typeinfo.
   CObList     m_pElements;      // list of elements in enumeration
                                 // (all type of ZConstant).
};

class ZDCTL_CLASS ZAlias  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZAlias )

// Attributes
public:
   ZAlias() {}
   ZAlias( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib );
   virtual ~ZAlias();

// Operations
public:

private: // methods

private: // members
   ZTypeDesc *m_pTypeDescBase;  // ZTypeDesc which describes the base type of this alias.

};

class ZDCTL_CLASS ZStruct  : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZStruct )

// Attributes
public:
   ZStruct() {}
   ZStruct( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib);
   virtual ~ZStruct();

// Operations
public:
   CString GetCPPDefinition(BOOL bTagged = TRUE);
   CObList* GetElements() { return &m_Elements;}

private: // methods
   zLONG LoadElements();

private: // members
   LPTYPEINFO m_ptinfo;            // ITypeInfo* of struct typeinfo.
   CObList    m_Elements;          // Collection of members in struct.

};

class ZDCTL_CLASS ZUnion : public ZTypeInfo
{
   DECLARE_DYNAMIC( ZUnion )

// Attributes
public:
   ZUnion() {}
   ZUnion( LPTYPEINFO ptinfo, ZTypeLibrary * pTypeLib );
   virtual ~ZUnion();

// Operations
public:
   CObList * GetElements() { return &m_Elements; }
   CString GetCPPDefinition( BOOL bTagged = TRUE );

private: // methods
   zLONG LoadElements();

private: // members
   LPTYPEINFO m_ptinfo;            // ITypeInfo* of struct typeinfo.
   CObList    m_Elements;          // Collection of members in struct.
};

class ZDCTL_CLASS ZFunction : public ZTlbBaseProperties
{
   DECLARE_DYNAMIC( ZFunction )

// Attributes
public:
   ZFunction( LPTYPEINFO ptinfo, USHORT nIndex, ZInterface *pInterface );
   ZFunction( LPTYPEINFO ptinfo, USHORT nIndex, ZDispinterface *pInterface );
   ZFunction( LPTYPEINFO ptinfo, USHORT nIndex, ZModule *pModule );
   virtual ~ZFunction();

   friend ZGlobalOperation;
   friend ZTypeLibrary;
// Operations
public:
   BOOL IsOptionalParameter( SHORT i);
   BOOL IsOptionalParameter( CString strParam );
   CString GetCPPDefinition();
   CString GetCPPDeclaration( zBOOL bHeader = TRUE );
   CString GetClassName();
   CString GetInvokeKindString();
   CString GetPropertyMethodPrefix();
   ZTypeLibrary * GetTypeLibrary() { return m_pTypeLib; }

   MEMBERID    GetMemberID() { return m_pfuncdesc->memid; }
   FUNCKIND    GetFunctionKind() { return m_pfuncdesc->funckind; }
   INVOKEKIND  GetInvokeKind() { return m_pfuncdesc->invkind; }
   CALLCONV    GetCallConv() { return m_pfuncdesc->callconv;}
   SHORT       GetNumOfParams() {return m_pfuncdesc->cParams;}
   SHORT       GetNumOfOptionalParams() {return m_pfuncdesc->cParamsOpt;}
   SHORT       GetOffsetInVTable() {return m_pfuncdesc->oVft;}
   WORD        GetFunctionFlags() {return m_pfuncdesc->wFuncFlags;}
   CObList     *GetParameter() { return &m_pdispParameters; }
   ZTypeDesc   *GetType() { return m_pdispTypeDesc; }
protected:
   CString GetMethodCall();
   CObject *GetParentInterface() { return m_pParentInterface; }
   ZTypeDesc  *m_pdispTypeDesc;         // return type.
   CObList    m_pdispParameters;        // List of parameters of the current function.
   LPFUNCDESC m_pfuncdesc;              // FUNCDESC of function.
   LPTYPEINFO m_ptinfoFunction;         // TypeInfo of which this function is an element.

private: // methods

   zLONG LoadParameter(); // load parameter for the specific function
   BOOL Initialize( LPTYPEINFO ptinfo, zUSHORT nIndex ); // Initialize itself

private: // members
   ZTypeLibrary *m_pTypeLib;
   CObject    *m_pParentInterface;      // can be type of ZInterface or ZDispinterface
                                        // and is used to retierve the class name while generating c++ code
};

class ZDCTL_CLASS ZTypeDesc : public CObject
{
   DECLARE_DYNAMIC( ZTypeDesc )

// Attributes
public:
   ZTypeDesc( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib,
              TYPEDESC FAR *ptypedesc );
   virtual ~ZTypeDesc();

// Operations
public:
   USHORT GetStackSize();
   CString GetVariantType();
   CString GetCPPDefinition();
   VARTYPE GetVartype() { return m_vartype; }
   CObject *GetDescription() { return m_pDescription; }
   BOOL IsDispatchPointer();
   ZTypeLibrary *GetTypeLibrary() { return m_pTypeLib; }

private: // methods

private: // members
   VARTYPE m_vartype;       // Type
   CObject *m_pDescription; // If m_vartype==VT_USERDEFINED, m_pDescribtion contains
                            // a ZTypeInfo of user-defined type.,
                            // if m_vartype==VT_CARRAY, is a ZArrayDesc
                            // if m_vartype==VT_PTR is a ZTypeDesc of type pointed to.
   ZTypeLibrary *m_pTypeLib;
};

class ZDCTL_CLASS ZArrayBound : public CObject
{
   DECLARE_DYNAMIC( ZArrayBound )

// Attributes
public:
   ZArrayBound( zULONG cElements, zLONG lLBound);
   virtual ~ZArrayBound();

// Operations
public:
   zLONG GetLowerBound() { return m_lLowerBound; }
   zULONG GetElementCount() { return m_ulElementCount; }
private: // methods

private: // members
   zULONG m_ulElementCount;
   zLONG m_lLowerBound;

};

class ZDCTL_CLASS ZArrayDesc : public CObject
{
   DECLARE_DYNAMIC( ZArrayDesc )

// Attributes
public:
   ZArrayDesc( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib,
               ARRAYDESC FAR *parraydesc);
   virtual ~ZArrayDesc();

// Operations
public:
   CString GetCPPDefinition();
   ZTypeDesc * GetTypeDescribtion() { return m_pTypeDesc; }
   CObList * GetArrayBounds() { return &m_ArrayBounds; }
private: // methods

private: // members
   ZTypeDesc   *m_pTypeDesc;
   CObList     m_ArrayBounds;

};

class ZDCTL_CLASS ZConstant: public ZTypeInfo
{
   DECLARE_DYNAMIC( ZConstant )

// Attributes
public:
   ZConstant( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib, LPVARDESC pvardesc );
   virtual ~ZConstant();

// Operations
public:
   ZTypeDesc   *GetType() { return m_pTypeDesc; }
   MEMBERID    GetMemberID() { return m_memid; }
   COleVariant GetValue() { return m_vValue; }

private: // methods

private: // members
   ZTypeDesc   *m_pTypeDesc;            // describes type of constant.
   MEMBERID    m_memid;                 // MEMBERID.
   COleVariant m_vValue;                // Constant value.
};

class ZDCTL_CLASS ZProperty : public ZTlbBaseProperties
{
   DECLARE_DYNAMIC( ZProperty )

// Attributes
public:
   ZProperty( LPTYPEINFO ptinfo, LPVARDESC pvardesc, ZTypeLibrary *pTypeLib,
              ZDispinterface *pDispinterface = NULL );
   virtual ~ZProperty();

// Operations
public:
   CString GetClassName();
   CString GetCPPDeclaration( );

   MEMBERID    GetMemberID() { return m_memid; }
   ZTypeDesc * GetType() { return m_pdispTypeDesc; }
   CString GetCPPDefinition();

   ZTypeLibrary * GetTypeLibrary( ) { return m_pTypeLib; }
private: // methods

private: // members
   ZDispinterface *m_pDispinterface;
   ZTypeDesc      *m_pdispTypeDesc;       // Property type.
   MEMBERID       m_memid;                // MEMBERID of property.

   ZTypeLibrary *m_pTypeLib;
};

class ZDCTL_CLASS ZParameter : public ZTlbBaseProperties
{
   DECLARE_DYNAMIC( ZParameter )

// Attributes
public:
   ZParameter( LPTYPEINFO ptinfo, ZTypeLibrary *pTypeLib, BSTR bstrName,
               TYPEDESC FAR *ptypedesc, IDLDESC FAR *pidldesc,
               MEMBERID memid );
   virtual ~ZParameter();

// Operations
public:
   CString GetVariantType();

   MEMBERID  GetMemberID() { return m_memid; }
   ZTypeDesc * GetType() { return m_pdispTypeDesc; }
   USHORT    GetIDLFlags() { return m_usFlags; }
   ZTypeLibrary * GetTypeLibrary() { return m_pTypeLib; }

private: // methods

private: // members
   ZTypeDesc  *m_pdispTypeDesc;         // Parameter type.
   MEMBERID m_memid;                    // MEMBERID of property.
   USHORT   m_usFlags;                  // specifies in/out/in-out/none
   ZTypeLibrary * m_pTypeLib;
};

class ZGlobalOperation : public CObject
{
   DECLARE_DYNAMIC( ZGlobalOperation )

// Attributes
public:
   ZGlobalOperation() { }
   ZGlobalOperation( ZFunction *f, ZGlobalDispatch *pDI )
   {
      SetFunction( f );
      SetDispatchInterface( pDI );
   }
   virtual ~ZGlobalOperation() {}
   void SetDispatchInterface( ZGlobalDispatch *pDI ) { m_pParentDispatch = pDI; }

// Operations
public:
   CString GetCPPDefinition( zBOOL bUsedForHeader = FALSE );
   CString GetDefEntry( int *iOrdinal );
   CString GetCPPDeclaration();
   void SetFunction( ZFunction *f ) { m_pFunction = f; }

private: // methods

private: // members
   ZFunction *m_pFunction;
   ZGlobalDispatch *m_pParentDispatch;
};

class ZDCTL_CLASS ZGlobalProperty : public CObject
{
   DECLARE_DYNAMIC( ZGlobalProperty )

// Attributes
public:
   ZGlobalProperty() { }
   ZGlobalProperty( ZProperty *f, ZGlobalDispatch *pParent )
   {
      SetProperty( f );
      SetDispatchInterface( pParent );
   }
   virtual ~ZGlobalProperty() {}

// Operations
public:
   CString GetCPPDefinition( zBOOL bUsedForHeader = FALSE );
   CString GetCPPDeclaration();
   CString GetDefEntry( int *iOrdinal );
   void SetProperty( ZProperty *f ) { m_pProperty = f; }
   void SetDispatchInterface( ZGlobalDispatch *pDI ) { m_pParentDispatch = pDI; }

private: // methods

private: // members
   ZProperty *m_pProperty;             // points to the related ZProperty
   ZGlobalDispatch *m_pParentDispatch; // pointer to the ZGlobalDispatch wher the property belongs to
};

class ZDCTL_CLASS ZGlobalDispatch : public CObject
{
   DECLARE_DYNAMIC( ZGlobalDispatch )

// Attributes
public:
   ZGlobalDispatch( ZDispinterface *i = NULL ) { SetInterface( i ); }
   virtual ~ZGlobalDispatch();
   CStringList * GetObjectNames( ) { return &m_lstrObjectNames; }

// Operations
public:
   CObList m_lProperties;
   CObList m_lOperations;
   CString GetCPPDefinition();
   CString GetCPPDeclaration();
   CString GetDefEntry( int * iOrdinal );
   void SetInterface( ZDispinterface *i );

private: // methods

private: // members
   ZDispinterface *m_pDispinterface;   // points to the related dispatch interface
   CStringList m_lstrObjectNames;      // keeps track about method names
                                       // when generating C-Code to avoid
                                       // duplicates
};

class ZDCTL_CLASS ZOperEntry : public CObject
{
   DECLARE_DYNAMIC( ZOperEntry )

// Attributes
public:
   ZOperEntry() {}
   virtual ~ZOperEntry() {}
   void SetEntry( CString strType, CString strName )
   {
      m_csName = strName;
      m_csType = strType;
   }

   CString GetName() { return m_csName; }
   CString GetType() { return m_csType; }

private: // methods

private: // members
   CString m_csType;
   CString m_csName;
};

class ZDCTL_CLASS ZOperation : public ZOperEntry
{
   DECLARE_DYNAMIC( ZOperation )

// Attributes
public:
   ZOperation() {}
   virtual ~ZOperation();

   void AddParameter( CString strType, CString strName )
   {
      ZOperEntry *e = new ZOperEntry();
      e->SetEntry( strType, strName );
      m_lParamList.AddTail( e );
   }

   POSITION GetFirstParameterPosition()
   {
      return m_lParamList.GetHeadPosition();
   }

   ZOperation *GetNextParameter( POSITION& pos )
   {
      return (ZOperation *) m_lParamList.GetNext( pos );
   }

private: // methods

private: // members
   CObList m_lParamList;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Export remote server helper classes
//

#ifdef zREMOTE_SERVER

#define zRMT_PROCESSED       0x00010000
#define zRMT_CHANGED_KEY     0x00010011
#define zRMT_CHANGED_ORDER   0x00010012
#define zRMT_CHANGED_DATA    0x00010014
#define zRMT_CHANGED         0x00010010
#define zRMT_ADDED           0x00010100
#define zRMT_DELETED         0x00010200

class ZDCTL_CLASS ZRmtDataItem : public CObject
{
   DECLARE_DYNAMIC( ZRmtDataItem )

public: // methods
   ZRmtDataItem( );
   ZRmtDataItem( CString csItemValue, zLONG lItemPos = 0 );
   ZRmtDataItem::ZRmtDataItem( const ZRmtDataItem& rdi );  // copy ctor

   ~ZRmtDataItem( );

   CString GetString( ) { return m_csItemValue; }
   zLONG   GetItemPos( ) { return m_lItemPos; }
   void    SetString( CString csText ) { m_csItemValue = csText; };
   zSHORT  Compare( const ZRmtDataItem& rdi ) const;
   void    CopyData( const ZRmtDataItem& rdi );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

private:
   zLONG    m_lItemPos;
   CString  m_csItemValue;
};

class ZDCTL_CLASS ZRmtDataRow : public CObject
{
   DECLARE_DYNAMIC( ZRmtDataRow )

public: // methods
   ZRmtDataRow( );
   ZRmtDataRow( zCPCHAR cpcValue, zLONG lKey, zLONG lPos = 0 );
   ZRmtDataRow::ZRmtDataRow( const ZRmtDataRow& rdr ); // copy ctor

   ~ZRmtDataRow( );

   CString GetRowDataText( zBOOL bUseSeparator = TRUE ) const;
   zLONG   SetRowDataText( CString csRowText );

   void   ResetContent( );
   void   AddString( zCPCHAR cpcStr, zLONG lPos = 0 );
   void   SetSel( zBOOL bSel );
   zBOOL  GetSel( ) const;
   CString GetString( zLONG lPos = 0 );
   void   SetItemData( DWORD dwItemData );
   void   SetKey( zLONG lKey );
   zLONG  GetKey( ) const;
   void   SetKeyOrig( zLONG lKey );
   zLONG  GetKeyOrig( ) const;
   DWORD  GetItemData( ) const;
   void   SetState( zULONG ulState, zSHORT nCmd );
   zULONG GetState( ) const;
   void   SetOrder( zLONG lOrderNbr );
   zLONG  GetOrder( ) const;
   zSHORT Compare( const ZRmtDataRow& rdr ) const;
   void   CopyData( const ZRmtDataRow& rdr );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

   CObList m_Cols;

private:
   zLONG UpdateColumn( zLONG lPos, CString csText );

   DWORD  m_dwItemData;
   zBOOL  m_bIsSelected;
   zLONG  m_lOrderNbr;
   zLONG  m_lKey;
   zLONG  m_lKeyOrig;
   zULONG m_ulState;
};

class ZDCTL_CLASS ZRmtDataSet : public CWnd
{
   DECLARE_DYNAMIC( ZRmtDataSet )

public:

   ZRmtDataSet( );
   ~ZRmtDataSet( );

   // methods to act like a listbox / combobox
   zLONG AddString( zCPCHAR cpcValue, zLONG lKey, zLONG lPos );
   zLONG InsertString( zCPCHAR cpcString, zLONG lKey, zLONG lPos );

   zLONG SetCurSel( zLONG lPos );
   zLONG SetItemData( zLONG lIdx, DWORD dwItemData );
   zLONG SetSel( zLONG lPos, zBOOL bSet );
   zLONG SetTopIndex( zLONG lIdx );
   zLONG SelectString( zLONG lStartAfter, zCPCHAR cpcString );

   DWORD GetItemData( zLONG lIdx);
   zLONG GetTopIndex( );
   zLONG GetLBText( zLONG lIdx, LPTSTR lpszText );
   void  GetLBText( zLONG lIdx, CString& rString );
   zLONG GetCurSel( );
   zLONG GetCaretIndex( ) { return LB_ERR; }
   zLONG GetSelCount( );
   zLONG GetCount( );
   zLONG GetSelItems( zLONG nMaxItems, LPINT rgIndex );

   void ResetContent();
   void ResetSelectionSet( zBOOL bSet = FALSE );
   void MergeRowData( zBOOL bReset );

   void SetRedraw( BOOL bRedraw = TRUE ) { }
   void Invalidate( BOOL bErase = TRUE ) { }

   // retrieval methods
   POSITION GetFirstDataRowPosition( );
   ZRmtDataRow * GetNextDataRow( POSITION& pos );
   ZRmtDataRow * FindDataRowByIndex( zLONG lIdx );
   ZRmtDataRow * FindDataRowByDataItem( DWORD dw );
   ZRmtDataRow * FindDataRowByString( zCPCHAR cpcString,
                                      zLONG *plIndex,
                                      zLONG lOffset = -1,
                                      zBOOL bExact = FALSE );
   void DisplayListData( );

#ifdef _DEBUG
   void Dump( CDumpContext& dc ) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

private:
   zLONG        m_lTopIndex;
   ZMultiIdxSet *m_pRowsOrig;
   ZMultiIdxSet *m_pRows;
};

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This DLL exports the standard Zeidon controls
//

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// TipWnd.h : header file
//

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         //
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       //
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZToolTip window

class ZToolTip : public CWnd
{
// Construction
public:
   ZToolTip( );

// Attributes
public:
protected:
   CFont    m_font;
   CString  m_csFont;
   int      m_nFontHeight;
   zBOOL    m_bResetFont;
   CString  m_csText;
   CSize    m_sizeExtent;

// Operations
public:
   BOOL Create( CWnd *pParent, zCPCHAR cpcText = 0,
                zCPCHAR cpcFont = 0, int nFontHeight = 0 );
   void ShowTips( CPoint pt, const CString& cs );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZToolTip )
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZToolTip( );

   // Generated message map functions
protected:
   //{{AFX_MSG(ZToolTip)
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnPaint( );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// class ZDCTLATX
//
// DESC: The ZDCTLATX class is a general class for using ActiveX Controls
//       at Runtime
//
/////////////////////////////////////////////////////////////////////////////

typedef struct tagCTRL_EVENT
{
   AFX_EVENT *m_pEvent;
   zULONG    m_ulRC;  // 0x00000000 ==> invoke default processing for event
                      // 0x00000001 ==> skip default processing for event

} zCTRL_EVENT;

zOPER_EXPORT COleDispatchDriver * OPERATION
GetActiveX_WrapperInstance( zVIEW    vSubtask,
                            zCPCHAR  cpcCtrlTag );

// Flags for Get or Set Data to an ActiveX - Control
#define zACTIVEX_INITIALIZE                           0L
#define zACTIVEX_TRANSFER_DATA_FROM_OI                1L
#define zACTIVEX_TRANSFER_DATA_TO_OI                  2L
#define zACTIVEX_GET_ATTRIBUTE_COUNT                  3L
#define zACTIVEX_RECEIVE_FOCUS                        4L
#define zACTIVEX_LOSE_FOCUS                           5L
#define zACTIVEX_SET_CTRL_STATE                       6L
#define zACTIVEX_GET_CTRL_STATE                       7L
#define zACTIVEX_HANDLE_EVENT                         8L
#define zACTIVEX_UNINITIALIZE                         9L
#define zACTIVEX_CREATE_ZEIDON_CONTROL               10L

// The parameters to an ActiveX Script are:
//  - Pointer to the current ctrl.
//  - Flag (e.g.  the flag passed in from MapToOI).
//  - one of the above flags (e.g.  zTRANSFER_DATA_FROM_OI).
typedef zLONG (POPERATION zFARPROC_ACTIVEX_SCRIPT)
                                   ( ZMapAct *, zLONG, zULONG, zLONG );

// defines for readability in the generated script and
// wrapper classes for ActiveX controls.
#define VTS_PUSERDEFINED   VTS_DISPATCH
#define VTS_USERDEFINED    VTS_I4

class ZDCTL_CLASS ZActiveX : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZActiveX )

public:
   ZActiveX( ZSubtask *pZSubtask,
             CWnd     *pWndParent,
             ZMapAct  *pzmaComposite,
             zVIEW    vDialog,
             zSHORT   nOffsetX,
             zSHORT   nOffsetY,
             zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   virtual ~ZActiveX();

// // Necessary evil for MI disambiguity
// void *operator new( size_t nSize )
//      { return CWnd::operator new( nSize ); }
// void operator delete( void *p )
//      { CWnd::operator delete( p ); }

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );

   // Function to communicate with the Property Dialog
   void     ShowProperties();

   zSHORT LoadMappingInfo( zVIEW vDialog );
   ZMapping * SeekMapItem( zCPCHAR cpcTag );

   // To catch the events from a Control
   virtual BOOL DestroyWindow( );
   virtual BOOL OnCmdMsg( UINT uId, int nCode, void *pExtra,
                          AFX_CMDHANDLERINFO *pHandlerInfo );
   void   QueryEventInfo();
   zSHORT PopulateEventParamEntities( ZEventItem *pEvent, zCTRL_EVENT ce );
   zBOOL  VariantTypeToZeidonType( COleVariant v, CString& str );

   zSHORT GetEventParmCnt( DISPID dispid );
   zSHORT GetEventParm( DISPID dispidEvent, zLONG lParmID,
                        zPCHAR pchValue, zSHORT nMaxLth );

   zLONG  HandleEvent( zLONG nEvent);

   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );
   afx_msg void OnKillFocus ( NMHDR * pNotifyStruct, LRESULT* result );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   void DestroyActiveX();

protected:

   virtual BOOL CreateActiveXObjectWithPersistProperties();

   BOOL RequestLicenseKey(BSTR &bstrLicenseKey, CString strClsid);
   void BuildLicKeyString();
public:

   CArray<ZMapping *, ZMapping *> m_MapList;
   zSHORT         m_nMapCnt;
   zULONG         m_ulFlag;
   zPVOID         GetCtrlBOI( ){ return m_pCtrlBOI; }
   zULONG         GetCtrlBOISize( ){ return m_ulCtrlBOILength; }

private:
   zULONG         m_ulPropertiesLth;
   zPVOID         m_pProperties;
   zULONG         m_ulCtrlBOILength;
   zPVOID         m_pCtrlBOI;
   zULONG         m_ulLicenseKeyLength;
   BSTR           m_bstrLicenseKey;
   CString        m_strLicenseKey;
   CString        m_csStorageFileName;
   CString        m_csClassID;
   CString        m_csTagPE;
   CString        m_csScript;
   CString        m_csScriptDLL;
   LPLIBRARY      m_hLibrary;    // Library from which to run scripts

   ZEventList     m_EventList;

#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CWnd::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }

public:
   static int m_iInstanceCount;
#endif

   DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//  C interface prototypes to C++ controls
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ActiveX( ZSubtask *pZSubtask,
         CWnd     *pWndParent,
         ZMapAct  *pzmaComposite,
         zVIEW    vDialog,
         zSHORT   nOffsetX,
         zSHORT   nOffsetY,
         zKZWDLGXO_Ctrl_DEF *pCtrlDef );

////////////////////////////////////////////////////
// Script-Function
AFX_EXT_API
void ActiveXScript(ZActiveX *pzActiveX, zLONG lMappingFlag);
////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void TraceDebug( zPCHAR pchFileName, zPCHAR pchFunctionName );

#if 0
#define MESSAGE_DEBUG( src_filename, function ) MessageBox( 0, src_filename, function, MB_OK )
#define TRACE_DEBUG( src_filename, function ) TraceDebug( src_filename, function )
#else
#define MESSAGE_DEBUG( src_filename, function )
#define TRACE_DEBUG( src_filename, function )
#endif

#define BUFFER_ALLOC_CNT 200
#define MemHugeCpy zmemcpy
#define MemHugeSet zmemset

#define  StrDeleteCh( szOrg, ch ) \
{\
   register zSHORT nNext = 0;\
   zPCHAR   szPtr = (szOrg);\
   zPCHAR   szStr = (szOrg);\
\
   while (*szStr)\
   {\
      if (*szStr == (char)(ch))\
      {\
         ++nNext;\
         *szStr = *(szStr + nNext);\
      }\
      else\
      {\
         ++szStr;\
         *szStr = *(szStr + nNext);\
      }\
   }\
   ((int)(szStr - szPtr));\
}

#define  StrReplaceCh( szOrg, ch )  \
{\
   zPCHAR   szPtr = (szOrg);\
   zPCHAR   szStr = (szOrg);\
\
   while (*szStr)\
   {\
      if ((BYTE)*szStr == HIBYTE((ch)))\
         *szStr = LOBYTE((ch));\
      ++szStr;\
   }\
   ((int)(szStr - szPtr));\
}

#define zstrlwr( a )             AnsiLower( (a) )
#define zstrupr( a )             AnsiUpper( (a) )
//#define zMAKEPOINT( l )          (*((POINT *) &(l)))

#define  InSet( a, b )      ((a) & (b))

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZColor
{
public:
   ZColor() {};
   ~ZColor() {};
   static COLORREF Black ( ) { return RGB(   0,   0,   0 ); }
   static COLORREF White ( ) { return RGB( 255, 255, 255 ); }
   static COLORREF Gray  ( ) { return RGB( 190, 190, 190 ); }
   static COLORREF Red   ( ) { return RGB( 255,   0,   0 ); }
   static COLORREF Green ( ) { return RGB(   0, 255,   0 ); }
   static COLORREF Brown ( ) { return RGB( 255, 255,   0 ); }
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef ZDCTL_CLASS_PREDEFINED
   #undef AFX_DATA
   #define AFX_DATA
#endif

/////////////////////////////////////////////////////////////////////////////
//
// These "ID's" are only here for informational purposes.  The driver and
// the controls themselves do not understand these values.  There are two
// conditions in which they may be used directly.  One is in the painter
// in which the control is asked to render itself (at that point, the
// control may need to know which sub-control is being rendered).  The
// other condition is in a control specific run-time operation.  In this
// case, the control may want to guarantee that the control type is valid
// so that the operation is not called for an invalid control type.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zTEXT_CONTROL                       1000L
#define zRTE_CONTROL                        1005L
#define zEDITGROUP_CONTROL                  1009L
#define zEDIT_CONTROL                       1010L
#define zMLE_CONTROL                        1015L
#define zPUSHBUTTON_CONTROL                 1020L
#define zTIMER_CONTROL                      1025L
#define zRADIOBUTTONGROUP_CONTROL           1030L
#define zRADIOBUTTON_CONTROL                1031L
#define zCHECKBOX_CONTROL                   1040L
#define zCOMBOBOX_CONTROL                   1050L
#define zCOMBOITEM_CONTROL                  1051L
#define zCALENDAR_CONTROL                   1060L
#define zDAYTIMER_CONTROL                   1065L
#define zCONTROLBAR_CONTROL                 1070L
#define zCONTROLCTL_CONTROL                 1071L
#define zLISTBOX_CONTROL                    1080L
#define zLISTCTL_CONTROL                    1081L
#define zGROUPBOX_CONTROL                   1090L
#define zBLOCK_CONTROL                      1091L
#define zPAINTER_CONTROL                    1999L
#define zPAINTERTOOLBAR_CONTROL             2000L
#define zER_DIAGRAM_CONTROL                 2005L
#define zGRID_CONTROL                       2010L
#define zGRID_EDIT_CONTROL                  2011L
#define zGRID_COMBO_CONTROL                 2012L
#define zGRID_CHECK_CONTROL                 2013L
#define zGRID_SUB_CONTROL                   2014L
#define zDIL_CONTROL                        2020L
#define zDILITEM_CONTROL                    2021L
#define zTREE_CONTROL                       2030L
#define zTREE_CTL_CONTROL                   2031L
#define zPALETTE_CONTROL                    2040L
#define zGRAPH_CONTROL                      2050L
#define zICON_CONTROL                       2060L
#define zHIER_DIAG_CONTROL                  2070L
#define zBITMAP_CONTROL                     2080L
#define zBITMAP_BUTTON_CONTROL              2085L
#define zTAB_CONTROL                        2090L
#define zTABCTL_CONTROL                     2091L
#define zTOOLBAR_CONTROL                    2100L
#define zTOOLITEM_CONTROL                   2101L
#define zGROUPSET_REPORT                    3000L
#define zGROUP_REPORT                       3010L
#define zTEXT_REPORT                        3020L
#define zMLE_REPORT                         3030L
#define zBMP_REPORT                         3040L
#define zSHAPE_REPORT                       3050L
#define zBLOCK_REPORT                       3060L
#define zLISTBOX_REPORT                     3070L
#define zLISTCTL_REPORT                     3071L
#define zACTIVEX                            4000L

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctl_h__ sentry.
