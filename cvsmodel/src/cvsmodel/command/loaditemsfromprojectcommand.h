#ifndef LOADITEMSFROMPROJECTCOMMAND_H
#define LOADITEMSFROMPROJECTCOMMAND_H

#include "commandbase.h"

class LoadItemsFromProjectCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit LoadItemsFromProjectCommand(ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // LOADITEMSFROMPROJECTCOMMAND_H
