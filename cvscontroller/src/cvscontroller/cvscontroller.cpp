#include "cvscontroller.h"

#include "view/maindialog.h"
#include "view/cvsprogressdialog.h"

#include "accesslib/accesslib.h"

#include "addinutil/addinutil.h"

//#include "cvsmodel/objectmodel.h"
//#include "cvsmodel/objectproxymodel.h"
#include "cvsmodel/accessobjectmodel.h"
#include "cvsmodel/accessobjectproxymodel.h"


CvsController::CvsController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_parentWidget(0)
    , m_dlg(0)
    , m_progressDlg(0)
    , m_model(0)
    , m_proxyModel(0)
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
    m_dlg->showAsManual();
}

void CvsController::prepareImport()
{
    prepare(Import, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoImport();
}

void CvsController::prepareExport()
{
    prepare(Export, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoExport();
}

void CvsController::clearCacheAndPrepareImport()
{
    prepare(Import, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoImport();
}

void CvsController::clearCacheAndPrepareExport()
{
    prepare(Export, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_dlg->showAsAutoExport();
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
    m_dlg->beginBatch();
    m_model->refreshItems();
    m_dlg->endBatch();
}

void CvsController::executeExport()
{
    m_dlg->beginBatch();
    m_model->executeExport();
    m_dlg->endBatch();
}

void CvsController::executeImport()
{
    m_dlg->beginBatch();
    m_model->executeImport();
    m_dlg->endBatch();
}

void CvsController::init()
{
    if (!m_model)
    {
        m_model = new AccessObjectModel(this);
        m_model->setApplication(m_application);
    }

    if (!m_proxyModel)
    {
        m_proxyModel = new AccessObjectProxyModel(this);
        m_proxyModel->setSourceModel(m_model);
    }

    if (!m_dlg)
    {
        m_dlg = new MainDialog( m_model, m_proxyModel, m_parentWidget );
    }

    if (!m_progressDlg)
    {
        m_progressDlg = new CvsProgressDialog( m_model, m_parentWidget );
    }

    connect(m_dlg, SIGNAL(selectAuto()), this, SLOT(selectAuto()) );
    connect(m_dlg, SIGNAL(clearCache()), this, SLOT(clearCache()) );
    connect(m_dlg, SIGNAL(refreshItems()), this, SLOT(refreshItems()) );
    connect(m_dlg, SIGNAL(executeExport()), this, SLOT(executeExport()) );
    connect(m_dlg, SIGNAL(executeImport()), this, SLOT(executeImport()) );
    connect(m_dlg, SIGNAL(showSelectedOnly(bool)), this, SLOT(showSelectedOnly(bool)) );
    connect(m_dlg, SIGNAL(showAllObject(bool)), this, SLOT(showAllObject(bool)) );
    connect(m_dlg, SIGNAL(showObject(int,bool)), this, SLOT(showObject(int,bool)) );
    connect(m_dlg, SIGNAL(selectAllObject(bool)), this, SLOT(selectAllObject(bool)) );
    connect(m_dlg, SIGNAL(selectObject(int,bool)), this, SLOT(selectObject(int,bool)) );

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

