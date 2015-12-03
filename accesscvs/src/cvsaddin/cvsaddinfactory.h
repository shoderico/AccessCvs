#ifndef CVSADDINFACTORY_H
#define CVSADDINFACTORY_H

#include "addin/addinfactory.h"

namespace Access {
class Application;
} // namespace Access

class CvsAddInFactory : public AddInFactory
{
    Q_OBJECT
public:
    CvsAddInFactory(const QUuid &app, const QUuid &lib);
    ~CvsAddInFactory();

    virtual QAxAggregated *createAggregate(QObject *parent = 0);
    virtual void setApplication(IDispatch *application);
    virtual void releaseApplication();
    virtual QAxObject *application() const;
    virtual int applicationHwnd();
    virtual void onBeforeConnectionEvent();
    virtual void onAfterDisconnectionEvent();

private:
    Access::Application *m_application;

};

#endif // CVSADDINFACTORY_H
