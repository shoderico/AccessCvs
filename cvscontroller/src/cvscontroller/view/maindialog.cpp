#include "maindialog.h"
#include "ui_maindialog.h"

#include "officelib/officelib.h"

#include "cvsmodel/objectmodel.h"
#include "cvsmodel/objectitem.h"
#include "cvsmodel/objectproxymodel.h"

#include "view/checkboxitemdelegate.h"
#include "view/boolcheckboxitemdelegate.h"

#include "util/comptr.h"

#include <QMessageBox>

#include <QMetaMethod>
#include <QApplication>

using namespace Access;

MainDialog::MainDialog(Access::Application *application, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
  , m_application(application)
  , m_showMode( UnkownMode )
{
    ui->setupUi(this);

    // FIXME: to be canceled

    m_model = new ObjectModel(this);
    m_proxyModel = new ObjectProxyModel(this);
    m_proxyModel->setSourceModel(m_model);

    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setIndentation(0);
    ui->treeView->setItemDelegateForColumn( ObjectModel::InProjectColumn,     new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( ObjectModel::InSourceDirColumn,   new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( ObjectModel::DifferentColumn,     new CheckBoxItemDelegate(Model::State_NoChange, Model::State_On, Model::State_Off) );
    ui->treeView->setItemDelegateForColumn( ObjectModel::HasDataColumn,       new BoolCheckBoxItemDelegate() );
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

    connect( ui->refreshItemsButton,  SIGNAL(clicked(bool)), this, SLOT(refreshItems()) );
    connect( ui->clearCacheButton, SIGNAL(clicked(bool)), this, SLOT(clearCache()) );
    connect( ui->executeExportButton, SIGNAL(clicked(bool)), this, SLOT(executeExport()) );
    connect( ui->executeImportButton, SIGNAL(clicked(bool)), this, SLOT(executeImport()) );
    connect( ui->selectAutoButton,    SIGNAL(clicked(bool)), this, SLOT(selectAuto()) );

    connect( ui->selectAllCheckBox,         SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectTableCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectQueryCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectFormCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectReportCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectMacroCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectModuleCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    connect( ui->selectReferenceCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );


    connect( ui->showSelectedOnlyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showSelectedOnly(int)) );

    connect( ui->showAllCheckBox,         SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showTableCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showQueryCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showFormCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showReportCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showMacroCheckBox,       SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showModuleCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    connect( ui->showReferenceCheckBox,   SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );

    connect( &m_progressTimer, SIGNAL(timeout()), this, SLOT(onTimeout()) );



    connect( m_model, SIGNAL(progressStart(int,int)), this, SLOT(progressStart(int,int)) );
    connect( m_model, SIGNAL(progressChange(int,int)), this, SLOT(progressChange(int,int)) );
    connect( m_model, SIGNAL(progressEnd(int)), this, SLOT(progressEnd(int)) );

    // default value
    ui->showAllCheckBox->setChecked(true);
    ui->showTableCheckBox->setChecked(true);
    ui->showQueryCheckBox->setChecked(true);
    ui->showFormCheckBox->setChecked(true);
    ui->showReportCheckBox->setChecked(true);
    ui->showMacroCheckBox->setChecked(true);
    ui->showModuleCheckBox->setChecked(true);
    ui->showReferenceCheckBox->setChecked(true);
    ui->showSelectedOnlyCheckBox->setChecked(true);


    ui->progressBar->reset();


    bool c;
    c = connect( m_application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    if (!c) QMessageBox::information(this, "", "connect exception failed");
    c = connect( m_application, SIGNAL(propertyChanged(QString)), this, SLOT(propertyChanged(QString)) );
    if (!c) QMessageBox::information(this, "", "connect propertyChanged failed");
    c = connect( m_application, SIGNAL(signal(QString,int,void*)), this, SLOT(signal(QString,int,void*)) );
    if (!c) QMessageBox::information(this, "", "connect signal failed");

    m_model->setApplication(m_application);
    m_proxyModel->setFilterShowObjectType( ObjectModel::AllObjectTypes );
    m_proxyModel->setFilterShowSelectedOnly( true/*selected*/ );

    m_processTypeNames[ ObjectModel::RefreshProcess ] = tr("RefreshProcess");
    m_processTypeNames[ ObjectModel::ExportProcess ] = tr("ExportProcess");
    m_processTypeNames[ ObjectModel::ImportProcess ] = tr("ImportProcess");
    m_processTypeNames[ ObjectModel::LoadItemFromProjectProcess ] = tr("LoadItemFromProjectProcess");
    m_processTypeNames[ ObjectModel::LoadItemFromSourceDirProcess ] = tr("LoadItemFromSourceDirProcess");
    m_processTypeNames[ ObjectModel::ExportFromProjectToTempDirProcess ] = tr("ExportFromProjectToTempDirProcess");
    m_processTypeNames[ ObjectModel::ImportFromTempDirToProjectProcess ] = tr("ImportFromTempDirToProjectProcess");
    m_processTypeNames[ ObjectModel::CopyFromTempDirToSourceDirProcess ] = tr("CopyFromTempDirToSourceDirProcess");
    m_processTypeNames[ ObjectModel::CopyFromSourceDirToTempDirProcess ] = tr("CopyFromSourceDirToTempDirProcess");
    m_processTypeNames[ ObjectModel::SanitizeTempDirProcess ] = tr("SanitizeTempDirProcess");
    m_processTypeNames[ ObjectModel::DesanitizeTempDirProcess ] = tr("DesanitizeTempDirProcess");
    m_processTypeNames[ ObjectModel::CompareTempDirProcess ] = tr("CompareTempDirProcess");
    m_processTypeNames[ ObjectModel::DeleteFromSourceDirProcess ] = tr("DeleteFromSourceDirProcess");
    m_processTypeNames[ ObjectModel::DeleteFromProjectProcess ] = tr("DeleteFromProjectProcess");
    m_processTypeNames[ ObjectModel::DeleteFromTempDirProcess ] = tr("DeleteFromTempDirProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsDifferenceByFileTimeProcess ] = tr("UpdateItemsDifferenceByFileTimeProcess");
    m_processTypeNames[ ObjectModel::UpdateFileTimeInTempDirByExportDateProcess ] = tr("UpdateFileTimeInTempDirByExportDateProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsExportDateProcess ] = tr("UpdateItemsExportDateProcess");
    m_processTypeNames[ ObjectModel::UpdateFileTimeInTempDirProcess ] = tr("UpdateFileTimeInTempDirProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsInProjectProcess ] = tr("UpdateItemsInProjectProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsInSourceDirProcess ] = tr("UpdateItemsInSourceDirProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsDifferenceProcess ] = tr("UpdateItemsDifferenceProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsDifferenceAsIsProcess ] = tr("UpdateItemsDifferenceAsIsProcess");
    m_processTypeNames[ ObjectModel::DeleteItemsProcess ] = tr("DeleteItemsProcess");
    m_processTypeNames[ ObjectModel::UpdateItemsCreateUpdateDateFromProjectProcess ] = tr("UpdateItemsCreateUpdateDateFromProjectProcess");
}

MainDialog::~MainDialog()
{
    delete ui;
    if (m_application)
    {
        disconnect( m_application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
        disconnect( m_application, SIGNAL(propertyChanged(QString)), this, SLOT(propertyChanged(QString)) );
        disconnect( m_application, SIGNAL(signal(QString,int,void*)), this, SLOT(signal(QString,int,void*)) );
    }
}

void MainDialog::showAsManual()
{
    m_showMode = ManualMode;
    ui->executeExportButton->show();
    ui->executeImportButton->show();
    ui->okButton->setText(tr("OK"));
    show();
}

void MainDialog::showAsAutoExport(const bool clearCache)
{
    m_showMode = AutoExportMode;
    ui->executeExportButton->hide();
    ui->executeImportButton->hide();
    ui->okButton->setText(tr("Export"));
    show();

    if (clearCache)
    {
        m_model->selectItems(ObjectModel::AllItems, true /*selected*/, true /*resetSelection*/ );
        m_model->clearItemsCache();
    }
    prepareExport();

    if (m_proxyModel->rowCount() == 0)
        accept();
}

void MainDialog::showAsAutoImport(const bool clearCache)
{
    m_showMode = AutoImportMode;
    ui->executeExportButton->hide();
    ui->executeImportButton->hide();
    ui->okButton->setText(tr("Import"));
    show();

    if (clearCache)
    {
        m_model->selectItems(ObjectModel::AllItems, true /*selected*/, true /*resetSelection*/ );
        m_model->clearItemsCache();
    }
    prepareImport();

    if (m_proxyModel->rowCount() == 0)
        accept();
}

void MainDialog::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    QMessageBox::information(this, "", QString().setNum(code) + "\n" + source + "\n" + desc + "\n" + help );
}

void MainDialog::propertyChanged(const QString &name)
{
    QMessageBox::information(this, "", QString("propertyChanged ") + name);
}

void MainDialog::signal(const QString &name, int argc, void *argv)
{
    Q_UNUSED(argc) Q_UNUSED(argv)
            QMessageBox::information(this, "", QString("propertyChanged ") + name);
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
            executeExport();
            accept();
            break;
        }

        case AutoImportMode:
        {
            executeImport();
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

void MainDialog::clearCache()
{
    m_model->clearItemsCache();
}

void MainDialog::refreshItems()
{
    beginBatch();

    m_model->refreshItems();

    endBatch();
}

void MainDialog::executeExport()
{
    beginBatch();

    m_model->executeExport();

    endBatch();
}

void MainDialog::executeImport()
{
    beginBatch();

    m_model->executeImport();

    endBatch();
}

void MainDialog::prepareExport()
{
    beginBatch();

    m_model->refreshItems();
    m_model->selectItemsForProcess( true/*selected*/, true/*resetSelection*/ );
    m_proxyModel->setFilterShowObjectType( ObjectModel::AllObjectTypes );
    m_proxyModel->setFilterShowSelectedOnly( true/*selected*/ );

    endBatch();
}

void MainDialog::prepareImport()
{
    beginBatch();

    m_model->refreshItems();
    m_model->selectItemsForProcess( true/*selected*/, true/*resetSelection*/ );
    m_proxyModel->setFilterShowObjectType( ObjectModel::AllObjectTypes );
    m_proxyModel->setFilterShowSelectedOnly( true/*selected*/ );

    endBatch();
}

void MainDialog::beginBatch()
{
    m_progressTime.restart();
    m_progressTimer.start(250);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->progressCountLabel->setText(tr("( 0000 / 0000 )"));
}

void MainDialog::endBatch()
{
    m_progressTimer.stop();
    onTimeout();
    QApplication::restoreOverrideCursor();
    ui->progressCountLabel->setText(tr("( 0000 / 0000 )"));
}

void MainDialog::selectAuto()
{
    m_model->selectItemsForProcess( true, true );
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
}

void MainDialog::showCheckStateChanged(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox)
        return;
    bool selected = (state == Qt::Checked);

    if (checkBox == ui->showAllCheckBox)
    {
        ui->showTableCheckBox->setChecked( selected );
        ui->showQueryCheckBox->setChecked( selected );
        ui->showFormCheckBox->setChecked( selected );
        ui->showReportCheckBox->setChecked( selected );
        ui->showMacroCheckBox->setChecked( selected );
        ui->showModuleCheckBox->setChecked( selected );
        ui->showReferenceCheckBox->setChecked( selected );
    }

    int objectTypes = 0;

    if (ui->showTableCheckBox->checkState() == Qt::Checked)     objectTypes |= ObjectModel::TableObjectType;
    if (ui->showQueryCheckBox->checkState() == Qt::Checked)     objectTypes |= ObjectModel::QueryObjectType;
    if (ui->showFormCheckBox->checkState() == Qt::Checked)      objectTypes |= ObjectModel::FormObjectType;
    if (ui->showReportCheckBox->checkState() == Qt::Checked)    objectTypes |= ObjectModel::ReportObjectType;
    if (ui->showMacroCheckBox->checkState() == Qt::Checked)     objectTypes |= ObjectModel::MacroObjectType;
    if (ui->showModuleCheckBox->checkState() == Qt::Checked)    objectTypes |= ObjectModel::ModuleObjectType;
    if (ui->showReferenceCheckBox->checkState() == Qt::Checked) objectTypes |= ObjectModel::ReferenceObjectType;

    m_proxyModel->setFilterShowObjectType( objectTypes );
}

void MainDialog::showSelectedOnly(int state)
{
    bool selected = (state == Qt::Checked);

    m_proxyModel->setFilterShowSelectedOnly( selected );
}

void MainDialog::onTimeout()
{
    int msecs = m_progressTime.elapsed() % 1000;
    int secs = m_progressTime.elapsed() / 1000;
    int mins = (secs / 60) % 60;
    int hours = (secs / 3600);
    secs = secs % 60;

    ui->elapsedTimeLabel->setText(
                QString("%1:%2:%3.%4")
                .arg(hours, 2, 10, QLatin1Char('0'))
                .arg(mins, 2, 10, QLatin1Char('0'))
                .arg(secs, 2, 10, QLatin1Char('0'))
                .arg(msecs, 3, 10, QLatin1Char('0'))
                );
}

void MainDialog::progressStart(int type, int count)
{
    ui->progressBar->setRange(0, count);
    ui->progressBar->setValue(0);
    setProcessTypeName(type);
    //QApplication::processEvents();
}

void MainDialog::progressChange(int type, int value)
{
    Q_UNUSED(type);
    ui->progressBar->setValue(value);
    ui->progressCountLabel->setText(QString("( %1 / %2 )")
                                    .arg(value, 4, 10, QLatin1Char('0'))
                                    .arg(ui->progressBar->maximum(), 4, 10, QLatin1Char('0'))
                                    );
    setProcessTypeName(type);
    QApplication::processEvents();
}

void MainDialog::progressEnd(int type)
{
    Q_UNUSED(type);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(1);
    ui->progressBar->reset();
    ui->processTypeNameLabel->setText("");
}

void MainDialog::setProcessTypeName(int type)
{
    ui->processTypeNameLabel->setText(QString("%1 ...").arg( m_processTypeNames.value(type) ));
}

