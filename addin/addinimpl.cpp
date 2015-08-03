#include "addinimpl.h"

#include <QUuid>
#include <QResource>
#include <QFile>
#include <QTextStream>

#include <QMessageBox>

#include <oleauto.h>
#include <ocidl.h>
#include <olectl.h>

#include <QtWin>
#include <QWinWidget>


#include "ui/maindialog.h"


#include "officelib/dao.h"
#include "officelib/adodb.h"
#include "officelib/office.h"
#include "officelib/vbide.h"
#include "officelib/access.h"

//#include <comdef.h>
//_COM_SMARTPTR_TYPEDEF(ITypeInfo, __uuidof(ITypeInfo));


AddInImpl::AddInImpl(ITypeInfo *pTypeInfo)
    : m_pTypeInfo(pTypeInfo)
//    , m_pDlg(0)
{
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
    //QMessageBox::information(0, QString(""), QString("GetTypeInfoCount"));

    if ( pctinfo == NULL )
        return E_INVALIDARG;

    *pctinfo = 1;
    return S_OK;
}

HRESULT AddInImpl::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
    //QMessageBox::information(0, QString(""), QString("GetTypeInfo"));
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
    //QMessageBox::information(0, QString(""), QString("GetIDsOfNames"));
    Q_UNUSED(riid); Q_UNUSED(lcid); // Q_UNUSED(rgszNames); Q_UNUSED(cNames); Q_UNUSED(lcid); Q_UNUSED(rgDispId);

    return m_pTypeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
}

HRESULT AddInImpl::Invoke(DISPID dispIdMember, const IID &riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    //QMessageBox::information(0, QString(""), QString("Invoke"));
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
    //QMessageBox::information(0, QString(""), QString("OnConnection"));
    Q_UNUSED(Application); Q_UNUSED(ConnectMode); Q_UNUSED(AddInInst); Q_UNUSED(custom);

    if ( !Application )
        return E_POINTER;

    m_pApplication = Application;
    m_pApplication->AddRef();

    m_pAddInInst = AddInInst;
    m_pAddInInst->AddRef();

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
    //QMessageBox::information(0, QString(""), QString("OnDisconnection"));
    Q_UNUSED(RemoveMode); Q_UNUSED(custom);

    // If this is not because of host shutdown, make sure we have cleaned
    // up our command bar...
   if (RemoveMode != ext_dm_HostShutdown)
       OnBeginShutdown(custom);

//   if (m_pDlg)
//   {
//       m_pDlg->close();
//       delete m_pDlg;
//       m_pDlg = 0;
//   }

   // Release the pointer...
   if (NULL != m_pApplication) {
      m_pApplication->Release();
      m_pApplication = NULL;
   }
   if (NULL != m_pAddInInst) {
      m_pAddInInst->Release();
      m_pAddInInst = NULL;
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
    //QMessageBox::information(0, QString(""), QString("OnStartupComplete"));
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
    //QMessageBox::information(0, QString(""), QString("OnBeginShutdown"));
    Q_UNUSED(custom);
    return S_OK;
}








// IRibbonExtensibility
HRESULT AddInImpl::GetCustomUI(BSTR RibbonID, BSTR *RibbonXml)
{
    //QMessageBox::information(0, QString(""), QString("GetCustomUI"));
    Q_UNUSED(RibbonID);

    if (!RibbonXml)
        return E_POINTER;

    QResource resource(":/addin/ribbon.xml");
    QFile file(resource.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(0, QString(""), QString("AddInImpl::GetCustomUI resource file open error"));
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
    //QMessageBox::information(0, QString(""), QString("ButtonClicked"));
    Q_UNUSED(ribbonControl);


    Access::Application application( m_pApplication );
    QWinWidget winWidget( (HWND)application.hWndAccessApp() );
    winWidget.showCentered();

    MainDialog dlg( m_pApplication, &winWidget );
    dlg.exec();

//    MainDialog dlg( m_pApplication );
//    dlg.exec();

//    m_pDlg = new MainDialog(m_pApplication);
//    m_pDlg->show();

    return S_OK;
}

HRESULT AddInImpl::GetButtonImage(IDispatch *ribbonControl, IPictureDisp **picture)
{
    HRESULT hr;

    IRibbonControl *rc = NULL;
    hr = ribbonControl->QueryInterface(IID_IRibbonControl, (void**)&rc);
    if ( FAILED(hr) || !rc )
    {
        QMessageBox::information(0, QString(""), QString("IRibbonControl is failed "));
        return hr;
    }
    BSTR bstrControlId = NULL;
    rc->get_Id( &bstrControlId );
    QString controlId = QString::fromWCharArray( bstrControlId );
    ::SysFreeString( bstrControlId );
    rc->Release();
    //QMessageBox::information(0, QString(""), controlId);

    QString imagePath = ":/addin/";
    if ( controlId == "CustomButton")
    {
        imagePath += "button.png";
    }
    QPixmap pixmap( imagePath );
    HBITMAP hbm = QtWin::toHBITMAP( pixmap, QtWin::HBitmapAlpha );


    IPictureDisp  *pd;
    PICTDESC pdesc;
    memset( &pdesc, 0, sizeof(pdesc) );
    pdesc.cbSizeofstruct = sizeof(pdesc);
    pdesc.picType = PICTYPE_BITMAP;

    pdesc.bmp.hbitmap = hbm;

    pdesc.bmp.hpal = NULL;
    hr = OleCreatePictureIndirect( &pdesc, IID_IPicture, TRUE, (void**)&pd );
    //::DeleteObject(hbm);  // no need to delete object

    if ( FAILED(hr) )
    {
        pd = NULL;
        QMessageBox::information(0, QString(""), QString("ButtonClicked failed "));
    }

    if ( pd )
    {
        //QMessageBox::information(0, QString(""), QString("ButtonClicked succeed "));
        *picture = pd;
    }


   return S_OK;
}
