#include "accessutilcontroller.h"

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/threadedinvoker.h"
#include "accessutil/accessutil.h"
#include "view/uiblocker.h"
#include "addinutil/addinutil.h"

#include <QMessageBox>


AccessUtilController::AccessUtilController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
    , m_threadedInvoker(0)
    , m_uiBlocker(0)
{
}

AccessUtilController::~AccessUtilController()
{
    delete m_uiBlocker;
    delete m_threadedInvoker;
}

void AccessUtilController::initialize(Access::Application *application, QWidget *parentWidget)
{
    m_application = application;
    m_parentWidget = parentWidget;
    m_threadedInvoker = new ThreadedInvoker(100, this);

    m_uiBlocker = new UiBlocker(m_parentWidget, this);
    m_uiBlocker->setText(tr("please wait ..."));

    connect(m_threadedInvoker, SIGNAL(finished()), m_uiBlocker, SLOT(hide()) );
}

QString AccessUtilController::ribbonXml()
{
    QString content =
        "<group id=\"UtilGroup\" "
            "label=\"Util\"> "
            "<button id=\"UtilDecompileAndCompactRepairButton\" "
            "   size=\"normal\" "
            "   label=\"Decompile &amp;&amp; CompactRepair\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"UtilDecompileButton\" "
            "   size=\"normal\" "
            "   label=\"Decompile\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"UtilCompactRepairButton\" "
            "   size=\"normal\" "
            "   label=\"CompactRepair\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
        "</group>"
        ;
    return content;
}

bool AccessUtilController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
{
    return false;
}

bool AccessUtilController::handleButtonClick(const QString &controlId)
{
    if (controlId == "UtilDecompileButton")
        decompile();
    else if (controlId == "UtilCompactRepairButton")
        compactRepair();
    else if (controlId == "UtilDecompileAndCompactRepairButton")
        decompileAndCompactRepair();
    else
        return false;
    return true;
}

void AccessUtilController::decompile()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doDecompile()) );
}

void AccessUtilController::compactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doCompactRepair()) );
}

void AccessUtilController::decompileAndCompactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doDecompileAndCompactRepair()) );
}

void AccessUtilController::doDecompile()
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

void AccessUtilController::doCompactRepair()
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

void AccessUtilController::doDecompileAndCompactRepair()
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

bool AccessUtilController::getCurrentFileName(QString &fileName)
{
    fileName = "";
    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (currentProject.is())
        fileName = currentProject->FullName();

    if (fileName.isEmpty())
    {
        QMessageBox::information(m_parentWidget, "", tr("no project is opened!"));
        return false;
    }
    return true;
}

