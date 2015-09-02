#ifndef WINDOWWIDGETMANAGER_H
#define WINDOWWIDGETMANAGER_H

#include <QObject>

class QWinWidget;

namespace Access {
class Application;
} // namespace Access

class WindowWidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowWidgetManager(Access::Application *application, QObject *parent = 0);
    ~WindowWidgetManager();
    QWidget *widget() const;


private:
    Access::Application *m_application;
    QWinWidget *m_winWidget;
};

#endif // WINDOWWIDGETMANAGER_H
