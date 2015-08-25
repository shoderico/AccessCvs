#ifndef ACCESSUTILMANAGER_H
#define ACCESSUTILMANAGER_H

#include <QObject>

class WindowWidgetManager;
class UiBlocker;
class ThreadedInvoker;

namespace Access {
class Application;
} // namespace Access

class AccessUtilManager : public QObject
{
    Q_OBJECT
public:
    explicit AccessUtilManager(Access::Application *application, WindowWidgetManager *winWidgetManager, QObject *parent = 0);
    ~AccessUtilManager();

    void decompile();
    void compactRepair();
    void decompileAndCompactRepair();

private slots:
    void doDecompile();
    void doCompactRepair();
    void doDecompileAndCompactRepair();

private:
    bool getCurrentFileName(QString &fileName);

private:
    Access::Application *m_application;
    WindowWidgetManager *m_winWidgetManager;
    UiBlocker *m_uiBlocker;
    ThreadedInvoker *m_threadedInvoker;
};

#endif // ACCESSUTILMANAGER_H
