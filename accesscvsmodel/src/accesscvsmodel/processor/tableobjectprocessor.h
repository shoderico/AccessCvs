#ifndef TABLEOBJECTPROCESSOR_H
#define TABLEOBJECTPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "cvsmodel/processor/objectprocessor.h"

#include "util/comptr.h"

namespace DAO {
class TableDefs;
}

class TableObjectProcessor : public ObjectProcessor
{
    Q_OBJECT
public:
    explicit TableObjectProcessor(ProjectContainer *parent);
    virtual bool        isTargetObject(QAxObject *object) const;
    virtual ObjectItem *createItemFromProject(QAxObject* object, QObject *parent = 0);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);

    virtual void determineCodecForProject();

protected:
    ComPtr<DAO::TableDefs> m_tableDefs;
};

#endif // TABLEOBJECTPROCESSOR_H
