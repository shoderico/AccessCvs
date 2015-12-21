#ifndef EXCELCVSADDINFACTORY_H
#define EXCELCVSADDINFACTORY_H

#include "addin/addinfactory.h"

namespace Excel {
class Application;
} // namespace Access

class ExcelCvsAddInFactory : public AddInFactory
{
    Q_OBJECT
public:
    ExcelCvsAddInFactory(const QUuid &app, const QUuid &lib);
    ~ExcelCvsAddInFactory();

    virtual QAxAggregated *createAggregate(QObject *parent = 0);
    virtual void setApplication(IDispatch *application);
    virtual void releaseApplication();
    virtual QAxObject *application() const;
    virtual int applicationHwnd();
    virtual void onBeforeConnectionEvent();
    virtual void onAfterDisconnectionEvent();

private:
    Excel::Application *m_application;

};

#endif // EXCELCVSADDINFACTORY_H
