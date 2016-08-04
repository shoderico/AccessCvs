#include "cvsaddincontroller.h"

#include "view/maindialog.h"
#include "view/cvsprogressdialog.h"

#include "addinutil/addinutil.h"

#include "cvsmodel/objectmodel.h"
#include "cvsmodel/objectproxymodel.h"

#include "view/settingdialog.h"

#include <QMessageBox>

#include "pch.hpp"


CvsAddinController::CvsAddinController(QObject *parent)
    : QObject(parent)
    , m_application(0)
    , m_model(0)
    , m_proxyModel(0)
    , m_mainDlg(0)
    , m_parentWidget(0)
    , m_progressDlg(0)
{
}

CvsAddinController::~CvsAddinController()
{
}

void CvsAddinController::initialize(QAxObject *application, QWidget *parentWidget)
{
    m_application = application;
    m_parentWidget = parentWidget;
    init();
}

QString CvsAddinController::ribbonXml()
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
            "<button id=\"StandardSettingButton\" "
            "   size=\"normal\" "
            "   label=\"Setting\" "
            "   onAction=\"ButtonClicked\" "
            "   getImage=\"GetButtonImage\" "
            "   /> "
        "</group>"
        ;
    return content;
}

bool CvsAddinController::imagePath(const QString &controlId, QString &imagePath, QSize &size)
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

bool CvsAddinController::handleButtonClick(const QString &controlId)
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
    else if (controlId == "StandardSettingButton")
        showSettingDialog();
    else
        return false;
    return true;
}

void CvsAddinController::prepareManual()
{
    m_mainDlg->showAsManual();
}

void CvsAddinController::prepareImport()
{
    prepare(Import, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoImport();
}

void CvsAddinController::prepareExport()
{
    prepare(Export, false /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoExport();
}

void CvsAddinController::clearCacheAndPrepareImport()
{
    prepare(Import, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoImport();
}

void CvsAddinController::clearCacheAndPrepareExport()
{
    prepare(Export, true /*clearCache*/);
    if ( m_model->selectedRowCount() == 0)
        return;
    m_mainDlg->showAsAutoExport();
}

void CvsAddinController::selectAuto()
{
    m_model->selectItemsForProcess( true/*selected*/, true/*resetSelection*/ );
}

void CvsAddinController::selectAllObject(bool select)
{
    m_model->selectItems( Model::AllItems, select, true/*resetSelection*/ );
}

void CvsAddinController::selectObject(int objectType, bool select)
{
    m_model->selectItemsByObjectType( (Model::SelectObjectType)objectType, select, false /*resetSelection*/ );
}

void CvsAddinController::showSelectedOnly(bool selectedOnly)
{
    m_proxyModel->setFilterShowSelectedOnly( selectedOnly );
}

void CvsAddinController::showAllObject(bool show)
{
    int objectTypes = show ? Model::AllObjectTypes : 0 ;
    m_proxyModel->setFilterShowObjectType( objectTypes );
    // TODO : update view's check state
    // model emit the signal for filter changed.
    // view slot handle it and update checkstate itself
}

void CvsAddinController::showObject(int objectType, bool show)
{
    int objectTypes = m_proxyModel->showObjectType();
    if (show)
        objectTypes |= objectType;
    else
        objectTypes &= ~(objectType);
    m_proxyModel->setFilterShowObjectType( objectTypes );
}

void CvsAddinController::clearCache()
{
    m_model->clearItemsCache();
}

void CvsAddinController::refreshItems()
{
    m_mainDlg->beginBatch();
    m_model->refreshItems();
    m_mainDlg->endBatch();
}

void CvsAddinController::executeExport()
{
    m_mainDlg->beginBatch();
    m_model->executeExport();
    m_mainDlg->endBatch();
}

void CvsAddinController::executeImport()
{
    m_mainDlg->beginBatch();
    m_model->executeImport();
    m_mainDlg->endBatch();
}

void CvsAddinController::showSettingDialog()
{
    if ( !m_model->checkProjectState() )
    {
        QMessageBox::warning(m_parentWidget, tr("OfficeCvs"), tr("Open project !"), QMessageBox::Ok );
        return;
    }
    SettingDialog dlg(m_parentWidget);
    dlg.exec();
}

void CvsAddinController::init()
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

void CvsAddinController::prepare(const CvsAddinController::PrepareType prepareType, const bool clearCache)
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

