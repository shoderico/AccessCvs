#ifndef FORMPROCESSOR_H
#define FORMPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "accessdesignobjectprocessor.h"

#include "util/comptr.h"

class ProjectSetting;

class QAxObject;
class QVariant;

namespace Access {
class AllForms;
}

class FormProcessor : public AccessDesignObjectProcessor
{
public:
    explicit FormProcessor(ProjectSetting *parent);
    virtual bool        prepareItemCollection();
    virtual int         itemCount();
    virtual QAxObject  *itemUnsafePtr(const QVariant &index);
protected:
    ComPtr<Access::AllForms> m_objects;
};

#endif // FORMPROCESSOR_H
