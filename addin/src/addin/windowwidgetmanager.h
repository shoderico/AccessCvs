#ifndef WINDOWWIDGETMANAGER_H
#define WINDOWWIDGETMANAGER_H

#include <QObject>

class QWinWidget;

class WindowWidgetManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowWidgetManager(int applicationHwnd, QObject *parent = 0);
    ~WindowWidgetManager();
    QWidget *widget() const;


private:
    QWinWidget *m_winWidget;
};

#endif // WINDOWWIDGETMANAGER_H
