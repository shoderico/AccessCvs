#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>

class WindowWidgetManager;
class MainDialog;

namespace Access {
class Application;
} // namespace Access

class ActionManager : public QObject
{
    Q_OBJECT
public:
    explicit ActionManager(Access::Application *application, WindowWidgetManager *winWidgetManager, QObject *parent = 0);
    ~ActionManager();

    void manual();
    void autoImport();
    void autoExport();

signals:

public slots:

private:
    void init();

private:
    Access::Application *m_application;
    WindowWidgetManager *m_winWidgetManager;
    MainDialog *m_dlg;
};

#endif // ACTIONMANAGER_H
