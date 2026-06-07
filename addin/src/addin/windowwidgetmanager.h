#ifndef WINDOWWIDGETMANAGER_H
#define WINDOWWIDGETMANAGER_H

#include "addin_global.h"
#include <QObject>

class QWinWidget;

class ADDIN_SHARED_EXPORT WindowWidgetManager : public QObject
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
