#include "actionmanager.h"

#include <QWinWidget>

#include "ui/maindialog.h"
#include "officelib/officelib.h"

ActionManager::ActionManager(Access::Application *application, QObject *parent)
    : QObject(parent)
    , m_application(application)
    , m_winWidget(0)
    , m_dlg(0)
{
    init();
}

ActionManager::~ActionManager()
{
    if (m_dlg)
    {
        m_dlg->close();
        delete m_dlg;
        m_dlg = 0;

        delete m_winWidget;
        m_winWidget = 0;
    }
}

void ActionManager::manual()
{
    m_dlg->showAsManual();
}

void ActionManager::autoImport()
{
    m_dlg->showAsAutoImport();
}

void ActionManager::autoExport()
{
    m_dlg->showAsAutoExport();
}

void ActionManager::init()
{
    if (!m_winWidget)
    {
        m_winWidget = new QWinWidget( (HWND)m_application->hWndAccessApp() );
        m_winWidget->showCentered();
        m_dlg = new MainDialog( m_application, m_winWidget );
    }
}

