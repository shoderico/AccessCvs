#include "queryassqlprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/fileutil.h"

#include "cvsmodel/projectsetting.h"
#include "cvsmodel/objectitem.h"




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
