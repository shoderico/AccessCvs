#include "accessutilmanager.h"

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/threadedinvoker.h"
#include "accessutil/accessutil.h"
#include "managers/windowwidgetmanager.h"
#include "view/uiblocker.h"

#include <QMessageBox>


AccessUtilManager::AccessUtilManager(Access::Application *application, WindowWidgetManager *winWidgetManager, QObject *parent)
    : QObject(parent)
    , m_application(application)
    , m_winWidgetManager(winWidgetManager)
    , m_threadedInvoker(new ThreadedInvoker(100, this))
{
    m_uiBlocker = new UiBlocker(m_winWidgetManager->widget(), this);
    m_uiBlocker->setText(tr("please wait ..."));

    connect(m_threadedInvoker, SIGNAL(finished()), m_uiBlocker, SLOT(hide()) );
}

AccessUtilManager::~AccessUtilManager()
{
    delete m_uiBlocker;
    delete m_threadedInvoker;
}

void AccessUtilManager::decompile()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doDecompile()) );
}

void AccessUtilManager::compactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doCompactRepair()) );
}

void AccessUtilManager::decompileAndCompactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doDecompileAndCompactRepair()) );
}

void AccessUtilManager::doDecompile()
{
    QString fileName;
    if (getCurrentFileName(fileName))
    {
        m_application->CloseCurrentDatabase();

        AccessUtil au;
        quint64 currentThreadId = au.getAccessThreadId(m_application);
        au.decompile(fileName, currentThreadId);

        au.openCurrentDatabase(m_application, fileName);
    }
    m_threadedInvoker->finish();
}

void AccessUtilManager::doCompactRepair()
{
    QString fileName;
    if (getCurrentFileName(fileName))
    {
        m_application->CloseCurrentDatabase();

        AccessUtil au;
        au.compactRepair(m_application, fileName, 3);

        au.openCurrentDatabase(m_application, fileName);
    }
    m_threadedInvoker->finish();
}

void AccessUtilManager::doDecompileAndCompactRepair()
{
    QString fileName;
    if (getCurrentFileName(fileName))
    {
        m_application->CloseCurrentDatabase();

        AccessUtil au;
        quint64 currentThreadId = au.getAccessThreadId(m_application);
        au.decompile(fileName, currentThreadId);
        au.compactRepair(m_application, fileName, 3);

        au.openCurrentDatabase(m_application, fileName);
    }
    m_threadedInvoker->finish();
}

bool AccessUtilManager::getCurrentFileName(QString &fileName)
{
    fileName = "";
    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (currentProject.is())
        fileName = currentProject->FullName();

    if (fileName.isEmpty())
    {
        QMessageBox::information(m_winWidgetManager->widget(), "", tr("no project is opened!"));
        return false;
    }
    return true;
}

