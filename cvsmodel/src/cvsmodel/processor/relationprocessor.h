#ifndef RELATIONPROCESSOR_H
#define RELATIONPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "objectprocessor.h"

class ProjectSetting;


class RelationProcessor : public ObjectProcessor
{
public:
    explicit RelationProcessor(ProjectSetting *parent);
};


#endif // RELATIONPROCESSOR_H
