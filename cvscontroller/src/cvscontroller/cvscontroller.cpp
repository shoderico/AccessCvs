#include "cvscontroller.h"

#include "view/maindialog.h"
#include "officelib/officelib.h"
#include "addinutil/addinutil.h"

CvsController::CvsController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
    , m_dlg(0)
{
}

CvsController::~CvsController()
{
    if (m_dlg)
    {
        m_dlg->close();
        delete m_dlg;
        m_dlg = 0;
    }
}

void CvsController::initialize(Access::Application *application, QWidget *parentWidget)
{
    m_application = application;
    m_parentWidget = parentWidget;
    init();
}

QString CvsController::ribbonXml()
{
    QString content;
    content =
        "<group id=\"StandardGroup\" "
            "label=\"Standard\"> "
            "<button id=\"StandardExportButton\" "
            "   size=\"large\" "
            "   label=\"Export\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"StandardImportButton\" "
            "   size=\"large\" "
            "   label=\"Import\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"StandardManualButton\" "
            "   size=\"large\" "
            "   label=\"Manual\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
        "</group>"
        ;
    return content;
}

bool CvsController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
{
    // Standard
    if ( controlId == "StandardManualButton")
    {
        imagePath = ":/images/manual.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else if ( controlId == "StandardExportButton")
    {
        imagePath = ":/images/export.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else if ( controlId == "StandardImportButton")
    {
        imagePath = ":/images/import.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Large);
    }
    else
        return false;

    return true;
}

bool CvsController::handleButtonClick(const QString &controlId)
{
    if (controlId == "StandardManualButton")
        manual();
    else if (controlId == "StandardExportButton")
        autoExport();
    else if (controlId == "StandardImportButton")
        autoImport();
    else
        return false;
    return true;
}

void CvsController::manual()
{
    m_dlg->showAsManual();
}

void CvsController::autoImport()
{
    m_dlg->showAsAutoImport();
}

void CvsController::autoExport()
{
    m_dlg->showAsAutoExport();
}

void CvsController::init()
{
    if (!m_dlg)
    {
        m_dlg = new MainDialog( m_application, m_parentWidget );
    }
}

