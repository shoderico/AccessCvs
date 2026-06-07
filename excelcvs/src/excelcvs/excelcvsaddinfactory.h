#ifndef EXCELCVSADDINFACTORY_H
#define EXCELCVSADDINFACTORY_H

#include "addin/addinfactory.h"

namespace Excel {
class Application;
} // namespace Access

class WindowWidgetManager;

class ExcelCvsAddInFactory : public AddInFactory
{
    Q_OBJECT
public:
    ExcelCvsAddInFactory(const QUuid &app, const QUuid &lib);
    ~ExcelCvsAddInFactory();

    virtual QAxAggregated *createAggregate(QObject *parent = 0);

    virtual QAxObject *application() const;

signals:
    void addInConnection(QAxObject *application, QWidget *parentWidget);

public slots:
    void onAddInConnection(IDispatch *application);
    void onAddInDisconnection();

private:
    Excel::Application *m_application;
    WindowWidgetManager *m_winWidgetManager;

};

#endif // EXCELCVSADDINFACTORY_H
