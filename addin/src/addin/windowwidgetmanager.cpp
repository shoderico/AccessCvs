#include "windowwidgetmanager.h"

#include "officelib/officelib.h"

#include <QWinWidget>

WindowWidgetManager::WindowWidgetManager(Access::Application *application, QObject *parent)
    : QObject(parent)
    , m_application(application)
{
    m_winWidget = new QWinWidget( (HWND)m_application->hWndAccessApp() );
    m_winWidget->showCentered();
}

WindowWidgetManager::~WindowWidgetManager()
{
    delete m_winWidget;
}

QWidget *WindowWidgetManager::widget() const
{
    return m_winWidget;
}

