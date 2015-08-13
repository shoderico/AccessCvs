#include "gitmanager.h"

#include "model/projectsetting.h"
#include "qgit2.h"

#include <QMessageBox>
#include <QPointer>
#include <QDir>


GitManager::GitManager(Access::Application *application, QObject *parent)
    : QObject(parent)
    , m_application(application)
{
}

void GitManager::init()
{
    ProjectSetting setting(this);
    setting.initialize(m_application);
    if (!setting.isProjectOpened())
    {
        QMessageBox::information(0, tr(""), tr("no project is opened!"));
        return;
    }

    if ( QDir( setting.projectPath() + "\\.git" ).exists() )
    {
        QMessageBox::information(0, tr(""), tr("already initialized!"));
        return;
    }

    // git init setting.projectPath()
    QString repoPath = setting.projectPath();
    QPointer<LibQGit2::Repository> repo;
    repo = new LibQGit2::Repository();
    bool succeed = false;
    try
    {
        repo->init(repoPath, false);
        succeed = true;
    }
    catch (const LibQGit2::Exception& ex)
    {
        QMessageBox::warning(0, tr(""), ex.what());
    }

    if (succeed)
    {
        QMessageBox::information(0, tr(""), tr("git init succeed!"));
    }
}

void GitManager::gitIgnore()
{

}

