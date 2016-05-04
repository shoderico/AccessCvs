#include "accessobjectprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/comptr.h"
#include "util/fileutil.h"

#include "cvsmodel/projectsetting.h"
#include "cvsmodel/setting.h"
#include "cvsmodel/objectitem.h"


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
        item->setSelectObjectType( m_selectObjectType );
        item->setIconPath( m_iconPath );
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
        item->setSelectObjectType( m_selectObjectType );
        item->setIconPath( m_iconPath );
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
