#include "gitmanager.h"

#include "model/projectsetting.h"
#include <QMessageBox>

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

    // FIXME: git init setting.projectPath()
}

void GitManager::gitIgnore()
{

}

