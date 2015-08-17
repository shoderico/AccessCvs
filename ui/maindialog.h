#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QTimer>
#include "model/objectmodel.h"

namespace Ui {
class MainDialog;
}

namespace Access {
class Application;
}

class ObjectModel;
class ObjectProxyModel;


class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(Access::Application *application,  QWidget *parent = 0);
    ~MainDialog();

    void showAsManual();
    void showAsAutoExport();
    void showAsAutoImport();
    enum ShowMode
    {
        ManualMode,
        AutoExportMode,
        AutoImportMode,
        UnkownMode,
    };
    Q_DECLARE_FLAGS(ShowModes, ShowMode)

public slots:
    void exception(int code, const QString & source, const QString & desc, const QString & help);
    void	propertyChanged(const QString & name);
    void	signal(const QString & name, int argc, void * argv);

private slots:
    void onAccepted();
    void onRejected();

    void clearCache();
    void refreshItems();
    void executeExport();
    void executeImport();
    void prepareExport();
    void prepareImport();

    void beginBatch();
    void endBatch();

    void selectAuto();
    void selectCheckStateChanged(int state);
    void showCheckStateChanged(int state);
    void showSelectedOnly(int state);

    void onTimeout();

    void progressStart(int type, int count);
    void progressChange(int type, int value);
    void progressEnd(int type);

private:
    Ui::MainDialog *ui;
    Access::Application *m_application;
    ObjectModel *m_model;
    ObjectProxyModel *m_proxyModel;
    ShowModes m_showMode;
    QTime m_progressTime;
    QTimer m_progressTimer;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(MainDialog::ShowModes)


#endif // MAINDIALOG_H
