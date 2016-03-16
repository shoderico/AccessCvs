#ifndef ACCESSOBJECTPROCESSOR_H
#define ACCESSOBJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "objectprocessor.h"

#include "util/comptr.h"

class ProjectSetting;
class ObjectItem;

class QAxObject;
class QVariant;

namespace DAO {
class Containers;
class Container;
class Documents;
}


class AccessObjectProcessor : public ObjectProcessor
{
public:
    explicit AccessObjectProcessor(ProjectSetting *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<DAO::Containers> m_containers;
    ComPtr<DAO::Container> m_container;
    ComPtr<DAO::Documents> m_documents;
};


#endif // ACCESSOBJECTPROCESSOR_H
