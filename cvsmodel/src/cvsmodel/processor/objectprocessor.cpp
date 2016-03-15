#include "objectprocessor.h"

#include <QDebug>
#include <QDir>

#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QRegularExpression>
#include <QUuid>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/projectsetting.h"
#include "cvsmodel/sanitizer/accessdesignobjectsanitizer.h"
#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"
#include "cvsmodel/setting.h"

#include <windows.h>
#include <wingdi.h>

ObjectProcessor::ObjectProcessor(ProjectSetting *parent)
    : QObject(parent)
    , m_projectSetting(parent)
    , m_codecForCvs(new CodecInfo(this))
    , m_codecForProject(0)
{
    m_codecForCvs->setCodec( QTextCodec::codecForName("UTF-8") );
    m_codecForCvs->setBom(false);
    m_codecForCvs->setLineEnd("\r\n");
}


QString ObjectProcessor::sourceObjectPath() const
{
    return objectPath(SourceDir);
}

QString ObjectProcessor::tempObjectPath() const
{
    return objectPath(TempDir);
}

void ObjectProcessor::mkdirTempObjectPath()
{
    mkpathObjectPath(TempDir);
}

void ObjectProcessor::mkdirSourceObjectPath()
{
    mkpathObjectPath(SourceDir);
}



ObjectItem *ObjectProcessor::createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    item->setObjectType( m_objectType );
    item->setName( fileInfo.completeBaseName() );
    item->setInSourceDir( Model::Present );

    return item;
}

bool ObjectProcessor::afterImportFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    return true;
}

bool ObjectProcessor::copyFromTempDirToSourceDir(const QString &objectName)
{
    deleteAllFileFromSourceDir(objectName);
    copyFile(TempDir, SourceDir, DesignFile, objectName);
    copyFile(TempDir, SourceDir, ModuleFile, objectName);
    copyFile(TempDir, SourceDir, DataFile,   objectName);
    copyFile(TempDir, SourceDir, ReportPropFile, objectName);
    return true;
}

bool ObjectProcessor::copyFromSourceDirToTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    copyFile(SourceDir, TempDir, DesignFile, objectName);
    copyFile(SourceDir, TempDir, ModuleFile, objectName);
    copyFile(SourceDir, TempDir, DataFile,   objectName);
    copyFile(SourceDir, TempDir, ReportPropFile, objectName);
    return true;
}

bool ObjectProcessor::compareTempDir(const QString &objectName, bool *pisDifferent)
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

bool ObjectProcessor::deleteFromSourceDir(const QString &objectName)
{
    deleteAllFileFromSourceDir(objectName);
    return true;
}

bool ObjectProcessor::deleteFromProject(const QString &objectName)
{
    // TODO: cache DoCmd object
    if (m_accessObjectType != -1)
    {
        ComPtr<Access::DoCmd> doCmd = m_projectSetting->application()->DoCmd();
        doCmd->DeleteObject( (Access::AcObjectType)m_accessObjectType, objectName );
    }
    return true;
}

bool ObjectProcessor::deleteFromTempDir(const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);
    return true;
}

bool ObjectProcessor::prepareItemCollection()
{
    // in subclass, override this function
    // and prepare collection object to call Count() and Item().
    return true;
}

int ObjectProcessor::itemCount()
{
    // in subclass, override this function
    // and return the value of Count().
    return 0;
}

QAxObject *ObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    // in subclass, override this function
    // and return QAxObject pointer.
    // this pointer must be deleted by CALLER.
    Q_UNUSED(index)
    return NULL;
}

void ObjectProcessor::updateFileTimeInTempDir(const QString &objectName, const QDateTime &fileTime)
{
    FileUtil::setFileTime( filePath(TempDir, TempFile,     objectName), fileTime, fileTime );
    FileUtil::setFileTime( filePath(TempDir, DataTempFile, objectName), fileTime, fileTime );
}

void ObjectProcessor::determineCodecForProject()
{
    // do nothing here.
    // if m_codecForProject is requred, override this function in subclass
}

bool ObjectProcessor::deleteCvsFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, DesignFile, objectName);
    deleteFile(TempDir, ModuleFile, objectName);
    deleteFile(TempDir, DataFile,   objectName);
    deleteFile(TempDir, ReportPropFile, objectName);
    return true;
}

bool ObjectProcessor::deleteTempFileFromTempDir(const QString &objectName)
{
    deleteFile(TempDir, TempFile,     objectName);
    deleteFile(TempDir, DataTempFile, objectName);
    return true;
}

bool ObjectProcessor::deleteAllFileFromTempDir(const QString &objectName)
{
    deleteTempFileFromTempDir(objectName);
    deleteCvsFileFromTempDir(objectName);
    return true;
}

bool ObjectProcessor::deleteAllFileFromSourceDir(const QString &objectName)
{
    deleteFile(SourceDir, DesignFile, objectName);
    deleteFile(SourceDir, ModuleFile, objectName);
    deleteFile(SourceDir, DataFile,   objectName);
    deleteFile(SourceDir, ReportPropFile, objectName);
    return true;
}





bool ObjectProcessor::copyFile(ObjectProcessor::DirectoryType dirTypeSrc, ObjectProcessor::DirectoryType dirTypeDst, ObjectProcessor::FileType fileType, const QString &objectName)
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

bool ObjectProcessor::deleteFile(ObjectProcessor::DirectoryType dirType, ObjectProcessor::FileType fileType, const QString &objectName)
{
    if (!fileExtension(fileType).isEmpty())
        FileUtil::deleteFile( filePath(dirType, fileType, objectName) );

    return true;
}








QString ObjectProcessor::rootPath(ObjectProcessor::DirectoryType dirType) const
{
    switch (dirType)
    {
        case TempDir:      return m_projectSetting->tempPath();
        case SourceDir:    return m_projectSetting->sourcePath();
    }
    return QString();
}

QString ObjectProcessor::objectPath(ObjectProcessor::DirectoryType dirType) const
{
    QString path = rootPath(dirType);
    if ( !m_objectPathName.isEmpty() )
        path += "\\" + m_objectPathName;
    return path;
}

QString ObjectProcessor::fileExtension(ObjectProcessor::FileType fileType) const
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

QString ObjectProcessor::filePath(ObjectProcessor::DirectoryType dirType, ObjectProcessor::FileType fileType, const QString &objectName) const
{
    return objectPath(dirType) + "\\" + objectName + "." + fileExtension(fileType);
}

void ObjectProcessor::mkpathObjectPath(ObjectProcessor::DirectoryType dirType)
{
    QDir dir( objectPath(dirType) );
    dir.mkpath(".");
}


//=============================================================================
// TableDef

TableDefProcessor::TableDefProcessor(ProjectSetting *parent)
    : ObjectProcessor(parent)
    , m_tableDefSanitizer(new TableDefSanitizer(this))
    , m_tableDataSanitizer(new TableDataSanitizer(this))
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

bool TableDefProcessor::isTargetObject(QAxObject *object) const
{
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        QString name = tableDef->Name();
        return !name.startsWith("MSys") && !name.startsWith("~") && tableDef->Connect().isEmpty();
    }
    return false;
}

ObjectItem *TableDefProcessor::createItemFromProject(QAxObject *object, QObject *parent)
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

ObjectItem *TableDefProcessor::createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = ObjectProcessor::createItemFromSourceDir(fileInfo, parent);
    if (item)
    {
        item->setHasData( m_tableDataTargets.contains( item->name() ) );
    }
    return item;
}

bool TableDefProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
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

bool TableDefProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
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

bool TableDefProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // codec
//    determineCodecForProject();

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

bool TableDefProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)

    // we have to convert ONLY codec.

    // codec
//    determineCodecForProject();

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

bool TableDefProcessor::prepareItemCollection()
{
    if (!m_projectSetting->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_tableDefs.set( currentDb->TableDefs() );
    return m_tableDefs.is();
}

int TableDefProcessor::itemCount()
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Count();
}

QAxObject *TableDefProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Item(index);
}

void TableDefProcessor::loadSettings(QSettings *settings)
{
    Q_UNUSED(settings)

    // QSettings
    /*
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
    */

    // Implement Own
    /*
    QString settingsFilePath = m_projectSetting->sourcePath() + "\\" + "TableDef.settings";

    m_tableDataTargets.clear();

    QFile file(settingsFilePath);
    if (!file.exists())
        return;

    file.open(QIODevice::ReadOnly);
    QTextStream stream( &file );
    stream.setGenerateByteOrderMark( m_codecForCvs->bom() );
    stream.setCodec( m_codecForCvs->codec() );

    QRegularExpression regExpBegin;
    regExpBegin.setPattern("^\\s*Begin (.*)$");
    QRegularExpression regExpEnd;
    regExpEnd.setPattern("^\\s*End$");
    QRegularExpression regExpKeyValue;
    regExpKeyValue.setPattern("^\\s*([^=]+) =(.*)$");

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        QRegularExpressionMatch matchBegin = regExpBegin.match(line);
        if (matchBegin.hasMatch())
        {
            QString elementName = matchBegin.captured(1);
            if (elementName == "TableData")
            {
                while (!stream.atEnd())
                {
                    line = stream.readLine();

                    if (regExpEnd.match(line).hasMatch())
                        break;

                    QRegularExpressionMatch matchKeyValue = regExpKeyValue.match(line);
                    if (matchKeyValue.hasMatch())
                    {
                        QString value = matchKeyValue.captured(2);
                        m_tableDataTargets.append(value);
                    }
                }
            }
        }
    }

    file.close();
    */

    // Setting
    Setting setting(m_projectSetting->sourcePath() + "\\" + "TableDef.settings", m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd());
    if (setting.load())
    {
        SettingElement *element = setting.at(0)->toElement();
        Q_ASSERT(element != NULL);
        Q_ASSERT(element->name() == "TableData");
        for ( int i = 0 ; i < element->count() ; ++i )
        {
            SettingKeyValue *keyValue = element->at(i)->toKeyValue();
            Q_ASSERT(keyValue != NULL);
            Q_ASSERT(keyValue->key() == "TableName");
            Q_ASSERT(keyValue->value().isNull() == false);
            Q_ASSERT(keyValue->value().toString().isEmpty() == false);
            m_tableDataTargets.append( keyValue->value().toString() );
        }
    }
}

void TableDefProcessor::saveSettings(QSettings *settings)
{
    Q_UNUSED(settings)

    // QSettings
    /*
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
    */

    // Implement Own
    /*
    QString settingsFilePath = m_projectSetting->sourcePath() + "\\" + "TableDef.settings";

    if (QFile(settingsFilePath).exists())
        QFile(settingsFilePath).remove();

    QFile file(settingsFilePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream( &file );
    stream.setGenerateByteOrderMark( m_codecForCvs->bom() );
    stream.setCodec( m_codecForCvs->codec() );
    QString lineEnd = m_codecForCvs->lineEnd();

    stream << "Begin TableData" << lineEnd;
    {
        QStringList tableNames = m_tableDataTargets;
        tableNames.sort(Qt::CaseSensitive);
        for ( QStringList::iterator it = tableNames.begin() ; it != tableNames.end() ; ++it )
        {
            stream << "    " << "TableName" << " =" << (*it) << lineEnd;
        }
    }
    stream << "End" << lineEnd;

    file.close();
    */

    // Setting
    Setting setting(m_projectSetting->sourcePath() + "\\" + "TableDef.settings", m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd());
    SettingElement *element = setting.append("TableData");
    {
        QStringList tableNames = m_tableDataTargets;
        tableNames.sort(Qt::CaseSensitive);
        for ( QStringList::iterator it = tableNames.begin() ; it != tableNames.end() ; ++it )
        {
            element->append("TableName", (*it) );
        }
    }
    setting.save();
}

void TableDefProcessor::setTableDataTargets(QStringList *newTargets)
{
    m_tableDataTargets.clear();
    for ( QStringList::iterator it = newTargets->begin() ; it != newTargets->end() ; ++it )
        m_tableDataTargets.append( (*it) );
}

void TableDefProcessor::determineCodecForProject()
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

//TableDataSetting::TableDataSetting(ProjectSetting *parent)
//    : ObjectSetting(parent)
//{
//    m_objectType          = Model::TableData;
//    m_accessObjectType    = Access::acTable;
//    m_objectPathName      = "tabledatas";
//    m_containerName       = "";

//    m_tempFileExtension   = "tmp";
//    m_designFileExtension = "csv";
//    m_moduleFileExtension = "";
//    m_existCheckExtension = m_designFileExtension;
//}


//=============================================================================
// Relation
// TODO: imeplement relationship export/import

RelationProcessor::RelationProcessor(ProjectSetting *parent)
    : ObjectProcessor(parent)
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
// QueryAsSqlProcessor

QueryAsSqlProcessor::QueryAsSqlProcessor(ProjectSetting *parent)
    : ObjectProcessor(parent)
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

bool QueryAsSqlProcessor::isTargetObject(QAxObject *object) const
{
    DAO::QueryDef *queryDef = dynamic_cast<DAO::QueryDef*>(object);
    if (queryDef)
    {
        return !queryDef->Name().startsWith("~") && queryDef->Connect().isEmpty();
    }
    return false;
}

ObjectItem *QueryAsSqlProcessor::createItemFromProject(QAxObject *object, QObject *parent)
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

bool QueryAsSqlProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
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

bool QueryAsSqlProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
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

bool QueryAsSqlProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool QueryAsSqlProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sinitization required
    return true;
}

bool QueryAsSqlProcessor::prepareItemCollection()
{
    if (!m_projectSetting->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectSetting->application()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_queryDefs.set( currentDb->QueryDefs() );
    return m_queryDefs.is();
}

int QueryAsSqlProcessor::itemCount()
{
    if (!m_queryDefs.is())
        return 0;
    return m_queryDefs->Count();
}

QAxObject *QueryAsSqlProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_queryDefs.is())
        return 0;
    return m_queryDefs->Item(index);
}



//=============================================================================
// QueryAsObjectProcessor
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
QueryAsObjectProcessor::QueryAsObjectProcessor(ProjectSetting *parent)
    : QueryAsSqlProcessor(parent)
{
}

bool QueryAsObjectProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    Q_UNUSED(object)
    {
        m_projectSetting->application()->SaveAsText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );
        return true;
    }
}

bool QueryAsObjectProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
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

AccessObjectProcessor::AccessObjectProcessor(ProjectSetting *parent)
    : ObjectProcessor(parent)
{
}

bool AccessObjectProcessor::isTargetObject(QAxObject *object) const
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

ObjectItem *AccessObjectProcessor::createItemFromProject(QAxObject *object, QObject *parent)
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

bool AccessObjectProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    Q_UNUSED(object)
    {
        m_projectSetting->application()->SaveAsText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );
        return true;
    }
    return false;
}

bool AccessObjectProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    {
        m_projectSetting->application()->LoadFromText( (Access::AcObjectType)m_accessObjectType, objectName, filePath(TempDir, TempFile, objectName) );

        return true;
    }
    return true;
}

bool AccessObjectProcessor::prepareItemCollection()
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

int AccessObjectProcessor::itemCount()
{
    if (!m_documents.is())
        return 0;
    return m_documents->Count();
}

QAxObject *AccessObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_documents.is())
        return 0;
    return m_documents->Item(index);
}
