#include "maindialog.h"
#include "ui_maindialog.h"

#include "cvsmodel/processor/objectprocessor.h"
#include "cvsmodel/projectcontainer.h"
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

    foreach( Model::ObjectType objectType, model->projectContainer()->objectTypes()  )
    {
        ObjectProcessor *processor = model->projectContainer()->operator []( objectType );

        m_selectionChecks .append( new ObjectItemCheckBox( processor, this) );
        m_filterChecks    .append( new ObjectItemCheckBox( processor, this) );
    }




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
    connect( ui->selectAutoButton, SIGNAL(clicked(bool)), this, SIGNAL(selectAuto()) );
    connect( ui->selectAllCheckBox, SIGNAL(stateChanged(int)), this, SLOT(selectAllCheckStateChanged(int)) );
    foreach( ObjectItemCheckBox *checkBox, m_selectionChecks )
    {
        ui->selectionHorizontalLayout->addWidget( checkBox );
        connect( checkBox,  SIGNAL(stateChanged(int)), this, SLOT(selectCheckStateChanged(int)) );
    }

    // filter
    connect( ui->showSelectedOnlyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showSelectedOnlyCheckStateChanged(int)) );
    connect( ui->showAllCheckBox, SIGNAL(stateChanged(int)), this, SLOT(showAllCheckStateChanged(int)) );
    foreach( ObjectItemCheckBox *checkBox, m_filterChecks )
    {
        ui->filterHorizontalLayout->addWidget( checkBox );
        connect( checkBox,  SIGNAL(stateChanged(int)), this, SLOT(showCheckStateChanged(int)) );
    }



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

void MainDialog::beginBatch()
{
    m_progressHelper->beginBatch();
}

void MainDialog::endBatch()
{
    m_progressHelper->endBatch();
}



void MainDialog::selectAllCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit selectAllObject(selected);
}

void MainDialog::showAllCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit showAllObject(selected);
}

void MainDialog::showSelectedOnlyCheckStateChanged(int state)
{
    bool selected = (state == Qt::Checked);
    emit showSelectedOnly(selected);
}




void MainDialog::selectCheckStateChanged(int state)
{
    ObjectItemCheckBox *checkBox = qobject_cast<ObjectItemCheckBox *>(sender());
    if (!checkBox)
        return;

    bool selected = (state == Qt::Checked);
    int objectType = checkBox->selectObjectType();

    emit selectObject(objectType, selected);
}

void MainDialog::showCheckStateChanged(int state)
{
    ObjectItemCheckBox *checkBox = qobject_cast<ObjectItemCheckBox *>(sender());
    if (!checkBox)
        return;

    bool selected = (state == Qt::Checked);
    int objectType = checkBox->selectObjectType();

    emit showObject(objectType, selected);
}




void MainDialog::setShownObjectType(int objectTypes)
{
    // build blocker
    QList<const QSignalBlocker*> blockers;
    blockers.append( new QSignalBlocker( ui->showAllCheckBox ) );
    foreach( ObjectItemCheckBox* checkBox, m_filterChecks )
        blockers.append( new QSignalBlocker( checkBox ) );

    // set check based on objectTypes
    ui->showAllCheckBox ->setChecked( objectTypes == Model::AllObjectTypes );
    foreach( ObjectItemCheckBox* checkBox, m_filterChecks )
    {
        checkBox->setChecked( objectTypes & checkBox->selectObjectType() );
    }

    // release blocker
    qDeleteAll( blockers );
}

void MainDialog::setSelectObjectType(int objectTypes)
{
    // build blocker
    QList<const QSignalBlocker*> blockers;
    blockers.append( new QSignalBlocker( ui->selectAllCheckBox ) );
    foreach( ObjectItemCheckBox* checkBox, m_selectionChecks )
        blockers.append( new QSignalBlocker( checkBox ) );

    // set check based on objectTypes
    ui->selectAllCheckBox ->setChecked( objectTypes == Model::AllObjectTypes );
    foreach( ObjectItemCheckBox* checkBox, m_selectionChecks )
    {
        checkBox->setChecked( objectTypes & checkBox->selectObjectType() );
    }

    // release blocker
    qDeleteAll( blockers );
}


ObjectItemCheckBox::ObjectItemCheckBox(ObjectProcessor *processor, QWidget *parent)
    : QCheckBox(parent)
    , m_selectObjectType(processor->selectObjectType())
{
    if ( !processor->iconPath().isEmpty() )
        setIcon( QIcon(processor->iconPath()) );

    if ( !processor->uiText().isEmpty())
        setToolTip( processor->uiText() );

}
