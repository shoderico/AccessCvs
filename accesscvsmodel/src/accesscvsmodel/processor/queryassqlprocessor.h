#ifndef QUERYASSQLPROCESSOR_H
#define QUERYASSQLPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "cvsmodel/processor/objectprocessor.h"

#include "util/comptr.h"

class ProjectContainer;
class ObjectItem;

class QAxObject;
class QVariant;


namespace DAO {
class QueryDefs;
}


class QueryAsSqlProcessor : public ObjectProcessor
{
public:
    explicit QueryAsSqlProcessor(ProjectContainer *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<DAO::QueryDefs> m_queryDefs;
};


#endif // QUERYASSQLPROCESSOR_H
