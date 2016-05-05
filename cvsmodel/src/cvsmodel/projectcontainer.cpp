#include "projectcontainer.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "util/fileutil.h"
#include "processor/objectprocessor.h"


ProjectContainer::ProjectContainer(QObject *parent)
    : QObject(parent)
{
    m_DefaultSourcePathName   = "source";
    m_DefaultTempPathName     = ".officecvs";
    m_DefaultSettingsFileName = "Project.settings";
}

ProjectContainer::~ProjectContainer()
{
    foreach ( Model::ObjectType objectType, m_objectProcessors.keys() )
    {
        delete m_objectProcessors[ objectType ];
        m_objectProcessors[ objectType ] = NULL;
    }
}

void ProjectContainer::initialize(QAxObject *application)
{
    m_application = application;

    m_sourcePathName   = "";
    m_tempPathName     = "";
    m_settingsFileName = "";
    m_projectPath      = "";

    if (isProjectOpened())
    {
        QString projectConfigFileName = m_currentProjectName + ".config";
        QString projectConfigFilePath = m_currentProjectPath + "\\" + projectConfigFileName;
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
            m_sourcePathName   = config->value( "SourcePathName",  m_DefaultSourcePathName   ).toString();
            m_tempPathName     = config->value( "TempPathName",    m_DefaultTempPathName     ).toString();
            m_settingsFileName = config->value( "SettingFileName", m_DefaultSettingsFileName ).toString();
            m_projectPath      = QDir::cleanPath( QDir( m_currentProjectPath ).filePath( config->value( "ProjectPath", "." ).toString() ) ).replace(QString('/'),QString('\\'));
        }
        else
        {
            qDebug() << " projectConfigFilePath NOT exists ";
            m_sourcePathName   = m_DefaultSourcePathName;
            m_tempPathName     = m_DefaultTempPathName;
            m_settingsFileName = m_DefaultSettingsFileName;
            m_projectPath      = m_currentProjectPath;
        }

        qDebug() << "m_projectPath     : " << m_projectPath;
        qDebug() << "m_sourcePathName  : " << m_sourcePathName;
        qDebug() << "m_tempPathName    : " << m_tempPathName;
        qDebug() << "m_settingFileName : " << m_settingsFileName;

        loadSettings();
    }
}

QString ProjectContainer::currentProjectFullName() const
{
    return m_currentProjectFullName;
}

QString ProjectContainer::currentProjectPath() const
{
    return m_currentProjectPath;
}

QString ProjectContainer::projectPath() const
{
    return m_projectPath;
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

QAxObject *ProjectContainer::application() const
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



