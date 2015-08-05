#include "projectsetting.h"

#include <QDebug>

#include "officelib/officelib.h"
#include "util/comptr.h"

#include "objectsetting.h"


ProjectSetting::ProjectSetting(QObject *parent)
    : QObject(parent)
    , m_projectType(-1)
{
    m_sourcePathName = "source";
    m_tempPathName   = "source_temp";

    m_objectSettings.insert( Model::TableDef,   new TableDefSetting (this) );
//  m_objectSettings.insert( Model::TableData,  new TableDataSetting(this) );
//  m_objectSettings.insert( Model::Relation,   new RelationSetting (this) );
    m_objectSettings.insert( Model::Query,      new QuerySetting    (this) );
    m_objectSettings.insert( Model::Form,       new FormSetting     (this) );
    m_objectSettings.insert( Model::Report,     new ReportSetting   (this) );
    m_objectSettings.insert( Model::Macro,      new MacroSetting    (this) );
    m_objectSettings.insert( Model::Module,     new ModuleSetting   (this) );
    m_objectSettings.insert( Model::Reference,  new ReferenceSetting(this) );
}

ProjectSetting::~ProjectSetting()
{
    QList<Model::ObjectType> objectTypes;
    foreach ( Model::ObjectType objectType, m_objectSettings.keys() )
    {
        delete m_objectSettings[ objectType ];
        m_objectSettings[ objectType ] = NULL;
    }
}

void ProjectSetting::initialize(Access::Application *application)
{
    m_application = application;
    ComPtr<Access::CurrentProject> currentProject = application->CurrentProject();
    if (currentProject.is())
    {
        m_projectPath = currentProject->Path();
        m_projectType = currentProject->ProjectType();
    }

}

bool ProjectSetting::isMDB() const
{
    return (m_projectType == Access::acMDB);
}

bool ProjectSetting::isADP() const
{
    return (m_projectType == Access::acADP);
}

QString ProjectSetting::sourcePath() const
{
    return m_projectPath + "\\" + m_sourcePathName;
}

QString ProjectSetting::tempPath() const
{
    return m_projectPath + "\\" + m_tempPathName;
}

ObjectSetting *ProjectSetting::operator[](Model::ObjectType objectType)
{
    return m_objectSettings[ objectType ];
}

Access::Application *ProjectSetting::application() const
{
    return m_application;
}

void ProjectSetting::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    // whatever connect() succeed, never called exception().
    //connect( application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    qDebug() << code << source << desc << help;
}

