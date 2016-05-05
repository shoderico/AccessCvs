#include "projectlevelobjectprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/fileutil.h"

#include "cvsmodel/accessprojectcontainer.h"
#include "cvsmodel/setting.h"
#include "cvsmodel/objectitem.h"


ProjectLevelObjectProcessor::ProjectLevelObjectProcessor(ProjectContainer *parent)
    : ObjectProcessor(parent)
//    , m_objectName("Reference")
{
//    m_objectType          = Model::Reference;
//    m_accessObjectType    = -1;
//    m_objectPathName      = "";
//    m_containerName       = "";

//    m_tempFileExtension   = "ref";
//    m_designFileExtension = m_tempFileExtension;
//    m_moduleFileExtension = "";
//    m_existCheckExtension = m_tempFileExtension;
}

bool ProjectLevelObjectProcessor::isTargetObject(QAxObject *object) const
{
    Q_UNUSED(object)
    return true;
}

ObjectItem *ProjectLevelObjectProcessor::createItemFromProject(QAxObject *object, QObject *parent)
{
    Q_UNUSED(object)
    ObjectItem *item = new ObjectItem(parent);

    {
        item->setObjectType( m_objectType );
        item->setSelectObjectType( m_selectObjectType );
        item->setIconPath( m_iconPath );
        item->setName( m_objectName );
        item->setInProject( Model::Present );
        item->setExportDate( FileUtil::fileTime( filePath(TempDir, TempFile, item->name()) ) );
    }

    return item;
}

bool ProjectLevelObjectProcessor::sanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool ProjectLevelObjectProcessor::desanitizeTempDir(QAxObject *object, const QString &objectName)
{
    Q_UNUSED(object)
    Q_UNUSED(objectName)
    // no sanitization required
    return true;
}

bool ProjectLevelObjectProcessor::prepareItemCollection()
{
    // different from others.
    if (projectContainer<AccessProjectContainer>()->isMDB() || projectContainer<AccessProjectContainer>()->isADP())
        return true;
    return false;
}

int ProjectLevelObjectProcessor::itemCount()
{
    // different from others.
    if (projectContainer<AccessProjectContainer>()->isMDB() || projectContainer<AccessProjectContainer>()->isADP())
        return 1;
    return 0;
}

QAxObject *ProjectLevelObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    Q_UNUSED(index)
    // different from others.
    if (projectContainer<AccessProjectContainer>()->isMDB() || projectContainer<AccessProjectContainer>()->isADP())
        return NULL; // unused
    return 0;
}
