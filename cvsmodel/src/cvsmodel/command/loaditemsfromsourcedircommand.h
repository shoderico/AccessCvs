#ifndef LOADITEMSFROMSOURCEDIRCOMMAND_H
#define LOADITEMSFROMSOURCEDIRCOMMAND_H

#include "commandbase.h"

class LoadItemsFromSourceDirCommand : public CommandBase
{
    Q_OBJECT
public:
    explicit LoadItemsFromSourceDirCommand(ProjectContainer *project, QAxObject *application, QList<ObjectItem*> *items, QObject *parent = 0);

    virtual void execute(ObjectItemMap *allTargets);
};

#endif // LOADITEMSFROMSOURCEDIRCOMMAND_H
