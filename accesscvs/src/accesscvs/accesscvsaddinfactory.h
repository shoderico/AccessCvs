#ifndef ACCESSCVSADDINFACTORY_H
#define ACCESSCVSADDINFACTORY_H

#include "addin/addinfactory.h"

namespace Access {
class Application;
} // namespace Access

class AddInRibbonTab;

class AccessCvsAddInFactory : public AddInFactory
{
    Q_OBJECT
public:
    AccessCvsAddInFactory(const QUuid &app, const QUuid &lib);
    ~AccessCvsAddInFactory();

    virtual QAxAggregated *createAggregate(QObject *parent = 0);
    virtual void setApplication(IDispatch *application);
    virtual void releaseApplication();
    virtual QAxObject *application() const;
    virtual int applicationHwnd();
    virtual void onBeforeConnectionEvent();
    virtual void onAfterConnectionEvent();
    virtual void onAfterDisconnectionEvent();

private:
    Access::Application *m_application;
    AddInRibbonTab *m_addInRibbonTab;

};

#endif // ACCESSCVSADDINFACTORY_H
