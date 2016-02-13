

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Wed Feb 10 15:49:27 2016
 */
/* Compiler settings for tbedit.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __tbedit_h_h__
#define __tbedit_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DTBEdit_FWD_DEFINED__
#define ___DTBEdit_FWD_DEFINED__
typedef interface _DTBEdit _DTBEdit;

#endif 	/* ___DTBEdit_FWD_DEFINED__ */


#ifndef ___DTBEditEvents_FWD_DEFINED__
#define ___DTBEditEvents_FWD_DEFINED__
typedef interface _DTBEditEvents _DTBEditEvents;

#endif 	/* ___DTBEditEvents_FWD_DEFINED__ */


#ifndef __TBEdit_FWD_DEFINED__
#define __TBEdit_FWD_DEFINED__

#ifdef __cplusplus
typedef class TBEdit TBEdit;
#else
typedef struct TBEdit TBEdit;
#endif /* __cplusplus */

#endif 	/* __TBEdit_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __TBEDITLib_LIBRARY_DEFINED__
#define __TBEDITLib_LIBRARY_DEFINED__

/* library TBEDITLib */
/* [control][helpstring][helpfile][version][uuid] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion

DEFINE_GUID(LIBID_TBEDITLib,0xDEA843D2,0xDAA2,0x11D1,0x95,0xB2,0x00,0x60,0x97,0x3B,0x3B,0x55);

#ifndef ___DTBEdit_DISPINTERFACE_DEFINED__
#define ___DTBEdit_DISPINTERFACE_DEFINED__

/* dispinterface _DTBEdit */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DTBEdit,0xDEA843D3,0xDAA2,0x11D1,0x95,0xB2,0x00,0x60,0x97,0x3B,0x3B,0x55);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DEA843D3-DAA2-11D1-95B2-0060973B3B55")
    _DTBEdit : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DTBEditVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DTBEdit * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DTBEdit * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DTBEdit * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DTBEdit * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DTBEdit * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DTBEdit * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DTBEdit * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DTBEditVtbl;

    interface _DTBEdit
    {
        CONST_VTBL struct _DTBEditVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DTBEdit_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DTBEdit_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DTBEdit_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DTBEdit_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DTBEdit_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DTBEdit_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DTBEdit_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DTBEdit_DISPINTERFACE_DEFINED__ */


#ifndef ___DTBEditEvents_DISPINTERFACE_DEFINED__
#define ___DTBEditEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DTBEditEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DTBEditEvents,0xDEA843D4,0xDAA2,0x11D1,0x95,0xB2,0x00,0x60,0x97,0x3B,0x3B,0x55);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DEA843D4-DAA2-11D1-95B2-0060973B3B55")
    _DTBEditEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DTBEditEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DTBEditEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DTBEditEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DTBEditEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DTBEditEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DTBEditEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DTBEditEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DTBEditEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DTBEditEventsVtbl;

    interface _DTBEditEvents
    {
        CONST_VTBL struct _DTBEditEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DTBEditEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DTBEditEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DTBEditEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DTBEditEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DTBEditEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DTBEditEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DTBEditEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DTBEditEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_TBEdit,0xDEA843D5,0xDAA2,0x11D1,0x95,0xB2,0x00,0x60,0x97,0x3B,0x3B,0x55);

#ifdef __cplusplus

class DECLSPEC_UUID("DEA843D5-DAA2-11D1-95B2-0060973B3B55")
TBEdit;
#endif
#endif /* __TBEDITLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


