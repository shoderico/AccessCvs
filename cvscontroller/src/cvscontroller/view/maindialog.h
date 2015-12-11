#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QTimer>
#include "cvsmodel/objectmodel.h"

namespace Ui {
class MainDialog;
}

namespace Access {
class Application;
}

class ObjectModel;
class ObjectProxyModel;
class ProgressHelper;


class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(ObjectModel *model, QWidget *parent = 0);
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

signals:
    void selectAuto();

private slots:
    void onAccepted();
    void onRejected();

    void clearCache();
    void refreshItems();
    void executeExport();
    void executeImport();

    void beginBatch();
    void endBatch();

    void selectCheckStateChanged(int state);
    void showCheckStateChanged(int state);
    void showSelectedOnly(int state);

private:
    Ui::MainDialog *ui;
    ObjectModel *m_model;
    ObjectProxyModel *m_proxyModel;
    ShowModes m_showMode;

    ProgressHelper *m_progressHelper;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MainDialog::ShowModes)


#endif // MAINDIALOG_H
