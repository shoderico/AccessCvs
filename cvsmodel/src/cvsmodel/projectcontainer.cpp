#include "projectcontainer.h"

#include <QDebug>
#include <QSettings>
#include <QDir>

#include "util/fileutil.h"
#include "processor/objectprocessor.h"


ProjectContainer::ProjectContainer(QObject *parent)
    : QObject(parent)
{
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



