#include "projectsetting.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "officelib/officelib.h"
#include "util/comptr.h"
#include "util/fileutil.h"

#include "objectsetting.h"


ProjectSetting::ProjectSetting(QObject *parent)
    : QObject(parent)
    , m_projectType(-1)
{
    m_sourcePathName = "source";
    m_tempPathName   = ".accesscvs";

    m_settingsFileName = "project.settings";

    m_objectSettings.insert( Model::TableDef,   new TableDefSetting (this) );
//  m_objectSettings.insert( Model::TableData,  new TableDataSetting(this) );
//  m_objectSettings.insert( Model::Relation,   new RelationSetting (this) );
    m_objectSettings.insert( Model::Query,      new QueryAsSqlSetting    (this) );
//  m_objectSettings.insert( Model::Query,      new QueryObjectSetting(this) );
    m_objectSettings.insert( Model::Form,       new FormSetting     (this) );
    m_objectSettings.insert( Model::Report,     new ReportSetting   (this) );
    m_objectSettings.insert( Model::Macro,      new MacroSetting    (this) );
    m_objectSettings.insert( Model::Module,     new ModuleSetting   (this) );
    m_objectSettings.insert( Model::Reference,  new ReferenceSetting(this) );
    m_objectSettings.insert( Model::ProjectFile,  new ProjectFileSetting(this) );
    m_objectSettings.insert( Model::VBProject,  new VBProjectSetting(this) );
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
        loadSettings();
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

bool ProjectSetting::isProjectOpened() const
{
    return (isMDB() || isADP());
}

QString ProjectSetting::sourcePath() const
{
    return m_projectPath + "\\" + m_sourcePathName;
}

QString ProjectSetting::tempPath() const
{
    return m_projectPath + "\\" + m_tempPathName;
}

QString ProjectSetting::projectPath() const
{
    return m_projectPath;
}

ObjectSetting *ProjectSetting::operator[](Model::ObjectType objectType)
{
    return m_objectSettings[ objectType ];
}

Access::Application *ProjectSetting::application() const
{
    return m_application;
}

QList<Model::ObjectType> ProjectSetting::objectTypes() const
{
    return m_objectSettings.keys();
}

void ProjectSetting::loadSettings()
{
    QSettings *settings = createSettings();
    foreach ( Model::ObjectType objectType, m_objectSettings.keys() )
    {
        m_objectSettings[ objectType ]->loadSettings( settings );
    }
    delete settings;
}


void ProjectSetting::saveSettings()
{
    FileUtil::deleteFile( settingsFilePath() );
    QSettings *settings = createSettings();
    foreach ( Model::ObjectType objectType, m_objectSettings.keys() )
    {
        m_objectSettings[ objectType ]->saveSettings( settings );
    }
    delete settings;
}

void ProjectSetting::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    // whatever connect() succeed, never called exception().
    //connect( application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    qDebug() << code << source << desc << help;
}

QSettings *ProjectSetting::createSettings()
{
    QDir(sourcePath()).mkpath(".");
    QSettings *settings = new QSettings( settingsFilePath(), QSettings::IniFormat, this);
    settings->setIniCodec( "UTF-8" );
    return settings;
}

QString ProjectSetting::settingsFilePath() const
{
    return sourcePath() + "\\" + m_settingsFileName;
}

