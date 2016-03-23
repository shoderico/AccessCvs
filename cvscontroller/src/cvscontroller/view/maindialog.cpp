#include "maindialog.h"
#include "ui_maindialog.h"

#include "cvsmodel/objectmodel.h"
#include "cvsmodel/objectproxymodel.h"

#include "view/checkboxitemdelegate.h"
#include "hasdatacolumnitemdelegate.h"
#include "progresshelper.h"

#include <QSignalBlocker>

MainDialog::MainDialog(ObjectModel *model, ObjectProxyModel *proxyModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
  , m_model(model)
  , m_proxyModel(proxyModel)
  , m_showMode( UnkownMode )
  , m_progressHelper( new ProgressHelper(this) )
{
    ui->setupUi(this);

    // FIXME: to be canceled

    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setIndentation(0);
    ui->treeView->setItemDelegateForColumn( Model::InProjectColumn,     new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( Model::InSourceDirColumn,   new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( Model::DifferentColumn,     new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( Model::HasDataColumn,       new HasDataColumnItemDelegate() );
    ui->treeView->setColumnWidth(Model::InProjectColumn,    35);
    ui->treeView->setColumnWidth(Model::InSourceDirColumn, 35);
    ui->treeView->setColumnWidth(Model::DifferentColumn,    35);
    ui->treeView->setColumnWidth(Model::HasDataColumn,      35);
    ui->treeView->setColumnWidth(Model::NameColumn, 200);
    ui->treeView->setColumnWidth(Model::CreateDateColumn, 150);
    ui->treeView->setColumnWidth(Model::UpdateDateColumn, 150);
    ui->treeView->setColumnWidth(Model::ExportDateColumn, 150);

    ui->treeView->setSortingEnabled(true);
    ui->treeView->sortByColumn(Model::NameColumn, Qt::AscendingOrder);

    // command
    connect( ui->okButton,     SIGNAL(clicked(bool)), this, SLOT(onAccepted()) );
    connect( ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onRejected()) );

    connect( ui->refreshItemsButton,  SIGNAL(clicked(bool)), this, SIGNAL(refreshItems()) );
    connect( ui->clearCacheButton,    SIGNAL(clicked(bool)), this, SIGNAL(clearCache()) );
    connect( ui->executeExportButton, SIGNAL(clicked(bool)), this, SIGNAL(executeExport()) );
    connect( ui->executeImportButton, SIGNAL(clicked(bool)), this, SIGNAL(executeImport()) );

    // selection
    connect( ui->selectAutoButton,    SIGNAL(clicked(bool)), this, SIGNAL(selectAuto()) );

    connect( ui->selectAllCheckBox,         SIGNAL(stateChanged(int)), this, SLOT(selectAllCheckStateChanged(int)) );

    connect( ui->selectTableCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectQueryCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectFormCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectReportCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectMacroCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectModuleCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectReferenceCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectProjectFileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectVBProjectCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );

    // filter
    connect( ui->showSelectedOnlyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showSelectedOnlyCheckStateChanged(int)) );

    connect( ui->showAllCheckBox,         SIGNAL(stateChanged(int)), this, SLOT(showAllCheckStateChanged(int)) );

    connect( ui->showTableCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showQueryCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showFormCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showReportCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showMacroCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showModuleCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showReferenceCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showProjectFileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showVBProjectCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );

    m_progressHelper->initialize(
                  ui->elapsedTimeLabel
                , ui->processTypeNameLabel
                , ui->progressCountLabel
                , ui->progressBar
                , m_model
                );

    connect(m_proxyModel, SIGNAL(showSelectedOnlyChanged(bool)), ui->showSelectedOnlyCheckBox, SLOT(setChecked(bool)) );
    connect(m_proxyModel, SIGNAL(showObjectTypeChanged(int)), this, SLOT(setShownObjectType(int)) );

    connect(m_model, SIGNAL(selectionChanged(int)), this, SLOT(setSelectObjectType(int)) );
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::showAsManual()
{
    m_showMode = ManualMode;
    ui->executeExportButton->show();
    ui->executeImportButton->show();
    ui->okButton->setText(tr("OK"));
    setWindowTitle(tr("Manual"));
    show();
}

void MainDialog::showAsAutoExport()
{
    m_showMode = AutoExportMode;
    ui->executeExportButton->hide();
    ui->executeImportButton->hide();
    ui->okButton->setText(tr("Export"));
    setWindowTitle(tr("Export"));
    show();
}

void MainDialog::showAsAutoImport()
{
    m_showMode = AutoImportMode;
    ui->executeExportButton->hide();
    ui->executeImportButton->hide();
    ui->okButton->setText(tr("Import"));
    setWindowTitle(tr("Import"));
    show();
}

void MainDialog::onAccepted()
{
    switch (m_showMode) {
        case ManualMode:
        {
            accept();
            break;
        }

        case AutoExportMode:
        {
            emit executeExport();
            accept();
            break;
        }

        case AutoImportMode:
        {
            emit executeImport();
            accept();
            break;
        }

        default:
        {
            accept();
            break;
        }
    }
}

void MainDialog::onRejected()
{
    endBatch();
    reject();
}

void MainDialog::selectAllCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit selectAllObject(selected);
}

void MainDialog::beginBatch()
{
    m_progressHelper->beginBatch();
}

void MainDialog::endBatch()
{
    m_progressHelper->endBatch();
}

void MainDialog::selectCheckStateChanged(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox)
        return;

    bool selected = (state == Qt::Checked);
    int objectType = checkBoxToObjectType(checkBox);
    emit selectObject(objectType, selected);
}

void MainDialog::showAllCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit showAllObject(selected);
}

void MainDialog::showCheckStateChanged(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox)
        return;
    bool selected = (state == Qt::Checked);

    int objectType = checkBoxToObjectType(checkBox);
    emit showObject(objectType, selected);
}

void MainDialog::showSelectedOnlyCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit showSelectedOnly(selected);
}

void MainDialog::setShownObjectType(int objectTypes)
{
    const QSignalBlocker blockAll( ui->showAllCheckBox );
    const QSignalBlocker blockTable( ui->showTableCheckBox );
    const QSignalBlocker blockQuery( ui->showQueryCheckBox );
    const QSignalBlocker blockForm( ui->showFormCheckBox );
    const QSignalBlocker blockReport( ui->showReportCheckBox );
    const QSignalBlocker blockMacro( ui->showMacroCheckBox );
    const QSignalBlocker blockModule( ui->showModuleCheckBox );
    const QSignalBlocker blockReference( ui->showReferenceCheckBox );
    const QSignalBlocker blockProjectFile( ui->showProjectFileCheckBox );
    const QSignalBlocker blockVBProject( ui->showVBProjectCheckBox );

    ui->showAllCheckBox         ->setChecked( objectTypes == Model::AllObjectTypes );

    ui->showTableCheckBox       ->setChecked( objectTypes & Model::TableObjectType );
    ui->showQueryCheckBox       ->setChecked( objectTypes & Model::QueryObjectType );
    ui->showFormCheckBox        ->setChecked( objectTypes & Model::FormObjectType );
    ui->showReportCheckBox      ->setChecked( objectTypes & Model::ReportObjectType );
    ui->showMacroCheckBox       ->setChecked( objectTypes & Model::MacroObjectType );
    ui->showModuleCheckBox      ->setChecked( objectTypes & Model::ModuleObjectType );
    ui->showReferenceCheckBox   ->setChecked( objectTypes & Model::ReferenceObjectType );
    ui->showProjectFileCheckBox ->setChecked( objectTypes & Model::ProjectFileType );
    ui->showVBProjectCheckBox   ->setChecked( objectTypes & Model::VBProjectType );
}

void MainDialog::setSelectObjectType(int objectTypes)
{
    const QSignalBlocker blockAll( ui->selectAllCheckBox );
    const QSignalBlocker blockTable( ui->selectTableCheckBox );
    const QSignalBlocker blockQuery( ui->selectQueryCheckBox );
    const QSignalBlocker blockForm( ui->selectFormCheckBox );
    const QSignalBlocker blockReport( ui->selectReportCheckBox );
    const QSignalBlocker blockMacro( ui->selectMacroCheckBox );
    const QSignalBlocker blockModule( ui->selectModuleCheckBox );
    const QSignalBlocker blockReference( ui->selectReferenceCheckBox );
    const QSignalBlocker blockProjectFile( ui->selectProjectFileCheckBox );
    const QSignalBlocker blockVBProject( ui->selectVBProjectCheckBox );

    ui->selectAllCheckBox         ->setChecked( objectTypes == Model::AllObjectTypes );

    ui->selectTableCheckBox       ->setChecked( objectTypes & Model::TableObjectType );
    ui->selectQueryCheckBox       ->setChecked( objectTypes & Model::QueryObjectType );
    ui->selectFormCheckBox        ->setChecked( objectTypes & Model::FormObjectType );
    ui->selectReportCheckBox      ->setChecked( objectTypes & Model::ReportObjectType );
    ui->selectMacroCheckBox       ->setChecked( objectTypes & Model::MacroObjectType );
    ui->selectModuleCheckBox      ->setChecked( objectTypes & Model::ModuleObjectType );
    ui->selectReferenceCheckBox   ->setChecked( objectTypes & Model::ReferenceObjectType );
    ui->selectProjectFileCheckBox ->setChecked( objectTypes & Model::ProjectFileType );
    ui->selectVBProjectCheckBox   ->setChecked( objectTypes & Model::VBProjectType );
}

int MainDialog::checkBoxToObjectType(QCheckBox *checkBox) const
{
    int objectType = 0;
    if (ui->showTableCheckBox        == checkBox) objectType = Model::TableObjectType;
    if (ui->showQueryCheckBox        == checkBox) objectType = Model::QueryObjectType;
    if (ui->showFormCheckBox         == checkBox) objectType = Model::FormObjectType;
    if (ui->showReportCheckBox       == checkBox) objectType = Model::ReportObjectType;
    if (ui->showMacroCheckBox        == checkBox) objectType = Model::MacroObjectType;
    if (ui->showModuleCheckBox       == checkBox) objectType = Model::ModuleObjectType;
    if (ui->showReferenceCheckBox    == checkBox) objectType = Model::ReferenceObjectType;
    if (ui->showProjectFileCheckBox  == checkBox) objectType = Model::ProjectFileType;
    if (ui->showVBProjectCheckBox    == checkBox) objectType = Model::VBProjectType;

    if (ui->selectTableCheckBox        == checkBox) objectType = Model::TableObjectType;
    if (ui->selectQueryCheckBox        == checkBox) objectType = Model::QueryObjectType;
    if (ui->selectFormCheckBox         == checkBox) objectType = Model::FormObjectType;
    if (ui->selectReportCheckBox       == checkBox) objectType = Model::ReportObjectType;
    if (ui->selectMacroCheckBox        == checkBox) objectType = Model::MacroObjectType;
    if (ui->selectModuleCheckBox       == checkBox) objectType = Model::ModuleObjectType;
    if (ui->selectReferenceCheckBox    == checkBox) objectType = Model::ReferenceObjectType;
    if (ui->selectProjectFileCheckBox  == checkBox) objectType = Model::ProjectFileType;
    if (ui->selectVBProjectCheckBox    == checkBox) objectType = Model::VBProjectType;

    return objectType;
}
