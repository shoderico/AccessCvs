#include "addinaggregated.h"

#include <QAxFactory>
#include <QUuid>
#include <QDebug>

#include "addinfactory.h"

#include "pch.hpp"

//#include <comdef.h>
//_COM_SMARTPTR_TYPEDEF(ITypeInfo, __uuidof(ITypeInfo));


AddInAggregated::AddInAggregated(AddInFactory *factory, QObject *parent)
    : QObject(parent)
    , m_applicationIDisp(0)
    , m_addInInstIDisp(0)
    , m_factory(factory)
{
    HRESULT hr;
    ITypeLib *pTypeLib = NULL;
    ITypeInfo *pTypeInfo = NULL;
    QString typeLibResourcePath = QAxFactory::serverFilePath() + "\\999";
    hr = LoadTypeLib( reinterpret_cast<const OLECHAR *>( typeLibResourcePath.utf16() ), &pTypeLib);
    if ( SUCCEEDED( hr ) )
    {
        hr = pTypeLib->GetTypeInfoOfGuid( IID_IRibbonCallback, &pTypeInfo );
        if ( !SUCCEEDED( hr ) )
             qCritical() << "GetTypeInfoOfGuid failed";

        pTypeLib->Release();
    }
    else
        qCritical() << "LoadTypeLib failed";

    m_pTypeInfo = pTypeInfo;
}

long AddInAggregated::queryInterface(const QUuid &iid, void **iface)
{
    *iface = 0;

    if (iid == QUuid( IID_IDispatch ))
        *iface = static_cast<IDispatch*>(  static_cast<IDTExtensibility2*>(this) ) ;
    else if (iid == QUuid( IID__IDTExtensibility2 ))
        *iface = (IDTExtensibility2 *)this;
    else if (iid == QUuid( IID_IRibbonExtensibility ))
        *iface = (IRibbonExtensibility *)this;
    else if (iid == QUuid( IID_IRibbonCallback ))
        *iface = (IRibbonCallback *)this;
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

// https://john.nachtimwald.com/2012/04/08/wrapping-a-c-library-in-comactivex/
// http://stackoverflow.com/questions/19253471/implement-com-idispatch-without-atl
// http://www.codeproject.com/Articles/3173/A-simple-yet-debuggable-COM-skeleton-code

// IDispatch
HRESULT AddInAggregated::GetTypeInfoCount(UINT *pctinfo)
{
    if ( pctinfo == NULL )
        return E_INVALIDARG;

    *pctinfo = 1;
    return S_OK;
}

HRESULT AddInAggregated::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
    Q_UNUSED(lcid); //Q_UNUSED(iTInfo);  Q_UNUSED(ppTInfo);

    if ( ppTInfo == NULL )
        return E_INVALIDARG;
    *ppTInfo = NULL;

    if ( iTInfo != 0 )
        return DISP_E_BADINDEX;

    m_pTypeInfo->AddRef();
    *ppTInfo = m_pTypeInfo;

    return S_OK;
}

HRESULT AddInAggregated::GetIDsOfNames(const IID &riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
    Q_UNUSED(riid); Q_UNUSED(lcid); // Q_UNUSED(rgszNames); Q_UNUSED(cNames); Q_UNUSED(lcid); Q_UNUSED(rgDispId);

    return m_pTypeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
}

HRESULT AddInAggregated::Invoke(DISPID dispIdMember, const IID &riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    Q_UNUSED(riid); Q_UNUSED(lcid); //Q_UNUSED(dispIdMember); Q_UNUSED(wFlags); Q_UNUSED(pDispParams); Q_UNUSED(pVarResult); Q_UNUSED(pExcepInfo); Q_UNUSED(puArgErr);

    return m_pTypeInfo->Invoke( static_cast<IRibbonCallback*>(this), dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}


// IDTExtensibility2
/******************************************************************
 * OnConnection()
 *
 *  Called by Office app when the addin is being loaded. ConnectMode
 *  identifies whether the addin is being loaded:
 *
 *     0  -- after startup (by the end user)
 *     1  -- during startup (normal mode)
 *     2  -- from an external source (like a VBA macro)
 *     3  -- from the command line
 *
 *  Also passed to the addin are two dispatch pointers, one to the
 *  host application and the other to the object that represents
 *  this addin in the AddIns collection. OnConnection is where the
 *  addin saves these pointers for use in calling the host back.
 *  The pointers should be released when OnDisconnection is called.
 *
 ******************************************************************/
HRESULT AddInAggregated::OnConnection(IDispatch *Application, ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom)
{
    Q_UNUSED(Application); Q_UNUSED(ConnectMode); Q_UNUSED(AddInInst); Q_UNUSED(custom);

    if ( !Application )
        return E_POINTER;

    m_applicationIDisp = Application;
    m_applicationIDisp->AddRef();

    m_addInInstIDisp = AddInInst;
    m_addInInstIDisp->AddRef();

    m_factory->setApplication(m_applicationIDisp);
    m_factory->onBeforeConnectionEvent();
    onConnectionEvent();
    m_factory->onAfterConnectionEvent();

    // If we are connecting during startup, we should wait for OnStartupComplete
    // before modifying the user-interface and prompting the user. Otherwise, we
    // can call OnStartupComplete to do the work now...
   if (ConnectMode != ext_cm_Startup)
       OnStartupComplete(custom);

    return S_OK;
}

/******************************************************************
 * OnDisconnection()
 *
 *  Called by Office app when the addin is being unloaded. The
 *  RemoveMode parameter will notify the Addin if it is being unloaded
 *  by the end user (1) or the application is shutting down (0).
 *
 ******************************************************************/
HRESULT AddInAggregated::OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY **custom)
{
    Q_UNUSED(RemoveMode); Q_UNUSED(custom);

    // If this is not because of host shutdown, make sure we have cleaned
    // up our command bar...
   if (RemoveMode != ext_dm_HostShutdown)
       OnBeginShutdown(custom);

   onDisconnectionEvent();
   m_factory->onAfterDisconnectionEvent();
   m_factory->releaseApplication();

   // Release the pointer...
   if (NULL != m_applicationIDisp) {
      m_applicationIDisp->Release();
      m_applicationIDisp = NULL;
   }
   if (NULL != m_addInInstIDisp) {
      m_addInInstIDisp->Release();
      m_addInInstIDisp = NULL;
   }

    return S_OK;
}

/******************************************************************
 * OnAddInsUpdate()
 *
 *  Notifies the Addin that the Addins collection has been changed.
 *
 ******************************************************************/
HRESULT AddInAggregated::OnAddInsUpdate(SAFEARRAY **custom)
{
    //QMessageBox::information(0, QString(""), QString("OnAddInsUpdate"));
    Q_UNUSED(custom);
    return S_OK;
}

/******************************************************************
 * OnStartupComplete()
 *
 *  Called by Office when the addin is loaded during startup and
 *  the startup process is complete. Addins loaded by the user or by
 *  VBA code don't normally receive this message, but we added code
 *  in the OnConnection event to fire this no matter how the addin
 *  is loaded.
 *
 ******************************************************************/
HRESULT AddInAggregated::OnStartupComplete(SAFEARRAY **custom)
{
    Q_UNUSED(custom);
    return S_OK;
}

/******************************************************************
 * OnBeginShutdown()
 *
 *  Called just before the host application is about to shut down.
 *
 ******************************************************************/
HRESULT AddInAggregated::OnBeginShutdown(SAFEARRAY **custom)
{
    Q_UNUSED(custom);
    return S_OK;
}








// IRibbonExtensibility
HRESULT AddInAggregated::GetCustomUI(BSTR RibbonID, BSTR *RibbonXml)
{
    Q_UNUSED(RibbonID);

    if (!RibbonXml)
        return E_POINTER;

    QString content = ribbomXml();
    if ( content.isEmpty() )
    {
        qCritical() << "AddInImpl::GetCustomUI resource file open error";
        return E_OUTOFMEMORY;
    }

    *RibbonXml = SysAllocString( reinterpret_cast<const OLECHAR *>( content.utf16() ) );

    return (*RibbonXml ? S_OK : E_OUTOFMEMORY);
}





// IRibbonCallback
HRESULT AddInAggregated::ButtonClicked(IDispatch *ribbonControl)
{
    HRESULT hr;

    // Query interface for IRibbonControl
    IRibbonControl *rc = NULL;
    hr = ribbonControl->QueryInterface(IID_IRibbonControl, (void**)&rc);
    if ( FAILED(hr) || !rc )
    {
        qCritical() << "IRibbonControl QueryInterface is failed ";
        return hr;
    }

    // retreive control-id
    BSTR bstrControlId = NULL;
    rc->get_Id( &bstrControlId );
    QString controlId = QString::fromWCharArray( bstrControlId );
    ::SysFreeString( bstrControlId );
    rc->Release();

    return onButtonClicked( controlId );
}

HRESULT AddInAggregated::GetButtonImage(IDispatch *ribbonControl, IPictureDisp **picture)
{
    HRESULT hr;

    // Query interface for IRibbonControl
    IRibbonControl *rc = NULL;
    hr = ribbonControl->QueryInterface(IID_IRibbonControl, (void**)&rc);
    if ( FAILED(hr) || !rc )
    {
        qCritical() << "IRibbonControl QueryInterface is failed ";
        return hr;
    }

    // retreive control-id
    BSTR bstrControlId = NULL;
    rc->get_Id( &bstrControlId );
    QString controlId = QString::fromWCharArray( bstrControlId );
    ::SysFreeString( bstrControlId );
    rc->Release();

    // load picutre
    IPictureDisp  *pd = buttonImage( controlId );

    if ( pd )
        *picture = pd;

    return S_OK;
}

AddInFactory *AddInAggregated::factory() const
{
    return m_factory;
}
