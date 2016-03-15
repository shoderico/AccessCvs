#ifndef VBPROJECTPROCESSOR_H
#define VBPROJECTPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "projectlevelobjectprocessor.h"

class ProjectSetting;

class QAxObject;


namespace VBIDE {
class VBProject;
}

class VBProjectProcessor : public ProjectLevelObjectProcessor
{
public:
    explicit VBProjectProcessor(ProjectSetting *parent);
    virtual bool        exportFromProjectToTempDir(QAxObject* object, const QString &objectName);
    virtual bool        importFromTempDirToProject(QAxObject* object, const QString &objectName);

private:
    VBIDE::VBProject *currentVBProject();
};


#endif // VBPROJECTPROCESSOR_H
