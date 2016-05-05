#ifndef CVSCONTROLLER_H
#define CVSCONTROLLER_H

#include "cvscontroller_global.h"

#include <QObject>
#include "addinutil/addincontroller.h"

class MainDialog;
class CvsProgressDialog;
class ObjectModel;
class ObjectProxyModel;

//namespace Access {
//class Application;
//} // namespace Access

class CVSCONTROLLER_SHARED_EXPORT CvsController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit CvsController(QObject *parent = 0);
    ~CvsController();

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

private:
    QWidget *m_parentWidget;
    MainDialog *m_dlg;
    CvsProgressDialog *m_progressDlg;
};

#endif // CVSCONTROLLER_H
