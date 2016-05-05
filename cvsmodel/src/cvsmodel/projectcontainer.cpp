#include "projectcontainer.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "accesslib/accesslib.h"
#include "util/comptr.h"
#include "util/fileutil.h"

//#include "processor/objectprocessor.h"
#include "processor/tabledefprocessor.h"
#include "processor/queryassqlprocessor.h"
#include "processor/formprocessor.h"
#include "processor/reportprocessor.h"
#include "processor/macroprocessor.h"
#include "processor/moduleprocessor.h"
#include "processor/referenceprocessor.h"
#include "processor/projectfileprocessor.h"
#include "processor/vbprojectprocessor.h"


ProjectContainer::ProjectContainer(QObject *parent)
    : QObject(parent)
    , m_projectType(-1)
{
    m_objectProcessors.insert( Model::TableDef,   new TableDefProcessor (this) );
//  m_objectSettings.insert( Model::TableData,  new TableDataSetting(this) );
//  m_objectSettings.insert( Model::Relation,   new RelationProcessor (this) );
    m_objectProcessors.insert( Model::Query,      new QueryAsSqlProcessor    (this) );
//  m_objectSettings.insert( Model::Query,      new QueryObjectSetting(this) );
    m_objectProcessors.insert( Model::Form,       new FormProcessor     (this) );
    m_objectProcessors.insert( Model::Report,     new ReportProcessor   (this) );
    m_objectProcessors.insert( Model::Macro,      new MacroProcessor    (this) );
    m_objectProcessors.insert( Model::Module,     new ModuleProcessor   (this) );
    m_objectProcessors.insert( Model::Reference,  new ReferenceProcessor(this) );
    m_objectProcessors.insert( Model::ProjectFile,  new ProjectFileProcessor(this) );
    m_objectProcessors.insert( Model::VBProject,  new VBProjectProcessor(this) );
}

ProjectContainer::~ProjectContainer()
{
    QList<Model::ObjectType> objectTypes;
    foreach ( Model::ObjectType objectType, m_objectProcessors.keys() )
    {
        delete m_objectProcessors[ objectType ];
        m_objectProcessors[ objectType ] = NULL;
    }
}

void ProjectContainer::initialize(QAxObject *application)
{
    m_application = static_cast<Access::Application*>(application);
    ComPtr<Access::CurrentProject> currentProject = m_application->CurrentProject();
    if (currentProject.is())
    {
        QString projectPath = currentProject->Path();
        QString projectConfigFileName = currentProject->Name() + ".config";
        QString projectConfigFilePath = projectPath + "\\" + projectConfigFileName;
        qDebug() << projectConfigFilePath;
        if ( QFile::exists( projectConfigFilePath ) )
        {
            qDebug() << " projectConfigFilePath exists ";
            // read projectConfig file
            //-----------------------------------
            // SourcePathName  : default 'source'
            // TempPathName    : default '.accesscvs'
            // SettingFileName : default 'project.settings'
            // ProjectPath     : relative to currentProject->Path(). default currentProject->Path().
            QSettings *config = new QSettings( projectConfigFilePath, QSettings::IniFormat, this);
            config->setIniCodec( "UTF-8" );
            m_sourcePathName   = config->value( "SourcePathName",  "source"           ).toString();
            m_tempPathName     = config->value( "TempPathName",    ".accesscvs"       ).toString();
            m_settingsFileName = config->value( "SettingFileName", "project.settings" ).toString();
            m_projectPath = QDir::cleanPath( QDir( projectPath ).filePath( config->value( "ProjectPath", "." ).toString() ) ).replace(QString('/'),QString('\\'));
            qDebug() << "m_projectPath : " << m_projectPath;
            qDebug() << "m_sourcePathName : " << m_sourcePathName;
            qDebug() << "m_tempPathName : " << m_tempPathName;
            qDebug() << "m_settingFileName : " << m_settingsFileName;
        }
        else
        {
            qDebug() << " projectConfigFilePath NOT exists ";
            m_sourcePathName   = "source";
            m_tempPathName     = ".accesscvs";
            m_settingsFileName = "project.settings";
            m_projectPath      = currentProject->Path();
            qDebug() << "m_projectPath : " << m_projectPath;
            qDebug() << "m_sourcePathName : " << m_sourcePathName;
            qDebug() << "m_tempPathName : " << m_tempPathName;
            qDebug() << "m_settingFileName : " << m_settingsFileName;
        }

        m_projectType = currentProject->ProjectType();
        loadSettings();
    }
}

bool ProjectContainer::isMDB() const
{
    return (m_projectType == Access::acMDB);
}

bool ProjectContainer::isADP() const
{
    return (m_projectType == Access::acADP);
}

bool ProjectContainer::isProjectOpened() const
{
    return (isMDB() || isADP());
}

QString ProjectContainer::sourcePath() const
{
    return m_projectPath + "\\" + m_sourcePathName;
}

QString ProjectContainer::tempPath() const
{
    return m_projectPath + "\\" + m_tempPathName;
}

QString ProjectContainer::settingsFilePath() const
{
    return sourcePath() + "\\" + m_settingsFileName;
}

ObjectProcessor *ProjectContainer::operator[](Model::ObjectType objectType)
{
    return m_objectProcessors[ objectType ];
}

Access::Application *ProjectContainer::application() const
{
    return m_application;
}

QList<Model::ObjectType> ProjectContainer::objectTypes() const
{
    return m_objectProcessors.keys();
}

void ProjectContainer::loadSettings()
{
    QSettings *settings = createSettings();
    foreach ( Model::ObjectType objectType, m_objectProcessors.keys() )
    {
        m_objectProcessors[ objectType ]->loadSettings( settings );
    }
    delete settings;
}


void ProjectContainer::saveSettings()
{
    FileUtil::deleteFile( settingsFilePath() );
    QSettings *settings = createSettings();
    foreach ( Model::ObjectType objectType, m_objectProcessors.keys() )
    {
        m_objectProcessors[ objectType ]->saveSettings( settings );
    }
    delete settings;
}

void ProjectContainer::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    // whatever connect() succeed, never called exception().
    //connect( application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    qDebug() << code << source << desc << help;
}

QSettings *ProjectContainer::createSettings()
{
    QDir(sourcePath()).mkpath(".");
    QSettings *settings = new QSettings( settingsFilePath(), QSettings::IniFormat, this);
    settings->setIniCodec( "UTF-8" );
    return settings;
}


