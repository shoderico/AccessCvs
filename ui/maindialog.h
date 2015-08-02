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

class IDispatch;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(IDispatch *application,  QWidget *parent = 0);
    ~MainDialog();
public slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);
    void	propertyChanged(const QString & name);
    void	signal(const QString & name, int argc, void * argv);

private slots:
    void exportObjects();
    void executeExport();
    void executeImport();

    void processStart(ObjectModel::ProcessData progressData);
    void processEnd(ObjectModel::ProcessData progressData);
    void subProcessStart(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData);
    void subProcessEnd(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData);
    void subProcessProgress(ObjectModel::ProcessData progressData, ObjectModel::SubProcessData subProcessData);

private:
    Ui::MainDialog *ui;
    Access::Application *m_application;
    ObjectModel *m_model;
};

#endif // MAINDIALOG_H
