#include "accessprojectcontainer.h"

#include <QDebug>

#include "accesslib/accesslib.h"
#include "util/comptr.h"

#include "processor/tabledefprocessor.h"
#include "processor/queryassqlprocessor.h"
#include "processor/formprocessor.h"
#include "processor/reportprocessor.h"
#include "processor/macroprocessor.h"
#include "processor/moduleprocessor.h"

#include "processor/referenceprocessor.h"
#include "processor/vbprojectprocessor.h"

#include "processor/projectfileprocessor.h"


AccessProjectContainer::AccessProjectContainer(QObject *parent)
    : ProjectContainer(parent)
    , m_projectType(-1)
{

    m_DefaultTempPathName     = ".accesscvs";

    m_objectProcessors.insert( Model::TableDef,   new TableDefProcessor (this) );
    m_objectProcessors.insert( Model::Query,      new QueryAsSqlProcessor    (this) );
    m_objectProcessors.insert( Model::Form,       new FormProcessor     (this) );
    m_objectProcessors.insert( Model::Report,     new ReportProcessor   (this) );
    m_objectProcessors.insert( Model::Macro,      new MacroProcessor    (this) );
    m_objectProcessors.insert( Model::Module,     new ModuleProcessor   (this) );

    m_objectProcessors.insert( Model::Reference,  new ReferenceProcessor(this) );
    m_objectProcessors.insert( Model::VBProject,  new VBProjectProcessor(this) );

    m_objectProcessors.insert( Model::ProjectFile,  new ProjectFileProcessor(this) );

}

void AccessProjectContainer::initialize(QAxObject *application)
{
    m_application = static_cast<Access::Application*>(application);

    m_currentProjectName = "";
    m_currentProjectPath = "";
    m_currentProjectFullName = "";
    m_projectType = -1;

    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (currentProject.is())
    {
        m_currentProjectName        = currentProject->Name();
        m_currentProjectPath        = currentProject->Path();
        m_currentProjectFullName    = currentProject->FullName();
        m_projectType               = currentProject->ProjectType();
    }

    ProjectContainer::initialize(application);
}

bool AccessProjectContainer::isMDB() const
{
    return (m_projectType == Access::acMDB);
}

bool AccessProjectContainer::isADP() const
{
    return (m_projectType == Access::acADP);
}

bool AccessProjectContainer::isProjectOpened() const
{
    return (isMDB() || isADP());
}

