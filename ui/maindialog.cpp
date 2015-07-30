#include "maindialog.h"
#include "ui_maindialog.h"

#include "officelib/officelib.h"

#include "model/objectmodel.h"
#include "model/objectitem.h"

#include "view/checkboxitemdelegete.h"

#include "util/comptr.h"

#include <QMessageBox>


using namespace Access;

MainDialog::MainDialog(IUnknown *application, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
  , m_pApplication( new Application(application, parent) )
{
    ui->setupUi(this);


    m_model = new ObjectModel(this);
    m_model->setApplication(m_pApplication);
    m_model->loadFromProject();
    m_model->loadFromFileSystem();
//    m_model->exportToTempDir();

    ui->treeView->setModel(m_model);
    ui->treeView->setIndentation(0);
    ui->treeView->setItemDelegateForColumn( ObjectModel::InProjectColumn,     new CheckBoxItemDelegete() );
    ui->treeView->setItemDelegateForColumn( ObjectModel::InFileSystemColumn,  new CheckBoxItemDelegete() );
    ui->treeView->setColumnWidth(ObjectModel::InProjectColumn, 24);
    ui->treeView->setColumnWidth(ObjectModel::InFileSystemColumn, 24);
    ui->treeView->setColumnWidth(ObjectModel::NameColumn, 200);

    connect( ui->okButton, SIGNAL(clicked(bool)), this, SLOT(accept()) );
    connect( ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()) );
    connect( ui->exportButton, SIGNAL(clicked(bool)), this, SLOT(exportObjects()) );

    connect( m_model, SIGNAL(processStart(ObjectModel::ProcessData)), this, SLOT(processStart(ObjectModel::ProcessData)) );
    connect( m_model, SIGNAL(processEnd(ObjectModel::ProcessData)), this, SLOT(processEnd(ObjectModel::ProcessData)) );
    connect( m_model, SIGNAL(subProcessStart(ObjectModel::ProcessData,ObjectModel::SubProcessData)), this, SLOT(subProcessStart(ObjectModel::ProcessData,ObjectModel::SubProcessData)) );
    connect( m_model, SIGNAL(subProcessEnd(ObjectModel::ProcessData,ObjectModel::SubProcessData)), this, SLOT(subProcessEnd(ObjectModel::ProcessData,ObjectModel::SubProcessData)) );
    connect( m_model, SIGNAL(subProcessProgess(ObjectModel::ProcessData,ObjectModel::SubProcessData)), this, SLOT(subProcessProgress(ObjectModel::ProcessData,ObjectModel::SubProcessData)) );

    ui->progressBar->reset();

}

MainDialog::~MainDialog()
{
    delete ui;
    if (m_pApplication)
        delete m_pApplication;
}

void MainDialog::exportObjects()
{
    // export objects grouped by object-type
    m_model->exportToTempDir();
    //    m_model->exportToTempDirTableDef();
}

void MainDialog::processStart(ObjectModel::ProcessData progressData)
{
//    QMessageBox::information(this, QString(), QString("MainDialog::processEnd"));
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum( progressData.count );
}

void MainDialog::processEnd(ObjectModel::ProcessData progressData)
{
    Q_UNUSED(progressData);
    ui->progressBar->setValue(0);
//    QMessageBox::information(this, QString(), QString("MainDialog::processEnd"));
}

void MainDialog::subProcessStart(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData)
{
    Q_UNUSED(progressData);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(subProcessData.count);
}

void MainDialog::subProcessEnd(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData)
{
    Q_UNUSED(progressData);
    Q_UNUSED(subProcessData);
    ui->progressBar->setValue(0);
//    QMessageBox::information(this, QString(), QString("MainDialog::subProcessEnd"));
}

void MainDialog::subProcessProgress(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData)
{
    Q_UNUSED(progressData);
    ui->progressBar->setValue(subProcessData.position);
}
