#include "tabledefprocessor.h"

#include <QDebug>
#include <QTextCodec>
#include <QFile>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/projectsetting.h"
#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"
#include "cvsmodel/setting.h"

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
