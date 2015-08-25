#include "addinimpl.h"

#include <QAxFactory>
#include <QUuid>
#include <QResource>
#include <QFile>
#include <QTextStream>

#include <QDebug>

#include "comutil.h"
#include "addinutil.h"
#include "ui/actionmanager.h"
#include "git/gitmanager.h"
#include "managers/accessutilmanager.h"
#include "managers/windowwidgetmanager.h"


#include "officelib/officelib.h"

//#include <comdef.h>
//_COM_SMARTPTR_TYPEDEF(ITypeInfo, __uuidof(ITypeInfo));


AddInImpl::AddInImpl(QObject *parent)
    : QObject(parent)
    , m_applicationIDisp(0)
    , m_addInInstIDisp(0)
    , m_application(0)
    , m_actionManager(0)
    , m_gitManager(0)
    , m_accessUtilManager(0)
    , m_winWidgetManager(0)
{
    HRESULT hr;
    ITypeLib *pTypeLib = NULL;
    ITypeInfo *pTypeInfo = NULL;
    QString typeLibResourcePath = QAxFactory::serverFilePath() + "\\2";
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

long AddInImpl::queryInterface(const QUuid &iid, void **iface)
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
HRESULT AddInImpl::GetTypeInfoCount(UINT *pctinfo)
{
    if ( pctinfo == NULL )
        return E_INVALIDARG;

    *pctinfo = 1;
    return S_OK;
}

HRESULT AddInImpl::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
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

HRESULT AddInImpl::GetIDsOfNames(const IID &riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
    Q_UNUSED(riid); Q_UNUSED(lcid); // Q_UNUSED(rgszNames); Q_UNUSED(cNames); Q_UNUSED(lcid); Q_UNUSED(rgDispId);

    return m_pTypeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
}

HRESULT AddInImpl::Invoke(DISPID dispIdMember, const IID &riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
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
HRESULT AddInImpl::OnConnection(IDispatch *Application, ext_ConnectMode ConnectMode, IDispatch *AddInInst, SAFEARRAY **custom)
{
    Q_UNUSED(Application); Q_UNUSED(ConnectMode); Q_UNUSED(AddInInst); Q_UNUSED(custom);

    if ( !Application )
        return E_POINTER;

    m_applicationIDisp = Application;
    m_applicationIDisp->AddRef();

    m_addInInstIDisp = AddInInst;
    m_addInInstIDisp->AddRef();

    Access::_Application *_application = new Access::_Application(m_applicationIDisp/*, this*/);
    m_application = new Access::Application(_application);

    m_winWidgetManager = new WindowWidgetManager(m_application, this);
    m_actionManager = new ActionManager(m_application, m_winWidgetManager, this);
    m_gitManager = new GitManager(m_application, this);
    m_accessUtilManager = new AccessUtilManager(m_application, m_winWidgetManager, this);

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
HRESULT AddInImpl::OnDisconnection(ext_DisconnectMode RemoveMode, SAFEARRAY **custom)
{
    Q_UNUSED(RemoveMode); Q_UNUSED(custom);

    // If this is not because of host shutdown, make sure we have cleaned
    // up our command bar...
   if (RemoveMode != ext_dm_HostShutdown)
       OnBeginShutdown(custom);

   if (m_actionManager)
   {
       delete m_actionManager;
       m_actionManager = 0;
   }

   if (m_gitManager)
   {
       delete m_gitManager;
       m_gitManager = 0;
   }

   if (m_accessUtilManager)
   {
       delete m_accessUtilManager;
       m_accessUtilManager = NULL;
   }

   if (m_winWidgetManager)
   {
       delete m_winWidgetManager;
       m_winWidgetManager = 0;
   }

   if (m_application)
   {
       delete m_application;
       m_application = NULL;
   }

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
HRESULT AddInImpl::OnAddInsUpdate(SAFEARRAY **custom)
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
HRESULT AddInImpl::OnStartupComplete(SAFEARRAY **custom)
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
HRESULT AddInImpl::OnBeginShutdown(SAFEARRAY **custom)
{
    Q_UNUSED(custom);
    return S_OK;
}








// IRibbonExtensibility
HRESULT AddInImpl::GetCustomUI(BSTR RibbonID, BSTR *RibbonXml)
{
    Q_UNUSED(RibbonID);

    if (!RibbonXml)
        return E_POINTER;

    QResource resource(":/addin/ribbon.xml");
    QFile file(resource.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "AddInImpl::GetCustomUI resource file open error";
        return E_OUTOFMEMORY;
    }
    QTextStream in( &file );
    QString content = in.readAll();

    *RibbonXml = SysAllocString( reinterpret_cast<const OLECHAR *>( content.utf16() ) );

    return (*RibbonXml ? S_OK : E_OUTOFMEMORY);
}





// IRibbonCallback
HRESULT AddInImpl::ButtonClicked(IDispatch *ribbonControl)
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

    if (controlId == "StandardManualButton")
        m_actionManager->manual();
    else if (controlId == "StandardExportButton")
        m_actionManager->autoExport();
    else if (controlId == "StandardImportButton")
        m_actionManager->autoImport();

    else if (controlId == "GitInitButton")
        m_gitManager->init();
    else if (controlId == "GitIgnoreButton")
        m_gitManager->gitIgnore();
    else if (controlId == "GitManageRemotesButton")
        m_gitManager->manageRemotes();
    else if (controlId == "GitPullButton")
        m_gitManager->pull();

    else if (controlId == "UtilDecompileButton")
        m_accessUtilManager->decompile();
    else if (controlId == "UtilCompactRepairButton")
        m_accessUtilManager->compactRepair();
    else if (controlId == "UtilDecompileAndCompactRepairButton")
        m_accessUtilManager->decompileAndCompactRepair();

    return S_OK;
}

HRESULT AddInImpl::GetButtonImage(IDispatch *ribbonControl, IPictureDisp **picture)
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


    // determine icon image path and size
    QString imagePath = ":/ui/images/";
    QSize size(16,16);
    // Standard
    if ( controlId == "StandardManualButton")
    {
        imagePath += "manual.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else if ( controlId == "StandardExportButton")
    {
        imagePath += "export.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else if ( controlId == "StandardImportButton")
    {
        imagePath += "import.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    // Git
    else if ( controlId == "GitInitButton")
    {
        imagePath += "git-init.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else if ( controlId == "GitIgnoreButton")
    {
        imagePath += "git-update-gitignore.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else
        return S_OK;

    // load picutre
    IPictureDisp  *pd = ComUtil::loadPictureFromSvg( imagePath, size );

    if ( pd )
        *picture = pd;

   return S_OK;
}
