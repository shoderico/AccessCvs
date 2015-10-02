#include "cvsprogressdialog.h"
#include "ui_cvsprogressdialog.h"

#include "cvsmodel/objectmodel.h"
#include "progresshelper.h"

CvsProgressDialog::CvsProgressDialog(ObjectModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CvsProgressDialog)
  , m_model(model)
  , m_progressHelper( new ProgressHelper(this) )
{
    setWindowFlags( windowFlags() & ~Qt::WindowCloseButtonHint );
    setWindowFlags( windowFlags() & ~Qt::WindowContextHelpButtonHint );

    ui->setupUi(this);

    m_progressHelper->initialize(
                  ui->elapsedTimeLabel
                , ui->processTypeNameLabel
                , ui->progressCountLabel
                , ui->progressBar
                , m_model
                );
}

CvsProgressDialog::~CvsProgressDialog()
{
    delete ui;
}

void CvsProgressDialog::beginBatch()
{
    m_progressHelper->beginBatch();
}

void CvsProgressDialog::endBatch()
{
    m_progressHelper->endBatch();
}
