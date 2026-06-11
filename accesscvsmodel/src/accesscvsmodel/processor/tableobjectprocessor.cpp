#include "tableobjectprocessor.h"

#include <QDebug>
#include <QFile>
#include <QTextCodec>

#include "accesslib/accesslib.h"

#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"

#include "util/fileutil.h"
#include "util/codecinfo.h"
#include "util/setting.h"

#include "cvsmodel/objectitem.h"
#include "accesscvsmodel/accessprojectcontainer.h"

#include "pch.hpp"

TableObjectProcessor::TableObjectProcessor(ProjectContainer *parent)
    : ObjectProcessor(parent)
{

}

bool TableObjectProcessor::isTargetObject(QAxObject *object) const
{
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        QString name = tableDef->Name();
        return !name.startsWith("MSys")
                && !name.startsWith("~")
                ;
    }
    return false;
}

ObjectItem *TableObjectProcessor::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = new ObjectItem(parent);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        item->setObjectType( m_objectType );
        item->setSelectObjectType( m_selectObjectType );
        item->setIconPath( m_iconPath );
        item->setName( tableDef->Name() );
        item->setInProject( Model::Present );
        item->setCreateDate( tableDef->DateCreated().toDateTime() );
        item->setUpdateDate( tableDef->LastUpdated().toDateTime() );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool TableObjectProcessor::prepareItemCollection()
{
    if (!projectContainer<AccessProjectContainer>()->isMDB())
        return false;

    ComPtr<DAO::Database> currentDb = m_projectContainer->application<Access::Application>()->CurrentDb();
    if (!currentDb.is())
        return false;

    m_tableDefs.set( currentDb->TableDefs() );
    return m_tableDefs.is();
}

int TableObjectProcessor::itemCount()
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Count();
}

QAxObject *TableObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    if (!m_tableDefs.is())
        return 0;
    return m_tableDefs->Item(index);
}

void TableObjectProcessor::determineCodecForProject()
{
    if (!m_codecForProject)
    {
        m_codecForProject = new CodecInfo(this);
        m_codecForProject->setCodec( QTextCodec::codecForName("UTF-8") );
        m_codecForProject->setBom( false );
        m_codecForProject->setLineEnd("\r\n");
    }
}

//-----------------------------------------------------------------------------
// Common Export/Import helpers for table structure and data
// Used by TableDefProcessor and OdbcTableProcessor
//-----------------------------------------------------------------------------

bool TableObjectProcessor::exportTableStructure(const QString &objectName, const QString &schemaTarget)
{
    m_projectContainer->application<Access::Application>()
        ->ExportXML(
                Access::acExportTable
                ,objectName
                ,QString()                                   // DataTarget
                ,schemaTarget                                // SchemaTarget
                ,QString()                                   // PresentationTarget
                ,QString()                                   // ImageTarget
                ,Access::acUTF8                              // Encoding
                ,Access::acExportAllTableAndFieldProperties  // OtherFlags
                ,QString()                                   // WhereCondition
                ,QVariant()                                  // AdditionalData
                );
    return true;
}

bool TableObjectProcessor::exportTableData(const QString &objectName, const QString &dataTarget)
{
    m_projectContainer->application<Access::Application>()
        ->ExportXML(
                Access::acExportTable
                ,objectName
                ,dataTarget          // DataTarget
                ,QString()           // SchemaTarget
                ,QString()           // PresentationTarget
                ,QString()           // ImageTarget
                ,Access::acUTF8      // Encoding
                );
    return true;
}

bool TableObjectProcessor::importTableData(const QString &objectName, const QString &dataTarget)
{
    if (QFile(dataTarget).exists())
    {
        m_projectContainer->application<Access::Application>()
            ->ImportXML(dataTarget, Access::acAppendData);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Common sanitize helpers for table structure and data
// Used by TableDefProcessor and OdbcTableProcessor
//-----------------------------------------------------------------------------

bool TableObjectProcessor::sanitizeTableStructure(const QString &objectName)
{
    QFile fileSrc( filePath(TempDir, TempFile,   objectName) );
    QFile fileDst( filePath(TempDir, DesignFile, objectName) );

    FileUtil::deleteFile( fileDst.fileName() );

    if ( !fileSrc.open( QIODevice::ReadOnly) )
        return false;
    if ( !fileDst.open(QIODevice::WriteOnly) )
    {
        fileSrc.close();
        return false;
    }

    QTextStream streamSrc( &fileSrc );
    streamSrc.setCodec(m_codecForProject->codec());

    QTextStream streamDst( &fileDst );
    streamDst.setCodec(m_codecForCvs->codec());
    streamDst.setGenerateByteOrderMark(m_codecForCvs->bom());

    // sanitize table-def
    TableDefSanitizer sanitizer(this);
    sanitizer.sanitize( streamSrc, streamDst, m_codecForCvs );

    fileSrc.close();
    fileDst.close();

    return true;
}

bool TableObjectProcessor::sanitizeTableData(const QString &objectName)
{
    QFile fileSrc( filePath(TempDir, DataTempFile, objectName) );
    QFile fileDst( filePath(TempDir, DataFile,     objectName) );

    FileUtil::deleteFile( fileDst.fileName() );

    if ( !fileSrc.open( QIODevice::ReadOnly) )
        return false;
    if ( !fileDst.open(QIODevice::WriteOnly) )
    {
        fileSrc.close();
        return false;
    }

    QTextStream streamSrc( &fileSrc );
    streamSrc.setCodec(m_codecForProject->codec());

    QTextStream streamDst( &fileDst );
    streamDst.setCodec(m_codecForCvs->codec());
    streamDst.setGenerateByteOrderMark(m_codecForCvs->bom());

    // sanitize table-data
    TableDataSanitizer sanitizer(this);
    sanitizer.sanitize( streamSrc, streamDst, m_codecForCvs );

    fileSrc.close();
    fileDst.close();

    return true;
}

//-----------------------------------------------------------------------------
// Common TableData target loader
//-----------------------------------------------------------------------------

void TableObjectProcessor::loadTableDataTargets(Setting *setting)
{
    if (!setting) return;

    foreach (const SettingNode *node, setting->nodes())
    {
        if (node->isElement())
        {
            SettingElement *element = node->toElement();
            if (element && element->name() == "TableData")
            {
                for ( int i = 0 ; i < element->count() ; ++i )
                {
                    SettingKeyValue *keyValue = element->at(i)->toKeyValue();
                    if (keyValue && keyValue->key() == "TableName" && !keyValue->value().toString().isEmpty())
                    {
                        m_tableDataTargets.append( keyValue->value().toString() );
                    }
                }
            }
        }
    }
}

