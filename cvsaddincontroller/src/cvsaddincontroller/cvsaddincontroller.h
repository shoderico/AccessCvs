#ifndef CVSADDINCONTROLLER_H
#define CVSADDINCONTROLLER_H

#include "cvsaddincontroller_global.h"

#include <QObject>
#include "addinutil/addincontroller.h"

class MainDialog;
class CvsProgressDialog;
class ObjectModel;
class ObjectProxyModel;

//namespace Access {
//class Application;
//} // namespace Access

class CVSADDINCONTROLLER_SHARED_EXPORT CvsAddinController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit CvsAddinController(QObject *parent = 0);
    ~CvsAddinController();

public slots:
    void initialize(QAxObject *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

    void prepareManual();
    void prepareImport();
    void prepareExport();

    void clearCacheAndPrepareExport();
    void clearCacheAndPrepareImport();

signals:

public slots:
    void selectAuto();
    void selectAllObject(bool select);
    void selectObject(int objectType, bool select);
    void showSelectedOnly(bool selectedOnly);
    void showAllObject(bool show);
    void showObject(int objectType, bool show);
    void clearCache();
    void refreshItems();
    void executeExport();
    void executeImport();
    void showSettingDialog();

protected:
    virtual void init();

private:
    enum PrepareType
    {
        Import
        ,Export
    };
    void prepare(const PrepareType prepareType, const bool clearCache);

protected:
    QAxObject *m_application;
    ObjectModel *m_model;
    ObjectProxyModel *m_proxyModel;
    MainDialog *m_mainDlg;
    QWidget *m_parentWidget;

private:
    CvsProgressDialog *m_progressDlg;
};

#endif // CVSADDINCONTROLLER_H
