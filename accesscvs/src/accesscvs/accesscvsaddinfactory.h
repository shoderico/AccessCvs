#ifndef ACCESSCVSADDINFACTORY_H
#define ACCESSCVSADDINFACTORY_H

#include "addin/addinfactory.h"

namespace Access {
class Application;
} // namespace Access

class WindowWidgetManager;
class AddInRibbonTab;

class AccessCvsAddInFactory : public AddInFactory
{
    Q_OBJECT
public:
    AccessCvsAddInFactory(const QUuid &app, const QUuid &lib);
    ~AccessCvsAddInFactory();

    virtual QAxAggregated *createAggregate(QObject *parent = 0);

    virtual QAxObject *application() const;

signals:
    void addInConnection(QAxObject *application, QWidget *parentWidget);

public slots:
    void onAddInConnection(IDispatch *application);
    void onAddInDisconnection();

private:
    Access::Application *m_application;
    WindowWidgetManager *m_winWidgetManager;
    AddInRibbonTab *m_addInRibbonTab;

};

#endif // ACCESSCVSADDINFACTORY_H
