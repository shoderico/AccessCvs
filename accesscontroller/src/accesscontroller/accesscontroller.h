#ifndef ACCESSCONTROLLER_H
#define ACCESSCONTROLLER_H

#include "accesscontroller_global.h"

#include "addinutil/addincontroller.h"

#include <QObject>

class WindowWidgetManager;
class UiBlocker;
class ThreadedInvoker;

namespace Access {
class Application;
} // namespace Access

class ACCESSCONTROLLER_SHARED_EXPORT AccessController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit AccessController(QObject *parent = 0);
    ~AccessController();

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

#endif // ACCESSCONTROLLER_H
