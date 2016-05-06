#include "cvscontroller.h"

#include "view/maindialog.h"
#include "view/cvsprogressdialog.h"

#include "addinutil/addinutil.h"

#include "cvsmodel/objectmodel.h"
#include "cvsmodel/objectproxymodel.h"


CvsController::CvsController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_model(0)
    , m_proxyModel(0)
    , m_mainDlg(0)
    , m_parentWidget(0)
    , m_progressDlg(0)
{
}

CvsController::~CvsController()
{
    if (m_mainDlg)
    {
        m_mainDlg->close();
        delete m_mainDlg;
        m_mainDlg = 0;
    }

    if (m_progressDlg)
    {
        delete m_progressDlg;
    }
}

void CvsController::initialize(QAxObject *application, QWidget *parentWidget)
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
        prepareManual();
    else if (controlId == "StandardExportButton")
        prepareExport();
    else if (controlId == "StandardImportButton")
        prepareImport();
    else if (controlId == "StandardClearCacheAndExportButton")
        clearCacheAndPrepareExport();
    else if (controlId == "StandardClearCacheAndImportButton")
        clearCacheAndPrepareImport();
    else
        return false;
    return true;
}

void CvsController::prepareManual()
{
    m_mainDlg->showAsManual();
}

void CvsController::prepareImport()
{
    prepare(Import, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoImport();
}

void CvsController::prepareExport()
{
    prepare(Export, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoExport();
}

void CvsController::clearCacheAndPrepareImport()
{
    prepare(Import, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoImport();
}

void CvsController::clearCacheAndPrepareExport()
{
    prepare(Export, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoExport();
}

void CvsController::selectAuto()
{
    m_model->selectItemsForProcess( true/*selected*/, true/*resetSelection*/ );
}

void CvsController::selectAllObject(bool select)
{
    m_model->selectItems( Model::AllItems, select, true/*resetSelection*/ );
}

void CvsController::selectObject(int objectType, bool select)
{
    m_model->selectItemsByObjectType( (Model::SelectObjectType)objectType, select, false /*resetSelection*/ );
}

void CvsController::showSelectedOnly(bool selectedOnly)
{
    m_proxyModel->setFilterShowSelectedOnly( selectedOnly );
}

void CvsController::showAllObject(bool show)
{
    int objectTypes = show ? Model::AllObjectTypes : 0 ;
    m_proxyModel->setFilterShowObjectType( objectTypes );
    // TODO : update view's check state
    // model emit the signal for filter changed.
    // view slot handle it and update checkstate itself
}

void CvsController::showObject(int objectType, bool show)
{
    int objectTypes = m_proxyModel->showObjectType();
    if (show)
        objectTypes |= objectType;
    else
        objectTypes &= ~(objectType);
    m_proxyModel->setFilterShowObjectType( objectTypes );
}

void CvsController::clearCache()
{
    m_model->clearItemsCache();
}

void CvsController::refreshItems()
{
    m_mainDlg->beginBatch();
    m_model->refreshItems();
    m_mainDlg->endBatch();
}

void CvsController::executeExport()
{
    m_mainDlg->beginBatch();
    m_model->executeExport();
    m_mainDlg->endBatch();
}

void CvsController::executeImport()
{
    m_mainDlg->beginBatch();
    m_model->executeImport();
    m_mainDlg->endBatch();
}

void CvsController::init()
{
    if (!m_progressDlg)
    {
        m_progressDlg = new CvsProgressDialog( m_model, m_parentWidget );
    }

    connect(m_mainDlg, SIGNAL(selectAuto()), this, SLOT(selectAuto()) );
    connect(m_mainDlg, SIGNAL(clearCache()), this, SLOT(clearCache()) );
    connect(m_mainDlg, SIGNAL(refreshItems()), this, SLOT(refreshItems()) );
    connect(m_mainDlg, SIGNAL(executeExport()), this, SLOT(executeExport()) );
    connect(m_mainDlg, SIGNAL(executeImport()), this, SLOT(executeImport()) );
    connect(m_mainDlg, SIGNAL(showSelectedOnly(bool)), this, SLOT(showSelectedOnly(bool)) );
    connect(m_mainDlg, SIGNAL(showAllObject(bool)), this, SLOT(showAllObject(bool)) );
    connect(m_mainDlg, SIGNAL(showObject(int,bool)), this, SLOT(showObject(int,bool)) );
    connect(m_mainDlg, SIGNAL(selectAllObject(bool)), this, SLOT(selectAllObject(bool)) );
    connect(m_mainDlg, SIGNAL(selectObject(int,bool)), this, SLOT(selectObject(int,bool)) );

    m_proxyModel->setFilterShowObjectType( Model::AllObjectTypes );
    m_proxyModel->setFilterShowSelectedOnly( true/*selected*/ );
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
        m_model->selectItems(Model::AllItems, true /*selected*/, true /*resetSelection*/ );
        m_model->clearItemsCache();
    }
    m_model->refreshItems();
    m_model->selectItemsForProcess( true/*selected*/, true/*resetSelection*/ );

    m_progressDlg->endBatch();
    m_progressDlg->hide();
}

