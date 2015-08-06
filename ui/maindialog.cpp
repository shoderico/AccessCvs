#include "maindialog.h"
#include "ui_maindialog.h"

#include "officelib/officelib.h"

#include "model/objectmodel.h"
#include "model/objectitem.h"

#include "view/checkboxitemdelegete.h"

#include "util/comptr.h"

#include <QMessageBox>

#include <QMetaMethod>
#include <QApplication>

using namespace Access;

MainDialog::MainDialog(IDispatch *application, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
//  , m_pApplication( new Application(application, parent) )
  , m_application(0)
{
    ui->setupUi(this);


    m_model = new ObjectModel(this);

    ui->treeView->setModel(m_model);
    ui->treeView->setIndentation(0);
    ui->treeView->setItemDelegateForColumn( ObjectModel::InProjectColumn,     new CheckBoxItemDelegete() );
    ui->treeView->setItemDelegateForColumn( ObjectModel::InFileSystemColumn,  new CheckBoxItemDelegete() );
    ui->treeView->setItemDelegateForColumn( ObjectModel::DifferentColumn,     new CheckBoxItemDelegete() );
    ui->treeView->setColumnWidth(ObjectModel::InProjectColumn,    40);
    ui->treeView->setColumnWidth(ObjectModel::InFileSystemColumn, 40);
    ui->treeView->setColumnWidth(ObjectModel::DifferentColumn,    40);
    ui->treeView->setColumnWidth(ObjectModel::NameColumn, 200);
    ui->treeView->setColumnWidth(ObjectModel::CreateDateColumn, 150);
    ui->treeView->setColumnWidth(ObjectModel::UpdateDateColumn, 150);
    ui->treeView->setColumnWidth(ObjectModel::ExportDateColumn, 150);

    connect( ui->okButton,     SIGNAL(clicked(bool)), this, SLOT(accept()) );
    connect( ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()) );

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

    connect( m_model, SIGNAL(progressStart(int,int)), this, SLOT(progressStart(int,int)) );
    connect( m_model, SIGNAL(progressChange(int,int)), this, SLOT(progressChange(int,int)) );
    connect( m_model, SIGNAL(progressEnd(int)), this, SLOT(progressEnd(int)) );


    ui->progressBar->reset();

    Access::_Application *_application = new Access::_Application(application/*, this*/);
    m_application = new Access::Application(_application);


    bool c;
    c = connect( m_application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    if (!c) QMessageBox::information(this, "", "connect exception failed");
    c = connect( m_application, SIGNAL(propertyChanged(QString)), this, SLOT(propertyChanged(QString)) );
    if (!c) QMessageBox::information(this, "", "connect propertyChanged failed");
    c = connect( m_application, SIGNAL(signal(QString,int,void*)), this, SLOT(signal(QString,int,void*)) );
    if (!c) QMessageBox::information(this, "", "connect signal failed");



    m_model->setApplication(m_application);
//    m_model->loadFromProject();
//    m_model->loadFromFileSystem();

//    m_application->OpenAccessProject("C:\\test.mdb"); // Access dialog is shwon
    const QMetaObject *metaObject = m_application->metaObject();
    QStringList methods;
    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
        methods << QString::fromLatin1(metaObject->method(i).methodSignature());

//    QMessageBox::information(this, "", methods.join("\n"));

//    m_model->exportToTempDir();


//    ComPtr<Access::CurrentProject> currentProject = m_pApplication->CurrentProject();

//    Access::CurrentProject* currentProject = m_pApplication->CurrentProject();
//    currentProject->clear();
//    delete currentProject;

}

MainDialog::~MainDialog()
{
    delete ui;
    if (m_application)
    {
        disconnect( m_application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
        disconnect( m_application, SIGNAL(propertyChanged(QString)), this, SLOT(propertyChanged(QString)) );
        disconnect( m_application, SIGNAL(signal(QString,int,void*)), this, SLOT(signal(QString,int,void*)) );
        delete m_application;
    }
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

void MainDialog::clearCache()
{
    m_model->clearItemsCache();
}

void MainDialog::refreshItems()
{
    m_model->refreshItems();
    m_model->selectItemsForProcess( true, false );
    // FIXME: i don't know why but cursor stays with WaitCursor in several seconds.
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
}

void MainDialog::executeExport()
{
    m_model->executeExport();
    QMessageBox::information(0,"","done");
}

void MainDialog::executeImport()
{
    m_model->executeImport();
    QMessageBox::information(0,"","done");
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

void MainDialog::progressStart(int type, int count)
{
    Q_UNUSED(type);
    ui->progressBar->setRange(0, count);
    ui->progressBar->setValue(0);
}

void MainDialog::progressChange(int type, int value)
{
    Q_UNUSED(type);
    ui->progressBar->setValue(value);
    QApplication::processEvents();
}

void MainDialog::progressEnd(int type)
{
    Q_UNUSED(type);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(1);
    ui->progressBar->reset();
}

