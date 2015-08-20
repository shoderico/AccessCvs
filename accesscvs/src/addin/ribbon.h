

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Fri Jul 24 16:33:49 2015
 */
/* Compiler settings for IRibbon.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

//#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __iribbon_h__
#define __iribbon_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IRibbonUI_FWD_DEFINED__
#define __IRibbonUI_FWD_DEFINED__
typedef interface IRibbonUI IRibbonUI;
#endif 	/* __IRibbonUI_FWD_DEFINED__ */


#ifndef __IRibbonControl_FWD_DEFINED__
#define __IRibbonControl_FWD_DEFINED__
typedef interface IRibbonControl IRibbonControl;
#endif 	/* __IRibbonControl_FWD_DEFINED__ */


#ifndef __IRibbonExtensibility_FWD_DEFINED__
#define __IRibbonExtensibility_FWD_DEFINED__
typedef interface IRibbonExtensibility IRibbonExtensibility;
#endif 	/* __IRibbonExtensibility_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

//void * __RPC_USER MIDL_user_allocate(size_t);
//void __RPC_USER MIDL_user_free( void * );


#ifndef __Office_LIBRARY_DEFINED__
#define __Office_LIBRARY_DEFINED__

/* library Office */
/* [custom][helpcontext][helpfile][helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Office;

#ifndef __IRibbonUI_INTERFACE_DEFINED__
#define __IRibbonUI_INTERFACE_DEFINED__

/* interface IRibbonUI */
/* [object][oleautomation][dual][helpcontext][uuid] */ 


EXTERN_C const IID IID_IRibbonUI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("000C03A7-0000-0000-C000-000000000046")
    IRibbonUI : public IDispatch
    {
    public:
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE Invalidate( void) = 0;
        
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE InvalidateControl( 
            /* [in] */ BSTR ControlID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRibbonUIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRibbonUI * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRibbonUI * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRibbonUI * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRibbonUI * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRibbonUI * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRibbonUI * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRibbonUI * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *Invalidate )( 
            IRibbonUI * This);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *InvalidateControl )( 
            IRibbonUI * This,
            /* [in] */ BSTR ControlID);
        
        END_INTERFACE
    } IRibbonUIVtbl;

    interface IRibbonUI
    {
        CONST_VTBL struct IRibbonUIVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRibbonUI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRibbonUI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRibbonUI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRibbonUI_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRibbonUI_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRibbonUI_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRibbonUI_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRibbonUI_Invalidate(This)	\
    (This)->lpVtbl -> Invalidate(This)

#define IRibbonUI_InvalidateControl(This,ControlID)	\
    (This)->lpVtbl -> InvalidateControl(This,ControlID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE IRibbonUI_Invalidate_Proxy( 
    IRibbonUI * This);


void __RPC_STUB IRibbonUI_Invalidate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE IRibbonUI_InvalidateControl_Proxy( 
    IRibbonUI * This,
    /* [in] */ BSTR ControlID);


void __RPC_STUB IRibbonUI_InvalidateControl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRibbonUI_INTERFACE_DEFINED__ */


#ifndef __IRibbonControl_INTERFACE_DEFINED__
#define __IRibbonControl_INTERFACE_DEFINED__

/* interface IRibbonControl */
/* [object][oleautomation][dual][helpcontext][uuid] */ 


EXTERN_C const IID IID_IRibbonControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("000C0395-0000-0000-C000-000000000046")
    IRibbonControl : public IDispatch
    {
    public:
        virtual /* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ BSTR *Id) = 0;
        
        virtual /* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE get_Context( 
            /* [retval][out] */ IDispatch **Context) = 0;
        
        virtual /* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE get_Tag( 
            /* [retval][out] */ BSTR *Tag) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRibbonControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRibbonControl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRibbonControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRibbonControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRibbonControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRibbonControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRibbonControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRibbonControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IRibbonControl * This,
            /* [retval][out] */ BSTR *Id);
        
        /* [helpcontext][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Context )( 
            IRibbonControl * This,
            /* [retval][out] */ IDispatch **Context);
        
        /* [helpcontext][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Tag )( 
            IRibbonControl * This,
            /* [retval][out] */ BSTR *Tag);
        
        END_INTERFACE
    } IRibbonControlVtbl;

    interface IRibbonControl
    {
        CONST_VTBL struct IRibbonControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRibbonControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRibbonControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRibbonControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRibbonControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRibbonControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRibbonControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRibbonControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRibbonControl_get_Id(This,Id)	\
    (This)->lpVtbl -> get_Id(This,Id)

#define IRibbonControl_get_Context(This,Context)	\
    (This)->lpVtbl -> get_Context(This,Context)

#define IRibbonControl_get_Tag(This,Tag)	\
    (This)->lpVtbl -> get_Tag(This,Tag)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE IRibbonControl_get_Id_Proxy( 
    IRibbonControl * This,
    /* [retval][out] */ BSTR *Id);


void __RPC_STUB IRibbonControl_get_Id_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE IRibbonControl_get_Context_Proxy( 
    IRibbonControl * This,
    /* [retval][out] */ IDispatch **Context);


void __RPC_STUB IRibbonControl_get_Context_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpcontext][propget][id] */ HRESULT STDMETHODCALLTYPE IRibbonControl_get_Tag_Proxy( 
    IRibbonControl * This,
    /* [retval][out] */ BSTR *Tag);


void __RPC_STUB IRibbonControl_get_Tag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRibbonControl_INTERFACE_DEFINED__ */


#ifndef __IRibbonExtensibility_INTERFACE_DEFINED__
#define __IRibbonExtensibility_INTERFACE_DEFINED__

/* interface IRibbonExtensibility */
/* [object][oleautomation][dual][helpcontext][uuid] */ 


EXTERN_C const IID IID_IRibbonExtensibility;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("000C0396-0000-0000-C000-000000000046")
    IRibbonExtensibility : public IDispatch
    {
    public:
        virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetCustomUI( 
            /* [in] */ BSTR RibbonID,
            /* [retval][out] */ BSTR *RibbonXml) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRibbonExtensibilityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRibbonExtensibility * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRibbonExtensibility * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRibbonExtensibility * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRibbonExtensibility * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRibbonExtensibility * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRibbonExtensibility * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRibbonExtensibility * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][id] */ HRESULT ( STDMETHODCALLTYPE *GetCustomUI )( 
            IRibbonExtensibility * This,
            /* [in] */ BSTR RibbonID,
            /* [retval][out] */ BSTR *RibbonXml);
        
        END_INTERFACE
    } IRibbonExtensibilityVtbl;

    interface IRibbonExtensibility
    {
        CONST_VTBL struct IRibbonExtensibilityVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRibbonExtensibility_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRibbonExtensibility_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRibbonExtensibility_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRibbonExtensibility_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRibbonExtensibility_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRibbonExtensibility_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRibbonExtensibility_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRibbonExtensibility_GetCustomUI(This,RibbonID,RibbonXml)	\
    (This)->lpVtbl -> GetCustomUI(This,RibbonID,RibbonXml)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE IRibbonExtensibility_GetCustomUI_Proxy( 
    IRibbonExtensibility * This,
    /* [in] */ BSTR RibbonID,
    /* [retval][out] */ BSTR *RibbonXml);


void __RPC_STUB IRibbonExtensibility_GetCustomUI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRibbonExtensibility_INTERFACE_DEFINED__ */

#endif /* __Office_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


