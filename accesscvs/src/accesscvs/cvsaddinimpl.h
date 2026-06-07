#ifndef CVSADDINIMPL_H
#define CVSADDINIMPL_H

#include "addin/addinimpl.h"

class WindowWidgetManager;
class AddInController;
class AddInFactory;

class CvsAddInImpl : public AddInImpl
{
    Q_OBJECT
public:
    explicit CvsAddInImpl(AddInFactory *factory, QObject *parent = 0);

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

#endif // CVSADDINIMPL_H
