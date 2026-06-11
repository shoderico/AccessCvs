#include "tabledefprocessor.h"

#include <QDebug>
#include <QTextCodec>
#include <QFile>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/codecinfo.h"
#include "util/fileutil.h"

#include "accesscvsmodel/accessprojectcontainer.h"
#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"
#include "util/setting.h"
#include "cvsmodel/objectitem.h"

#include "pch.hpp"

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

    m_tempFileExtension   = "xmltmp";
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
        // table structure (.xml)
        exportTableStructure(objectName, filePath(TempDir, TempFile, objectName));

        // table data (.data) — only for target tables
        if ( m_tableDataTargets.contains( objectName ) )
        {
            exportTableData(objectName, filePath(TempDir, DataTempFile, objectName));
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
        // table structure (.xml)
        importTableStructure(objectName, filePath(TempDir, TempFile, objectName));

        // table data (.data) import (only for target tables)
        if ( m_tableDataTargets.contains(objectName) && QFile( filePath(TempDir, DataTempFile, objectName) ).exists() )
        {
            importTableData(objectName, filePath(TempDir, DataTempFile, objectName));
        }

        return true;
    }
    return false;
}

bool TableDefProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object);

    sanitizeTableStructure(objectName);
    if ( m_tableDataTargets.contains(objectName) )
        sanitizeTableData(objectName);

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
    Q_UNUSED(projectSetting);
    m_tableDataTargets.clear();

    Setting *setting = createSetting();
    if (setting->load())
    {
        loadTableDataTargets(setting);
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

//-----------------------------------------------------------------------------
// Table structure import helper
//-----------------------------------------------------------------------------

bool TableDefProcessor::importTableStructure(const QString &objectName, const QString &schemaSource)
{
    // very slow but very accurate.
    m_projectContainer->application<Access::Application>()
        ->ImportXML(schemaSource, Access::acStructureOnly);
    return true;
}
