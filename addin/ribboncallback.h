

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Jul 27 17:52:33 2015
 */
/* Compiler settings for ribboncallback.idl:
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


#ifndef __ribboncallback_h__
#define __ribboncallback_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IRibbonCallback_FWD_DEFINED__
#define __IRibbonCallback_FWD_DEFINED__
typedef interface IRibbonCallback IRibbonCallback;
#endif 	/* __IRibbonCallback_FWD_DEFINED__ */


/* header files for imported files */
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

//void * __RPC_USER MIDL_user_allocate(size_t);
//void __RPC_USER MIDL_user_free( void * );


#ifndef __RibbonCallback_LIBRARY_DEFINED__
#define __RibbonCallback_LIBRARY_DEFINED__

/* library RibbonCallback */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_RibbonCallback;

#ifndef __IRibbonCallback_INTERFACE_DEFINED__
#define __IRibbonCallback_INTERFACE_DEFINED__

/* interface IRibbonCallback */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRibbonCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4199dd4a-0bdc-4a00-91eb-c0397217fd14")
    IRibbonCallback : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ButtonClicked( 
            /* [in] */ IDispatch *ribbonControl) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetButtonImage( 
            /* [in] */ IDispatch *ribbonControl,
            /* [retval][out] */ IPictureDisp **picture) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRibbonCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRibbonCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRibbonCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRibbonCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRibbonCallback * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRibbonCallback * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRibbonCallback * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRibbonCallback * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ButtonClicked )( 
            IRibbonCallback * This,
            /* [in] */ IDispatch *ribbonControl);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetButtonImage )( 
            IRibbonCallback * This,
            /* [in] */ IDispatch *ribbonControl,
            /* [retval][out] */ IPictureDisp **picture);
        
        END_INTERFACE
    } IRibbonCallbackVtbl;

    interface IRibbonCallback
    {
        CONST_VTBL struct IRibbonCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRibbonCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRibbonCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRibbonCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRibbonCallback_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRibbonCallback_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRibbonCallback_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRibbonCallback_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRibbonCallback_ButtonClicked(This,ribbonControl)	\
    (This)->lpVtbl -> ButtonClicked(This,ribbonControl)

#define IRibbonCallback_GetButtonImage(This,ribbonControl,picture)	\
    (This)->lpVtbl -> GetButtonImage(This,ribbonControl,picture)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRibbonCallback_ButtonClicked_Proxy( 
    IRibbonCallback * This,
    /* [in] */ IDispatch *ribbonControl);


void __RPC_STUB IRibbonCallback_ButtonClicked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRibbonCallback_GetButtonImage_Proxy( 
    IRibbonCallback * This,
    /* [in] */ IDispatch *ribbonControl,
    /* [retval][out] */ IPictureDisp **picture);


void __RPC_STUB IRibbonCallback_GetButtonImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRibbonCallback_INTERFACE_DEFINED__ */

#endif /* __RibbonCallback_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


