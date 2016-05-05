#include "odbctableprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/fileutil.h"
#include "util/codecinfo.h"

#include "cvsmodel/accessprojectcontainer.h"
#include "cvsmodel/setting.h"

#include <QUuid>
#include <windows.h>

OdbcTableProcessor::OdbcTableProcessor(ProjectContainer *parent)
    : TableObjectProcessor(parent)
{
    m_objectType          = Model::OdbcTable;
    m_selectObjectType    = Model::OdbcTableType;
    m_accessObjectType    = Access::acTable;
    m_objectPathName      = "tabledefs";
    m_containerName       = "";
    m_iconPath            = ":/images/table.png";

    m_tempFileExtension   = "tmp";
    m_designFileExtension = "odbc";
    m_moduleFileExtension = "";
    m_existCheckExtension = m_designFileExtension;

}

bool OdbcTableProcessor::isTargetObject(QAxObject *object) const
{
    bool result = TableObjectProcessor::isTargetObject(object);
    if (result)
    {
        DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
        if (tableDef)
        {
            if (!tableDef->Connect().contains("ODBC;"))
                result = false;
        }
    }
    return result;
}

bool OdbcTableProcessor::exportFromProjectToTempDir(QAxObject *object, const QString &objectName)
{
    deleteAllFileFromTempDir(objectName);

    DAO::TableDef *tableDef = dynamic_cast<DAO::TableDef*>(object);
    if (tableDef)
    {
        // Setting
        Setting setting( filePath(TempDir, TempFile, objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
        SettingElement *element = setting.append("TableDef");
        element->append("Connect",          tableDef->Connect() );
        element->append("SourceTableName",  tableDef->SourceTableName() );
        element->append("Attributes",       tableDef->Attributes() );
        setting.save();

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
        // Setting
        Setting setting( filePath(TempDir, TempFile, objectName), m_codecForCvs->codec(), m_codecForCvs->bom(), m_codecForCvs->lineEnd() );
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
    return false;
}

bool OdbcTableProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    FileUtil::copyContents( filePath(TempDir, TempFile,   objectName), m_codecForProject,
                            filePath(TempDir, DesignFile, objectName), m_codecForCvs, true/*removeTrailingSpaces*/ );
    return true;
}

bool OdbcTableProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    FileUtil::copyContents( filePath(TempDir, DesignFile, objectName), m_codecForCvs,
                            filePath(TempDir, TempFile,   objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
    return true;
}

