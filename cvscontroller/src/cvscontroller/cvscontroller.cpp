#include "cvscontroller.h"

#include "view/maindialog.h"
#include "view/cvsprogressdialog.h"
#include "accesslib/accesslib.h"
#include "addinutil/addinutil.h"
#include "cvsmodel/objectmodel.h"

CvsController::CvsController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
    , m_dlg(0)
    , m_progressDlg(0)
    , m_model(0)
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

    if (m_progressDlg)
    {
        delete m_progressDlg;
    }
}

void CvsController::initialize(QAxObject *application, QWidget *parentWidget)
{
    m_application = static_cast<Access::Application*>(application);
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
            "<button id=\"StandardClearCacheAndExportButton\" "
            "   size=\"normal\" "
            "   label=\"Clear Cache &amp;&amp; Export\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
            "<button id=\"StandardClearCacheAndImportButton\" "
            "   size=\"normal\" "
            "   label=\"Clear Cache &amp;&amp; Import\" "
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
    else if ( controlId == "StandardClearCacheAndExportButton")
    {
        imagePath = ":/images/export.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
    }
    else if ( controlId == "StandardClearCacheAndImportButton")
    {
        imagePath = ":/images/import.svg";
        size = AddInUtil::ribbonIconSize(AddInUtil::Small);
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
    else if (controlId == "StandardClearCacheAndExportButton")
        clearCacheAndExport();
    else if (controlId == "StandardClearCacheAndImportButton")
        clearCacheAndImport();
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
//    m_dlg->showAsAutoImport(false);

    // model owner change
    prepare(Import, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoImport(false);
}

void CvsController::autoExport()
{
//    m_dlg->showAsAutoExport(false);

    // model owner change
    prepare(Export, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoExport(false);
}

void CvsController::clearCacheAndImport()
{
//    m_dlg->showAsAutoImport(true);

    // model owner change
    prepare(Import, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoImport(false);
}

void CvsController::clearCacheAndExport()
{
//    m_dlg->showAsAutoExport(true);

    // model owner change
    prepare(Export, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoExport(false);
}

void CvsController::init()
{
    if (!m_model)
    {
        m_model = new ObjectModel(this);
        m_model->setApplication(m_application);
    }

    if (!m_dlg)
    {
        m_dlg = new MainDialog( m_application, m_model, m_parentWidget );
    }

    if (!m_progressDlg)
    {
        m_progressDlg = new CvsProgressDialog( m_model, m_parentWidget );
    }
}

void CvsController::prepare(const CvsController::PrepareType prepareType, const bool clearCache)
{
    Q_UNUSED(prepareType)
    // this must be shown in tiny progress dialog

    QString title;
    switch (prepareType) {
        case Export: title = tr("Exporting..."); break;
        case Import: title = tr("Importing..."); break;
    }

    m_progressDlg->setWindowTitle(title);
    m_progressDlg->show();
    m_progressDlg->beginBatch();

    if (clearCache)
    {
        m_model->refreshItems();
        m_model->selectItems(ObjectModel::AllItems, true /*selected*/, true /*resetSelection*/ );
        m_model->clearItemsCache();
    }
    m_model->refreshItems();
    m_model->selectItemsForProcess( true/*selected*/, true/*resetSelection*/ );

    m_progressDlg->endBatch();
    m_progressDlg->hide();
}

