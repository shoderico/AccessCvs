#ifndef MACROPROCESSOR_H
#define MACROPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "accessdesignobjectprocessor.h"

#include "util/comptr.h"

class ProjectContainer;

class QAxObject;
class QVariant;

namespace Access {
class AllMacros;
}


class MacroProcessor : public AccessDesignObjectProcessor
{
public:
    explicit MacroProcessor(ProjectContainer *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllMacros> m_objects;
};


#endif // MACROPROCESSOR_H
