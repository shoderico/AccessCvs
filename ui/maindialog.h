#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "model/objectmodel.h"

namespace Ui {
class MainDialog;
}

namespace Access {
class Application;
}

class ObjectModel;

class IUnknown;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(IUnknown *application,  QWidget *parent = 0);
    ~MainDialog();

private slots:
    void exportObjects();

    void processStart(ObjectModel::ProcessData progressData);
    void processEnd(ObjectModel::ProcessData progressData);
    void subProcessStart(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData);
    void subProcessEnd(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData);
    void subProcessProgress(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData);

private:
    Ui::MainDialog *ui;
    Access::Application *m_pApplication;
    ObjectModel *m_model;
};

#endif // MAINDIALOG_H
