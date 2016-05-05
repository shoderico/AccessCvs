#ifndef VBPROJECTPROCESSOR_H
#define VBPROJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "vbeprocessor.h"

class ProjectContainer;

class QAxObject;


namespace VBIDE {
class VBProject;
}

class VBProjectProcessor : public VBEProcessor
{
public:
    explicit VBProjectProcessor(ProjectContainer *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);
};


#endif // VBPROJECTPROCESSOR_H
