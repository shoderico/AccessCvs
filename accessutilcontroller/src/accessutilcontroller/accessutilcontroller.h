#ifndef ACCESSUTILCONTROLLER_H
#define ACCESSUTILCONTROLLER_H

#include "accessutilcontroller_global.h"

#include "addin/addincontroller.h"

#include <QObject>

class WindowWidgetManager;
class UiBlocker;
class ThreadedInvoker;

namespace Access {
class Application;
} // namespace Access

class ACCESSUTILCONTROLLERSHARED_EXPORT AccessUtilController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit AccessUtilController(QObject *parent = 0);
    ~AccessUtilController();

    void initialize(Access::Application *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

    void decompile();
    void compactRepair();
    void decompileAndCompactRepair();

private slots:
    void doDecompile();
    void doCompactRepair();
    void doDecompileAndCompactRepair();

private:
    bool getCurrentFileName(QString &fileName);

private:
    Access::Application *m_application;
    QWidget *m_parentWidget;
    UiBlocker *m_uiBlocker;
    ThreadedInvoker *m_threadedInvoker;
};

#endif // ACCESSUTILCONTROLLER_H
