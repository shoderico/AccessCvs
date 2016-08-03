#ifndef ACCESSADDINCONTROLLER_H
#define ACCESSADDINCONTROLLER_H

#include "accessaddincontroller_global.h"

#include "addinutil/addincontroller.h"

#include <QObject>

class WindowWidgetManager;
class UiBlocker;
class ThreadedInvoker;

namespace Access {
class Application;
} // namespace Access

class ACCESSADDINCONTROLLER_SHARED_EXPORT AccessAddinController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit AccessAddinController(QObject *parent = 0);
    ~AccessAddinController();

    void initialize(QAxObject *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

    void decompile();
    void compactRepair();
    void decompileAndCompactRepair();

    void openInExplorer();
    void openInConsole();

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

#endif // ACCESSADDINCONTROLLER_H
