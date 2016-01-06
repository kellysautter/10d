#if !defined(__ZEIDON_ACTIVEX_TLB_H__)
#define __ZEIDON_ACTIVEX_TLB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"


class CTypeDesc;
class CDispinterface;
class CZGlobalOperation;
class CZOperEntry;

class CTlbBaseProperties : public CObject
{
   DECLARE_DYNAMIC(CTlbBaseProperties)
public:
   CString GetDocumentationAsComment();
   CTlbBaseProperties ();
   virtual ~CTlbBaseProperties ();

   CString GetHelpfile();
   CString GetDocumentation();
   CString GetName();

   ULONG GetHelpContext(){ return m_ulHelpContext; }

   BSTR    m_bstrName;                     // Property name.
   BSTR    m_bstrDocumentation;            // Documentation.
   ULONG   m_ulHelpContext;                // Help Context.
   BSTR    m_bstrHelpFile;                 // Helpfile.
};

/////////////////////////////////////////////////////////////////////////////
// CTypeLibrary command target

class CTypeLibrary : public CTlbBaseProperties
{
   DECLARE_DYNAMIC(CTypeLibrary)

// Attributes
public:
   CTypeLibrary();           // protected constructor used by dynamic creation
   virtual ~CTypeLibrary();


public: // methods
   CDispinterface * GetDispatchOfName( CString strInterfaceName );
   BOOL GenerateCode(LPCSTR lpszFileName = NULL,
                     LPCSTR strScriptFunctionName = NULL,
                     LPCSTR lpszNewDefaultName = NULL, BOOL bAppend = FALSE);

   CDispinterface* GetEventInterface();
   CDispinterface* GetDefaultDispatch();
   POSITION GetFirstGlobalOpationPosition(){ return m_lGlobalOperations.GetHeadPosition(); }
   CZOperEntry *GetNextOperation( POSITION &pos ){
      return (CZOperEntry *)m_lGlobalOperations.GetNext( pos );
   }


   CString GetGUID();

   BOOL LoadTypeLibrary(LPCSTR lpLibraryName);

   LCID GetLocalID(){ return m_lcid;}
   void GetVersionInformation(PUSHORT pushMinorVersion,
                              PUSHORT pushMajorVersion)
   {
      *pushMinorVersion = m_wMinorVer;
      *pushMajorVersion = m_wMajorVer;
   }

   ULONG GetHelpContext(){ return m_ulHelpContext; }

private: // methods

   long BrowseLibComplete();
   void DestroyContent();
   BOOL WriteFileHeader(CFile *, BOOL bCPPHeader = FALSE);
   BOOL CreateCPP(LPCSTR lpszFileName);
   BOOL CreateHeader(LPCSTR lpszFileName);
   BOOL CreateC ( LPCSTR strFileName , CString strScriptFunctionName );
   void PopulateGlobalOperations(CDispinterface *i);

private: // members
   CString     m_csLibraryFileName;
   CString     m_csNewDefaultClassName;
   CString     m_csCPPFileName;
   CString     m_csHeaderFileName;
   LPTYPELIB   m_TypeLib;
   GUID        m_guid;                   // GUID
   LCID        m_lcid;                   // Local ID
   USHORT      m_wMajorVer;              // Major version
   USHORT      m_wMinorVer;              // Minor version
   CObList     m_pdispTypeInfos;         // List of typeinfos contained by this typelib
   BOOL        m_bAppend;                // Overwrite or append to existing code
   CObList     m_lGlobalOperations;
   CObList     m_lGlobalDispatch;
};


class CTypeInfo : public CTlbBaseProperties
{
   DECLARE_DYNAMIC( CTypeInfo )

public:
   CTypeInfo (LPTYPEINFO ptinfo = NULL);
   virtual ~CTypeInfo ();

// Operations
public:
   CString GetGUID();

   TYPEKIND GetTypeKind(){ return m_typekind; }

   BOOL InitTypeInfo(LPTYPEINFO ptinfo);

protected:
   short m_wTypeFlags;            // TYPEFLAGS


private: // methods

private: // members
   TYPEKIND m_typekind;           // Type of TypeInfo. See TYPEKIND enumeration.
   GUID m_guid;                   // GUID
};


class CInterface  : public CTypeInfo
{
   DECLARE_DYNAMIC( CInterface )

// Attributes
public:
   CInterface(){};
   CInterface (LPTYPEINFO ptinfo, int nImplFlag = 0);
   virtual ~CInterface ();

// Operations
public:
   CObList     *GetFunctions() {return &m_pdispFunctions;}
   CInterface  *GetBaseInterface(){ return m_pdispBaseInterface;}

private: // methods
   long QueryFunctions();
   BOOL QueryBaseInterface();

private: // members
   int m_nImplFlag;
   LPTYPEINFO  m_ptinfo;             // ITypeInfo* of interface typeinfo.
   CObList     m_pdispFunctions;   // List of functions in interface.
   CInterface  *m_pdispBaseInterface; // Base interface of this interface.
};



class CDispinterface  : public CTypeInfo
{
   DECLARE_DYNAMIC(CDispinterface)

// Attributes
public:
   CDispinterface (LPTYPEINFO ptinfo = NULL);
   virtual ~CDispinterface ();

   friend CTypeLibrary;
public: // methods
   CString GetCPPDefinition();
   CString GetCPPDeclaration();
   CObList *GetProperties(){ return &m_pdispProperties; }
   CObList *GetMethods(){ return &m_pdispMethods; }
   CInterface *GetInterface(){ return m_pInterface; }

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
   CInterface  *m_pInterface;         // Interface part of dual interface, if this is a dual interface
};



class CModule  : public CTypeInfo
{
   DECLARE_DYNAMIC(CModule )

// Attributes
public:
   CModule(LPTYPEINFO ptinfo = NULL);
   virtual ~CModule();

// Operations
public:
   CObList* GetFunctions(){ return &m_pFunctions; }

private: // methods
   LONG LoadFunctions();

private: // members
   LPTYPEINFO  m_ptinfo;               // ITypeInfo* of CModul.
   CObList     m_pFunctions;            // List of functions
};



class CCoClass  : public CTypeInfo
{
   DECLARE_DYNAMIC(CCoClass)

// Attributes
public:
   CCoClass(LPTYPEINFO ptinfo = NULL, int m_nImplFlag = 0);
   virtual ~CCoClass();

   friend CTypeLibrary;

// Operations
public:
   CString GetEventInterfaceName();
   CString GetDefaultDispatchInterfaceName();
   CObList *GetInterfaces(){ return &m_Interfaces; }
   BOOL IsEventSource( );
   BOOL IsDefault( );

private: // methods
   LONG  LoadInterfaces();

private: // members
   CObList m_Interfaces;
   LPTYPEINFO m_ptinfo;              // TYPEINFO of coclass typeinfo.
   int m_nImplFlag;
protected:
   CInterface* GetEventInterface();
   CInterface* GetDefaultDispatch();
};



class CEnum  : public CTypeInfo
{
   DECLARE_DYNAMIC(CEnum )

// Attributes
public:
   CEnum (LPTYPEINFO ptinfo = NULL);
   virtual ~CEnum ();

// Operations
public:
   CString GetCPPDeclaration(BOOL bTagged = TRUE);
   CObList *GetElements(){ return &m_pElements; }

private: // methods
   LONG LoadEnumElements();

private: // members
   LPTYPEINFO  m_ptinfo;         // enum typeinfo.
   CObList     m_pElements;      // list of elements in enumeration
                                 // (all type of CConstant).

};



class CAlias  : public CTypeInfo
{
   DECLARE_DYNAMIC(CAlias )

// Attributes
public:
   CAlias (LPTYPEINFO ptinfo = NULL);
   virtual ~CAlias ();

// Operations
public:

private: // methods

private: // members
   CTypeDesc *m_pTypeDescBase;  // CTypeDesc which describes the base type of this alias.

};



class CStruct  : public CTypeInfo
{
   DECLARE_DYNAMIC(CStruct)

// Attributes
public:
   CStruct(LPTYPEINFO ptinfo = NULL);
   virtual ~CStruct();

// Operations
public:
   CString GetCPPDefinition(BOOL bTagged = TRUE);
   CObList* GetElements(){ return &m_Elements;}

private: // methods
   LONG LoadElements();

private: // members
   LPTYPEINFO m_ptinfo;            // ITypeInfo* of struct typeinfo.
   CObList    m_Elements;          // Collection of members in struct.

};



class CUnion : public CTypeInfo
{
   DECLARE_DYNAMIC(CUnion)

// Attributes
public:
   CUnion(LPTYPEINFO ptinfo = NULL);
   virtual ~CUnion();

// Operations
public:
   CObList* GetElements(){ return &m_Elements;}
   CString GetCPPDefinition(BOOL bTagged = TRUE);

private: // methods
   LONG LoadElements();

private: // members
   LPTYPEINFO m_ptinfo;            // ITypeInfo* of struct typeinfo.
   CObList    m_Elements;          // Collection of members in struct.
};


class CFunction : public CTlbBaseProperties
{
   DECLARE_DYNAMIC(CFunction)

// Attributes
public:
   CFunction(LPTYPEINFO ptinfo, USHORT nIndex, CInterface * pInterface);
   CFunction(LPTYPEINFO ptinfo, USHORT nIndex, CDispinterface * pInterface);
   CFunction(LPTYPEINFO ptinfo, USHORT nIndex, CModule * pModule);
   virtual ~CFunction();

   friend CZGlobalOperation;
   friend CTypeLibrary;
// Operations
public:
   BOOL IsOptionalParameter( SHORT i);
   IsOptionalParameter( CString strParam );
   CString GetCPPDefinition();
   CString GetCPPDeclaration( BOOL bHeader = TRUE );
   CString GetClassName();
   CString GetInvokeKindString();
   CString GetPropertyMethodPrefix();

   MEMBERID    GetMemberID(){ return m_pfuncdesc->memid; }
   FUNCKIND    GetFunctionKind(){ return m_pfuncdesc->funckind; }
   INVOKEKIND  GetInvokeKind(){ return m_pfuncdesc->invkind; }
   CALLCONV    GetCallConv(){ return m_pfuncdesc->callconv;}
   SHORT       GetNumOfParams(){return m_pfuncdesc->cParams;}
   SHORT       GetNumOfOptionalParams(){return m_pfuncdesc->cParamsOpt;}
   SHORT       GetOffsetInVTable(){return m_pfuncdesc->oVft;}
   WORD        GetFunctionFlags(){return m_pfuncdesc->wFuncFlags;}
   CObList     *GetParameter(){ return &m_pdispParameters; }
   CTypeDesc   *GetType() { return m_pdispTypeDesc; }
protected:
   CString GetMethodCall();
   CObject *GetParentInterface(){ return m_pParentInterface; }
   CTypeDesc  *m_pdispTypeDesc;         // return type.
   CObList    m_pdispParameters;        // List of parameters of the current function.
   LPFUNCDESC m_pfuncdesc;              // FUNCDESC of function.
   LPTYPEINFO m_ptinfoFunction;         // TypeInfo of which this function is an element.

private: // methods

   LONG LoadParameter(); // load parameter for the specific function
   BOOL Initialize(LPTYPEINFO ptinfo, USHORT nIndex); // Initialize itself

private: // members
   CObject    *m_pParentInterface;      // can be type of CInterface or CDispinterface
                                        // and is used to retierve the class name while generating c++ code
};


class CTypeDesc : public CObject
{
   DECLARE_DYNAMIC(CTypeDesc)

// Attributes
public:
   CTypeDesc(LPTYPEINFO ptinfo, TYPEDESC FAR* ptypedesc);
   virtual ~CTypeDesc();

// Operations
public:
   CString GetVariantType();
   CString GetCPPDefinition();
   VARTYPE GetVartype () { return m_vartype; }
   CObject *GetDescribtion() { return m_pDescribtion; }


private: // methods

private: // members
    VARTYPE m_vartype;       // Type
    CObject *m_pDescribtion; // If m_vartype==VT_USERDEFINED, m_pDescribtion contains
                             // a CTypeInfo of user-defined type.,
                             // if m_vartype==VT_CARRAY, is a CArrayDesc
                             // if m_vartype==VT_PTR is a CTypeDesc of type pointed to.

};



class CArrayBound : public CObject
{
   DECLARE_DYNAMIC( CArrayBound )

// Attributes
public:
   CArrayBound ( ULONG cElements, LONG lLBound);
   virtual ~CArrayBound ();

// Operations
public:
   LONG  GetLowerBound(){ return m_lLowerBound; }
   ULONG GetElementCount(){ return m_ulElementCount; }
private: // methods

private: // members
   ULONG m_ulElementCount;
   LONG  m_lLowerBound;

};


class CArrayDesc : public CObject
{
   DECLARE_DYNAMIC(CArrayDesc )

// Attributes
public:
   CArrayDesc (LPTYPEINFO ptinfo, ARRAYDESC FAR* parraydesc);
   virtual ~CArrayDesc ();

// Operations
public:
   CString GetCPPDefinition();
   CTypeDesc*  GetTypeDescribtion(){ return m_pTypeDesc; }
   CObList  *  GetArrayBounds(){return &m_ArrayBounds; }
private: // methods

private: // members
   CTypeDesc   *m_pTypeDesc;
   CObList     m_ArrayBounds;

};



class CConstant: public CTypeInfo
{
   DECLARE_DYNAMIC(CConstant)

// Attributes
public:
   CConstant(LPTYPEINFO ptinfo, LPVARDESC pvardesc);
   virtual ~CConstant();

// Operations
public:
   CTypeDesc   *GetType() { return m_pTypeDesc; }
   MEMBERID    GetMemberID(){ return m_memid; }
   COleVariant GetValue(){ return m_vValue; }

private: // methods

private: // members
   CTypeDesc   *m_pTypeDesc;            // describes type of constant.
   MEMBERID    m_memid;                 // MEMBERID.
   COleVariant m_vValue;                // Constant value.
};

class CProperty : public CTlbBaseProperties
{
   DECLARE_DYNAMIC(CProperty)

// Attributes
public:
   CProperty(LPTYPEINFO ptinfo, LPVARDESC pvardesc, CDispinterface *pDispinterface = NULL );
   virtual ~CProperty();

// Operations
public:
   CString GetClassName();
   CString GetCPPDeclaration( );

   MEMBERID    GetMemberID(){ return m_memid; }
   CTypeDesc  *GetType () { return m_pdispTypeDesc; }
   CString GetCPPDefinition();

private: // methods

private: // members
   CDispinterface * m_pDispinterface;
   CTypeDesc  *m_pdispTypeDesc;         // Property type.
   MEMBERID m_memid;                    // MEMBERID of property.
};


class CParameter : public CTlbBaseProperties
{
   DECLARE_DYNAMIC(CParameter)

// Attributes
public:
   CParameter(LPTYPEINFO ptinfo, BSTR bstrName, TYPEDESC FAR* ptypedesc,
     IDLDESC FAR* pidldesc);
   virtual ~CParameter();

// Operations
public:
   CString GetVariantType();

   MEMBERID    GetMemberID(){ return m_memid; }
   CTypeDesc  *GetType () { return m_pdispTypeDesc; }
   USHORT      GetIDLFlags() { return m_usFlags; }


private: // methods

private: // members
   CTypeDesc  *m_pdispTypeDesc;         // Parameter type.
   MEMBERID m_memid;                    // MEMBERID of property.
   USHORT   m_usFlags;                  // specifies in/out/in-out/none
};

class CZGlobalOperation : public CObject
{
   DECLARE_DYNAMIC(CZGlobalOperation)

// Attributes
public:
   CZGlobalOperation() { }
   CZGlobalOperation(CFunction * f ) { SetFunction (f);}
   virtual ~CZGlobalOperation(){}

// Operations
public:
   CString GetCPPDefinition();
   void SetFunction(CFunction *f){ m_pFunction = f; }


private: // methods

private: // members
   CFunction * m_pFunction;
};


class CZGlobalProperty : public CObject
{
   DECLARE_DYNAMIC(CZGlobalProperty)

// Attributes
public:
   CZGlobalProperty() { }
   CZGlobalProperty(CProperty * f ) { SetProperty (f);}
   virtual ~CZGlobalProperty(){}

// Operations
public:
   CString GetCPPDefinition();
   void SetProperty(CProperty *f){ m_pProperty = f; }


private: // methods

private: // members
   CProperty * m_pProperty;
};



class CZGlobalDispatch : public CObject
{
   DECLARE_DYNAMIC(CZGlobalDispatch)

// Attributes
public:
   CZGlobalDispatch(CDispinterface * i = NULL) { SetInterface (i);}
   virtual ~CZGlobalDispatch();

// Operations
public:
   CObList m_lProperties;
   CObList m_lOperations;
   CString GetCPPDefinition();
   void SetInterface(CDispinterface  *i);


private: // methods

private: // members
   CDispinterface * m_pDispinterface;
};

class CZOperEntry : public CObject
{
   DECLARE_DYNAMIC(CZOperEntry)

// Attributes
public:
   CZOperEntry() {}
   virtual ~CZOperEntry(){}
   void SetEntry(CString strType, CString strName){
      m_csName = strName;
      m_csType = strType;
   }

   CString GetName(){ return m_csName; }
   CString GetType(){ return m_csType; }

private: // methods

private: // members
   CString m_csType;
   CString m_csName;
};

class CZOperation : public CZOperEntry
{
   DECLARE_DYNAMIC(CZOperation)

// Attributes
public:
   CZOperation() {}
   virtual ~CZOperation();

   void AddParameter( CString strType, CString strName ){
      CZOperEntry *e = new CZOperEntry();
      e->SetEntry(strType, strName);
      m_lParamList.AddTail( e );
   }
   POSITION GetFirstParameterPosition(){ return m_lParamList.GetHeadPosition(); }
   CZOperEntry *GetNextParameter( POSITION &pos ){
      return (CZOperEntry *)m_lParamList.GetNext( pos );
   }

private: // methods

private: // members
   CObList m_lParamList;
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(__ZEIDON_ACTIVEX_TLB_H__)
