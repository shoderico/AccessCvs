#include "maindialog.h"
#include "ui_maindialog.h"

#include "accesslib/accesslib.h"

#include "cvsmodel/objectmodel.h"
#include "cvsmodel/objectitem.h"
#include "cvsmodel/objectproxymodel.h"

#include "view/checkboxitemdelegate.h"
#include "hasdatacolumnitemdelegate.h"
#include "progresshelper.h"

#include "util/comptr.h"

#include <QMessageBox>

#include <QMetaMethod>
#include <QApplication>

using namespace Access;

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
    ui->treeView->setItemDelegateForColumn( ObjectModel::InProjectColumn,     new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( ObjectModel::InSourceDirColumn,   new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( ObjectModel::DifferentColumn,     new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( ObjectModel::HasDataColumn,       new HasDataColumnItemDelegate() );
    ui->treeView->setColumnWidth(ObjectModel::InProjectColumn,    35);
    ui->treeView->setColumnWidth(ObjectModel::InSourceDirColumn, 35);
    ui->treeView->setColumnWidth(ObjectModel::DifferentColumn,    35);
    ui->treeView->setColumnWidth(ObjectModel::HasDataColumn,      35);
    ui->treeView->setColumnWidth(ObjectModel::NameColumn, 200);
    ui->treeView->setColumnWidth(ObjectModel::CreateDateColumn, 150);
    ui->treeView->setColumnWidth(ObjectModel::UpdateDateColumn, 150);
    ui->treeView->setColumnWidth(ObjectModel::ExportDateColumn, 150);

    ui->treeView->setSortingEnabled(true);
    ui->treeView->sortByColumn(ObjectModel::NameColumn, Qt::AscendingOrder);

    connect( ui->okButton,     SIGNAL(clicked(bool)), this, SLOT(onAccepted()) );
    connect( ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onRejected()) );

    connect( ui->refreshItemsButton,  SIGNAL(clicked(bool)), this, SIGNAL(refreshItems()) );
    connect( ui->clearCacheButton,    SIGNAL(clicked(bool)), this, SIGNAL(clearCache()) );
    connect( ui->executeExportButton, SIGNAL(clicked(bool)), this, SIGNAL(executeExport()) );
    connect( ui->executeImportButton, SIGNAL(clicked(bool)), this, SIGNAL(executeImport()) );
    connect( ui->selectAutoButton,    SIGNAL(clicked(bool)), this, SIGNAL(selectAuto()) );

    connect( ui->selectAllCheckBox,         SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectTableCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectQueryCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectFormCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectReportCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectMacroCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectModuleCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectReferenceCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectProjectFileCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectVBProjectCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );


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

    // default value
    ui->showAllCheckBox->setChecked(true);
    ui->showTableCheckBox->setChecked(true);
    ui->showQueryCheckBox->setChecked(true);
    ui->showFormCheckBox->setChecked(true);
    ui->showReportCheckBox->setChecked(true);
    ui->showMacroCheckBox->setChecked(true);
    ui->showModuleCheckBox->setChecked(true);
    ui->showReferenceCheckBox->setChecked(true);
    ui->showProjectFileCheckBox->setChecked(true);
    ui->showVBProjectCheckBox->setChecked(true);
    ui->showSelectedOnlyCheckBox->setChecked(true);


    m_proxyModel->setFilterShowObjectType( ObjectModel::AllObjectTypes );
    m_proxyModel->setFilterShowSelectedOnly( true/*selected*/ );

    connect(m_proxyModel, SIGNAL(showSelectedOnlyChanged(bool)), ui->showSelectedOnlyCheckBox, SLOT(setChecked(bool)) );
    connect(m_proxyModel, SIGNAL(showObjectTypeChanged(int)), this, SLOT(setShownObjectType(int)) );

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

    if (checkBox == ui->selectAllCheckBox)
    {
        m_model->selectItems( ObjectModel::AllItems, selected , true );

        ui->selectTableCheckBox->setChecked( selected );
        ui->selectQueryCheckBox->setChecked( selected );
        ui->selectFormCheckBox->setChecked( selected );
        ui->selectReportCheckBox->setChecked( selected );
        ui->selectMacroCheckBox->setChecked( selected );
        ui->selectModuleCheckBox->setChecked( selected );
        ui->selectReferenceCheckBox->setChecked( selected );
        ui->selectProjectFileCheckBox->setChecked( selected );
        ui->selectVBProjectCheckBox->setChecked( selected );
    }
    else if (checkBox == ui->selectTableCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::TableObjectType, selected, false );
    }
    else if (checkBox == ui->selectQueryCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::QueryObjectType, selected, false );
    }
    else if (checkBox == ui->selectFormCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::FormObjectType, selected, false );
    }
    else if (checkBox == ui->selectReportCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::ReportObjectType, selected, false );
    }
    else if (checkBox == ui->selectMacroCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::MacroObjectType, selected, false );
    }
    else if (checkBox == ui->selectModuleCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::ModuleObjectType, selected, false );
    }
    else if (checkBox == ui->selectReferenceCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::ReferenceObjectType, selected, false );
    }
    else if (checkBox == ui->selectProjectFileCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::ProjectFileType, selected, false );
    }
    else if (checkBox == ui->selectVBProjectCheckBox)
    {
        m_model->selectItemsByObjectType( ObjectModel::VBProjectType, selected, false );
    }
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

//    if (checkBox == ui->showAllCheckBox)
//    {
//        ui->showTableCheckBox->setChecked( selected );
//        ui->showQueryCheckBox->setChecked( selected );
//        ui->showFormCheckBox->setChecked( selected );
//        ui->showReportCheckBox->setChecked( selected );
//        ui->showMacroCheckBox->setChecked( selected );
//        ui->showModuleCheckBox->setChecked( selected );
//        ui->showReferenceCheckBox->setChecked( selected );
//        ui->showProjectFileCheckBox->setChecked( selected );
//        ui->showVBProjectCheckBox->setChecked( selected );
//    }

//    int objectTypes = 0;

//    if (ui->showTableCheckBox->checkState() == Qt::Checked)     objectTypes |= ObjectModel::TableObjectType;
//    if (ui->showQueryCheckBox->checkState() == Qt::Checked)     objectTypes |= ObjectModel::QueryObjectType;
//    if (ui->showFormCheckBox->checkState() == Qt::Checked)      objectTypes |= ObjectModel::FormObjectType;
//    if (ui->showReportCheckBox->checkState() == Qt::Checked)    objectTypes |= ObjectModel::ReportObjectType;
//    if (ui->showMacroCheckBox->checkState() == Qt::Checked)     objectTypes |= ObjectModel::MacroObjectType;
//    if (ui->showModuleCheckBox->checkState() == Qt::Checked)    objectTypes |= ObjectModel::ModuleObjectType;
//    if (ui->showReferenceCheckBox->checkState() == Qt::Checked) objectTypes |= ObjectModel::ReferenceObjectType;
//    if (ui->showProjectFileCheckBox->checkState() == Qt::Checked) objectTypes |= ObjectModel::ProjectFileType;
//    if (ui->showVBProjectCheckBox->checkState() == Qt::Checked) objectTypes |= ObjectModel::VBProjectType;

//    m_proxyModel->setFilterShowObjectType( objectTypes );

    int objectType = 0;
    if (ui->showTableCheckBox        == checkBox) objectType = ObjectModel::TableObjectType;
    if (ui->showQueryCheckBox        == checkBox) objectType = ObjectModel::QueryObjectType;
    if (ui->showFormCheckBox         == checkBox) objectType = ObjectModel::FormObjectType;
    if (ui->showReportCheckBox       == checkBox) objectType = ObjectModel::ReportObjectType;
    if (ui->showMacroCheckBox        == checkBox) objectType = ObjectModel::MacroObjectType;
    if (ui->showModuleCheckBox       == checkBox) objectType = ObjectModel::ModuleObjectType;
    if (ui->showReferenceCheckBox    == checkBox) objectType = ObjectModel::ReferenceObjectType;
    if (ui->showProjectFileCheckBox  == checkBox) objectType = ObjectModel::ProjectFileType;
    if (ui->showVBProjectCheckBox    == checkBox) objectType = ObjectModel::VBProjectType;
    emit showObject(objectType, selected);
}

void MainDialog::showSelectedOnlyCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit showSelectedOnly(selected);
}

void MainDialog::setShownObjectType(int objectTypes)
{
    ui->showAllCheckBox         ->setChecked( objectTypes == ObjectModel::AllObjectTypes );

    ui->showTableCheckBox       ->setChecked( objectTypes & ObjectModel::TableObjectType );
    ui->showQueryCheckBox       ->setChecked( objectTypes & ObjectModel::QueryObjectType );
    ui->showFormCheckBox        ->setChecked( objectTypes & ObjectModel::FormObjectType );
    ui->showReportCheckBox      ->setChecked( objectTypes & ObjectModel::ReportObjectType );
    ui->showMacroCheckBox       ->setChecked( objectTypes & ObjectModel::MacroObjectType );
    ui->showModuleCheckBox      ->setChecked( objectTypes & ObjectModel::ModuleObjectType );
    ui->showReferenceCheckBox   ->setChecked( objectTypes & ObjectModel::ReferenceObjectType );
    ui->showProjectFileCheckBox ->setChecked( objectTypes & ObjectModel::ProjectFileType );
    ui->showVBProjectCheckBox   ->setChecked( objectTypes & ObjectModel::VBProjectType );
}
