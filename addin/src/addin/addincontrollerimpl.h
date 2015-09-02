#ifndef ADDINCONTROLLERIMPL_H
#define ADDINCONTROLLERIMPL_H

#include "addin_global.h"

#include "addinimpl.h"

class WindowWidgetManager;
class AddInController;
class AddInFactory;

class ADDINSHARED_EXPORT AddInControllerImpl : public AddInImpl
{
    Q_OBJECT
public:
    explicit AddInControllerImpl(AddInFactory *factory, QObject *parent = 0);

    void appendController(AddInController *controller);

protected:
    virtual void onConnectionEvent();
    virtual void onDisconnectionEvent();
    virtual QString ribbomXml();
    virtual IPictureDisp *buttonImage(const QString &controlId);
    virtual HRESULT onButtonClicked(const QString &controlId);

private:
    WindowWidgetManager     *m_winWidgetManager;
    QList<AddInController *> m_controllers;
};

#endif // ADDINCONTROLLERIMPL_H
