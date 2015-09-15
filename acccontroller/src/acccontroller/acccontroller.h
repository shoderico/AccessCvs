#ifndef ACCCONTROLLER_H
#define ACCCONTROLLER_H

#include "acccontroller_global.h"

#include "addinutil/addincontroller.h"

#include <QObject>

class WindowWidgetManager;
class UiBlocker;
class ThreadedInvoker;

namespace Access {
class Application;
} // namespace Access

class ACCCONTROLLER_SHARED_EXPORT AccController : public QObject, public AddInController
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
    Q_INTERFACES(AddInController)
public:
    explicit AccController(QObject *parent = 0);
    ~AccController();

    void initialize(Access::Application *application, QWidget *parentWidget);
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

#endif // ACCCONTROLLER_H
