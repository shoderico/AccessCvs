#include "accessobjectmodel.h"

#include "accesslib/accesslib.h"
#include "util/comptr.h"

#include "accessprojectcontainer.h"

#include <QDebug>

AccessObjectModel::AccessObjectModel(QObject *parent)
    : ObjectModel(parent)
{
    ProjectContainer *projectContainer = new AccessProjectContainer(this);

    init(projectContainer);
}

void AccessObjectModel::setApplication(QAxObject *application)
{
    ObjectModel::setApplication(application);
    m_application = static_cast<Access::Application*>(application);
}

bool AccessObjectModel::executeImport()
{
    ObjectModel::executeImport();
    m_application->RefreshDatabaseWindow();
    return true;
}

bool AccessObjectModel::checkProjectState()
{
    ComPtr<Access::CurrentProject> currentProject  = m_application->CurrentProject();
    ComPtr<DAO::Database> currentDb = m_application->CurrentDb();
    if ( !currentProject.is() )
    {
        qDebug() << "currentProject is null";
        return false;
    }
    int projectType = currentProject->ProjectType();
    if ( projectType == Access::acMDB && !currentDb.is() )
    {
        qDebug() << "currentDb is null";
        return false;
    }

    if ( (projectType != Access::acMDB) && (projectType != Access::acADP) )
    {
        qDebug() << "unknown project type";
        return false;
    }
    return true;
}

