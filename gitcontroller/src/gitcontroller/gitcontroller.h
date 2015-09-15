#ifndef GITCONTROLLER_H
#define GITCONTROLLER_H

#include "gitcontroller_global.h"

#include "addinutil/addincontroller.h"

#include <QObject>

class IDispatch;
namespace Access {
class Application;
} // namespace Access

class ProjectSetting;

class GITCONTROLLERSHARED_EXPORT GitController : public QObject, public AddInController
{
    Q_OBJECT
    //    Q_PLUGIN_METADATA(IID "com.shoderitz.addin.AddInController" FILE "basictools.json")
        Q_INTERFACES(AddInController)
public:
    explicit GitController(QObject *parent = 0);

    void initialize(Access::Application *application, QWidget *parentWidget);
    QString ribbonXml();
    bool imagePath(const QString &controlId, QString &imagePath, QSize &size);
    bool handleButtonClick(const QString &controlId);

    void init();
    void gitIgnore();
    bool isSupportedSsh() const;

    void manageRemotes();
    void pull();

    bool checkProjectOpened(const ProjectSetting *setting);
    bool checkRepositoryInitialized(const ProjectSetting *setting);
    bool checkRepositoryNotInitialized(const ProjectSetting *setting);

signals:
public slots:

private:
    ProjectSetting *createSetting();

private:
    Access::Application *m_application;
    QWidget *m_parentWidget;
};

#endif // GITCONTROLLER_H
