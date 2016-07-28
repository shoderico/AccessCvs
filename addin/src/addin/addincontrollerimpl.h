#ifndef ADDINCONTROLLERIMPL_H
#define ADDINCONTROLLERIMPL_H

#include "addin_global.h"

#include "addinaggregated.h"

class WindowWidgetManager;
class AddInController;
class AddInFactory;

class ADDIN_SHARED_EXPORT AddInControllerImpl : public AddInAggregated
{
    Q_OBJECT
public:
    explicit AddInControllerImpl(AddInFactory *factory, QObject *parent = 0);

    void appendController(AddInController *controller);

    void setRibbonTabId(const QString &tabId);
    void setRibbonTabLabel(const QString &tabLabel);

    virtual HRESULT onButtonClicked(const QString &controlId);

protected:
    virtual void onConnectionEvent();
    virtual void onDisconnectionEvent();
    virtual QString ribbomXml();
    virtual IPictureDisp *buttonImage(const QString &controlId);

private:
    WindowWidgetManager     *m_winWidgetManager;
    QList<AddInController *> m_controllers;

    QString m_ribbonTabId;
    QString m_ribbonTabLabel;
};

#endif // ADDINCONTROLLERIMPL_H
