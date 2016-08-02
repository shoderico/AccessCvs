#include "projectcontainer.h"

#include <QDebug>
#include <QSettings>
#include <QDir>
#include <QTextCodec>

#include "util/fileutil.h"
#include "util/codecinfo.h"

#include "processor/objectprocessor.h"
#include "util/setting.h"

#include "pch.hpp"

ProjectContainer::ProjectContainer(QObject *parent)
    : QObject(parent)
{
    m_DefaultSourcePathName   = "source";
    m_DefaultTempPathName     = ".officecvs";
    m_DefaultSettingFileName  = "Project.settings";
}

ProjectContainer::~ProjectContainer()
{
    qDeleteAll( m_objectProcessorList );
}

void ProjectContainer::initialize(QAxObject *application)
{
    m_application = application;

    m_sourcePathName   = "";
    m_tempPathName     = "";
    m_settingFileName = "";
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
            m_settingFileName  = config->value( "SettingFileName", m_DefaultSettingFileName  ).toString();
            m_projectPath      = QDir::cleanPath( QDir( m_currentProjectPath ).filePath( config->value( "ProjectPath", "." ).toString() ) ).replace(QString('/'),QString('\\'));
        }
        else
        {
            qDebug() << " projectConfigFilePath NOT exists ";
            m_sourcePathName   = m_DefaultSourcePathName;
            m_tempPathName     = m_DefaultTempPathName;
            m_settingFileName  = m_DefaultSettingFileName;
            m_projectPath      = m_currentProjectPath;
        }

        qDebug() << "m_projectPath     : " << m_projectPath;
        qDebug() << "m_sourcePathName  : " << m_sourcePathName;
        qDebug() << "m_tempPathName    : " << m_tempPathName;
        qDebug() << "m_settingFileName : " << m_settingFileName;

        loadSetting();
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

ObjectProcessor *ProjectContainer::operator[](Model::ObjectType objectType)
{
    foreach( ObjectProcessor *processor, m_objectProcessorList )
    {
        if ( processor->objectType() == objectType)
            return processor;
    }
    return NULL;
}

QAxObject *ProjectContainer::application() const
{
    return m_application;
}

QList<Model::ObjectType> ProjectContainer::objectTypes() const
{
    QList<Model::ObjectType> list;
    foreach( ObjectProcessor *processor, m_objectProcessorList )
    {
        list << processor->objectType();
    }
    return list;
}

const QList<Model::ObjectType> &ProjectContainer::objectTypesForItemMap() const
{
    return m_objectTypesForItemMap;
}

void ProjectContainer::loadSetting()
{
    Setting *setting = createSetting();
    setting->load();
    foreach( ObjectProcessor *processor, m_objectProcessorList )
    {
        processor->loadSetting( setting );
    }
    delete setting;
}


void ProjectContainer::saveSetting()
{
    Setting *setting = createSetting();
    foreach( ObjectProcessor *processor, m_objectProcessorList )
    {
        processor->saveSetting( setting );
    }
    setting->save();
    delete setting;
}

void ProjectContainer::updateSetting(QList<ObjectItem *> *items)
{
    foreach( ObjectProcessor *processor, m_objectProcessorList )
    {
        processor->updateSetting( items );
    }
}

void ProjectContainer::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    // whatever connect() succeed, never called exception().
    //connect( application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    qDebug() << code << source << desc << help;
}

Setting *ProjectContainer::createSetting()
{
    QDir(sourcePath()).mkpath(".");

    CodecInfo codec;
    codec.setCodec( QTextCodec::codecForName("UTF-8") );
    codec.setBom(false);
    codec.setLineEnd("\r\n");

    Setting *setting = new Setting(sourcePath()+ "\\" + m_settingFileName, codec.codec(), codec.bom(), codec.lineEnd());
    return setting;
}



