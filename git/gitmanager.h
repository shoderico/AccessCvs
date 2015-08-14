#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>

class IDispatch;
namespace Access {
class Application;
} // namespace Access

class ProjectSetting;

class GitManager : public QObject
{
    Q_OBJECT
public:
    explicit GitManager(Access::Application *application, QObject *parent = 0);

    void init();
    void gitIgnore();
    bool isSupportedSsh() const;

    void manageRemotes();

    bool checkProjectOpened(const ProjectSetting *setting);
    bool checkRepositoryInitialized(const ProjectSetting *setting);
    bool checkRepositoryNotInitialized(const ProjectSetting *setting);

signals:
public slots:

private:
    ProjectSetting *createSetting();

private:
    Access::Application *m_application;
};

#endif // GITMANAGER_H
