#include "accesscontroller.h"

#include "accesslib/accesslib.h"
#include "util/comptr.h"
#include "util/threadedinvoker.h"
#include "accessmodel/accessmodel.h"
#include "view/uiblocker.h"
#include "addinutil/addinutil.h"

#include <QMessageBox>

// for openInExplorer
#include <QProcess>
#include <QFileInfo>
#include <QDir>

#include "pch.hpp"

AccController::AccController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
    , m_uiBlocker(0)
    , m_threadedInvoker(0)
{
}

AccController::~AccController()
{
    delete m_uiBlocker;
    delete m_threadedInvoker;
}

void AccController::initialize(QAxObject *application, QWidget *parentWidget)
{
    m_application = static_cast<Access::Application*>(application);
    m_parentWidget = parentWidget;
    m_threadedInvoker = new ThreadedInvoker(100, this);

    m_uiBlocker = new UiBlocker(m_parentWidget, this);
    m_uiBlocker->setText(tr("please wait ..."));

    connect(m_threadedInvoker, SIGNAL(finished()), m_uiBlocker, SLOT(hide()) );
}

QString AccController::ribbonXml()
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
            "<button id=\"UtilOpenInExplorerButton\" "
            "   size=\"normal\" "
            "   label=\"Open in Explorer\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"UtilOpenInConsoleButton\" "
            "   size=\"normal\" "
            "   label=\"Open in Console\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
        "</group>"
        ;
    return content;
}

bool AccController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
{
    Q_UNUSED(controlId)
    Q_UNUSED(imagePath)
    Q_UNUSED(size)
    return false;
}

bool AccController::handleButtonClick(const QString &controlId)
{
    if (controlId == "UtilDecompileButton")
        decompile();
    else if (controlId == "UtilCompactRepairButton")
        compactRepair();
    else if (controlId == "UtilDecompileAndCompactRepairButton")
        decompileAndCompactRepair();

    else if (controlId == "UtilOpenInExplorerButton")
        openInExplorer();
    else if (controlId == "UtilOpenInConsoleButton")
        openInConsole();

    else
        return false;
    return true;
}

void AccController::decompile()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doDecompile()) );
}

void AccController::compactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doCompactRepair()) );
}

void AccController::decompileAndCompactRepair()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    m_uiBlocker->show();
    m_threadedInvoker->start(this, SLOT(doDecompileAndCompactRepair()) );
}

void AccController::openInExplorer()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    const QString explorer = "explorer.exe";

    QString param;
    if (!QFileInfo(fileName).isDir())
        param = QLatin1String("/select,");
    param += QDir::toNativeSeparators(fileName);
    QString command = explorer + " " + param;
    QProcess::startDetached(command);

}

void AccController::openInConsole()
{
    QString fileName;
    if (!getCurrentFileName(fileName))
        return;

    QString driveLetter;
    QStringList drives = fileName.split(':');
    if (drives.length() > 0)
        driveLetter = drives[ 0 ];
    if (driveLetter.isEmpty())
        return;

    const QString cmd = "cmd.exe";

    QString param;
    param += QDir::toNativeSeparators( QFileInfo(fileName).absolutePath()  );
    QString command = cmd + " /k " + driveLetter + ": && cd " + param;
    QProcess::startDetached(command);
}

void AccController::doDecompile()
{
    QString fileName;
    if (getCurrentFileName(fileName))
    {
        m_application->CloseCurrentDatabase();

        AccessModel au;
        quint64 currentThreadId = au.getAccessThreadId(m_application);
        au.decompile(fileName, currentThreadId);

        au.openCurrentDatabase(m_application, fileName);
    }
    m_threadedInvoker->finish();
}

void AccController::doCompactRepair()
{
    QString fileName;
    if (getCurrentFileName(fileName))
    {
        m_application->CloseCurrentDatabase();

        AccessModel au;
        au.compactRepair(m_application, fileName, 1);

        au.openCurrentDatabase(m_application, fileName);
    }
    m_threadedInvoker->finish();
}

void AccController::doDecompileAndCompactRepair()
{
    QString fileName;
    if (getCurrentFileName(fileName))
    {
        m_application->CloseCurrentDatabase();

        AccessModel au;
        quint64 currentThreadId = au.getAccessThreadId(m_application);
        au.decompile(fileName, currentThreadId);
        au.compactRepair(m_application, fileName, 1);

        au.openCurrentDatabase(m_application, fileName);
    }
    m_threadedInvoker->finish();
}

bool AccController::getCurrentFileName(QString &fileName)
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

