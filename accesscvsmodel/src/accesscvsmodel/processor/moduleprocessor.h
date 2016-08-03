#ifndef MODULEPROCESSOR_H
#define MODULEPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "accessobjectprocessor.h"

class ProjectContainer;

class QAxObject;


namespace Access {
class AllModules;
}

class ModuleProcessor : public AccessObjectProcessor
{
public:
    explicit ModuleProcessor(ProjectContainer *parent);
    virtual bool        sanitizeTempDir(QAxObject* object, const QString &objectName);
    virtual bool        desanitizeTempDir(QAxObject* object, const QString &objectName);

    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
    virtual void determineCodecForProject();
protected:
    ComPtr<Access::AllModules> m_objects;
};


#endif // MODULEPROCESSOR_H
