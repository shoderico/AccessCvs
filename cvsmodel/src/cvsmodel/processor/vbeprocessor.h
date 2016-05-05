#ifndef VBEPROCESSOR_H
#define VBEPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "projectlevelobjectprocessor.h"

class ProjectContainer;

class QAxObject;


namespace VBIDE {
class VBProject;
}

class VBEProcessor : public ProjectLevelObjectProcessor
{
    Q_OBJECT
public:
    explicit VBEProcessor(ProjectContainer *parent);

protected:
    VBIDE::VBProject *currentVBProject();

};

#endif // VBEPROCESSOR_H
