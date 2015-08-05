#include "objectsetting.h"

#include <QDebug>
#include <QDir>

#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include "officelib/officelib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "projectsetting.h"
#include "sanitizesetting.h"

ObjectSetting::ObjectSetting(ProjectSetting *parent)
    : QObject(parent)
    , m_projectSetting(parent)
    , m_codecForCvs(new CodecInfo(this))
    , m_codecForProject(0)
{
    m_codecForCvs->setCodec( QTextCodec::codecForName("UTF-8") );
    m_codecForCvs->setBom(false);
    m_codecForCvs->setLineEnd("\r\n");
}

QString ObjectSetting::tempFilePathInTempDir(const QString &objectName)
{
    return filePath(TempDir, TempFile, objectName);
}

QString ObjectSetting::designFilePathInTempDir(const QString &objectName)
{
    return filePath(TempDir, DesignFile, objectName);
}

QString ObjectSetting::moduleFilePathInTempDir(const QString &objectName)
{
    return filePath(TempDir, ModuleFile, objectName);
}

QString ObjectSetting::tempFilePathInSourceDir(const QString &objectName)
{
    return filePath(SourceDir, TempFile, objectName);
}

QString ObjectSetting::designFilePathInSourceDir(const QString &objectName)
{
    return filePath(SourceDir, DesignFile, objectName);
}

QString ObjectSetting::moduleFilePathInSourceDir(const QString &objectName)
{
    return filePath(SourceDir, ModuleFile, objectName);
}

QString ObjectSetting::sourceObjectPath() const
{
    return objectPath(SourceDir);
}

QString ObjectSetting::tempObjectPath() const
{
    return objectPath(TempDir);
}

void ObjectSetting::mkdirTempObjectPath()
{
    mkpathObjectPath(TempDir);
}

void ObjectSetting::mkdirSourceObjectPath()
{
    mkpathObjectPath(SourceDir);
}



ObjectItem *ObjectSetting::createItemFromFileSystem(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( m_objectType );
    item->setName( fileInfo.completeBaseName() );
    item->setInFileSystem( Model::Present );

    return item;
}

bool ObjectSetting::copyFromTempDirToFileSystem(const QString &objectName)
{
    deleteFromFileSystem(objectName);
    copyFile(TempDir, SourceDir, DesignFile, objectName);
    copyFile(TempDir, SourceDir, ModuleFile, objectName);
    return true;
}

bool ObjectSetting::copyFromFileSystemToTempDir(const QString &objectName)
{
    deleteFromTempDir(objectName);
    copyFile(SourceDir, TempDir, DesignFile, objectName);
    copyFile(SourceDir, TempDir, ModuleFile, objectName);
    return true;
}

bool ObjectSetting::compareTempDir(const QString &objectName, bool *pisDifferent)
{
    bool isSame = true;

    // design
    if (isSame && !m_designFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( designFilePathInTempDir(objectName),
                                    designFilePathInSourceDir(objectName) );
    }
    // module
    if (isSame && !m_moduleFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( moduleFilePathInTempDir(objectName),
                                    moduleFilePathInSourceDir(objectName) );
    }

    *pisDifferent = !isSame;

    return true;
}

bool ObjectSetting::deleteFromFileSystem(const QString &objectName)
{
    deleteFile(SourceDir, DesignFile, objectName);
    deleteFile(SourceDir, ModuleFile, objectName);

    return true;
}

bool ObjectSetting::deleteFromProject(const QString &objectName)
{
    // TODO: cache DoCmd object
    if (m_accessObjectType != -1)
    {
        ComPtr<Access::DoCmd> doCmd = m_projectSetting->application()->DoCmd();
        doCmd->DeleteObject( (Access::AcObjectType)m_accessObjectType, objectName );
    }
    return true;
}

bool ObjectSetting::prepareItemCollection()
{
    // in subclass, override this function
    // and prepare collection object to call Count() and Item().
    return true;
}

int ObjectSetting::itemCount()
{
    // in subclass, override this function
    // and return the value of Count().
    return 0;
}

QAxObject *ObjectSetting::itemUnsafePtr(const QVariant &index)
{
    // in subclass, override this function
    // and return QAxObject pointer.
    // this pointer must be deleted by CALLER.
    Q_UNUSED(index)
    return NULL;
}

void ObjectSetting::determineCodecForProject()
{
    // do nothing here.
    // if m_codecForProject is requred, override this function in subclass
}

bool ObjectSetting::deleteFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, DesignFile, objectName);
    deleteFile(TempDir, ModuleFile, objectName);

    return true;
}





bool ObjectSetting::copyFile(ObjectSetting::DirectoryType dirTypeSrc, ObjectSetting::DirectoryType dirTypeDst, ObjectSetting::FileType fileType, const QString &objectName)
{
    if (!fileExtension(fileType).isEmpty())
    {
        QFile fileSrc(    filePath(dirTypeSrc, fileType, objectName) );
        if (fileSrc.exists())
        {
            mkpathObjectPath(dirTypeDst);
            fileSrc.copy( filePath(dirTypeDst, fileType, objectName) );
        }
    }
    return true;
}

bool ObjectSetting::deleteFile(ObjectSetting::DirectoryType dirType, ObjectSetting::FileType fileType, const QString &objectName)
{
    if (!fileExtension(fileType).isEmpty())
        FileUtil::deleteFile( filePath(dirType, fileType, objectName) );

    return true;
}








QString ObjectSetting::rootPath(ObjectSetting::DirectoryType dirType) const
{
    switch (dirType)
    {
        case TempDir:      return m_projectSetting->tempPath();
        case SourceDir:    return m_projectSetting->sourcePath();
    }
    return QString();
}

QString ObjectSetting::objectPath(ObjectSetting::DirectoryType dirType) const
{
    QString path = rootPath(dirType);
    if ( !m_objectPathName.isEmpty() )
        path += "\\" + m_objectPathName;
    return path;
}

QString ObjectSetting::fileExtension(ObjectSetting::FileType fileType) const
{
    switch (fileType)
    {
        case TempFile:      return m_tempFileExtension;
        case DesignFile:    return m_designFileExtension;
        case ModuleFile:    return m_moduleFileExtension;
    }
    return QString();
}

QString ObjectSetting::filePath(ObjectSetting::DirectoryType dirType, ObjectSetting::FileType fileType, const QString &objectName) const
{
    return objectPath(dirType) + "\\" + objectName + "." + fileExtension(fileType);
}

void ObjectSetting::mkpathObjectPath(ObjectSetting::DirectoryType dirType)
{
    QDir dir( objectPath(dirType) );
    dir.mkpath(".");
}


//=============================================================================
// TableDef

TableDefSetting::TableDefSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::TableDef;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledefs";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "xml";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}

bool TableDefSetting::isTargetObject(QAxObject *object) const
{
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        return !tableDef->Name().startsWith("MSys") && tableDef->Connect().isEmpty();
    }
    return false;
}

ObjectItem *TableDefSetting::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        item->setObjectType( m_objectType );
        item->setName( tableDef->Name() );
        item->setInProject( Model::Present );
        item->setCreateDate( tableDef->DateCreated().toDateTime() );
        item->setUpdateDate( tableDef->LastUpdated().toDateTime() );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool TableDefSetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(objectName)
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        // this makes force termination...
        m_projectSetting->application()
            ->ExportXML(
                    Access::acExportTable
                    ,objectName
                    ,QString() // DataTarget
                    ,tempFilePathInTempDir(objectName) // SchemaTarget
                    ,QString() //PresentationTarget
                    ,QString() //ImageTarget
                    ,Access::acUTF16 //Encoding
                    ,Access::acExportAllTableAndFieldProperties //OtherFlags
                    ,QString() //WhereCondition
                    ,QVariant()//AdditionalData
                    );

        return true;
    }
    return false;
}

bool TableDefSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // TODO: import tabledef
    return true;
}

bool TableDefSetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // we have to convert ONLY codec.

    // codec
    determineCodecForProject();

    FileUtil::copyContents(   tempFilePathInTempDir(objectName), m_codecForProject,
                            designFilePathInTempDir(objectName), m_codecForCvs );
//    FileUtil::deleteFile(     tempFilePathInTempDir(objectName) ); // keep original files

    return true;
}

bool TableDefSetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // we have to convert ONLY codec.

    // codec
    determineCodecForProject();

    FileUtil::copyContents( designFilePathInTempDir(objectName), m_codecForCvs,
                              tempFilePathInTempDir(objectName), m_codecForProject );
//    FileUtil::deleteFile(   designFilePathInTempDir(objectName) ); keep original files

    return true;
}

bool TableDefSetting::prepareItemCollection()
{
    if (!m_projectSetting->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_tableDefs.set( currentDb->TableDefs() );
    return m_tableDefs.is();
}

int TableDefSetting::itemCount()
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Count();
}

QAxObject *TableDefSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Item(index);
}

void TableDefSetting::determineCodecForProject()
{
    // TableDef is saved with UTF-16LE in xml format
    // so we have to convert ONLY codec.
    if (!m_codecForProject)
    {
        m_codecForProject = new CodecInfo(this);
        m_codecForProject->setCodec( QTextCodec::codecForName("UTF-16LE") );
        m_codecForProject->setBom( true );
        m_codecForProject->setLineEnd("\r\n");
    }
}


//=============================================================================
// TableData
// TODO: imeplement tabledata export/import

TableDataSetting::TableDataSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::TableData;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledatas";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "csv";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}


//=============================================================================
// Relation
// TODO: imeplement relationship export/import

RelationSetting::RelationSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Relation;
    m_accessObjectType    = -1;
    m_objectPathName      = "relations";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rel";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}


//=============================================================================
// Query

QuerySetting::QuerySetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
    m_objectType          = Model::Query;
    m_accessObjectType    = Access::acQuery;
    m_objectPathName      = "queries";
    m_containerName       = "";

    m_tempFileExtension   = "sql";
    m_designFileExtension = m_tempFileExtension;
    m_moduleFileExtension = "";
    m_existCheckExtension = m_tempFileExtension;
}

bool QuerySetting::isTargetObject(QAxObject *object) const
{
    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        return !queryDef->Name().startsWith("~") && queryDef->Connect().isEmpty();
    }
    return false;
}

ObjectItem *QuerySetting::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        item->setObjectType( m_objectType );
        item->setName( queryDef->Name() );
        item->setInProject( Model::Present );
        item->setCreateDate( queryDef->DateCreated().toDateTime() );
        item->setUpdateDate( queryDef->LastUpdated().toDateTime() );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool QuerySetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        QString sql = queryDef->SQL();
        FileUtil::saveToFile( sql, tempFilePathInTempDir(objectName), m_codecForCvs );

        return true;
    }
    return false;
}

bool QuerySetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    bool inProject = (object != NULL);

    QString sql = FileUtil::loadAsString( tempFilePathInTempDir(objectName), m_codecForCvs );

    if ( inProject )
    {
        // set SQL property
        DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
        if (queryDef)
            queryDef->SetSQL(sql);
    }
    else
    {
        // create new query def

        // TODO: cache currentDb for performance conideration
        ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();

        ComPtr<DAO::QueryDef> queryDef = currentDb->CreateQueryDef( objectName, sql );
        Q_UNUSED(queryDef)
    }

    return true;
}

bool QuerySetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool QuerySetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sinitization required
    return true;
}

bool QuerySetting::prepareItemCollection()
{
    if (!m_projectSetting->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_queryDefs.set( currentDb->QueryDefs() );
    return m_queryDefs.is();
}

int QuerySetting::itemCount()
{
    if (!m_queryDefs.is())
        return 0;
    return m_queryDefs->Count();
}

QAxObject *QuerySetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_queryDefs.is())
        return 0;
    return m_queryDefs->Item(index);
}




//=============================================================================
// AccessObject

AccessObjectSetting::AccessObjectSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
{
}

bool AccessObjectSetting::isTargetObject(QAxObject *object) const
{
    DAO::Document *document = dynamic_cast<DAO::Document*>(object);
    if (document)
    {
        return !document->Name().startsWith("~");
    }

    Access::AccessObject *accObject = dynamic_cast<Access::AccessObject*>(object);
    if (accObject)
    {
        return !accObject->Name().startsWith("~");
    }

    return false;
}

ObjectItem *AccessObjectSetting::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    Access::AccessObject *accessObject = dynamic_cast<Access::AccessObject*>(object);
    if (accessObject)
    {
        item->setObjectType( m_objectType );
        item->setName( accessObject->Name() );
        item->setInProject( Model::Present );
        item->setCreateDate( accessObject->DateCreated() );
        item->setUpdateDate( accessObject->DateModified() );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    DAO::Document *daoDocument = dynamic_cast<DAO::Document*>(object);
    if (daoDocument)
    {
        item->setObjectType( m_objectType );
        item->setName( daoDocument->Name() );
        item->setInProject( Model::Present );
        item->setCreateDate( daoDocument->DateCreated().toDateTime() );
        item->setUpdateDate( daoDocument->LastUpdated().toDateTime() );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool AccessObjectSetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    {
        m_projectSetting->application()->SaveAsText( (Access::AcObjectType)m_accessObjectType, objectName, tempFilePathInTempDir(objectName) );
        return true;
    }
    return false;
}

bool AccessObjectSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    {
        m_projectSetting->application()->LoadFromText( (Access::AcObjectType)m_accessObjectType, objectName, tempFilePathInTempDir(objectName) );
        return true;
    }
    return true;
}

bool AccessObjectSetting::prepareItemCollection()
{
    if (!m_projectSetting->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
    if (!currentDb.is())
        return false;
    m_containers.set( currentDb->Containers() );
    m_container.set( m_containers->Item( m_containerName ) );
    m_documents.set( m_container->Documents() );

    return m_documents.is();
}

int AccessObjectSetting::itemCount()
{
    if (!m_documents.is())
        return 0;
    return m_documents->Count();
}

QAxObject *AccessObjectSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_documents.is())
        return 0;
    return m_documents->Item(index);
}


//=============================================================================
// AccessDesignObjectSetting

AccessDesignObjectSetting::AccessDesignObjectSetting(ProjectSetting *parent)
    : AccessObjectSetting(parent)
    , m_sanitizer(new SanitizeSetting(this))
{
}

bool AccessDesignObjectSetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // Target object types are Form, Report, Macro
    // Logic is very large and unstable.

    bool hasModule = !m_moduleFileExtension.isEmpty();

    // prepare files
    QFile fileSrc      ( tempFilePathInTempDir  (objectName) );
    QFile fileDstDesign( designFilePathInTempDir(objectName) );
    QFile fileDstModule;
    if (hasModule)
        fileDstModule.setFileName( moduleFilePathInTempDir(objectName) );

    // delete existing files
    FileUtil::deleteFile( fileDstDesign.fileName() );
    if (hasModule)
        FileUtil::deleteFile( fileDstModule.fileName() );


    // open files
    fileSrc.open( QIODevice::ReadOnly );
    fileDstDesign.open( QIODevice::WriteOnly );
    if (hasModule)
        fileDstModule.open( QIODevice::WriteOnly );

    // codec
    determineCodecForProject();

    // streams
    QTextStream streamSrc( &fileSrc );
    QTextStream streamDstDesign( &fileDstDesign );
    QTextStream streamDstModule;

    streamSrc.setDevice( &fileSrc );
    streamSrc.setCodec( m_codecForProject->codec() );
    // bom is auto

    streamDstDesign.setDevice( &fileDstDesign );
    streamDstDesign.setCodec( m_codecForCvs->codec() );
    streamDstDesign.setGenerateByteOrderMark( m_codecForCvs->bom() );

    if (hasModule)
    {
        streamDstModule.setDevice( &fileDstModule );
        streamDstModule.setCodec( m_codecForCvs->codec() );
        streamDstModule.setGenerateByteOrderMark( m_codecForCvs->bom() );
    }


    // sanitize
    m_sanitizer->sanitize( streamSrc, streamDstDesign, streamDstModule, m_codecForCvs );


    // TODO: add post process for subclasses.


    // close files
    fileSrc.close();
    fileDstDesign.close();
    if (hasModule)
        fileDstModule.close();


    // delete temp file
//    FileUtil::deleteFile( fileSrc.fileName() ); // keep original files

    return true;
}

bool AccessDesignObjectSetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // Target object types are Form, Report, Macro
    // Logic is very large and unstable.

    // join design and module file into one.
    // and save to tempFile in codecForProject


    bool hasModule = !m_moduleFileExtension.isEmpty();

    // prepare files
    QFile fileSrc      ( tempFilePathInTempDir  (objectName) );
    QFile fileDstDesign( designFilePathInTempDir(objectName) );
    QFile fileDstModule;
    if (hasModule)
    {
        fileDstModule.setFileName( moduleFilePathInTempDir(objectName) );

        // treat as no-module if module file is absent
        if (!fileDstModule.exists())
            hasModule = false;
    }

    // delete existing files
    FileUtil::deleteFile( fileSrc.fileName() );


    // open files
    fileSrc.open( QIODevice::WriteOnly );
    fileDstDesign.open( QIODevice::ReadOnly );
    if (hasModule)
        fileDstModule.open( QIODevice::ReadOnly );

    // codec
    determineCodecForProject();

    // streams
    QTextStream streamSrc( &fileSrc );
    QTextStream streamDstDesign( &fileDstDesign );
    QTextStream streamDstModule;

    streamSrc.setDevice( &fileSrc );
    streamSrc.setCodec( m_codecForProject->codec() );
    streamSrc.setGenerateByteOrderMark( m_codecForProject->bom() );

    streamDstDesign.setDevice( &fileDstDesign );
    streamDstDesign.setCodec( m_codecForCvs->codec() );
    streamDstDesign.setGenerateByteOrderMark( m_codecForCvs->bom() );

    if (hasModule)
    {
        streamDstModule.setDevice( &fileDstModule );
        streamDstModule.setCodec( m_codecForCvs->codec() );
        streamDstModule.setGenerateByteOrderMark( m_codecForCvs->bom() );
    }


    // de-sanitize : just join two files
    {
        while (!streamDstDesign.atEnd())
            streamSrc << streamDstDesign.readLine() << m_codecForProject->lineEnd();
    }
    if (hasModule)
    {
        while (!streamDstModule.atEnd())
            streamSrc << streamDstModule.readLine() << m_codecForProject->lineEnd();
    }


    // TODO: add post process for subclasses.


    // close files
    fileSrc.close();
    fileDstDesign.close();
    if (hasModule)
        fileDstModule.close();


    // delete files
//    FileUtil::deleteFile( fileDstDesign.fileName() ); // keep original files
//    FileUtil::deleteFile( fileDstModule.fileName() ); // keep original files

    return true;
}

void AccessDesignObjectSetting::determineCodecForProject()
{


    if (!m_codecForProject)
    {
        bool isUcs2 = true;

        if (false)
        {
            // https://github.com/timabell/msaccess-vcs-integration/blob/master/MSAccess-VCS/VCS_File.bas

            // In this way, we have to export at least one object from project.

            QString objectName = "????";
            QFile fileCodec ( tempFilePathInTempDir(objectName) );
            fileCodec.open( QIODevice::ReadOnly );
            QByteArray header = fileCodec.read( 2 );
            fileCodec.close();

            if ( header.at(0) == '\xFF' && header.at(1) == '\xFE' )
                isUcs2 = true;
            else
                isUcs2 = false;
        }



        if (true)
        {
            // TODO: we need some research this logic is right

            if ( m_projectSetting->isADP())
            {
                isUcs2 = false;
            }
            else
            {
                ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
                int fileFormat = currentProject->FileFormat();
                switch (fileFormat)
                {
                    case Access::acFileFormatAccess2:       isUcs2 = false; break;
                    case Access::acFileFormatAccess95:      isUcs2 = false; break;
                    case Access::acFileFormatAccess97:      isUcs2 = false; break;
                    case Access::acFileFormatAccess2000:    isUcs2 = false; break;
                    case Access::acFileFormatAccess2002:    isUcs2 = false; break;
                    case Access::acFileFormatAccess2007:    isUcs2 = true;  break;
                    default:                                isUcs2 = true;  break;
                }
            }
        }

        m_codecForProject = new CodecInfo(this);
        if ( isUcs2 )
        {
            m_codecForProject->setCodec( QTextCodec::codecForName( "UTF-16LE" ) );
            m_codecForProject->setBom( true );
        }
        else
        {
            m_codecForProject->setCodec( QTextCodec::codecForName( "Shift_JIS" ) );
            m_codecForProject->setBom( false );
        }

        m_codecForProject->setLineEnd( "\r\n" );
    }
}


//=============================================================================
// Form

FormSetting::FormSetting(ProjectSetting *parent)
    : AccessDesignObjectSetting(parent)
{
    m_objectType          = Model::Form;
    m_accessObjectType    = Access::acForm;
    m_objectPathName      = "forms";
    m_containerName       = "Forms";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "frm";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = m_designFileExtension;
}

bool FormSetting::prepareItemCollection()
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllForms() );

    return m_objects.is();
}

int FormSetting::itemCount()
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *FormSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}


//=============================================================================
// Report

ReportSetting::ReportSetting(ProjectSetting *parent)
    : AccessDesignObjectSetting(parent)
{
    m_objectType          = Model::Report;
    m_accessObjectType    = Access::acReport;
    m_objectPathName      = "reports";
    m_containerName       = "Reports";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "rpt";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = m_designFileExtension;
}

bool ReportSetting::prepareItemCollection()
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllReports() );

    return m_objects.is();
}

int ReportSetting::itemCount()
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *ReportSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}


//=============================================================================
// Macro

MacroSetting::MacroSetting(ProjectSetting *parent)
    : AccessDesignObjectSetting(parent)
{
    m_objectType          = Model::Macro;
    m_accessObjectType    = Access::acMacro;
    m_objectPathName      = "macros";
    m_containerName       = "Scripts";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "mcr";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;
}

bool MacroSetting::prepareItemCollection()
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllMacros() );

    return m_objects.is();
}

int MacroSetting::itemCount()
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *MacroSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP())
        return AccessDesignObjectSetting::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}


//=============================================================================
// Module

ModuleSetting::ModuleSetting(ProjectSetting *parent)
    : AccessObjectSetting(parent)
{
    m_objectType          = Model::Module;
    m_accessObjectType    = Access::acModule;
    m_objectPathName      = "modules";
    m_containerName       = "Modules";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "";
    m_moduleFileExtension = "bas";
    m_existCheckExtension = m_moduleFileExtension;
}

bool ModuleSetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // we have to convert ONLY codec.

    // codec
    determineCodecForProject();

    FileUtil::copyContents(   tempFilePathInTempDir(objectName), m_codecForProject,
                            moduleFilePathInTempDir(objectName), m_codecForCvs );
//    FileUtil::deleteFile(     tempFilePathInTempDir(objectName) ); // keep original files

    return true;
}

bool ModuleSetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    // we have to convert ONLY codec.

    // codec
    determineCodecForProject();

    FileUtil::copyContents( moduleFilePathInTempDir(objectName), m_codecForCvs,
                              tempFilePathInTempDir(objectName), m_codecForProject );
//    FileUtil::deleteFile(   moduleFilePathInTempDir(objectName) ); // keep original files

    return true;
}

bool ModuleSetting::prepareItemCollection()
{
    if (!m_projectSetting->isADP())
        return AccessObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllModules() );

    return m_objects.is();
}

int ModuleSetting::itemCount()
{
    if (!m_projectSetting->isADP())
        return AccessObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *ModuleSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP())
        return AccessObjectSetting::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}

void ModuleSetting::determineCodecForProject()
{
    // Module is always saved in Shift_JIS.
    // so we have to convert ONLY codec.
    if (!m_codecForProject)
    {
        m_codecForProject = new CodecInfo(this);
        m_codecForProject->setCodec( QTextCodec::codecForName("Shift_JIS") );
        m_codecForProject->setBom(false);
        m_codecForProject->setLineEnd("\r\n");
    }
}


//=============================================================================
// Reference

ReferenceSetting::ReferenceSetting(ProjectSetting *parent)
    : ObjectSetting(parent)
    , m_objectName("Reference")
{
    m_objectType          = Model::Reference;
    m_accessObjectType    = -1;
    m_objectPathName      = "";
    m_containerName       = "";

    m_tempFileExtension   = "csv";
    m_designFileExtension = m_tempFileExtension;
    m_moduleFileExtension = "";
    m_existCheckExtension = m_tempFileExtension;
}

bool ReferenceSetting::isTargetObject(QAxObject *object) const
{
    Q_UNUSED(object)
    return true;
}

ObjectItem *ReferenceSetting::createItemFromProject(QAxObject *object, QObject *parent)
{
    Q_UNUSED(object)
    ObjectItem *item = new ObjectItem(parent);

    {
        item->setObjectType( m_objectType );
        item->setName( m_objectName );
        item->setInProject( Model::Present );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool ReferenceSetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    ComPtr<Access::References> references = m_projectSetting->application()->References();
    int nCount = references->Count();

    //------------------------------------------------------------------------------------------
    // Export References as Text
    QStringList referenceLines;
    for ( int i = 1 ; i <= nCount ; ++i )
    {
        QString referenceLine = "";
        QStringList refParts;

        ComPtr<Access::Reference> reference = references->Item( i );

        bool refBuiltIn = reference->BuiltIn();
        QString refName = reference->Name();
        QString refGuid = reference->Guid();

//        int  refKind = reference->Kind();
/*    enum vbext_RefKind {
        vbext_rk_TypeLib        = 0,
        vbext_rk_Project        = 1
    };
*/
//        bool refBroken = reference->IsBroken();

        refParts << refName;    //[0]
        refParts << (refBuiltIn ? QString("1") : QString("0"));   //[1]

        if ( refGuid.length() > 0 )
        {
            QString refMajor = QString().setNum( reference->Major() );
            QString refMinor = QString().setNum( reference->Minor() );
            refParts << refGuid;    //[2]
            refParts << refMajor;   //[3]
            refParts << refMinor;   //[4]
            refParts << "";         //[5]
        }
        else
        {
            // references of types mdb,accdb,mde etc don't have a GUID
            QString fullPath = reference->FullPath();
            refParts << "";         //[2]
            refParts << "";         //[3]
            refParts << "";         //[4]
            refParts << fullPath;   //[5]
        }

        referenceLine = refParts.join(',');
        referenceLines << referenceLine;
    }
    // write contents to file
    FileUtil::saveToFile(referenceLines.join( m_codecForCvs->lineEnd() ), tempFilePathInTempDir(m_objectName), m_codecForCvs );

    return true;
}

bool ReferenceSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    ComPtr<Access::References> references = m_projectSetting->application()->References();

    // clear current references
    {
        int nCount = references->Count();
        for ( int i = nCount ; i >= 1 ; --i )
        {

            ComPtr<Access::Reference> reference = references->Item( i );
            bool refBuiltIn = reference->BuiltIn();
            if (!refBuiltIn)
            {
                references->Remove( reference.ptr() );
            }
        }
    }

    // add references
    {
        QStringList referenceLines = FileUtil::loadAsStringList( tempFilePathInTempDir(m_objectName), m_codecForCvs );
        int nCount = referenceLines.count();
        for ( int i = 0 ; i < nCount ; ++i )
        {
            QString referenceLine = referenceLines[ i ];
            QStringList refParts = referenceLine.split(',');

            QString refName     =   refParts[ 0 ];
            bool    refBuiltIn  = ( refParts[ 1 ] == "1" );
            QString refGUID     =   refParts[ 2 ];
            int     refMajor    =   refParts[ 3 ].toInt();
            int     refMinor    =   refParts[ 4 ].toInt();
            QString refFullPath =   refParts[ 5 ];
            if ( !refBuiltIn )
            {
                if ( !refGUID.isEmpty() )
                {
                    ComPtr<Access::Reference> reference = references->AddFromGuid(refGUID, refMajor, refMinor);
                    Q_UNUSED(reference)
                    // NOTE: error 32813 may occur : The reference is already present in the access project
                }
                else
                {
                    ComPtr<Access::Reference> reference = references->AddFromFile(refFullPath);
                    Q_UNUSED(reference)
                }
            }
        }
    }

    return true;
}

bool ReferenceSetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool ReferenceSetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool ReferenceSetting::prepareItemCollection()
{
    // different from others.
    if (m_projectSetting->isMDB() || m_projectSetting->isADP())
        return true;
    return false;
}

int ReferenceSetting::itemCount()
{
    // different from others.
    if (m_projectSetting->isMDB() || m_projectSetting->isADP())
        return 1;
    return 0;
}

QAxObject *ReferenceSetting::itemUnsafePtr(const QVariant &index)
{
    Q_UNUSED(index)
    // different from others.
    if (m_projectSetting->isMDB() || m_projectSetting->isADP())
        return m_projectSetting->application()->References();
    return 0;
}




