#include "odbctableprocessor.h"

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/fileutil.h"

#include "cvsmodel/accessprojectcontainer.h"


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
        // table-def
        // TODO : export property
        //  Connection
        //  SourceTableName
        //  Attributes

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
        // table-def
        // TODO : create table-def from property
        //  Connection
        //  SourceTableName
        //  Attributes

        return true;
    }
    return false;
}

bool OdbcTableProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    FileUtil::copyContents( filePath(TempDir, TempFile,   objectName), m_codecForProject,
                            filePath(TempDir, ModuleFile, objectName), m_codecForCvs, true/*removeTrailingSpaces*/ );
    return true;
}

bool OdbcTableProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    FileUtil::copyContents( filePath(TempDir, ModuleFile, objectName), m_codecForCvs,
                            filePath(TempDir, TempFile,   objectName), m_codecForProject, false/*removeTrailingSpaces*/ );
    return true;
}

