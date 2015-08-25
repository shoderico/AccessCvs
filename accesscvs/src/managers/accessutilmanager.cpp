#include "accessutilmanager.h"

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "accessutil/accessutil.h"
#include "managers/windowwidgetmanager.h"


#include <QMessageBox>
#include <QApplication>


AccessUtilManager::AccessUtilManager(Access::Application *application, WindowWidgetManager *winWidgetManager, QObject *parent)
    : QObject(parent)
    , m_application(application)
    , m_winWidgetManager(winWidgetManager)
{
}

void AccessUtilManager::decompile()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_application->CloseCurrentDatabase();

    AccessUtil au;
    quint64 currentThreadId = au.getAccessThreadId(m_application);
    au.decompile(fileName, currentThreadId);

    au.openCurrentDatabase(m_application, fileName);
}

void AccessUtilManager::compactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_application->CloseCurrentDatabase();

    AccessUtil au;
    au.compactRepair(fileName, 3);

    au.openCurrentDatabase(m_application, fileName);
}

void AccessUtilManager::decompileAndCompactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_application->CloseCurrentDatabase();

    AccessUtil au;
    quint64 currentThreadId = au.getAccessThreadId(m_application);
    au.decompile(fileName, currentThreadId);
    au.compactRepair(fileName, 3);

    au.openCurrentDatabase(m_application, fileName);
}

bool AccessUtilManager::getCurrentFileName(QString &fileName)
{
    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (!currentProject.is())
    {
        QMessageBox::information(m_winWidgetManager->widget(), "", tr("no project is opened!"));
        return false;
    }
    fileName = currentProject->FullName();
    return true;
}

