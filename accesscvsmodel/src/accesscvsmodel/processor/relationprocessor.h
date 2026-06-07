#ifndef RELATIONPROCESSOR_H
#define RELATIONPROCESSOR_H

#include "accesscvsmodel/accesscvsmodel_global.h"
#include "cvsmodel/processor/objectprocessor.h"

class ProjectContainer;


class RelationProcessor : public ObjectProcessor
{
public:
    explicit RelationProcessor(ProjectContainer *parent);
};


#endif // RELATIONPROCESSOR_H
