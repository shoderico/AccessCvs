#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "../cvsaddincontroller_global.h"

#include <QDialog>

namespace Ui {
class MainDialog;
}

namespace Access {
class Application;
}

class ObjectModel;
class ObjectProxyModel;
class ProgressHelper;
class ObjectItemCheckBox;
class ObjectItemCheckConfig;
class ObjectProcessor;

//class QCheckBox;
#include <QCheckBox>

class CVSADDINCONTROLLER_SHARED_EXPORT MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(ObjectModel *model, ObjectProxyModel *proxyModel, QWidget *parent = 0);
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

    void beginBatch();
    void endBatch();

signals:
    void selectAuto();
    void selectAllObject(bool select);
    void selectObject(int objectType, bool select);
    void showAllObject(bool show);
    void showObject(int objectType, bool show);
    void showSelectedOnly(bool selectedOnly);
    void clearCache();
    void refreshItems();
    void executeExport();
    void executeImport();

private slots:
    void onAccepted();
    void onRejected();


    void selectAllCheckStateChanged(int state);
    void selectCheckStateChanged(int state);
    void showAllCheckStateChanged(int state);
    void showCheckStateChanged(int state);
    void showSelectedOnlyCheckStateChanged(int state);

    void setShownObjectType(int objectTypes);
    void setSelectObjectType(int objectTypes);

private:
    Ui::MainDialog *ui;
    ObjectModel *m_model;
    ObjectProxyModel *m_proxyModel;
    ShowModes m_showMode;

    ProgressHelper *m_progressHelper;

    QList<ObjectItemCheckBox*> m_selectionChecks;
    QList<ObjectItemCheckBox*> m_filterChecks;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MainDialog::ShowModes)

class ObjectItemCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit ObjectItemCheckBox(ObjectProcessor *processor, QWidget * parent = 0);
    int selectObjectType() const { return m_selectObjectType; }
private:
    int m_selectObjectType;
};

class ObjectItemCheckConfig
{
public:
    ObjectItemCheckConfig(const QString &name, const int value)
    {
        this->name = name;
        this->value = value;
    }

    QString name;
    int value;
};


#endif // MAINDIALOG_H
