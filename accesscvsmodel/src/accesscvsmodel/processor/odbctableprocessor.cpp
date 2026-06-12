#include "odbctableprocessor.h"

#include <QDebug>
#include <QFile>

#include "cvsmodel/objectitem.h"

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/fileutil.h"
#include "util/codecinfo.h"

#include "accesscvsmodel/accessprojectcontainer.h"
#include "util/setting.h"

#include "cvsmodel/sanitizer/tabledefsanitizer.h"
#include "cvsmodel/sanitizer/tabledatasanitizer.h"

#include <QUuid>
#include <windows.h>

#include "pch.hpp"

OdbcTableProcessor::OdbcTableProcessor(ProjectContainer *parent)
    : TableObjectProcessor(parent)
    , m_tableDefSanitizer(new TableDefSanitizer(this))
    , m_tableDataSanitizer(new TableDataSanitizer(this))
{
    m_objectType          = Model::OdbcTable;
    m_selectObjectType    = Model::OdbcTableType;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "odbctables";          // changed from "tabledefs"
    m_containerName       = "";
    m_iconPath            = ":/images/table_link.png";
    m_uiText              = "OdbcTables";

    m_tempFileExtension   = "xmltmp";              // temporary output for structure (.xml) — same as TableDefProcessor
    m_designFileExtension = "xml";                 // table structure (.xml)
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;

    m_dataTempFileExtension = "dattmp";
    m_dataFileExtension     = "dat";               // table data (.dat)

    // ODBC-specific property file (new)
    m_odbcTempFileExtension = "odbctmp";           // temporary ODBC property file
    m_odbcFileExtension     = "odbc";              // ODBC property (.odbc)

    m_settingFileName       = "OdbcTable.settings";
}

bool OdbcTableProcessor::isTargetObject(QAxObject *object) const
{
    if ( !m_toBeManaged )
        return false;

    bool result = TableObjectProcessor::isTargetObject(object);
    if (result)
    {
        DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
        if (tableDef)
        {
            if (!tableDef->Connect().startsWith("ODBC;"))
                result = false;
        }
    }
    return result;
}

ObjectItem *OdbcTableProcessor::createItemFromProject(QAxObject *object, QObject *parent)
{
    ObjectItem *item = TableObjectProcessor::createItemFromProject(object, parent);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        item->setHasData( m_tableDataTargets.contains( item->name() ) );
    }

    return item;
}

ObjectItem *OdbcTableProcessor::createItemFromSourceDir(QFileInfo &fileInfo, QObject *parent)
{
    ObjectItem *item = TableObjectProcessor::createItemFromSourceDir(fileInfo, parent);
    if (item)
    {
        item->setHasData( m_tableDataTargets.contains( item->name() ) );
    }
    return item;
}

bool OdbcTableProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        // ODBC property (.odbc)
        exportOdbcTable(object, objectName);

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

bool OdbcTableProcessor::importFromTempDirToProject(QAxObject *object, const QString &objectName)
{
    if (object)
    {
        // table is already exists. so we need to delete first
        ComPtr<Access::DoCmd> doCmd = m_projectContainer->application<Access::Application>()->DoCmd();
        doCmd->DeleteObject( (Access::AcObjectType)m_accessObjectType, objectName );
    }

    {
        // ODBC property (.odbc)
        importOdbcTable(objectName);

        // table data (.data) import (only for target tables)
        if ( m_tableDataTargets.contains(objectName) && QFile( filePath(TempDir, DataTempFile, objectName) ).exists() )
        {
            importTableData(objectName, filePath(TempDir, DataTempFile, objectName));
        }

        return true;
    }
    return false;
}

bool OdbcTableProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object);

    sanitizeTableStructure(objectName);
    if ( m_tableDataTargets.contains(objectName) )
        sanitizeTableData(objectName);
    sanitizeOdbcProperty(objectName);

    return true;
}

bool OdbcTableProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object);

    desanitizeOdbcProperty(objectName);

    return true;
}

void OdbcTableProcessor::loadSetting(Setting *projectSetting)
{
    Q_UNUSED(projectSetting);
    m_toBeManaged = false;
    m_tableDataTargets.clear();

    Setting *setting = createSetting();
    if (setting->load())
    {
        m_toBeManaged = setting->value( "ToBeManaged", false ).toBool();
        loadTableDataTargets(setting);
    }
    delete setting;
}

void OdbcTableProcessor::saveSetting(Setting *projectSetting)
{
    Q_UNUSED(projectSetting)

    // Setting
    Setting *setting = createSetting();

    setting->append( "ToBeManaged", m_toBeManaged );

    // save TableData section
    if ( !m_tableDataTargets.isEmpty() )
    {
        SettingElement *element = setting->append("TableData");
        foreach (const QString &tableName, m_tableDataTargets)
        {
            element->append("TableName", tableName);
        }
    }

    setting->save();
    delete setting;
}

void OdbcTableProcessor::updateSetting(QList<ObjectItem*> *items)
{
    m_tableDataTargets.clear();

    for (QList<ObjectItem*>::iterator it = items->begin() ; it != items->end() ; ++it  )
    {
        if ( (*it)->hasData() )
            m_tableDataTargets.append( (*it)->name() );
    }
}

//-----------------------------------------------------------------------------
// ODBC-specific sanitize/desanitize helpers
//-----------------------------------------------------------------------------

bool OdbcTableProcessor::sanitizeOdbcProperty(const QString &objectName)
{
    FileUtil::copyContents( filePath(TempDir, OdbcTempFile, objectName), m_codecForProject,
                            filePath(TempDir, OdbcFile,     objectName), m_codecForCvs, true/*removeTrailingSpaces*/ );
    return true;
}

bool OdbcTableProcessor::desanitizeOdbcProperty(const QString &objectName)
{
    FileUtil::copyContents( filePath(TempDir, OdbcFile,     objectName), m_codecForCvs,
                            filePath(TempDir, OdbcTempFile, objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
    return true;
}

//-----------------------------------------------------------------------------
// ODBC table property export/import helpers
//-----------------------------------------------------------------------------

bool OdbcTableProcessor::exportOdbcTable(QAxObject *object, const QString &objectName)
{
    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        Setting setting( filePath(TempDir, OdbcTempFile, objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
        SettingElement *element = setting.append("TableDef");
        element->append("Connect",          tableDef->Connect() );
        element->append("SourceTableName",  tableDef->SourceTableName() );
        element->append("Attributes",       tableDef->Attributes() );
        setting.save();
        return true;
    }
    return false;
}

bool OdbcTableProcessor::importOdbcTable(const QString &objectName)
{
    Setting setting( filePath(TempDir, OdbcTempFile, objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
    if (!setting.load())
    {
        qDebug() << "setting.load() is faled ";
        return false;
    }
    SettingElement *element = setting.at(0)->toElement();
    Q_ASSERT(element != NULL);
    Q_ASSERT(element->name() == "TableDef");

    QString connect         = element->value("Connect").toString();
    QString sourceTableName = element->value("SourceTableName").toString();
    int attributes          = element->value("Attributes", 0).toInt();

    qDebug() << "connect : " << connect;
    qDebug() << "sourceTableName : " << sourceTableName;
    qDebug() << "attributes : " << attributes;

    ComPtr<DAO::Database> currentDb = m_projectContainer->application<Access::Application>()->CurrentDb();
    ComPtr<DAO::TableDefs> tableDefs = currentDb->TableDefs();

    ComPtr<DAO::TableDef> tableDef = currentDb->CreateTableDef( objectName, (attributes & DAO::dbAttachSavePWD) );
    tableDef->SetConnect( connect );
    tableDef->SetSourceTableName( sourceTableName );
    tableDef->SetAttributes( attributes );

    IDispatch *idisp = 0;
    tableDef->queryInterface( QUuid(IID_IDispatch), (void**)&idisp);
    if (idisp)
    {
        tableDefs->Append( idisp );
        idisp->Release();
    }
    return true;
}

