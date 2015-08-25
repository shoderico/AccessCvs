#ifndef ACCESSUTILMANAGER_H
#define ACCESSUTILMANAGER_H

#include <QObject>

class WindowWidgetManager;

namespace Access {
class Application;
} // namespace Access

class AccessUtilManager : public QObject
{
    Q_OBJECT
public:
    explicit AccessUtilManager(Access::Application *application, WindowWidgetManager *winWidgetManager, QObject *parent = 0);

    void decompile();
    void compactRepair();
    void decompileAndCompactRepair();

signals:
public slots:

private:
    bool getCurrentFileName(QString &fileName);

private:
    Access::Application *m_application;
    WindowWidgetManager *m_winWidgetManager;
};

#endif // ACCESSUTILMANAGER_H
