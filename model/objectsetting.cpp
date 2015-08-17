#include "objectsetting.h"

#include <QDebug>
#include <QDir>

#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QSettings>

#include "officelib/officelib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "projectsetting.h"
#include "sanitizesetting.h"
#include "tabledefsanitizesetting.h"
#include "tabledatasanitizesetting.h"

#include <windows.h>
#include <wingdi.h>

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



ObjectItem *ObjectSetting::createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( m_objectType );
    item->setName( fileInfo.completeBaseName() );
    item->setInSourceDir( Model::Present );

    return item;
}

bool ObjectSetting::copyFromTempDirToSourceDir(const QString &objectName)
{
    deleteAllFileFromSourceDir(objectName);
    copyFile(TempDir, SourceDir, DesignFile, objectName);
    copyFile(TempDir, SourceDir, ModuleFile, objectName);
    copyFile(TempDir, SourceDir, DataFile,   objectName);
    copyFile(TempDir, SourceDir, ReportPropFile, objectName);
    return true;
}

bool ObjectSetting::copyFromSourceDirToTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    copyFile(SourceDir, TempDir, DesignFile, objectName);
    copyFile(SourceDir, TempDir, ModuleFile, objectName);
    copyFile(SourceDir, TempDir, DataFile,   objectName);
    copyFile(SourceDir, TempDir, ReportPropFile, objectName);
    return true;
}

bool ObjectSetting::compareTempDir(const QString &objectName, bool *pisDifferent)
{
    bool isSame = true;

    // design
    if (isSame && !m_designFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   DesignFile, objectName),
                                    filePath(SourceDir, DesignFile, objectName), false/*isSameIfBothNonExist*/ );
    }
    // module
    if (isSame && !m_moduleFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   ModuleFile, objectName),
                                    filePath(SourceDir, ModuleFile, objectName), false/*isSameIfBothNonExist*/ );
    }
    // data
    if (isSame && !m_dataFileExtension.isEmpty())
    {
        // We specify isSameIfBothNonExist is true.
        // This looks like wrong because isSameIfBothNonExist value must be determined by hasData.
        // but when hasData() is changed, we clear all files in TempDir. Therefore treat as Not-Same.

        isSame = FileUtil::compare( filePath(TempDir,   DataFile, objectName),
                                    filePath(SourceDir, DataFile, objectName), true/*isSameIfBothNonExist*/ );
    }
    // report-prop
    if (isSame && !m_reportPropFileExtension.isEmpty())
    {
        isSame = FileUtil::compare( filePath(TempDir,   ReportPropFile, objectName),
                                    filePath(SourceDir, ReportPropFile, objectName), false/*isSameIfBothNonExist*/ );
    }

    *pisDifferent = !isSame;

    return true;
}

bool ObjectSetting::deleteFromSourceDir(const QString &objectName)
{
    deleteAllFileFromSourceDir(objectName);
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

bool ObjectSetting::deleteFromTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
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

void ObjectSetting::updateFileTimeInTempDir(const QString &objectName, const QDateTime &fileTime)
{
    FileUtil::setFileTime( filePath(TempDir, TempFile,     objectName), fileTime, fileTime );
    FileUtil::setFileTime( filePath(TempDir, DataTempFile, objectName), fileTime, fileTime );
}

void ObjectSetting::determineCodecForProject()
{
    // do nothing here.
    // if m_codecForProject is requred, override this function in subclass
}

bool ObjectSetting::deleteCvsFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, DesignFile, objectName);
    deleteFile(TempDir, ModuleFile, objectName);
    deleteFile(TempDir, DataFile,   objectName);
    deleteFile(TempDir, ReportPropFile, objectName);
    return true;
}

bool ObjectSetting::deleteTempFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, TempFile,     objectName);
    deleteFile(TempDir, DataTempFile, objectName);
    return true;
}

bool ObjectSetting::deleteAllFileFromTempDir(const QString &objectName)
{
    deleteTempFileFromTempDir(objectName);
    deleteCvsFileFromTempDir(objectName);
    return true;
}

bool ObjectSetting::deleteAllFileFromSourceDir(const QString &objectName)
{
    deleteFile(SourceDir, DesignFile, objectName);
    deleteFile(SourceDir, ModuleFile, objectName);
    deleteFile(SourceDir, DataFile,   objectName);
    deleteFile(SourceDir, ReportPropFile, objectName);
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
        case DataTempFile:  return m_dataTempFileExtension;
        case DataFile:      return m_dataFileExtension;
        case ReportPropFile: return m_reportPropFileExtension;
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
    , m_tableDefSanitizer(new TableDefSanitizeSetting(this))
    , m_tableDataSanitizer(new TableDataSanitizeSetting(this))
{
    m_objectType          = Model::TableDef;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledefs";
    m_containerName       = "";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "xml";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;

    m_dataTempFileExtension = "dattmp";
    m_dataFileExtension     = "dat";
}

bool TableDefSetting::isTargetObject(QAxObject *object) const
{
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        QString name = tableDef->Name();
        return !name.startsWith("MSys") && !name.startsWith("~") && tableDef->Connect().isEmpty();
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
        item->setHasData( m_tableDataTargets.contains( item->name() ) );
    }

    return item;
}

bool TableDefSetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        // table-def
        {
            m_projectSetting->application()
                ->ExportXML(
                        Access::acExportTable
                        ,objectName
                        ,QString() // DataTarget
                        ,filePath(TempDir, TempFile, objectName) // SchemaTarget
                        ,QString() //PresentationTarget
                        ,QString() //ImageTarget
                        ,Access::acUTF8 //Encoding
                        ,Access::acExportAllTableAndFieldProperties //OtherFlags
                        ,QString() //WhereCondition
                        ,QVariant()//AdditionalData
                        );
        }
        // table-data
        if ( m_tableDataTargets.contains( objectName ) )
        {
            m_projectSetting->application()
                ->ExportXML(
                        Access::acExportTable
                        ,objectName
                        ,filePath(TempDir, DataTempFile, objectName) // DataTarget
                        ,QString() // SchemaTarget
                        ,QString() // PresentationTarget
                        ,QString() // ImageTarget
                        ,Access::acUTF8 // Encoding
                        );
        }

        return true;
    }
    return false;
}

bool TableDefSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    if (object)
    {
        // table is already exists. so we need to delete first
        ComPtr<Access::DoCmd> doCmd = m_projectSetting->application()->DoCmd();
        doCmd->DeleteObject( (Access::AcObjectType)m_accessObjectType, objectName );
    }

    {
        // table-def
        {
            // very slow but very accurate.
            m_projectSetting->application()
                ->ImportXML(
                        filePath(TempDir, TempFile, objectName)
                        ,Access::acStructureOnly
                        );
        }
        // table-data
        if ( m_tableDataTargets.contains(objectName) && QFile( filePath(TempDir, DataTempFile, objectName) ).exists() )
        {
            m_projectSetting->application()
                ->ImportXML(
                        filePath(TempDir, DataTempFile, objectName)
                        ,Access::acAppendData
                        );
        }

        return true;
    }
    return false;
}

bool TableDefSetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // codec
    determineCodecForProject();

    // sanitize table-def
    {
        QFile fileSrc( filePath(TempDir, TempFile,   objectName) );
        QFile fileDst( filePath(TempDir, DesignFile, objectName) );

        // delete existing file
        FileUtil::deleteFile( fileDst.fileName() );

        // open files
        if ( !fileSrc.open( QIODevice::ReadOnly) )
        {
            return false;
        }
        if ( !fileDst.open(QIODevice::WriteOnly) )
        {
            fileSrc.close();
            return false;
        }

        // open streams
        QTextStream streamSrc( &fileSrc );
        streamSrc.setCodec(m_codecForProject->codec());
        // bom resolved automatically

        QTextStream streamDst( &fileDst );
        streamDst.setCodec(m_codecForCvs->codec());
        streamDst.setGenerateByteOrderMark(m_codecForCvs->bom());

        // sanitize table-def
        m_tableDefSanitizer->sanitize( streamSrc, streamDst, m_codecForCvs );

        // close files
        fileSrc.close();
        fileDst.close();

    }

    // sanitize table-data
    if ( m_tableDataTargets.contains(objectName) )
    {
        QFile fileSrc( filePath(TempDir, DataTempFile, objectName) );
        QFile fileDst( filePath(TempDir, DataFile,     objectName) );

        // delete existing file
        FileUtil::deleteFile( fileDst.fileName() );

        // open files
        if ( !fileSrc.open( QIODevice::ReadOnly) )
        {
            return false;
        }
        if ( !fileDst.open(QIODevice::WriteOnly) )
        {
            fileSrc.close();
            return false;
        }

        // open streams
        QTextStream streamSrc( &fileSrc );
        streamSrc.setCodec(m_codecForProject->codec());
        // bom resolved automatically

        QTextStream streamDst( &fileDst );
        streamDst.setCodec(m_codecForCvs->codec());
        streamDst.setGenerateByteOrderMark(m_codecForCvs->bom());

        // sanitize table-data
        m_tableDataSanitizer->sanitize( streamSrc, streamDst, m_codecForCvs );

        // close files
        fileSrc.close();
        fileDst.close();
    }

    return true;
}

bool TableDefSetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // we have to convert ONLY codec.

    // codec
    determineCodecForProject();

    FileUtil::copyContents( filePath(TempDir, DesignFile, objectName), m_codecForCvs,
                            filePath(TempDir, TempFile,   objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
//    FileUtil::deleteFile(   designFilePathInTempDir(objectName) ); keep original files

    // de-sanitize data-file : just convert codec.
    if ( m_tableDataTargets.contains(objectName) )
    {
        FileUtil::copyContents( filePath(TempDir, DataFile,     objectName), m_codecForCvs,
                                filePath(TempDir, DataTempFile, objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
    }

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

void TableDefSetting::loadSettings(QSettings *settings)
{
    // FIXME: groupName, keyName must be class level
    settings->beginGroup("TableDef");
    {
        m_tableDataTargets.clear();
        int size = settings->beginReadArray("TableData");
        for ( int i = 0 ; i < size ; ++i )
        {
            settings->setArrayIndex(i);
            QVariant tableName = settings->value("TableName");
            if (tableName.isValid())
                m_tableDataTargets.append( tableName.toString() );
        }
        settings->endArray();
    }
    settings->endGroup();
}

void TableDefSetting::saveSettings(QSettings *settings)
{
    // FIXME: groupName, keyName must be class level
    settings->beginGroup("TableDef");
    {
        settings->beginWriteArray("TableData");
        int i = 0;
        for ( QStringList::iterator it = m_tableDataTargets.begin() ; it != m_tableDataTargets.end() ; ++it )
        {
            settings->setArrayIndex(i);
            settings->setValue("TableName", (*it) );
            ++i;
        }
        settings->endArray();
    }
    settings->endGroup();
}

void TableDefSetting::setTableDataTargets(QStringList *newTargets)
{
    m_tableDataTargets.clear();
    for ( QStringList::iterator it = newTargets->begin() ; it != newTargets->end() ; ++it )
        m_tableDataTargets.append( (*it) );
}

void TableDefSetting::determineCodecForProject()
{
    if (!m_codecForProject)
    {
        m_codecForProject = new CodecInfo(this);
        m_codecForProject->setCodec( QTextCodec::codecForName("UTF-8") );
        m_codecForProject->setBom( false );
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
// QueryAsSqlSetting

QueryAsSqlSetting::QueryAsSqlSetting(ProjectSetting *parent)
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

bool QueryAsSqlSetting::isTargetObject(QAxObject *object) const
{
    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        return !queryDef->Name().startsWith("~") && queryDef->Connect().isEmpty();
    }
    return false;
}

ObjectItem *QueryAsSqlSetting::createItemFromProject(QAxObject *object, QObject *parent)
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

bool QueryAsSqlSetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        QString sql = queryDef->SQL();
        FileUtil::saveToFile( sql, filePath(TempDir, TempFile, objectName), m_codecForCvs );

        return true;
    }
    return false;
}

bool QueryAsSqlSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    bool inProject = (object != NULL);

    QString sql = FileUtil::loadAsString( filePath(TempDir, TempFile, objectName), m_codecForCvs );

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

bool QueryAsSqlSetting::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool QueryAsSqlSetting::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sinitization required
    return true;
}

bool QueryAsSqlSetting::prepareItemCollection()
{
    if (!m_projectSetting->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_queryDefs.set( currentDb->QueryDefs() );
    return m_queryDefs.is();
}

int QueryAsSqlSetting::itemCount()
{
    if (!m_queryDefs.is())
        return 0;
    return m_queryDefs->Count();
}

QAxObject *QueryAsSqlSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_queryDefs.is())
        return 0;
    return m_queryDefs->Item(index);
}



//=============================================================================
// QueryAsObjectSetting
// NOTE: this class is export/import by SaveAsText/LoadFromText.
//       this class is implemented to solve the problem that
//       "join-on criterias are always different between import and export".
//       for example,
//           if   import as 'SELECT ... FROM A INNER JOIN B ON ( A.x = B.x ) AND (A.y = B.y)' ... statement(N)
//           then export as 'SELECT ... FROM A INNER JOIN B ON ( A.y = B.y ) AND (A.x = B.x)' ... statement(M)
//               ( swapped the criterias after ON )
//           and then import as (M) again, then export as (N)...
//        this problem cannot be solved simply.
//        we set it as Known Issue.
//
QueryAsObjectSetting::QueryAsObjectSetting(ProjectSetting *parent)
    : QueryAsSqlSetting(parent)
{
}

bool QueryAsObjectSetting::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    Q_UNUSED(object)
    {
        m_projectSetting->application()->SaveAsText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );
        return true;
    }
}

bool QueryAsObjectSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    {
        m_projectSetting->application()->LoadFromText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );

        return true;
    }
    return true;
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
    deleteAllFileFromTempDir(objectName);

    Q_UNUSED(object)
    {
        m_projectSetting->application()->SaveAsText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );
        return true;
    }
    return false;
}

bool AccessObjectSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    {
        m_projectSetting->application()->LoadFromText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );

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
    QFile fileSrc      ( filePath(TempDir, TempFile,   objectName) );
    QFile fileDstDesign( filePath(TempDir, DesignFile, objectName) );
    QFile fileDstModule;
    if (hasModule)
        fileDstModule.setFileName( filePath(TempDir, ModuleFile, objectName) );

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
    m_sanitizer->sanitize( streamSrc, m_codecForProject, streamDstDesign, streamDstModule, m_codecForCvs );


    // post process for subclasses.
    afterSanitizeTempDir(object, objectName);


    // close files
    fileSrc.close();
    fileDstDesign.close();
    if (hasModule)
        fileDstModule.close();


    // delete temp file
//    FileUtil::deleteFile( fileSrc.fileName() ); // keep original files

    // Check design data corruption for Unicode-Shift_JIS problem.
    // FIXME: this must be independent function and store results to ObjectItem
    //if ( m_codecForProject->codec()->name() ==  "???") // FIXME: check only for Shift_JIS.
    {
        QFile fileDstDesign( filePath(TempDir, DesignFile, objectName) );
        fileDstDesign.open( QIODevice::ReadOnly );
        QTextStream streamDstDesign( &fileDstDesign );
        QString line = "";
        int lineNum = 0;
        while (!streamDstDesign.atEnd())
        {
            ++lineNum;
            line = streamDstDesign.readLine();
            if (line.contains('?'))
            {
                qCritical() << QString("'?' found!  line :%1, content :%2, file :%3").arg(lineNum).arg(line).arg(fileDstDesign.fileName());
            }
        }
        fileDstDesign.close();
    }

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
    QFile fileSrc      ( filePath(TempDir, TempFile,   objectName) );
    QFile fileDstDesign( filePath(TempDir, DesignFile, objectName) );
    QFile fileDstModule;
    if (hasModule)
    {
        fileDstModule.setFileName( filePath(TempDir, ModuleFile, objectName) );

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
        if (m_codecForProject->codec()->name() == "Shift_JIS")
        {
            while (!streamDstDesign.atEnd())
            {
                QString line = streamDstDesign.readLine();
                line += m_codecForProject->lineEnd();

                QByteArray lineData = line.toLocal8Bit();
                fileSrc.write( lineData );
            }
        }
        else
        {
            while (!streamDstDesign.atEnd())
            {
                QString line = streamDstDesign.readLine();
                streamSrc << line << m_codecForProject->lineEnd();
            }
        }
    }
    if (hasModule)
    {
        if (m_codecForProject->codec()->name() == "Shift_JIS")
        {
            while (!streamDstModule.atEnd())
            {
                QString line = streamDstModule.readLine();
                line += m_codecForProject->lineEnd();
                QByteArray lineData = line.toLocal8Bit();
                fileSrc.write( lineData );
            }
        }
        else
        {
            while (!streamDstModule.atEnd())
                streamSrc << streamDstModule.readLine() << m_codecForProject->lineEnd();
        }
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
            QFile fileCodec ( filePath(TempDir, TempFile, objectName) );
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

bool AccessDesignObjectSetting::afterSanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    return true;
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
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllForms() );

    return m_objects.is();
}

int FormSetting::itemCount()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *FormSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
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

    m_reportPropFileExtension = "rpp";
}

bool ReportSetting::prepareItemCollection()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllReports() );

    return m_objects.is();
}

int ReportSetting::itemCount()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *ReportSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::itemUnsafePtr(index);

    if (!m_objects.is())
        return 0;
    return m_objects->Item(index);
}

bool ReportSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    if ( !AccessDesignObjectSetting::importFromTempDirToProject(object, objectName) )
        return false;

    // if report-prop-file doesn't exist, return true (it's normal)
    if ( !QFile( filePath( TempDir, ReportPropFile, objectName ) ).exists() )
        return true;

    // open report-prop-file
    QSettings settings( filePath( TempDir, ReportPropFile, objectName ), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    // PrtDevMode
    settings.beginGroup("PrtDevMode");
    {
        bool hasPrtDevMode = settings.value("hasPrtDevMode", false).toBool();

        if ( hasPrtDevMode )
        {
            // open target report in design view
            ComPtr<Access::DoCmd> doCmd = m_projectSetting->application()->DoCmd();
            doCmd->OpenReport( objectName, Access::acViewDesign );
            ComPtr<Access::Reports> reports = m_projectSetting->application()->Reports();
            ComPtr<Access::Report> report = reports->Item( objectName );

            // retreive PrtDevMode
            QByteArray prtDevModeDataSrc = report->PrtDevMode().toByteArray();

            // consturct new PrtDevMode
            DEVMODEA dm;
            mempcpy( &dm, (const void*)prtDevModeDataSrc.constData(), sizeof(dm) );
            dm.dmOrientation = settings.value("dmOrientation", DMORIENT_PORTRAIT).toInt();
            dm.dmPaperSize   = settings.value("dmPaperSize",   DMPAPER_A4).toInt();
            dm.dmPaperLength = settings.value("dmPaperLength", 0).toInt(); // FIXME: need meaningful default value
            dm.dmPaperWidth  = settings.value("dmPaperWidth",  0).toInt(); // FIXME: need meaningful default value
            dm.dmScale       = settings.value("dmScale",       100).toInt();
            dm.dmColor       = settings.value("dmColor",       DMCOLOR_COLOR).toInt();

            QByteArray prtDevModeDataDst( (const char *)&dm, sizeof(dm) );

            // set new PrtDevMode to report
            report->SetPrtDevMode( prtDevModeDataDst );

            // save report
            doCmd->Close( Access::acReport, objectName, Access::acSaveYes );
        }
    }
    settings.endGroup();

    return true;
}

bool ReportSetting::afterSanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object);

    // report properties

    FileUtil::deleteFile( filePath( TempDir, ReportPropFile, objectName ) );

    // open report-prop-file
    QSettings settings( filePath( TempDir, ReportPropFile, objectName ), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    // PrtDevMode
    settings.beginGroup("PrtDevMode");
    {
        QByteArray prtDevModeData = m_sanitizer->blockData( "PrtDevMode" );
        if ( prtDevModeData.size() > 0 )
        {
            const void *pprtDevModeData = (const void*)prtDevModeData.constData();
            const DEVMODEA *pdm = static_cast<const DEVMODEA*>(pprtDevModeData);

            settings.setValue( "hasPrtDevMode", true );
            settings.setValue( "dmOrientation", pdm->dmOrientation );
            settings.setValue( "dmPaperSize",   pdm->dmPaperSize );
            settings.setValue( "dmPaperLength", pdm->dmPaperLength );
            settings.setValue( "dmPaperWidth",  pdm->dmPaperWidth );
            settings.setValue( "dmScale",       pdm->dmScale );
            settings.setValue( "dmColor",       pdm->dmColor );

        }
        else
        {
            settings.setValue( "hasPrtDevMode", false );
        }
    }
    settings.endGroup();

    return true;
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
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllMacros() );

    return m_objects.is();
}

int MacroSetting::itemCount()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessDesignObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *MacroSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
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

    // FIXME: use fromLocal8Bit
    FileUtil::copyContents( filePath(TempDir, TempFile,   objectName), m_codecForProject,
                            filePath(TempDir, ModuleFile, objectName), m_codecForCvs, true/*removeTrailingSpaces*/ );
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

    // FIXME: use toLocal8Bit
    FileUtil::copyContents( filePath(TempDir, ModuleFile, objectName), m_codecForCvs,
                            filePath(TempDir, TempFile,   objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
//    FileUtil::deleteFile(   moduleFilePathInTempDir(objectName) ); // keep original files

    return true;
}

bool ModuleSetting::prepareItemCollection()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessObjectSetting::prepareItemCollection();

    ComPtr<Access::CurrentProject> currentProject = m_projectSetting->application()->CurrentProject();
    m_objects.set( currentProject->AllModules() );

    return m_objects.is();
}

int ModuleSetting::itemCount()
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
        return AccessObjectSetting::itemCount();

    if (!m_objects.is())
        return 0;
    return m_objects->Count();
}

QAxObject *ModuleSetting::itemUnsafePtr(const QVariant &index)
{
    if (!m_projectSetting->isADP() && !m_projectSetting->isMDB())
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

    m_tempFileExtension   = "ref";
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

    deleteAllFileFromTempDir(objectName);

    ComPtr<Access::References> references = m_projectSetting->application()->References();
    int nCount = references->Count();

    QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    settings.beginWriteArray("Reference");
    for ( int i = 1 ; i <= nCount ; ++i )
    {
        ComPtr<Access::Reference> reference = references->Item( i );

        settings.setArrayIndex( i-1 );

        QString name     = reference->Name();
        bool    builtIn  = reference->BuiltIn();
        QString guid     = reference->Guid();
        int     major    = reference->Major();
        int     minor    = reference->Minor();
        QString fullPath = reference->FullPath();

        if ( !guid.isEmpty() )
            fullPath = "";

        settings.setValue( "BuiltIn",  builtIn );
        settings.setValue( "Name",     name );
        settings.setValue( "Guid",     guid );
        settings.setValue( "Major",    major );
        settings.setValue( "Minor",    minor );
        settings.setValue( "FullPath", fullPath );

    }
    settings.endArray();

    return true;
}

bool ReferenceSetting::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)

    if ( !QFile( filePath(TempDir, TempFile, m_objectName) ).exists())
        return true;

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


    QSettings settings( filePath(TempDir, TempFile, m_objectName), QSettings::IniFormat, this );
    settings.setIniCodec( m_codecForCvs->codec() );

    // add references
    int nCount = settings.beginReadArray("Reference");
    for ( int i = 1 ; i <= nCount ; ++i )
    {
        settings.setArrayIndex( i-1 );

        QString name     = settings.value("Name").toString();   Q_UNUSED(name)
        bool    builtIn  = settings.value("BuiltIn").toBool();
        QString guid     = settings.value("Guid").toString();
        int     major    = settings.value("Major").toInt();
        int     minor    = settings.value("Minor").toInt();
        QString fullPath = settings.value("FullPath").toString();

        if ( !builtIn )
        {
            if ( !guid.isEmpty() )
            {
                ComPtr<Access::Reference> reference = references->AddFromGuid(guid, major, minor);
                Q_UNUSED(reference)
                // NOTE: error 32813 may occur : The reference is already present in the access project
            }
            else
            {
                ComPtr<Access::Reference> reference = references->AddFromFile(fullPath);
                Q_UNUSED(reference)
            }
        }
    }
    settings.endArray();

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




