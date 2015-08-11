#include "projectsetting.h"

#include <QDebug>
#include <QSettings>

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
    m_objectSettings.insert( Model::Query,      new QueryAsSqlSetting    (this) );
//  m_objectSettings.insert( Model::Query,      new QueryObjectSetting(this) );
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

QList<Model::ObjectType> ProjectSetting::objectTypes() const
{
    return m_objectSettings.keys();
}

void ProjectSetting::loadSettings()
{
    // FIXME: load settings

    /* FIXME: implement tabledata */
    // strategry
    //  * use QSettings to save/load settings
    //  * root owner of QSettings is ProjectSetting
    //  * each ObjectSetting has own setting-group i.e. 'Table'.
    //  * TableSetting saves/loads table names to store/restore table data
    //  * In UI, table-data checkbox represents to be store data or not.
    //      * these are saved into QSettings.
    //  * QSettings contents must be editable by user ( must be text format ).
    //  * QSettings file must be handled properly by CVS.
    //  * ObjectItem has a property for table-data handling, boolen value.
    //      * Table-specific property.
    //  * Save timing
    //      * setData()
    //  * Load timing
    //      * reloadAndMerge...()

    QString iniFilename = m_projectPath + "\\.accesscvs";
    QSettings settings(iniFilename, QSettings::IniFormat, this);
    settings.setIniCodec( "UTF-8" );

    foreach ( Model::ObjectType objectType, m_objectSettings.keys() )
    {
        m_objectSettings[ objectType ]->loadSettings( &settings );
    }
}


void ProjectSetting::saveSettings()
{
    // FIXME: save settings

    QString iniFilename = m_projectPath + "\\.accesscvs";
    QSettings settings(iniFilename, QSettings::IniFormat, this);
    settings.setIniCodec( "UTF-8" );

    foreach ( Model::ObjectType objectType, m_objectSettings.keys() )
    {
        m_objectSettings[ objectType ]->saveSettings( &settings );
    }
}

void ProjectSetting::exception(int code, const QString &source, const QString &desc, const QString &help)
{
    // whatever connect() succeed, never called exception().
    //connect( application, SIGNAL(exception(int,QString,QString,QString)), this, SLOT(exception(int,QString,QString,QString)) );
    qDebug() << code << source << desc << help;
}

