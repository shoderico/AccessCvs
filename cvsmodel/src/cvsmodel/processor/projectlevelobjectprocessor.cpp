#include "projectlevelobjectprocessor.h"

#include <QDebug>

#include "accesslib/accesslib.h"

#include "util/fileutil.h"

#include "cvsmodel/projectcontainer.h"
#include "cvsmodel/setting.h"
#include "cvsmodel/objectitem.h"


ProjectLevelObjectProcessor::ProjectLevelObjectProcessor(ProjectContainer *parent)
    : ObjectProcessor(parent)
{
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
    if (m_projectContainer->isProjectOpened())
        return true;
    return false;
}

int ProjectLevelObjectProcessor::itemCount()
{
    // different from others.
    if (m_projectContainer->isProjectOpened())
        return 1;
    return 0;
}

QAxObject *ProjectLevelObjectProcessor::itemUnsafePtr(const QVariant &index)
{
    Q_UNUSED(index)
    return 0;
}
