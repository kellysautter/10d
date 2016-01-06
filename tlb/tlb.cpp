// TLB.CPP : implementation file
//

#include "stdafx.h"
#include "ttblbrws.h"
#include "TLB.H"

#include "tlbconst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CTlbBaseProperties, CObject)

CTlbBaseProperties::CTlbBaseProperties()
{
   m_bstrName           = NULL;
   m_bstrDocumentation  = NULL;
   m_ulHelpContext      = NULL;
   m_bstrHelpFile       = NULL;
}

CTlbBaseProperties::~CTlbBaseProperties()
{
   if (m_bstrName)
   {
      SysFreeString(m_bstrName);
   }

   if (m_bstrDocumentation)
   {
      SysFreeString(m_bstrDocumentation);
   }

   if (m_bstrHelpFile)
   {
      SysFreeString(m_bstrHelpFile);
   }
}

CString CTlbBaseProperties::GetName()
{
   CString s;

   s.Empty();
   if( m_bstrName )
   {
      s = m_bstrName;
   }
   return s;
}

CString CTlbBaseProperties::GetDocumentation()
{
   CString s;

   s.Empty();

   if( m_bstrDocumentation )
   {
      s = m_bstrDocumentation;
   }
   return s;
}

CString CTlbBaseProperties::GetDocumentationAsComment()
{
   int i = 0;
   int j = 0;
   CString strHelp;

   CString strComment = GetDocumentation();
   // if empty documentation there is nothing to do
   if ( strComment.IsEmpty() )
   {
      return strHelp;
   }

   // if documentation is longer than 100 characters
   // insert some CR for formating purpose no matter
   // if there is a word or whitespace -> break it
   strHelp += "/*";
   strHelp += CRLF;
   j = strComment.GetLength();
   if ( i > 100)
   {
      while( j > 0 )
      {
         strHelp+=strComment.Mid(i, 100);
         strHelp+=CRLF;
         i+=100;
         j-=100;
      }
   }
   else
   {
      strHelp += strComment;
   }
   strHelp += CRLF;
   strHelp += "*/";

   return strHelp;
}


CString CTlbBaseProperties::GetHelpfile()
{
   CString s;

   s.Empty();

   if( m_bstrHelpFile )
   {
      s = m_bstrHelpFile;
   }
   return s;
}





/////////////////////////////////////////////////////////////////////////////
// CTypeLibrary

IMPLEMENT_DYNAMIC( CTypeLibrary, CObject )

CTypeLibrary::CTypeLibrary()
{
   m_TypeLib = NULL;
   m_bstrName           = NULL;
   m_bstrDocumentation  = NULL;
   m_bstrHelpFile       = NULL;
   m_bAppend            = FALSE;
}

CTypeLibrary::~CTypeLibrary()
{
   DestroyContent();
}

void CTypeLibrary::DestroyContent()
{
   if ( m_TypeLib )
   {
      m_TypeLib->Release();
      // Set to NULL to prevent destructor from attempting to free again
      m_TypeLib = NULL;
   }
   // destroy all dispatch type information
   while( !m_pdispTypeInfos.IsEmpty() )
   {
      CObject *p = m_pdispTypeInfos.RemoveHead();
      delete p;
   }
   // destroy all Dispatches that are use for generating global ops.
   while( !m_lGlobalDispatch.IsEmpty() )
   {
      CObject *p = m_lGlobalDispatch.RemoveHead();
      delete p;
   }

   // destroy all entries that are needed to display ops. in editor
   while( !m_lGlobalOperations.IsEmpty() )
   {
      CObject *p = m_lGlobalOperations.RemoveHead();
      delete p;
   }
}

BOOL CTypeLibrary::LoadTypeLibrary(LPCSTR lpszLibraryName)
{
   USES_CONVERSION;
   HRESULT hr = NULL;
   TLIBATTR *lptlAttributes;
   BOOL bReturn = TRUE;
   int i=0, j=0;

   if ( m_TypeLib )
   {
      m_TypeLib->Release();
   }

   TRY
   {
      // extract name without extension
      m_csLibraryFileName = lpszLibraryName;
      j = m_csLibraryFileName.ReverseFind('.');
      m_csLibraryFileName = m_csLibraryFileName.Mid( 0, m_csLibraryFileName.GetLength() - j );

      hr = LoadTypeLibEx( T2OLE( lpszLibraryName ),
                  REGKIND_DEFAULT, &m_TypeLib );
      if( !SUCCEEDED( hr ) )
      {
         AfxThrowOleException(hr);
      }
      //retrieving Documentation about the Typelib -1 means documentation about
      //the lib itself
      hr = m_TypeLib->GetDocumentation( -1, &m_bstrName,
                                    &m_bstrDocumentation,
                                    &m_ulHelpContext,
                                    &m_bstrHelpFile );
      if( !SUCCEEDED( hr ) )
      {
         AfxThrowOleException(hr);
      }

      //retrieving language and version information
      hr = m_TypeLib->GetLibAttr(&lptlAttributes);
      if( !SUCCEEDED( hr ) )
      {
         AfxThrowOleException(hr);
      }
      m_guid = lptlAttributes->guid;
      m_lcid = lptlAttributes->lcid;
      m_wMajorVer = lptlAttributes->wMajorVerNum;
      m_wMinorVer = lptlAttributes->wMinorVerNum;
      m_TypeLib->ReleaseTLibAttr(lptlAttributes);
      m_TypeLib->AddRef();
      BrowseLibComplete();
   }
   CATCH( COleException, e )
   {
      e->ReportError();
      DestroyContent();
      bReturn  = FALSE;
   }
   END_CATCH

   return bReturn;
}


long CTypeLibrary::BrowseLibComplete()
{
   HRESULT hr;
   CTypeInfo      *pTypeInfo;
   CInterface     *pInterface;
   CDispinterface *pDispinterface;
   CModule        *pModule;
   CCoClass       *pCoClass;
   CEnum          *pEnum;
   CAlias         *pAlias;
   CStruct        *pStruct;
   CUnion         *pUnion;
   LPTYPEINFO ptinfo = NULL;
   unsigned int cTypeInfo;
   unsigned int n;
   TYPEKIND typekind;

   if ( m_pdispTypeInfos.IsEmpty())
   {
      TRY
      {
         cTypeInfo = m_TypeLib->GetTypeInfoCount();
         // Enumerate the typeinfos in this type library
         for (n=0; n<cTypeInfo; n++)
         {
            hr = m_TypeLib->GetTypeInfo(n, &ptinfo);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = m_TypeLib->GetTypeInfoType(n, &typekind);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }

            switch (typekind)
            {
               case TKIND_INTERFACE:
                  pInterface = new CInterface(ptinfo, n);
                  m_pdispTypeInfos.AddTail(pInterface);
                  break;

               case TKIND_DISPATCH:
                  pDispinterface = new CDispinterface(ptinfo);
                  m_pdispTypeInfos.AddTail(pDispinterface);
                  break;

               case TKIND_MODULE:
                  pModule = new CModule(ptinfo);
                  m_pdispTypeInfos.AddTail(pModule);
                  break;

               case TKIND_COCLASS:
                  pCoClass = new CCoClass(ptinfo);
                  m_pdispTypeInfos.AddTail(pCoClass);
                  break;

               case TKIND_ENUM:
                  pEnum = new CEnum(ptinfo);
                  m_pdispTypeInfos.AddTail(pEnum);
                  break;

               case TKIND_ALIAS:
                  pAlias = new CAlias(ptinfo);
                  m_pdispTypeInfos.AddTail(pAlias);
                  break;

               case TKIND_RECORD:
                  pStruct = new CStruct(ptinfo);
                  m_pdispTypeInfos.AddTail(pStruct);
                  break;

               case TKIND_UNION:
                  pUnion = new CUnion(ptinfo);
                  m_pdispTypeInfos.AddTail(pUnion);
                  break;

               default:
                  // Currently unsupported TKINDs
                  pTypeInfo = new CTypeInfo(ptinfo);
                  m_pdispTypeInfos.AddTail(pTypeInfo);
                  break;
            }
         }
         // Work is done release interface
         ptinfo->Release();
         ptinfo = NULL;
      }
      CATCH(COleException,e)
      {
         e->ReportError( );
      }
      END_CATCH
   }

   CDispinterface * i = GetEventInterface();

   POSITION pos = m_pdispTypeInfos.GetHeadPosition();
   while ( pos )
   {
      CObject *o = m_pdispTypeInfos.GetAt(pos);
      if (o->IsKindOf(RUNTIME_CLASS( CDispinterface )))
      {
         if( i != o ) // following stuff is not need to be done in case of an event interface
         {
            PopulateGlobalOperations( ( CDispinterface * ) o );
            m_lGlobalDispatch.AddTail(new CZGlobalDispatch ( ( CDispinterface * ) o ) );
         }
      }
      m_pdispTypeInfos.GetNext(pos);
   }
   return m_pdispTypeInfos.GetCount();
}


CString CTypeLibrary::GetGUID()
{
   LPOLESTR pszGUID  = NULL;

   StringFromCLSID(m_guid, &pszGUID);

   return pszGUID;
}




IMPLEMENT_DYNAMIC( CTypeInfo, CTlbBaseProperties )


CTypeInfo::CTypeInfo(LPTYPEINFO ptinfo) : CTlbBaseProperties(  )
{
   if ( ptinfo != NULL )
   {
      InitTypeInfo(ptinfo);
   }
}

CTypeInfo::~CTypeInfo()
{
}


//---------------------------------------------------------------------
//
//
// Funktion Name  : InitTypeInfo
//                  Retrives the attributes of the current typeinfo
//
//
// Parameter      : ptinfo, I,   Pointer to a typeinfo structure
//
// Return         : TRUE when retrieving was successfull
//                  otherwise FALSE
//
//---------------------------------------------------------------------
BOOL CTypeInfo::InitTypeInfo(LPTYPEINFO ptinfo)
{
   HRESULT hr = 0;
   LPTYPELIB ptlib = NULL;
   UINT nIndex = 0;
   LPTYPEATTR ptypeattr = NULL;
   BOOL bReturn = FALSE;

   TRY
   {
      // Get information from TYPEATTR
      hr = ptinfo->GetTypeAttr(&ptypeattr);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }
      m_typekind = ptypeattr->typekind;
      m_guid = ptypeattr->guid;
      m_wTypeFlags = ptypeattr->wTypeFlags;

      ptinfo->ReleaseTypeAttr(ptypeattr);

      hr = ptinfo->GetContainingTypeLib(&ptlib, &nIndex);
      if (!SUCCEEDED(hr))
      {
        //Supportes runtime typeinfo browsing. (Type info
        //obtained from IDispatch::GetTypeInfo does not have a containing
        //type library).
        if (GetScode(hr) == E_NOTIMPL)
           bReturn = TRUE;
        else
           AfxThrowOleException(hr);
      }
      else
      {
         hr = ptlib->GetDocumentation(nIndex, &m_bstrName, &m_bstrDocumentation,
                &m_ulHelpContext, &m_bstrHelpFile);
         if (!SUCCEEDED(hr))
              AfxThrowOleException(hr);
      }
      ptlib->Release();
   }
   CATCH( COleException,e )
   {
      e->ReportError( );
      bReturn = FALSE;
   }
   END_CATCH

   return bReturn;
}


CString CTypeInfo::GetGUID()
{
   LPOLESTR pszGUID  = NULL;

   StringFromCLSID(m_guid, &pszGUID);

   return pszGUID;
}




IMPLEMENT_DYNAMIC( CInterface, CTypeInfo)

CInterface::CInterface (LPTYPEINFO ptinfo, int nImplFlag)
            :CTypeInfo(ptinfo)
{
   m_pdispBaseInterface = NULL;
   m_ptinfo = NULL;
   m_nImplFlag = nImplFlag;
   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      QueryFunctions();
      QueryBaseInterface();
   }
}

CInterface::~CInterface ()
{
   while ( !m_pdispFunctions.IsEmpty() )
   {
      CFunction *o = (CFunction *)m_pdispFunctions.RemoveHead();
      delete o;
   }

   if( m_pdispBaseInterface != NULL )
   {
      delete m_pdispBaseInterface;
   }

   if( m_ptinfo != NULL )
   {
      m_ptinfo->Release();
   }
}

long CInterface::QueryFunctions()
{
   HRESULT hr = 0;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   if (m_pdispFunctions.IsEmpty())
   {
      TRY
      {
         // Create collection of functions in interface.
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
          AfxThrowOleException( hr );
         }
         for (n=0; n<ptypeattr->cFuncs; n++)
         {
            m_pdispFunctions.AddTail( new CFunction(m_ptinfo, n, this));
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      CATCH(COleException, e)
      {
         if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }
         if (ptypeattr)
         {
            m_ptinfo->ReleaseTypeAttr(ptypeattr);
         }
         while ( !m_pdispFunctions.IsEmpty() )
         {
            CObject *o = m_pdispFunctions.RemoveHead();
            delete o;
         }
      }
      END_CATCH
   }
   return m_pdispFunctions.GetCount();
}

BOOL CInterface::QueryBaseInterface()
{
   HRESULT hr = NULL;
   LPTYPEINFO ptinfoInterface = NULL;
   HREFTYPE hreftype;
   BOOL bReturn = TRUE;

   if(m_pdispBaseInterface == NULL)
   {
      TRY
      {
         hr = m_ptinfo->GetRefTypeOfImplType(0, &hreftype);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         hr = m_ptinfo->GetRefTypeInfo(hreftype, &ptinfoInterface);

         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
//       m_pdispBaseInterface = new CInterface(ptinfoInterface);

         ptinfoInterface->Release();
      }
      CATCH(COleException,e)
      {
         if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }
         if (ptinfoInterface)
         {
            ptinfoInterface->Release();
         }
         m_pdispBaseInterface = NULL;
         bReturn = FALSE;
      }
      END_CATCH
   }
   return bReturn;
}



IMPLEMENT_DYNAMIC( CDispinterface, CTypeInfo)

CDispinterface::CDispinterface (LPTYPEINFO ptinfo)
            :CTypeInfo(ptinfo)
{
   m_pInterface = NULL;
   m_ptinfo = NULL;

   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadInterface();
      LoadMethods();
      LoadProperties();
   }
}


CDispinterface::~CDispinterface ()
{
   if ( m_pInterface )
   {
      delete (CInterface *)m_pInterface;
   }

   while( !m_pdispMethods.IsEmpty() )
   {
      CFunction *o = (CFunction *)m_pdispMethods.RemoveHead();
      delete o;
   }

   while( !m_pdispProperties.IsEmpty() )
   {
      CProperty *o = (CProperty *)m_pdispProperties.RemoveHead();
      delete o;
   }

   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }
}

long CDispinterface::LoadProperties()
{
   HRESULT hr;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   USHORT n = 0;

   TRY
   {
      if ( m_pdispProperties.IsEmpty() )
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         // Enumerate properties and and store it in a list.
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_pdispProperties.AddTail(new CProperty(m_ptinfo, pvardesc, this));
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_ptinfo->ReleaseVarDesc(pvardesc);
            pvardesc = NULL;
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e )
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }

      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      if (pvardesc)
      {
         m_ptinfo->ReleaseVarDesc(pvardesc);
      }

      while(!m_pdispProperties.IsEmpty())
      {
         CObject *o = m_pdispProperties.RemoveHead();
         delete o;
      }
   }
   END_CATCH

   return m_pdispProperties.GetCount();
}


long CDispinterface::LoadMethods()
{
   long lReturn = 0;
   HRESULT hr = NULL;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   if ( m_pdispMethods.IsEmpty() )
   {
      TRY
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException( hr );
         }
         // Enumerate methods and return a collection of these.
         for (n=0; n<ptypeattr->cFuncs; n++)
         {
            m_pdispMethods.AddTail(new CFunction(m_ptinfo, n, this));
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      CATCH( COleException, e )
      {
         if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }
         if (ptypeattr)
         {
            m_ptinfo->ReleaseTypeAttr(ptypeattr);
         }
         while ( !m_pdispMethods.IsEmpty() )
         {
            CObject *o = m_pdispMethods.RemoveHead();
            delete o;
         }
      }
      END_CATCH
   }

   return m_pdispMethods.GetCount();
}


BOOL CDispinterface::LoadInterface()
{
   HRESULT hr = NULL;
   LPTYPEINFO ptinfoInterface = NULL;
   HREFTYPE hreftype;
   BOOL bReturn = TRUE;

   if(m_pInterface == NULL)
   {
      TRY
      {
         // Get interface part of dual interface
         hr = m_ptinfo->GetRefTypeOfImplType(0xFFFFFFFF, &hreftype);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException( hr );
         }
         hr = m_ptinfo->GetRefTypeInfo(hreftype, &ptinfoInterface);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException( hr );
         }
//         m_pInterface = new CInterface(ptinfoInterface);
      }
      CATCH( COleException, e )
      {
         if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
         {
            e->ReportError();
         }

         if ( m_pInterface )
         {
            delete m_pInterface;
            m_pInterface = NULL;
         }
         bReturn = FALSE;
      }
      END_CATCH
   }
   return bReturn;
}

void CDispinterface::Rename(CString strNewName)
{
   ::SysFreeString( m_bstrName );
   m_bstrName = strNewName.AllocSysString();
}



IMPLEMENT_DYNAMIC( CModule, CTypeInfo)

CModule::CModule(LPTYPEINFO ptinfo)
            :CTypeInfo(ptinfo)
{
   if(ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadFunctions();
   }
}



CModule::~CModule()
{
   if(m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while(!m_pFunctions.IsEmpty())
   {
      CObject *o = m_pFunctions.RemoveHead();
      delete o;
   }
}

LONG CModule::LoadFunctions()
{
   HRESULT hr;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   TRY
   {
      if ( m_pFunctions.IsEmpty() )
      {
         // Create collection of functions of the interface.
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         for (n=0; n<ptypeattr->cFuncs; n++)
         {
            m_pFunctions.AddTail(new CFunction(m_ptinfo, n, this));
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      while(!m_pFunctions.IsEmpty())
      {
         CObject *o = m_pFunctions.RemoveHead();
         delete o;
      }

   }
   END_CATCH

   return m_pFunctions.GetCount();
}



IMPLEMENT_DYNAMIC( CCoClass, CTypeInfo)

CCoClass::CCoClass(LPTYPEINFO ptinfo, int nImplFlag )
            :CTypeInfo(ptinfo)
{
   m_ptinfo = NULL;
   m_nImplFlag = nImplFlag;

   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadInterfaces();
   }
}



CCoClass::~CCoClass()
{

   while (!m_Interfaces.IsEmpty())
   {
      CObject *o = m_Interfaces.RemoveHead();
      delete o;
   }

   if(m_ptinfo)
   {
      m_ptinfo->Release();
   }
}


LONG CCoClass::LoadInterfaces()
{
   HRESULT hr = NULL;
   HREFTYPE hreftype;
   LPTYPEATTR ptypeattr = NULL;
   LPTYPEINFO ptinfoInterface = NULL;
   LPTYPELIB ptlib = NULL;
   UINT nIndex = 0;
   USHORT n = 0;
   TYPEKIND typekind = TKIND_INTERFACE;
   int nImplFlag = 0;


   TRY
   {
      if ( m_Interfaces.IsEmpty() )
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }

         // Enumerate interfaces/dispinterfaces in coclass and return a collection of these.
         for (n=0; n<ptypeattr->cImplTypes; n++)
         {
            hr = m_ptinfo->GetImplTypeFlags( n, &nImplFlag );
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = m_ptinfo->GetRefTypeOfImplType(n, &hreftype);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = m_ptinfo->GetRefTypeInfo(hreftype, &ptinfoInterface);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = ptinfoInterface->GetContainingTypeLib(&ptlib, &nIndex);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            hr = ptlib->GetTypeInfoType(nIndex, &typekind);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            ptlib->Release();
            ptlib = NULL;

            switch (typekind)
            {
            case TKIND_INTERFACE:
               {
               CInterface *i = new CInterface(ptinfoInterface, m_nImplFlag);
               m_Interfaces.AddHead(i);
               break;
               }

            case TKIND_DISPATCH:
               m_Interfaces.AddHead(new CCoClass(ptinfoInterface, nImplFlag));
               break;
            }

         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH( COleException, e )
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      if (ptlib)
      {
         ptlib->Release();
      }

      if (ptinfoInterface)
      {
         ptinfoInterface->Release();
      }
   }
   END_CATCH

   return m_Interfaces.GetCount();
}

BOOL CCoClass::IsEventSource( )
{
   if ( m_nImplFlag & IMPLTYPEFLAG_FDEFAULT &&
        m_nImplFlag & IMPLTYPEFLAG_FSOURCE )
      return TRUE;
   else
      return FALSE;
}


BOOL CCoClass::IsDefault( )
{
   if ( IsEventSource() )
      return FALSE;
   if ( m_nImplFlag & IMPLTYPEFLAG_FDEFAULT )
      return TRUE;
   else
      return FALSE;
}

CString  CCoClass::GetDefaultDispatchInterfaceName()
{
   CInterface *i = NULL;
   BOOL bFound = FALSE;
   POSITION pos = m_Interfaces.GetHeadPosition( );
   CObject *o = NULL;
   CString strInterfaceName;

   if ( IsDefault() )
   {
      strInterfaceName = GetName();
   }

   // iterate all the CInterfaces and check whether they are
   // the default interface
   while( pos && strInterfaceName.IsEmpty() )
   {
      o = m_Interfaces.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(CCoClass)))
      {
         strInterfaceName = ((CCoClass *)o)->GetDefaultDispatchInterfaceName();
      }
      m_Interfaces.GetNext( pos );
   }

   return strInterfaceName;
}

CString CCoClass::GetEventInterfaceName()
{
   CInterface *i = NULL;
   BOOL bFound = FALSE;
   POSITION pos = m_Interfaces.GetHeadPosition( );
   CObject *o = NULL;
   CString strInterfaceName;

   if ( IsEventSource() )
   {
      strInterfaceName = GetName();
   }

   // iterate all the CInterfaces and check whether they are
   // the default interface
   while( pos && strInterfaceName.IsEmpty() )
   {
      o = m_Interfaces.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(CCoClass)))
      {
         strInterfaceName = ((CCoClass *)o)->GetEventInterfaceName();
      }
      m_Interfaces.GetNext( pos );
   }

   return strInterfaceName;
}



IMPLEMENT_DYNAMIC( CEnum, CTypeInfo)

CEnum::CEnum (LPTYPEINFO ptinfo)
            :CTypeInfo(ptinfo)
{
   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadEnumElements();
   }
}



CEnum::~CEnum ()
{
   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while(!m_pElements.IsEmpty())
   {
      CObject *o = m_pElements.RemoveHead();
      delete o;
   }
}

LONG CEnum::LoadEnumElements()
{
   HRESULT hr = NULL;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   unsigned short n;

   TRY
   {
      if (m_pElements.IsEmpty())
      {
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         // Enumerate enum constants and return a collection of these.
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_pElements.AddTail(new CConstant(m_ptinfo, pvardesc));
            m_ptinfo->ReleaseVarDesc(pvardesc);
            pvardesc = NULL;
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }

      if (pvardesc)
      {
         m_ptinfo->ReleaseVarDesc(pvardesc);
      }
   }
   END_CATCH

   return m_pElements.GetCount();
}


CString CEnum::GetCPPDeclaration(BOOL bTagged)
{
   POSITION pos = m_pElements.GetHeadPosition();
   LONG i = m_pElements.GetCount();
   LONG n = 0;
   CConstant *c = NULL;
   COleVariant vValue;
   VARIANT  v;
   CString strName = GetName();

   CString strOut = "typedef enum ";
   if ( bTagged )
   {
      strOut += "tag";
      strOut += strName;
   }
   strOut+=" {";
   strOut+=CRLF;
   strOut+=TAB;

   while(pos)
   {
      c = (CConstant*) m_pElements.GetAt(pos);
      // generate unique element names by extending
      // enum list element names with enum name separated by an underscore
      strOut += strName;
      strOut += "_";
      strOut += c->GetName();
      strOut += " = ";
      vValue = c->GetValue();

      vValue.ChangeType(VT_BSTR);
      v = vValue.Detach();
      strOut += v.bstrVal;

      if ( n < i-1 )
      {
         strOut+= ",";
         strOut+=CRLF;
         strOut+=TAB;
      }
      else
      {
         strOut+=CRLF;
      }
      m_pElements.GetNext(pos);
      n++;
   }

   strOut += "} ";
   strOut += strName;
   strOut += ";";

   return strOut;
}



IMPLEMENT_DYNAMIC( CAlias, CTypeInfo)

CAlias::CAlias (LPTYPEINFO ptinfo)
            :CTypeInfo(ptinfo)
{
   HRESULT hr;
   LPTYPEATTR ptypeattr = NULL;

   // Get base type of this alias.
   hr = ptinfo->GetTypeAttr(&ptypeattr);
   if (SUCCEEDED( hr ))
   {
      m_pTypeDescBase = new CTypeDesc(ptinfo, &ptypeattr->tdescAlias);
   }

   ptinfo->ReleaseTypeAttr(ptypeattr);
}



CAlias::~CAlias ()
{
   if ( m_pTypeDescBase != NULL )
   {
      delete m_pTypeDescBase;
   }
}



IMPLEMENT_DYNAMIC( CStruct, CTypeInfo)

CStruct::CStruct(LPTYPEINFO ptinfo)
            :CTypeInfo(ptinfo)
{
   m_ptinfo = NULL;
   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadElements();
   }
}



CStruct::~CStruct()
{
   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while(!m_Elements.IsEmpty())
   {
      CObject *o = m_Elements.RemoveHead();
      delete o;
   }
}


LONG CStruct::LoadElements()
{
   HRESULT hr = NULL;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   USHORT n = 0;

   TRY
   {
      if ( m_Elements.IsEmpty() )
      {
         // Create a list of structure members
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_Elements.AddTail(new CProperty(m_ptinfo, pvardesc));
            m_ptinfo->ReleaseVarDesc(pvardesc);
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      while(!m_Elements.IsEmpty())
      {
         CObject *o = m_Elements.RemoveHead();
         delete o;
      }
   }
   END_CATCH

   return m_Elements.GetCount();
}


CString CStruct::GetCPPDefinition(BOOL bTagged)
{
   CString strOut;
   POSITION pos = m_Elements.GetHeadPosition();
   LONG i = m_Elements.GetCount();
   LONG n = 0;
   CProperty *c = NULL;

   strOut = "struct ";
   if( bTagged )
   {
      strOut += "tag";
   }

   strOut += GetName();

   strOut+=CRLF;
   strOut +="{";
   strOut+=CRLF;
   strOut+=TAB;

   // step through all elements and look for type and name
   while(pos)
   {
      c = (CProperty*) m_Elements.GetAt(pos);
      strOut += c->GetCPPDefinition();
      if ( n < i-1 )
      {
         strOut += (",") ;
         strOut+=CRLF;
         strOut+=TAB;
      }
      else
      {
         strOut+=CRLF;
      }
      m_Elements.GetNext(pos);
      n++;
   }

   strOut += "}";
   strOut += GetName();
   strOut += ";";

   return strOut;
}




IMPLEMENT_DYNAMIC( CUnion, CTypeInfo)

CUnion::CUnion(LPTYPEINFO ptinfo)
            :CTypeInfo(ptinfo)
{
   m_ptinfo = NULL;
   if (ptinfo)
   {
      m_ptinfo = ptinfo;
      m_ptinfo->AddRef();
      LoadElements();
   }
}


CUnion::~CUnion()
{
   if (m_ptinfo)
   {
      m_ptinfo->Release();
   }

   while(!m_Elements.IsEmpty())
   {
      CObject *o = m_Elements.RemoveHead();
      delete o;
   }
}


LONG CUnion::LoadElements()
{
   HRESULT hr = NULL;
   LPVARDESC pvardesc = NULL;
   LPTYPEATTR ptypeattr = NULL;
   USHORT n = 0;

   TRY
   {
      if ( m_Elements.IsEmpty() )
      {
         // Create a list of structure members
         hr = m_ptinfo->GetTypeAttr(&ptypeattr);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         for (n=0; n<ptypeattr->cVars; n++)
         {
            hr = m_ptinfo->GetVarDesc(n, &pvardesc);
            if (!SUCCEEDED(hr))
            {
               AfxThrowOleException(hr);
            }
            m_Elements.AddTail(new CProperty(m_ptinfo, pvardesc));
            m_ptinfo->ReleaseVarDesc(pvardesc);
         }
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
   }
   CATCH(COleException, e)
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (ptypeattr)
      {
         m_ptinfo->ReleaseTypeAttr(ptypeattr);
      }
      while(!m_Elements.IsEmpty())
      {
         CObject *o = m_Elements.RemoveHead();
         delete o;
      }
   }
   END_CATCH

   return m_Elements.GetCount();
}


CString CUnion::GetCPPDefinition(BOOL bTagged)
{
   CString strOut;
   POSITION pos = m_Elements.GetHeadPosition();
   LONG i = m_Elements.GetCount();
   LONG n = 0;
   CProperty *c = NULL;

   strOut = "struct ";
   if( bTagged )
   {
      strOut += "tag";
   }

   strOut += GetName();

   strOut+=CRLF;
   strOut +="{";
   strOut+=CRLF;
   strOut+=TAB;

   // step through all elements and look for type and name
   while(pos)
   {
      c = (CProperty*) m_Elements.GetAt(pos);
      strOut += c->GetCPPDefinition();
      if ( n < i-1 )
      {
         strOut += (",") ;
         strOut+=CRLF;
         strOut+=TAB;
      }
      else
      {
         strOut+=CRLF;
      }
      m_Elements.GetNext(pos);
      n++;
   }

   strOut += "}";
   strOut += GetName();
   strOut += ";";

   return strOut;
}



IMPLEMENT_DYNAMIC(CFunction, CObject)

CFunction::CFunction(LPTYPEINFO ptinfo, USHORT nIndex, CInterface *pInterface)
{
   m_pParentInterface = pInterface;
   Initialize(ptinfo, nIndex);
}


CFunction::CFunction(LPTYPEINFO ptinfo, USHORT nIndex, CDispinterface *pInterface)
{
   m_pParentInterface = pInterface;
   Initialize(ptinfo, nIndex);
}

CFunction::CFunction(LPTYPEINFO ptinfo, USHORT nIndex, CModule *pModule)
{
   m_pParentInterface = pModule;
   Initialize(ptinfo, nIndex);
}

BOOL CFunction::Initialize(LPTYPEINFO ptinfo, USHORT nIndex)
{
   HRESULT hr = NULL;
   BOOL bReturn = TRUE;

   TRY
   {
      hr = ptinfo->GetFuncDesc(nIndex, &m_pfuncdesc);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }
      // Function return type.
      m_pdispTypeDesc = new CTypeDesc(ptinfo, &m_pfuncdesc->elemdescFunc.tdesc);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }

      hr = ptinfo->GetDocumentation(m_pfuncdesc->memid, &m_bstrName, &m_bstrDocumentation,
               &m_ulHelpContext, &m_bstrHelpFile);
      if (!SUCCEEDED(hr))
      {
         AfxThrowOleException(hr);
      }
      m_ptinfoFunction = ptinfo;
      m_ptinfoFunction->AddRef();
      LoadParameter();

   }
   CATCH( COleException, e)
   {
      if(e->m_sc != TYPE_E_ELEMENTNOTFOUND)
      {
         e->ReportError();
      }
      if (m_pfuncdesc)
      {
         ptinfo->ReleaseFuncDesc(m_pfuncdesc);
      }

      if (m_ptinfoFunction)
      {
         m_ptinfoFunction->Release();
      }

      // Set to NULL to prevent destructor from attempting to free again
      m_ptinfoFunction = NULL;
      m_pfuncdesc = NULL;
      bReturn = FALSE;
   }
   END_CATCH
   return bReturn;
}


LONG CFunction::LoadParameter()
{
   HRESULT hr = NULL;
   BSTR rgbstrNames[DISPATCH_MAX_PARAM];
   UINT cNames;
   USHORT n = 0;
   TYPEDESC  *pTypedesc;
   LPIDLDESC  pIDLdesc;

   for (UINT ui = 0 ; ui < DISPATCH_MAX_PARAM ; ui++)
        rgbstrNames[ui] = NULL ;


   TRY
   {  // Create a list of parameters.
      if (m_pdispParameters.IsEmpty())
      {
         // Get the name of the function and its parameters
         hr = m_ptinfoFunction->GetNames(m_pfuncdesc->memid, rgbstrNames,
                           DISPATCH_MAX_PARAM, &cNames);
         if (!SUCCEEDED(hr))
         {
            AfxThrowOleException(hr);
         }
         SysFreeString(rgbstrNames[0]);

         // a "property put" has no name, so name int "Value"
         if (m_pfuncdesc->invkind & INVOKE_PROPERTYPUT)
            rgbstrNames[n+1] = SysAllocString(OLESTR("Value"));
         // Add parameters to the list of parameter
         for (n=0; n<m_pfuncdesc->cParams; n++)
         {
            pTypedesc = &m_pfuncdesc->lprgelemdescParam[n].tdesc;
            pIDLdesc  = &m_pfuncdesc->lprgelemdescParam[n].idldesc;
            m_pdispParameters.AddTail( new CParameter(m_ptinfoFunction,
                                               rgbstrNames[n+1],
                                               pTypedesc,
                                               pIDLdesc));

            SysFreeString(rgbstrNames[n+1]);
         }
      }
   }
   CATCH( COleException, e )
   {
      if (rgbstrNames)
      {  // free only those that have not already been free'd. continue with current n;
         for (; n<m_pfuncdesc->cParams; n++)
         {
            SysFreeString(rgbstrNames[n+1]);
         }
      }
   }
   END_CATCH
   return m_pdispParameters.GetCount();

}

CFunction::~CFunction()
{
   while ( !m_pdispParameters.IsEmpty() )
   {
      CObject *p = m_pdispParameters.RemoveHead();
      delete p;
   }

   if (m_pfuncdesc && m_ptinfoFunction)
   {
      m_ptinfoFunction->ReleaseFuncDesc(m_pfuncdesc);
   }

   if (m_ptinfoFunction)
   {
      m_ptinfoFunction->Release();
   }

   if (m_pdispTypeDesc)
   {
      delete m_pdispTypeDesc;
   }
}



CString CFunction::GetCPPDeclaration(BOOL bHeader)
{
   CString strOut, csHelp;
   CObList *lpParameter = GetParameter();
   POSITION pos = NULL;
   INT i,j;

   strOut.Empty();
   i = j = 0;

   // there is nothig to do if one of those conditions are true
   if (m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }

   // write returntype
   strOut += m_pdispTypeDesc->GetCPPDefinition();
   strOut += TAB;

   // write classname followed by ::
   if( bHeader == FALSE )
   {
      strOut += GetClassName();
      strOut += "::";
   }

   // write methodname
   strOut += GetPropertyMethodPrefix() + GetName();

   strOut += "( ";

   // write parameter if there is any
   if (lpParameter != NULL )
   {
      pos = lpParameter->GetHeadPosition();
      i   = lpParameter->GetCount();
   }
   while ( pos )
   {
      CParameter *p =(CParameter *)lpParameter->GetAt(pos);
      csHelp = p->GetType()->GetCPPDefinition();
      csHelp += " ";

      CString strName = p->GetName();

      // if parameter has no name generate one that look like "param0"
      if ( strName.GetLength() < 1 )
      {
         strName.Format(lpszPARAMTEMPLATE, j);
      }

      // If current parameter is optional
      // add const and address operator
      if ( IsOptionalParameter(j) )
      {
         strName = "& " + strName;
         csHelp = "const " + csHelp;
      }

      strOut = strOut + csHelp + strName;
      if (j < i-1)
      {
         strOut += ", ";
      }
      j++;
      lpParameter->GetNext(pos);
   }
   strOut += " )";

   if ( bHeader )
   {
      strOut+=";";
   }

   return strOut;

}


CString CFunction::GetMethodCall()
{
   CString strOut, csHelp;
   CObList *lpParameter = GetParameter();
   POSITION pos = NULL;
   INT i,j;

   strOut.Empty();
   i = j = 0;

   // there is nothig to do if one of those conditions are true
   if (m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }

   // write methodname
   strOut += GetPropertyMethodPrefix() + GetName();

   strOut += "( ";

   // write parameter if there is any
   if (lpParameter != NULL )
   {
      pos = lpParameter->GetHeadPosition();
      i   = lpParameter->GetCount();
   }
   while ( pos )
   {
      CParameter *p =(CParameter *)lpParameter->GetAt(pos);
      CString strName = p->GetName();

      // if parameter has no name generate one that look like "param0"
      if ( strName.GetLength() < 1 )
         strName.Format(lpszPARAMTEMPLATE, j);

      strOut = strOut + strName;
      if (j < i-1)
         strOut += ", ";

      j++;
      lpParameter->GetNext(pos);
   }
   strOut += " );";

   return strOut;
}


CString CFunction::GetCPPDefinition()
{
   CString strOut;            //return string
   CString csHelp;           //just for help
   CString strParamNames;     //lists all parameters by name, separated by comma
   CString strParamTypes;     //lists all parameter types separated by blank
   CString strReturnType;     //Holds type of return
   CString strReturnName;     //Name of the Variable the holds the return value
   CString strMethodType;     //used to convert information about method type into string
   CString strDISPID;         //String that holds the dispatch ID of the current function
   CString strParmList;
   POSITION pos = NULL;       //list iterater
   CObList *pParams = NULL;   //points to the list of parameters
   BOOL bIsVoid = FALSE;

   strOut.Empty();
   strParamNames.Empty();

   // there is nothig to do if one of those conditions are true
   if (m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }


   // 1. write function declaration like this
   //    CString _DATX32::GetFooterFText(short whatFooter)
   strOut += GetCPPDeclaration(FALSE);
   strOut += CRLF;
   strOut += "{";
   strOut += CRLF;

   // 2. delcare a variable for return if it's not void
   strReturnType = m_pdispTypeDesc->GetCPPDefinition();
   if ( strReturnType != "void" )
   {
      strOut += TAB;
      strOut += strReturnType + " result;";
      strOut += CRLF;
      strReturnName = "(void*)&result";
      strReturnType = VTtoDispatchType( m_pdispTypeDesc->GetVartype() );
   }
   else
   {
      strReturnType = "VT_EMPTY";
      strReturnName = "NULL";
      bIsVoid = TRUE;
   }

   // 3.declare the array of parametertypes we are going to pass ( if there are any )
   //   static BYTE parms[] = VTS_I2;
   //   and list the name of the parameters we use to call InvokeHelper
   pParams = GetParameter();
   int i = pParams->GetCount();
   int j = 0;

   strParamTypes.Empty();

   if ( i > 0 )
   {
      strParamTypes += TAB;
      strParamTypes += "static BYTE parms[] =";
      strParamTypes += CRLF;
      strParamTypes += TAB;
      strParamTypes += TAB;

      strParmList = "parms";

      pos = pParams->GetHeadPosition();
      while ( pos )
      {
         CParameter *p = (CParameter *)pParams->GetAt( pos );
         strParamTypes += p->GetVariantType();
         csHelp = p->GetName();

         // if parameter has no name generate one that look like "param0"
         if ( csHelp.GetLength() < 1 )
         {
            csHelp.Format(lpszPARAMTEMPLATE, j);
         }

         // If current parameter is optional
         // add address operator
         if ( IsOptionalParameter(j) )
         {
            csHelp = "&" + csHelp;
         }

         strParamNames += csHelp;
         if ( j < i-1 )
         {
            strParamTypes += " ";          // each type is separated by a blank
            strParamNames += ", ";  // each parameter by comma
         }
         pParams->GetNext(pos);
         j++;
      }
      strParamTypes += ";";
   }
   else
   {
      strParamNames.Empty(); //Nothing, no code, not even a NULL-Value completely nothing
      strParmList = "NULL";
      strParamTypes.Empty();
   }

   strOut += strParamTypes;

   // 4. setup call InvokeHelper
   strDISPID.Empty();
   strDISPID = PredefinedDispID( GetMemberID() );
   if ( strDISPID.IsEmpty() )
   {
      strDISPID.Format("0x%x", GetMemberID());
   }

   strMethodType = GetInvokeKindString();

   strOut += CRLF;
   strOut += TAB;

   // setup the  string so that each parameter is separated by comma
   strMethodType = ", " + strMethodType;
   strReturnType = ", " + strReturnType;
   strReturnName = ", " + strReturnName;
   strParmList   = ", " + strParmList;

   // if there are no paramaternames the InvokeHelper function
   // has only 5 parameters so we need no comma between the list
   // of parameter names and the list name (parms) of parameter types
   if ( !strParamNames.IsEmpty() )
   {
      strParamNames = ",\r\n      " + strParamNames;
   }

   csHelp.Format( lpszINVOKE_HELPER, strDISPID, strMethodType, strReturnType, strReturnName, strParmList, strParamNames );

   strOut += csHelp;
   // Is function type of void?
   if ( bIsVoid == FALSE )
   {
      strOut += CRLF;
      strOut += TAB;
      strOut += "return result;";
   }

   strOut += CRLF;
   strOut += "}";
   return strOut;
}


CString CFunction::GetInvokeKindString()
{
   CString strOut;

   switch ( GetInvokeKind() )
   {
      case INVOKE_FUNC :
         strOut = _T("DISPATCH_METHOD");
         break;
      case INVOKE_PROPERTYGET :
         strOut = _T("DISPATCH_PROPERTYGET");
         break;
      case INVOKE_PROPERTYPUT :
         strOut = _T("DISPATCH_PROPERTYPUT");
         break;
      case INVOKE_PROPERTYPUTREF :
         strOut = _T("DISPATCH_PROPERTYPUTREF");
         break;
      default: // should not occur because it is enumeration but it is still Microsoft
         strOut.Format(_T("Unknown invoke type:   %d"), GetInvokeKind());
         break;
   }
   return strOut;
}


//
// returns get or set if it is a method
// for setting or getting a property
//
CString CFunction::GetPropertyMethodPrefix()
{
   CString strOut;

   switch ( GetInvokeKind() )
   {
      case INVOKE_PROPERTYGET :
         strOut = _T("Get");
         break;
      case INVOKE_PROPERTYPUT :
      case INVOKE_PROPERTYPUTREF :
         strOut = _T("Set");
         break;
      default:
         strOut = _T("");
         break;
   }
   return strOut;
}


CString CFunction::GetClassName()
{
   CString strOut;
   CInterface *pInterface = NULL;
   CDispinterface *pDispinterface = NULL;
   CModule *pModule = NULL;

   CObject *o = GetParentInterface();

   if ( o == NULL )
   {
      strOut.Empty();
   }
   else
   {  // savely cast to the specific object
      if( o->IsKindOf(RUNTIME_CLASS( CInterface ) ) )
      {
         pInterface = (CInterface *) o;
         strOut = pInterface->GetName();
      }
      else
      if( o->IsKindOf(RUNTIME_CLASS( CDispinterface ) ) )
      {
         pDispinterface = (CDispinterface *) o;
         strOut = pDispinterface->GetName();
      }
      else
      if( o->IsKindOf(RUNTIME_CLASS( CModule ) ) )
      {
         pModule = (CModule *) o;
         strOut = pModule->GetName();
      }

   }

   return strOut;

}


IMPLEMENT_DYNAMIC(CTypeDesc, CObject)

CTypeDesc::CTypeDesc(LPTYPEINFO ptinfo, TYPEDESC FAR* ptypedesc)
{
   HRESULT hr = NULL;
   LPTYPEINFO ptinfoUserDefined;

   m_pDescribtion = NULL;
   m_vartype = ptypedesc->vt;
   switch ( m_vartype )
   {
   case VT_USERDEFINED :
   {
      hr = ptinfo->GetRefTypeInfo(ptypedesc->hreftype, &ptinfoUserDefined);
      if ( SUCCEEDED( hr ) )
         m_pDescribtion = new CTypeInfo(ptinfoUserDefined);
      break;
   }
   case VT_CARRAY :
   {
      m_pDescribtion = new CArrayDesc(ptinfo, ptypedesc->lpadesc );
      break;
   }
   case VT_PTR :
   {
      m_pDescribtion = new CTypeDesc(ptinfo, ptypedesc->lptdesc );
      break;
   }
   default:
      break;
   }
}

CTypeDesc::~CTypeDesc()
{
   if( m_pDescribtion )
   {
      delete m_pDescribtion;
   }
}



CString CTypeDesc::GetCPPDefinition()
{
   CString str ;

   if ((GetVartype() & 0x0FFF) == VT_SAFEARRAY)
   {
      str = "SAFEARRAY( " ;
   }

   if ( GetVartype() == VT_PTR )
   {
      // m_pDescribtion points to a CTypeDesc that specifies the thing pointed to
      CTypeDesc *o = (CTypeDesc *)m_pDescribtion;

      str += o->GetCPPDefinition();
      str += "*" ;
   }
   else if ( (GetVartype() & 0x0FFF) == VT_CARRAY)
   {
      // m_pDescribtion  points to an CArrayDesc
      CArrayDesc *o = (CArrayDesc *)m_pDescribtion;
      str = o->GetCPPDefinition();
   }
   else if (GetVartype() == VT_USERDEFINED)
   {
      CTypeInfo *o = (CTypeInfo *)m_pDescribtion;
      str = o->GetName();
   }
   else
      str = VTtoCType( GetVartype() );

   if ((GetVartype() & 0x0FFF) == VT_SAFEARRAY)
   {
      str += " )" ;
   }

   return str;
}


CString CTypeDesc::GetVariantType()
{
   CString str;

   if ( GetVartype() == VT_PTR )
   {
      // m_pDescribtion points to a CTypeDesc that specifies the type pointed to
      CTypeDesc *o = (CTypeDesc *)m_pDescribtion;
      str += "VTS_P";
      str += VTtoVariantType( o->GetVartype() );
   }
   else
   {
      str += "VTS_";
      str += VTtoVariantType( GetVartype() );
   }

   return str;
}






IMPLEMENT_DYNAMIC(CArrayBound, CObject)

CArrayBound::CArrayBound(ULONG cElements, LONG lBounds)
{
   m_ulElementCount  = cElements;
   m_lLowerBound     = lBounds;
}


CArrayBound::~CArrayBound()
{
}


IMPLEMENT_DYNAMIC(CArrayDesc, CObject)

CArrayDesc::CArrayDesc(LPTYPEINFO ptinfo, ARRAYDESC FAR* parraydesc)
{
    HRESULT hr = NULL;
    INT n = 0;
    m_pTypeDesc = NULL;

    if(ptinfo == NULL)
       return;

    m_pTypeDesc = new CTypeDesc(ptinfo, &parraydesc->tdescElem);

    // Create a list of array bounds. One array bound per dimension.
    for (n=0; n<parraydesc->cDims; n++)
    {
        m_ArrayBounds.AddTail(new CArrayBound(parraydesc->rgbounds[n].cElements,
                                              parraydesc->rgbounds[n].lLbound));
    }
}


CArrayDesc::~CArrayDesc()
{
   if ( m_pTypeDesc == NULL )
   {
      delete m_pTypeDesc;
   }

   while ( !m_ArrayBounds.IsEmpty() )
   {
      CObject *o = m_ArrayBounds.RemoveHead();
      delete o;
   }
}

CString CArrayDesc::GetCPPDefinition()
{
   CString str;
   POSITION pos = m_ArrayBounds.GetHeadPosition();
   UINT n = 0;

   // Allocate Dimensions [ ][ ]....
   CString strTemp ;

   while( pos )
   {
      CArrayBound * a = (CArrayBound *)m_ArrayBounds.GetAt(pos);
      strTemp.Format( _T("[%d]"), a->GetElementCount()) ;
      str += strTemp ;
      n++;
      m_ArrayBounds.GetNext(pos);
   }

   return str;
}



IMPLEMENT_DYNAMIC(CConstant, CObject)

CConstant::CConstant(LPTYPEINFO ptinfo, LPVARDESC pvardesc)
{
   HRESULT hr;
   unsigned int cNames;

   m_memid = pvardesc->memid;
   ptinfo->GetNames(pvardesc->memid, &m_bstrName, 1, &cNames);

   // Type of constant.
   m_pTypeDesc = new CTypeDesc(ptinfo, &pvardesc->elemdescVar.tdesc);

   // Constant value.
   if (pvardesc->varkind == VAR_CONST)
   {
      m_vValue = *pvardesc->lpvarValue;

      hr = ptinfo->GetDocumentation(pvardesc->memid, NULL, &m_bstrDocumentation,
          &m_ulHelpContext, &m_bstrHelpFile);
      if (!SUCCEEDED(hr))
      {
         if (m_bstrName)
         {
            SysFreeString(m_bstrName);
         }

         if (m_bstrDocumentation)
         {
            SysFreeString(m_bstrDocumentation);
         }

         if (m_bstrHelpFile)
         {
            SysFreeString(m_bstrHelpFile);
         }

         // Set to NULL to prevent destructor from attempting to free again
         m_bstrName = NULL;
         m_bstrDocumentation = NULL;
         m_bstrHelpFile = NULL;
      }
   }
}

CConstant::~CConstant()
{
   if( m_pTypeDesc )
   {
      delete m_pTypeDesc;
   }

}



IMPLEMENT_DYNAMIC(CProperty, CTlbBaseProperties)

CProperty::CProperty(LPTYPEINFO ptinfo, LPVARDESC pvardesc, CDispinterface * pDispinterface)
{
   HRESULT hr = NULL;
   unsigned int cNames;
   m_pdispTypeDesc = NULL;
   m_pDispinterface = pDispinterface;

   m_memid = pvardesc->memid;
   ptinfo->GetNames(m_memid, &m_bstrName, 1, &cNames);

   // Property type
   m_pdispTypeDesc = new CTypeDesc(ptinfo, &pvardesc->elemdescVar.tdesc);

   hr = ptinfo->GetDocumentation(pvardesc->memid, NULL, &m_bstrDocumentation,
          &m_ulHelpContext, &m_bstrHelpFile);

   if (!SUCCEEDED(hr))
   {
      if (m_bstrName)
      {
         SysFreeString(m_bstrName);
      }

      if (m_bstrDocumentation)
      {
         SysFreeString(m_bstrDocumentation);
      }

      if (m_bstrHelpFile)
      {
         SysFreeString(m_bstrHelpFile);
      }
      // Set to NULL to prevent destructor from attempting to free again
      m_bstrName = NULL;
      m_bstrDocumentation = NULL;
      m_bstrHelpFile = NULL;
   }

}


CProperty::~CProperty()
{
   if ( m_pdispTypeDesc != NULL )
   {
      delete m_pdispTypeDesc;
   }

}


CString CProperty::GetCPPDefinition()
{
   CString strOut;
   CString strGet;
   CString strSet;


   // 1. setup definition for GetMethod
   strGet.Format(lpszGET_PROP_CPP,GetType()->GetCPPDefinition,
                                  GetClassName(),
                                  GetName(),
                                  GetType()->GetCPPDefinition,
                                  GetMemberID(),
                                  VTtoDispatchType( GetType()->GetVartype() ));

   // 2. setup definition for SetMethod
   strGet.Format(lpszSET_PROP_CPP,GetClassName(),
                                  GetName(),
                                  GetType()->GetCPPDefinition,
                                  GetMemberID(),
                                  VTtoDispatchType( GetType()->GetVartype() ));

   strOut = CRLF + strGet + CRLF + strSet + CRLF;

   return strOut;
}


CString CProperty::GetCPPDeclaration()
{
   CString strOut;
   CString strType;
   CString strName;

   strType.Empty();
   strOut.Empty();
   CTypeDesc  *t = GetType ();

   if( t != NULL )
   {
      // this should be the return type
      strType = t->GetCPPDefinition();

      // First the Get... method
      strOut += TAB;
      strOut += strType;
      strOut += TAB;

      strOut += "Get";
      strName += GetName();
      strOut += strName;
      strOut += "();";
      strOut += CRLF;

      // now the Set... method
      strOut += TAB;
      strOut += "void";  // Setting any property is type of void
      strOut += TAB;

      strOut += "Set";
      strOut += GetName();
      strOut += "( ";
      strOut += strType;
      strOut = strOut + " " + strName;
      strOut += " );";

      strOut += CRLF;
   }

   return strOut;
}

CString CProperty::GetClassName()
{
   CString strOut;

   strOut.Empty();

   if ( m_pDispinterface )
   {
      strOut = m_pDispinterface->GetName();
   }
   return strOut;
}


IMPLEMENT_DYNAMIC(CParameter, CTlbBaseProperties)

CParameter::CParameter(LPTYPEINFO ptinfo, BSTR bstrName,
                       TYPEDESC FAR* ptypedesc, IDLDESC FAR* pidldesc)
{
   m_pdispTypeDesc = NULL;

   m_bstrName = SysAllocString(bstrName);

   // Parameter type.
   m_pdispTypeDesc = new CTypeDesc(ptinfo, ptypedesc);
   m_usFlags = pidldesc->wIDLFlags;
}


CParameter::~CParameter()
{
   if( m_pdispTypeDesc )
   {
      delete m_pdispTypeDesc;
   }
}


CString CParameter::GetVariantType()
{
   CString strOut;
   if ( m_pdispTypeDesc )
   {
      strOut =  m_pdispTypeDesc->GetVariantType();
   }
   else
   {
      strOut.Empty();
   }

   return strOut;
}


BOOL CTypeLibrary::GenerateCode(LPCSTR lpszFileName,
                                LPCSTR strScriptFunctionName,
                                LPCSTR lpszNewDefaultName,
                                BOOL   bAppend)
{
   BOOL bReturn = FALSE;
   CString csHelp = lpszFileName;
   m_csNewDefaultClassName = lpszNewDefaultName;
   CDispinterface *pI = GetDefaultDispatch();

   m_bAppend = bAppend;

   if ( pI && lpszNewDefaultName )
      pI->Rename(m_csNewDefaultClassName);

   int j = 0;

   if ( csHelp.IsEmpty() )
   {
      csHelp = m_csLibraryFileName;
   }

   // extract name without extension
   j = csHelp.ReverseFind('.');
   if ( j >= 0 )
      csHelp = csHelp.Mid( 0, j );

   bReturn = CreateHeader( csHelp + ".h" );
   if ( bReturn )
   {
      bReturn = CreateCPP( csHelp + ".cpp");
   }

   // now create the C that for dll generation
   if ( bReturn )
   {
      CreateC( csHelp + ".cpp", strScriptFunctionName);
   }


   return bReturn;
}


BOOL CTypeLibrary::CreateCPP(LPCSTR lpszFileName)
{
   BOOL bReturn = TRUE;
   POSITION pos = NULL;
   CInterface     *pInterface       = NULL;
   CDispinterface *pDispinterface   = NULL;
   CModule        *pModule          = NULL;
   CCoClass       *pCoClass         = NULL;
   CString        csHelp;
   UINT           nOpenFlags        = 0;

   CFile *f = NULL;

   TRY
   {
      if ( m_bAppend )
         nOpenFlags = CFile::modeCreate|
                       CFile::modeNoTruncate|
                       CFile::modeReadWrite;
      else
         nOpenFlags = CFile::modeCreate|CFile::modeReadWrite;


      f = new CFile(lpszFileName, nOpenFlags );
      f->SeekToEnd();

      m_csCPPFileName = lpszFileName;

      WriteFileHeader(f);

      csHelp.Format( lpszDEFAULT_INCLUDE, m_csHeaderFileName );
      f->Write(csHelp,csHelp.GetLength());


      pos = m_pdispTypeInfos.GetHeadPosition();
      while (pos)
      {
         CTypeInfo * t = (CTypeInfo*) m_pdispTypeInfos.GetAt(pos);
         csHelp = CRLF;
         csHelp += CRLF;
         csHelp += t->GetDocumentationAsComment();
         csHelp += CRLF;
         switch ( t->GetTypeKind() )
         {
            case TKIND_INTERFACE:
               pInterface = (CInterface* )t;
               break;

            case TKIND_DISPATCH:
               pDispinterface = ( CDispinterface *) t;
               csHelp += pDispinterface->GetCPPDefinition();
               break;

            case TKIND_MODULE:
               pModule = ( CModule *) t;
               break;

            case TKIND_COCLASS:
               pCoClass = ( CCoClass *) t;
               break;

            default:
               // Currently unsupported TKINDs
               break;
            }
         f->Write(csHelp,csHelp.GetLength());
         m_pdispTypeInfos.GetNext(pos);
      }
      f->Close();
      delete f;
   }
   CATCH( CFileException, e )
   {
      e->ReportError();
      bReturn = FALSE;
      if ( f )
         delete f;
   }
   END_CATCH
   return bReturn;
}

BOOL CTypeLibrary::CreateHeader(LPCSTR lpszFileName)
{

   BOOL bReturn = TRUE;
   POSITION pos = NULL;
   CInterface     *pInterface       = NULL;
   CDispinterface *pDispinterface   = NULL;
   CModule        *pModule          = NULL;
   CCoClass       *pCoClass         = NULL;
   CEnum          *pEnum            = NULL;
   CAlias         *pAlias           = NULL;
   CStruct        *pStruct          = NULL;
   CUnion         *pUnion           = NULL;
   CString        csHelp;
   UINT           nOpenFlags = 0;

   CFile *f = NULL;

   TRY
   {
      if ( m_bAppend )
         nOpenFlags = CFile::modeCreate|
                       CFile::modeNoTruncate|
                       CFile::modeReadWrite;
      else
         nOpenFlags = CFile::modeCreate|CFile::modeReadWrite;


      f = new CFile(lpszFileName, nOpenFlags );
      f->SeekToEnd();

      m_csHeaderFileName = lpszFileName;

      WriteFileHeader(f);

      csHelp = lpszDEFAULT_MFC_INCLUDE;
      f->Write(csHelp,csHelp.GetLength());

      csHelp = lpszZeidonDefinitions;
      f->Write(csHelp,csHelp.GetLength());

      csHelp = lpszMFC_EXTENSION;
      f->Write(csHelp,csHelp.GetLength());


      pos = m_pdispTypeInfos.GetHeadPosition();
      while (pos)
      {
         CTypeInfo * t = (CTypeInfo*) m_pdispTypeInfos.GetAt(pos);
         csHelp = CRLF;
         csHelp += CRLF;
         csHelp += t->GetDocumentationAsComment();
         csHelp += CRLF;
         switch ( t->GetTypeKind() )
         {
            case TKIND_INTERFACE:
               pInterface = (CInterface* )t;
               break;

            case TKIND_DISPATCH:
               pDispinterface = ( CDispinterface *) t;
               csHelp += pDispinterface->GetCPPDeclaration();
               break;

            case TKIND_MODULE:
               pModule = ( CModule *) t;
               break;

            case TKIND_COCLASS:
               pCoClass = ( CCoClass *) t;
               break;

            case TKIND_ENUM:
               pEnum = ( CEnum *) t;
               csHelp += pEnum->GetCPPDeclaration();
               break;

            case TKIND_ALIAS:
               pAlias = ( CAlias *) t;
               break;

            case TKIND_RECORD:
               pStruct = ( CStruct *) t;
               csHelp += pStruct->GetCPPDefinition();
               break;

            case TKIND_UNION:
               pUnion = ( CUnion *) t;
               csHelp += pUnion->GetCPPDefinition();
               break;

            default:
               // Currently unsupported TKINDs
               csHelp += "unsupported TYPEKIND contact software vendor\n";
               csHelp += "Name is:";
               csHelp += TAB;
               csHelp += t->GetName();
               break;
            }
         f->Write(csHelp,csHelp.GetLength());
         m_pdispTypeInfos.GetNext(pos);
      }
      f->Close();
      delete f;
   }
   CATCH( CFileException, e )
   {
      e->ReportError();
      bReturn = FALSE;
      if ( f )
         delete f;
   }
   END_CATCH
   return bReturn;
}



BOOL CTypeLibrary::WriteFileHeader(CFile *f, BOOL bCPPHeader)
{
   CTime ct = CTime::GetCurrentTime();
   CString csHelp;

   BOOL bReturn = TRUE;

   // writing some default stuff
   csHelp = "/**************************************************************";
   csHelp += CRLF;
   csHelp += TAB;
   csHelp += "Generated wrapper for ActiveX library named: ";
   csHelp +=  GetName();
   csHelp += CRLF;
   csHelp += TAB;
   csHelp += "Generated at:";
   csHelp += TAB;
   csHelp += ct.Format( "%A, %B %d, %Y" );
   csHelp += CRLF;
   csHelp += "**************************************************************/";
   csHelp += CRLF;
   csHelp += CRLF;
   csHelp += CRLF;
   csHelp += CRLF;

   TRY
   {
      f->Write(csHelp, csHelp.GetLength());
   }
   CATCH(CFileException, e)
   {
      e->ReportError();
      bReturn = FALSE;
   }
   END_CATCH

   return bReturn;
}

void CTypeLibrary::PopulateGlobalOperations(CDispinterface *i)
{
   CString strName;
   CString strType;
   POSITION pos;
   CObList *l;
   CZOperation* o = NULL;

   l=i->GetProperties();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      CProperty *f = (CProperty *)l->GetAt( pos );

      // populating GetMethod
      strType = f->GetType()->GetCPPDefinition();        // returntype
      strName = f->GetClassName() + "_Get" + f->GetName();  // qualified name of global op
      o = new CZOperation();
      o->SetEntry( strType, strName );
      m_lGlobalOperations.AddHead( o );

      // population set method
      strType = "void";                                     // returntype alway void with set
      strName = f->GetClassName() + "_Set" + f->GetName();  // qualified name of global op
      o = new CZOperation();
      o->SetEntry( strType, strName );
      o->AddParameter(f->GetType()->GetCPPDefinition(), f->GetName());
      m_lGlobalOperations.AddHead( o );
      l->GetNext( pos );
   }

   l=i->GetMethods();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      CFunction *f = (CFunction *)l->GetAt( pos );
      // populating Method
   // there is nothing to do if one of those conditions are true
      if (!(f->GetFunctionFlags() & FUNCFLAG_FRESTRICTED ) && // Exist but should not be available to macro languages
          !(f->GetFunctionFlags() & FUNCFLAG_FHIDDEN )&&     // The function should not be displayed to the user, although it exists and is bindable.
          f->m_pfuncdesc != NULL)                          // Initialization error
      {
         strType = f->GetType()->GetCPPDefinition();;        // returntype
         strName = f->GetClassName() + "_"+ f->GetPropertyMethodPrefix() + f->GetName();  // qualified name of global op
         o = new CZOperation();
         o->SetEntry( strType, strName );
         m_lGlobalOperations.AddHead( o );
         POSITION posParam = f->GetParameter()->GetHeadPosition();
         while ( posParam )
         {
            CParameter *p = (CParameter *)f->GetParameter()->GetAt( posParam );
            o->AddParameter(p->GetType()->GetCPPDefinition(), p->GetName());
            f->GetParameter()->GetNext( posParam );
         }
      }
      l->GetNext( pos );
   }
}


/************************************************************************
** Use this method to create a string that contains the  declaration of
** dispatchinterface as a C++ Class for a header
**
*/
CString CDispinterface::GetCPPDeclaration()
{
   POSITION pos = NULL;
   CObList *l = NULL;
   CString strName = GetName();
   CString strOut;

   strOut.Format(lpszCPP_CLASS_DECLARATION,strName,lpszDISPATCH_DRIVER_BASE_CLASS);
   strOut+=lpszCPP_TOKEN_PUBLIC;

   // writing the constuctors there are three of them
   // an empty one
   strOut+=TAB;
   strOut+=strName;
   strOut+="() {}";
   strOut+=CRLF;

   // one that take an dispatchpointer as argument
   strOut+=TAB;
   strOut+=strName;
   strOut+="(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}";
   strOut+=CRLF;

   // and a copy constructor
   strOut+=TAB;
   strOut+=strName;
   strOut+="(const ";
   strOut+=strName;
   strOut+="& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}";
   strOut+=CRLF;
   strOut+=CRLF;


   // write the declaration for the property Set and Get Methods
   // therefore process the list of properties
   strOut+="public: // methods for setting and getting poperties";
   strOut+= CRLF;
   strOut+= TAB;
   l=GetProperties();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      CProperty *f = (CProperty *)l->GetAt( pos );
      strOut+= CRLF;
      strOut+=f->GetCPPDeclaration();
      l->GetNext( pos );
   }

   strOut+=CRLF;
   strOut+=CRLF;
   strOut+="public: // methods of the current Dispinterface";
   strOut+= CRLF;
   strOut+= TAB;
   l=GetMethods();
   pos=l->GetHeadPosition();
   while ( pos )
   {
      CFunction *f = (CFunction *)l->GetAt( pos );
      CString csHelp = f->GetCPPDeclaration();
      if ( !csHelp.IsEmpty() )
      {
         strOut+= CRLF;
         strOut+= TAB;
         strOut+= csHelp;
      }
      l->GetNext( pos );
   }

   //----------------------------------------------------------------
   // ok closing brace an out of here
   strOut+=CRLF;
   strOut+="};";
   return strOut;
}

CString CDispinterface::GetCPPDefinition()
{
   CString strOut;
   CObList *m = NULL;
   POSITION pos = NULL;

   // Write all the properties
   m = GetProperties();

   pos = m->GetHeadPosition();

   if ( pos )
   {
      strOut =  "/*------------------------------------------------------   \r\n";
      strOut += "**   Setting and getting properties of dispatch interface   \r\n";
      strOut += "**   ";
      strOut += GetName();
      strOut += CRLF;
      strOut += "**------------------------------------------------------*/\r\n";
   }

   while ( pos )
   {
      CProperty *f = ( CProperty * )m->GetAt( pos );
      strOut += CRLF;
      strOut += f->GetDocumentationAsComment();
      strOut += CRLF;
      strOut += f->GetCPPDefinition();
      m->GetNext( pos );
   }

   // write all methods
   m = GetMethods();

   pos = m->GetHeadPosition();

   if ( pos )
   {
      strOut =  "/*------------------------------------------------------   \r\n";
      strOut += "**   Wrapper for invoking methods of dispatch interface     \r\n";
      strOut += "**   ";
      strOut += GetName();
      strOut += CRLF;
      strOut += "**------------------------------------------------------*/\r\n";
   }
   while ( pos )
   {
      CFunction *f = ( CFunction * )m->GetAt( pos );
      strOut += CRLF;
      strOut += f->GetDocumentationAsComment();
      strOut += CRLF;
      strOut += f->GetCPPDefinition();
      m->GetNext( pos );
   }
   return strOut;
}


//
// returns whether the function parameter indexed by parameter i
// is optional or not
//
BOOL CFunction::IsOptionalParameter(SHORT i)
{
   BOOL bReturn = FALSE;

   if ((GetNumOfOptionalParams() == -1 && i == ( GetNumOfOptionalParams() - 1)) ||
       (i >= (GetNumOfParams() - GetNumOfOptionalParams())))
   {
      bReturn = TRUE;
   }
   else
   {
      bReturn = FALSE;
   }
   return bReturn;
}

//
// returns whether the function named by parameter strParam
// is optional or not
//
BOOL CFunction::IsOptionalParameter( CString strParam )
{
   POSITION pos = NULL;
   BOOL bFound = FALSE;
   BOOL bReturn = FALSE;
   CObList *l = GetParameter();

   int i = 0;

   pos = l->GetHeadPosition();
   while ( pos && bFound == FALSE )
   {
      CParameter *p = (CParameter *)l->GetAt( pos );
      if ( p->GetName() == strParam )
      {
         bFound = TRUE;
      }
      if ( !bFound )
      {
         i++;
      }
      else
      {
         bReturn = IsOptionalParameter( i );
      }
   }
   return bReturn;
}




BOOL CTypeLibrary::CreateC( LPCSTR lpszFileName, CString strScriptFunctionName )
{
   BOOL bReturn = TRUE;
   CString        csHelp, strClassName = "";
   CString strDllName = lpszFileName;
   CDispinterface *pI = GetDefaultDispatch();


   // retrieve the default class name "name of the default interface"
   if ( !m_csNewDefaultClassName.IsEmpty() )
      strClassName = m_csNewDefaultClassName;
   if (pI && m_csNewDefaultClassName.IsEmpty())
      strClassName = pI->GetName();

   // extract name without extension
   int j = strDllName.ReverseFind('.');
   strDllName = strDllName.Mid( 0, j );
   j = strDllName.ReverseFind('\\');
   strDllName = strDllName.Mid( j + 1, strDllName.GetLength() - j );


   CFile *f = NULL;

   TRY
   {
      f = new CFile(lpszFileName, CFile::modeCreate|
                                  CFile::modeNoTruncate|
                                  CFile::modeReadWrite);
      f->SeekToEnd();

//      WriteFileHeader(f);

/*
      csHelp = lpszDEFAULT_MFC_INCLUDE;
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp.Format( lpszDEFAULT_INCLUDE, m_csHeaderFileName );
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp = lpszZeidonDefinitions;
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp = lpszMFC_EXTENSION;
      f->Write( csHelp,csHelp.GetLength( ) );
*/

      csHelp = lpszDllMain;
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp.Format( lpszZeidonInitRoutine, strDllName );
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp.Format( lpszZeidonScriptInit, strScriptFunctionName, strClassName );
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp.Format( lpszZeidonScriptMapping );
      f->Write( csHelp,csHelp.GetLength( ) );

      csHelp.Format( lpszZeidonScriptMisc, strScriptFunctionName, strClassName );
      f->Write( csHelp,csHelp.GetLength( ) );

      POSITION pos = m_lGlobalDispatch.GetHeadPosition( );
      while ( pos )
      {
         csHelp = CRLF;
         CZGlobalDispatch *p = (CZGlobalDispatch *)m_lGlobalDispatch.GetAt(pos);
         csHelp += p->GetCPPDefinition();
         f->Write(csHelp,csHelp.GetLength());
         m_lGlobalDispatch.GetNext(pos);
      }


      f->Close();
      delete f;
   }
   CATCH( CFileException, e )
   {
      e->ReportError();
      bReturn = FALSE;
      if ( f )
         delete f;
   }
   END_CATCH
   return bReturn;

}


// return the default dispatch interface
CDispinterface * CTypeLibrary::GetDefaultDispatch()
{
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   CObject *o = NULL;
   CString strInterfaceName;

   // iterate all the CCoClass and check whether they are
   // the default interface
   while( pos && strInterfaceName.IsEmpty() )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(CCoClass)))
      {
         strInterfaceName = ((CCoClass *)o)->GetDefaultDispatchInterfaceName();
      }
      m_pdispTypeInfos.GetNext( pos );
   }

   if ( !strInterfaceName.IsEmpty() )
      return GetDispatchOfName( strInterfaceName );

   return NULL;
}

// return the event interface
CDispinterface *CTypeLibrary::GetEventInterface()
{
   POSITION pos = m_pdispTypeInfos.GetHeadPosition( );
   CObject *o = NULL;
   CString strInterfaceName;

   // iterate all the CCoClass and check whether they are
   // the default interface
   while( pos && strInterfaceName.IsEmpty() )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(CCoClass)))
      {
         strInterfaceName = ((CCoClass *)o)->GetEventInterfaceName();
      }
      m_pdispTypeInfos.GetNext( pos );
   }

   if ( !strInterfaceName.IsEmpty() )
      return GetDispatchOfName( strInterfaceName );

   return NULL;
}



CDispinterface * CTypeLibrary::GetDispatchOfName( CString strInterfaceName )
{
   // Get the name of that interface and look for it in the dispinterfaces
   POSITION pos = m_pdispTypeInfos.GetHeadPosition(  );
   CDispinterface *pD = NULL;
   BOOL bFound = FALSE;
   CObject *o = NULL;
   CString strName;

   while( pos && !bFound )
   {
      o = m_pdispTypeInfos.GetAt( pos );
      if ( o->IsKindOf(RUNTIME_CLASS(CDispinterface)))
      {
         CString strName = ((CDispinterface *)o)->GetName();
         if ( strInterfaceName == strName )
         {
            bFound = TRUE;
         }
      }
      if( !bFound )
      {
         m_pdispTypeInfos.GetNext( pos );
      }
      else
      {
         pD = ((CDispinterface *)o);
      }
   }
   return pD;
}



IMPLEMENT_DYNAMIC(CZGlobalOperation, CObject)

CString CZGlobalOperation::GetCPPDefinition()
{
   CString strOut, csHelp;
   CObList *lpParameter = m_pFunction->GetParameter();
   POSITION pos = NULL;
   INT i,j;

   LPCSTR lpszMethodProlog = "\r\n\
{\r\n\
   %s * pInstance  = (%s*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );\r\n\
   if ( pInstance == NULL );\r\n\
      return %s;\r\n\
   %s pInstance->%s\r\n\
   return %s;\r\n\
}\r\n";


   strOut.Empty();
   i = j = 0;

   // there is nothing to do if one of those conditions are true
   if (m_pFunction->m_pfuncdesc->wFuncFlags & FUNCFLAG_FRESTRICTED || // Exist but should not be available to macro languages
       m_pFunction->m_pfuncdesc->wFuncFlags & FUNCFLAG_FHIDDEN ||     // The function should not be displayed to the user, although it exists and is bindable.
       m_pFunction->m_pdispTypeDesc == NULL)                          // Initialization error
   {
      strOut.Empty();
      return strOut;
   }

   // write returntype
   strOut = m_pFunction->m_pdispTypeDesc->GetCPPDefinition();
   strOut.TrimLeft();
   strOut += " ";

   // write classname followed by _ to have unique names
   strOut += m_pFunction->GetClassName();
   strOut += "_";

   // write methodname
   strOut += m_pFunction->GetPropertyMethodPrefix() + m_pFunction->GetName();

   strOut += "( zVIEW vSubtask, zCPCHAR  cpcCtrlTag ";

   // write parameter if there is any
   if (lpParameter != NULL )
   {
      pos = lpParameter->GetHeadPosition();
      i   = lpParameter->GetCount();
   }
   while ( pos )
   {
      CParameter *p =(CParameter *)lpParameter->GetAt(pos);
      csHelp += ", ";
      csHelp += p->GetType()->GetCPPDefinition();

      CString strName = p->GetName();

      // if parameter has no name generate one that look like "param0"
      if ( strName.GetLength() < 1 )
      {
         strName.Format(lpszPARAMTEMPLATE, j);
      }

      // If current parameter is optional
      // add const and address operator
      if ( m_pFunction->IsOptionalParameter(j) )
      {
         strName = "& " + strName;
         csHelp = "const " + csHelp;
      }

      strOut = strOut + csHelp + strName;
      lpParameter->GetNext(pos);
      j++;
   }
   strOut += " )";

   CString strReturnType = m_pFunction->m_pdispTypeDesc->GetCPPDefinition();
   CString strReturnType2= strReturnType;
   CString strReturnVariable = " ReturnValue = ";
   if ( strReturnType == "void" )
   {
      strReturnType.Empty();
      strReturnType2.Empty();
      strReturnVariable.Empty();
   }
   else
   {
      strReturnType = "( " + strReturnType + ") (0)";
      strReturnType2 = strReturnType2 + strReturnVariable;
   }

// format method
   csHelp.Format( lpszMethodProlog, m_pFunction->GetClassName(),
                                     m_pFunction->GetClassName(),
                                     strReturnType,
                                     strReturnType2,
                                     m_pFunction->GetMethodCall(),
                                     strReturnVariable);
   strOut+=csHelp;
   strOut+=CRLF;
   return strOut;
}


IMPLEMENT_DYNAMIC(CZGlobalDispatch, CObject)

CZGlobalDispatch::~CZGlobalDispatch()
{
   while (!m_lOperations.IsEmpty())
   {
      CObject * o = m_lOperations.RemoveHead();
      delete o;
   }
}
void CZGlobalDispatch::SetInterface( CDispinterface *i )
{
   m_pDispinterface = i;

   if ( i )
   {
      CObList *l = i->GetMethods();
      for (POSITION pos = l->GetHeadPosition(); pos; l->GetNext(pos))
      {
         m_lOperations.AddHead( new CZGlobalOperation( (CFunction *)l->GetAt(pos)));
      }

      l = i->GetProperties();
      for (pos = l->GetHeadPosition(); pos; l->GetNext(pos))
      {
         m_lProperties.AddHead( new CZGlobalProperty( (CProperty *)l->GetAt(pos)));
      }
   }
}


CString CZGlobalDispatch::GetCPPDefinition()
{
   CString strOut;
   POSITION pos;
   CObList *l;

   l=&m_lProperties;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      CZGlobalProperty *f = (CZGlobalProperty *)l->GetAt( pos );
      strOut+= CRLF;
      strOut+=f->GetCPPDefinition();
      l->GetNext( pos );
   }

   l=&m_lOperations;
   pos=l->GetHeadPosition();
   while ( pos )
   {
      CZGlobalOperation *f = (CZGlobalOperation *)l->GetAt( pos );
      CString csHelp = f->GetCPPDefinition();
      if ( !csHelp.IsEmpty() )
      {
         strOut+= CRLF;
         strOut+= TAB;
         strOut+= csHelp;
      }
      l->GetNext( pos );
   }


   return strOut;


}

IMPLEMENT_DYNAMIC(CZGlobalProperty, CObject)

CString CZGlobalProperty::GetCPPDefinition()
{
   CString strOut;
   CString strType;
   CString strName;
   CString csHelp;
   LPCSTR lpszGetProlog = "\r\n\
{\r\n\
   %s * pInstance  = (%s*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );\r\n\
   if ( pInstance == NULL );\r\n\
      return (%s)(0);\r\n\
   return pInstance->Get%s;\r\n\
}\r\n";
   LPCSTR lpszSetProlog = "\r\n\
{\r\n\
   %s * pInstance  = (%s*)GetActiveX_WrapperInstance( vSubtask, cpcCtrlTag );\r\n\
   if ( pInstance == NULL );\r\n\
      return;\r\n\
   pInstance->Set%s( propVal );\r\n\
}\r\n";


   strType.Empty();
   strOut.Empty();
   CTypeDesc  *t = m_pProperty->GetType ();

   if( t != NULL )
   {
      // this should be the return type
      strType = t->GetCPPDefinition();

      // First the Get... method
      strOut += strType;
      strOut += TAB;

      strOut += m_pProperty->GetClassName();
      strOut += "_";
      strOut += "Get";
      strName += m_pProperty->GetName();
      strOut += strName;
      strOut += "( zVIEW vSubtask, zCPCHAR  cpcCtrlTag )";
      csHelp.Format( lpszGetProlog,
                      m_pProperty->GetClassName(),
                      m_pProperty->GetClassName(),
                      strType,
                      m_pProperty->GetName());
      strOut += csHelp;

      // now the Set... method
      strOut += "void";  // Setting any property is type of void
      strOut += TAB;

      strOut += m_pProperty->GetClassName();
      strOut += "_";
      strOut += "Set";
      strOut += m_pProperty->GetName();
      strOut += "( zVIEW vSubtask, zCPCHAR  cpcCtrlTag, ";
      strOut += strType;
      strOut += " propVal )";
      csHelp.Format( lpszSetProlog,
                      m_pProperty->GetClassName(),
                      m_pProperty->GetClassName(),
                      m_pProperty->GetName());
      strOut += csHelp;


      strOut += CRLF;
   }

   return strOut;
}

IMPLEMENT_DYNAMIC(CZOperEntry, CObject)

IMPLEMENT_DYNAMIC(CZOperation, CZOperEntry)

CZOperation::~CZOperation()
{
   TRACE2("Operation : %s %s\n", GetType(), GetName());
   while ( !m_lParamList.IsEmpty() )
   {
      CZOperEntry *o = (CZOperEntry *)m_lParamList.RemoveHead( );
      TRACE2("\t\t%s %s\n", o->GetType(), o->GetName());
      delete o;
   }
}

