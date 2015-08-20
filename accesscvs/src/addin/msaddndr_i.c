

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Jul 21 21:51:08 2015
 */
/* Compiler settings for MSADDNDR.IDL:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

//#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_AddInDesignerObjects,0xAC0714F2,0x3D04,0x11D1,0xAE,0x7D,0x00,0xA0,0xC9,0x0F,0x26,0xF4);


MIDL_DEFINE_GUID(IID, IID_IAddinDesigner,0xAC0714F3,0x3D04,0x11D1,0xAE,0x7D,0x00,0xA0,0xC9,0x0F,0x26,0xF4);


MIDL_DEFINE_GUID(IID, IID_IAddinInstance,0xAC0714F4,0x3D04,0x11D1,0xAE,0x7D,0x00,0xA0,0xC9,0x0F,0x26,0xF4);


MIDL_DEFINE_GUID(IID, IID__IDTExtensibility2,0xB65AD801,0xABAF,0x11D0,0xBB,0x8B,0x00,0xA0,0xC9,0x0F,0x27,0x44);


MIDL_DEFINE_GUID(CLSID, CLSID_AddinDesigner,0xAC0714F6,0x3D04,0x11D1,0xAE,0x7D,0x00,0xA0,0xC9,0x0F,0x26,0xF4);


MIDL_DEFINE_GUID(CLSID, CLSID_AddinDesigner2,0xE436987E,0xF427,0x4AD7,0x87,0x38,0x6D,0x08,0x95,0xA3,0xE9,0x3F);


MIDL_DEFINE_GUID(CLSID, CLSID_AddinInstance,0xAC0714F7,0x3D04,0x11D1,0xAE,0x7D,0x00,0xA0,0xC9,0x0F,0x26,0xF4);


MIDL_DEFINE_GUID(CLSID, CLSID_AddinInstance2,0xAB5357A7,0x3179,0x47F9,0xA7,0x05,0x96,0x6B,0x8B,0x93,0x6D,0x5E);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



