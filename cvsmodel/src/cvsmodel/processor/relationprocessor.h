#ifndef RELATIONPROCESSOR_H
#define RELATIONPROCESSOR_H

#include "cvsmodel/cvsmodel_global.h"
#include "objectprocessor.h"

class ProjectContainer;


class RelationProcessor : public ObjectProcessor
{
public:
    explicit RelationProcessor(ProjectContainer *parent);
};


#endif // RELATIONPROCESSOR_H
