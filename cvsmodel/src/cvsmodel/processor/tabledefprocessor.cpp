#include "tabledefprocessor.h"

#include <QDebug>
#include <QTextCodec>
#include <QFile>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "cvsmodel/accessprojectcontainer.h"
#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"
#include "cvsmodel/setting.h"
#include "cvsmodel/objectitem.h"

//=============================================================================
// TableDef

TableDefProcessor::TableDefProcessor(ProjectContainer *parent)
    : TableObjectProcessor(parent)
    , m_tableDefSanitizer(new TableDefSanitizer(this))
    , m_tableDataSanitizer(new TableDataSanitizer(this))
{
    m_objectType          = Model::TableDef;
    m_selectObjectType    = Model::TableDefObjectType;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledefs";
    m_containerName       = "";
    m_iconPath            = ":/images/table.png";
    m_uiText              = "Tables";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "xml";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;

    m_dataTempFileExtension = "dattmp";
    m_dataFileExtension     = "dat";

    m_settingFileName       = "TableDef.settings";
}

bool TableDefProcessor::isTargetObject(QAxObject *object) const
{
    bool result = TableObjectProcessor::isTargetObject(object);
    if (result)
    {
        DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
        if (tableDef)
        {
            if (!tableDef->Connect().isEmpty())
                result = false;
        }
    }
    return result;
}

ObjectItem *TableDefProcessor::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = TableObjectProcessor::createItemFromProject(object, parent);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        item->setHasData( m_tableDataTargets.contains( item->name() ) );
    }

    return item;
}

ObjectItem *TableDefProcessor::createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = TableObjectProcessor::createItemFromSourceDir(fileInfo, parent);
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
            m_projectContainer->application<Access::Application>()
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
            m_projectContainer->application<Access::Application>()
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
        ComPtr<Access::DoCmd> doCmd = m_projectContainer->application<Access::Application>()->DoCmd();
        doCmd->DeleteObject( (Access::AcObjectType)m_accessObjectType, objectName );
    }

    {
        // table-def
        {
            // very slow but very accurate.
            m_projectContainer->application<Access::Application>()
                ->ImportXML(
                        filePath(TempDir, TempFile, objectName)
                        ,Access::acStructureOnly
                        );
        }
        // table-data
        if ( m_tableDataTargets.contains(objectName) && QFile( filePath(TempDir, DataTempFile, objectName) ).exists() )
        {
            m_projectContainer->application<Access::Application>()
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

void TableDefProcessor::loadSetting(Setting *projectSetting)
{
    Q_UNUSED(projectSetting)

    m_tableDataTargets.clear();

    // Setting
    Setting *setting = createSetting();
    if (setting->load())
    {
        foreach (const SettingNode *node, setting->nodes())
        {
            if (node->isElement())
            {
                SettingElement *element = node->toElement();
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
    }
    delete setting;
}

void TableDefProcessor::saveSetting(Setting *projectSetting)
{
    Q_UNUSED(projectSetting)

    // Setting
    Setting *setting = createSetting();
    SettingElement *element = setting->append("TableData");
    {
        QStringList tableNames = m_tableDataTargets;
        tableNames.sort(Qt::CaseSensitive);
        for ( QStringList::iterator it = tableNames.begin() ; it != tableNames.end() ; ++it )
        {
            element->append("TableName", (*it) );
        }
    }
    setting->save();
    delete setting;
}

void TableDefProcessor::updateSetting(QList<ObjectItem *> *items)
{
    m_tableDataTargets.clear();

    for (QList<ObjectItem*>::iterator it = items->begin() ; it != items->end() ; ++it  )
    {
        if ( (*it)->hasData() )
            m_tableDataTargets.append( (*it)->name() );
    }
}
