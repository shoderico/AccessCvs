#include "actionmanager.h"

#include "managers/windowwidgetmanager.h"
#include "ui/maindialog.h"
#include "officelib/officelib.h"

ActionManager::ActionManager(Access::Application *application, WindowWidgetManager *winWidgetManager, QObject *parent)
    : QObject(parent)
    , m_application(application)
    , m_winWidgetManager(winWidgetManager)
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
    if (!m_dlg)
    {
        m_dlg = new MainDialog( m_application, m_winWidgetManager->widget() );
    }
}

