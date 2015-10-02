#ifndef CVSPROGRESSDIALOG_H
#define CVSPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class CvsProgressDialog;
}

class ObjectModel;
class ProgressHelper;

class CvsProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CvsProgressDialog(ObjectModel *model, QWidget *parent = 0);
    ~CvsProgressDialog();

    void beginBatch();
    void endBatch();

private:
    Ui::CvsProgressDialog *ui;
    ObjectModel *m_model;
    ProgressHelper *m_progressHelper;
};

#endif // CVSPROGRESSDIALOG_H
