#ifndef ADDINIMPL_H
#define ADDINIMPL_H

#include <QAxAggregated>
#include <QObject>

#include "msaddndr.h"
#include "ribbon.h"
#include "ribboncallback.h"


namespace Access {
class Application;
} // namespace Access

class AddInImpl
        : public QObject
        , public QAxAggregated
        , public IDTExtensibility2
        , public IRibbonExtensibility
        , public IRibbonCallback
{
    Q_OBJECT
public:
    explicit AddInImpl(QObject *parent = 0);

    long queryInterface(const QUuid &iid, void **iface);

    // IUnknown
    QAXAGG_IUNKNOWN

    // IDispatch
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
        UINT *pctinfo);

    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
        UINT iTInfo,
        LCID lcid,
        ITypeInfo **ppTInfo);

    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
        REFIID riid,
        LPOLESTR *rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID *rgDispId);

    virtual HRESULT STDMETHODCALLTYPE Invoke(
        DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS *pDispParams,
        VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo,
        UINT *puArgErr);

    // IDTExtensibility2
    virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnConnection(
        /* [in] */ IDispatch *Application,
        /* [in] */ ext_ConnectMode ConnectMode,
        /* [in] */ IDispatch *AddInInst,
        /* [in] */ SAFEARRAY * *custom);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnDisconnection(
        /* [in] */ ext_DisconnectMode RemoveMode,
        /* [in] */ SAFEARRAY * *custom);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnAddInsUpdate(
        /* [in] */ SAFEARRAY * *custom);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnStartupComplete(
        /* [in] */ SAFEARRAY * *custom);

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnBeginShutdown(
        /* [in] */ SAFEARRAY * *custom);

    // IRibbonExtensibility
    virtual /* [helpcontext][id] */ HRESULT STDMETHODCALLTYPE GetCustomUI(
        /* [in] */ BSTR RibbonID,
        /* [retval][out] */ BSTR *RibbonXml);


    // IRibbonCallback
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ButtonClicked(
        /* [in] */ IDispatch *ribbonControl);
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetButtonImage(
        /* [in] */ IDispatch *ribbonControl,
        /* [retval][out] */ /* external definition not present */ IPictureDisp **picture);

protected:
    virtual void onConnectionEvent() = 0;
    virtual void onDisconnectionEvent() = 0;
    virtual QString ribbomXml() = 0;
    virtual IPictureDisp *buttonImage(const QString &controlId) = 0;
    virtual HRESULT onButtonClicked(const QString &controlId) = 0;

    Access::Application *application() const;

private:
    ITypeInfo *m_pTypeInfo;

    IDispatch *m_applicationIDisp;
    IDispatch *m_addInInstIDisp;

    Access::Application *m_application;
};

#endif // ADDINIMPL_H
