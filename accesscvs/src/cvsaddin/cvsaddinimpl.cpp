#include "cvsaddinimpl.h"

#include <QResource>
#include <QFile>
#include <QTextStream>

#include "addin/comutil.h"
#include "addin/addinutil.h"
#include "git/gitmanager.h"
#include "managers/actionmanager.h"
#include "managers/accessutilmanager.h"
#include "managers/windowwidgetmanager.h"

CvsAddInImpl::CvsAddInImpl(QObject *parent)
    : AddInImpl(parent)
    , m_actionManager(0)
    , m_gitManager(0)
    , m_accessUtilManager(0)
    , m_winWidgetManager(0)
{

}

void CvsAddInImpl::onConnectionEvent()
{
    Q_INIT_RESOURCE(resource);

    m_winWidgetManager = new WindowWidgetManager(application(), this);
    m_actionManager = new ActionManager(application(), m_winWidgetManager, this);
    m_gitManager = new GitManager(application(), this);
    m_accessUtilManager = new AccessUtilManager(application(), m_winWidgetManager, this);
}

void CvsAddInImpl::onDisconnectionEvent()
{
    if (m_actionManager)
    {
        delete m_actionManager;
        m_actionManager = 0;
    }

    if (m_gitManager)
    {
        delete m_gitManager;
        m_gitManager = 0;
    }

    if (m_accessUtilManager)
    {
        delete m_accessUtilManager;
        m_accessUtilManager = NULL;
    }

    if (m_winWidgetManager)
    {
        delete m_winWidgetManager;
        m_winWidgetManager = 0;
    }

    Q_CLEANUP_RESOURCE(resource);
}

QString CvsAddInImpl::ribbomXml()
{
    QResource resource(":/addin/ribbon.xml");
    QFile file(resource.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream in( &file );
    QString content = in.readAll();
    file.close();

    return content;
}

IPictureDisp *CvsAddInImpl::buttonImage(const QString &controlId)
{
    // determine icon image path and size
    QString imagePath = ":/images/";
    QSize size(16,16);
    // Standard
    if ( controlId == "StandardManualButton")
    {
        imagePath += "manual.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else if ( controlId == "StandardExportButton")
    {
        imagePath += "export.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else if ( controlId == "StandardImportButton")
    {
        imagePath += "import.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    // Git
    else if ( controlId == "GitInitButton")
    {
        imagePath += "git-init.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else if ( controlId == "GitIgnoreButton")
    {
        imagePath += "git-update-gitignore.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else
        return NULL;

    // load picutre
    return ComUtil::loadPictureFromSvg( imagePath, size );
}

HRESULT CvsAddInImpl::onButtonClicked(const QString &controlId)
{
    if (controlId == "StandardManualButton")
        m_actionManager->manual();
    else if (controlId == "StandardExportButton")
        m_actionManager->autoExport();
    else if (controlId == "StandardImportButton")
        m_actionManager->autoImport();

    else if (controlId == "GitInitButton")
        m_gitManager->init();
    else if (controlId == "GitIgnoreButton")
        m_gitManager->gitIgnore();
    else if (controlId == "GitManageRemotesButton")
        m_gitManager->manageRemotes();
    else if (controlId == "GitPullButton")
        m_gitManager->pull();

    else if (controlId == "UtilDecompileButton")
        m_accessUtilManager->decompile();
    else if (controlId == "UtilCompactRepairButton")
        m_accessUtilManager->compactRepair();
    else if (controlId == "UtilDecompileAndCompactRepairButton")
        m_accessUtilManager->decompileAndCompactRepair();

    return S_OK;
}

