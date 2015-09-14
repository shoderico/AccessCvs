#ifndef CVSCONTROLLER_H
#define CVSCONTROLLER_H

#include "cvscontroller_global.h"

#include <QObject>
#include "addin/addincontroller.h"

class MainDialog;

namespace Access {
class Application;
} // namespace Access

class CVSCONTROLLERSHARED_EXPORT CvsController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit CvsController(QObject *parent = 0);
    ~CvsController();

    void initialize(Access::Application *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

    void manual();
    void autoImport();
    void autoExport();

    void clearCacheAndExport();
    void clearCacheAndImport();

signals:

public slots:

private:
    void init();

private:
    Access::Application *m_application;
    QWidget *m_parentWidget;
    MainDialog *m_dlg;
};

#endif // CVSCONTROLLER_H
